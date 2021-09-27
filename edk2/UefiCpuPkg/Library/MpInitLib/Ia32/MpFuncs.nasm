;------------------------------------------------------------------------------ ;
; Copyright (c) 2015 - 2018, Intel Corporation. All rights reserved.<BR>
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http://opensource.org/licenses/bsd-license.php.
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
; Module Name:
;
;   MpFuncs.nasm
;
; Abstract:
;
;   This is the assembly code for MP support
;
;-------------------------------------------------------------------------------

%include "MpEqu.inc"
extern ASM_PFX(InitializeFloatingPointUnits)

SECTION .text

;-------------------------------------------------------------------------------------
;RendezvousFunnelProc  procedure follows. All APs execute their procedure. This
;procedure serializes all the AP processors through an Init sequence. It must be
;noted that APs arrive here very raw...ie: real mode, no stack.
;ALSO THIS PROCEDURE IS EXECUTED BY APs ONLY ON 16 BIT MODE. HENCE THIS PROC
;IS IN MACHINE CODE.
;-------------------------------------------------------------------------------------
global ASM_PFX(RendezvousFunnelProc)
ASM_PFX(RendezvousFunnelProc):
RendezvousFunnelProcStart:
; At this point CS = 0x(vv00) and ip= 0x0.
BITS 16
    mov        ebp, eax                        ; save BIST information

    mov        ax, cs
    mov        ds, ax
    mov        es, ax
    mov        ss, ax
    xor        ax, ax
    mov        fs, ax
    mov        gs, ax

    mov        si,  BufferStartLocation
    mov        ebx, [si]

    mov        si,  DataSegmentLocation
    mov        edx, [si]

    ;
    ; Get start address of 32-bit code in low memory (<1MB)
    ;
    mov        edi, ModeTransitionMemoryLocation

    mov        si, GdtrLocation
o32 lgdt       [cs:si]

    mov        si, IdtrLocation
o32 lidt       [cs:si]

    ;
    ; Switch to protected mode
    ;
    mov        eax, cr0                        ; Get control register 0
    or         eax, 000000003h                 ; Set PE bit (bit #0) & MP
    mov        cr0, eax

    ; Switch to 32-bit code in executable memory (>1MB)
o32 jmp far    [cs:di]

;
; Following code may be copied to memory with type of EfiBootServicesCode.
; This is required at DXE phase if NX is enabled for EfiBootServicesCode of
; memory.
;
BITS 32
Flat32Start:                                   ; protected mode entry point
    mov        ds, dx
    mov        es, dx
    mov        fs, dx
    mov        gs, dx
    mov        ss, dx

    mov        esi, ebx

    ; Increment the number of APs executing here as early as possible
    ; This is decremented in C code when AP is finished executing
    mov        edi, esi
    add        edi, NumApsExecutingLocation
    lock inc   dword [edi]

    mov         edi, esi
    add         edi, EnableExecuteDisableLocation
    cmp         byte [edi], 0
    jz          SkipEnableExecuteDisable

    ;
    ; Enable IA32 PAE execute disable
    ;

    mov         ecx, 0xc0000080
    rdmsr
    bts         eax, 11
    wrmsr

    mov         edi, esi
    add         edi, Cr3Location
    mov         eax, dword [edi]
    mov         cr3, eax

    mov         eax, cr4
    bts         eax, 5
    mov         cr4, eax

    mov         eax, cr0
    bts         eax, 31
    mov         cr0, eax

SkipEnableExecuteDisable:
    mov        edi, esi
    add        edi, InitFlagLocation
    cmp        dword [edi], 1       ; 1 == ApInitConfig
    jnz        GetApicId

    ; AP init
    mov        edi, esi
    add        edi, LockLocation
    mov        eax, NotVacantFlag

TestLock:
    xchg       [edi], eax
    cmp        eax, NotVacantFlag
    jz         TestLock

    mov        ecx, esi
    add        ecx, ApIndexLocation
    inc        dword [ecx]
    mov        ebx, [ecx]

Releaselock:
    mov        eax, VacantFlag
    xchg       [edi], eax

    mov        edi, esi
    add        edi, StackSizeLocation
    mov        eax, [edi]
    mov        ecx, ebx
    inc        ecx
    mul        ecx                               ; EAX = StackSize * (CpuNumber + 1)
    mov        edi, esi
    add        edi, StackStartAddressLocation
    add        eax, [edi]
    mov        esp, eax
    jmp        CProcedureInvoke

GetApicId:
    mov        eax, 0
    cpuid
    cmp        eax, 0bh
    jb         NoX2Apic             ; CPUID level below CPUID_EXTENDED_TOPOLOGY

    mov        eax, 0bh
    xor        ecx, ecx
    cpuid
    test       ebx, 0ffffh
    jz         NoX2Apic             ; CPUID.0BH:EBX[15:0] is zero

    ; Processor is x2APIC capable; 32-bit x2APIC ID is already in EDX
    jmp        GetProcessorNumber

NoX2Apic:
    ; Processor is not x2APIC capable, so get 8-bit APIC ID
    mov        eax, 1
    cpuid
    shr        ebx, 24
    mov        edx, ebx

GetProcessorNumber:
    ;
    ; Get processor number for this AP
    ; Note that BSP may become an AP due to SwitchBsp()
    ;
    xor         ebx, ebx
    lea         eax, [esi + CpuInfoLocation]
    mov         edi, [eax]

GetNextProcNumber:
    cmp         [edi], edx                       ; APIC ID match?
    jz          ProgramStack
    add         edi, 20
    inc         ebx
    jmp         GetNextProcNumber

ProgramStack:
    mov         esp, [edi + 12]

CProcedureInvoke:
    push       ebp               ; push BIST data at top of AP stack
    xor        ebp, ebp          ; clear ebp for call stack trace
    push       ebp
    mov        ebp, esp

    mov        eax, ASM_PFX(InitializeFloatingPointUnits)
    call       eax               ; Call assembly function to initialize FPU per UEFI spec

    push       ebx               ; Push ApIndex
    mov        eax, esi
    add        eax, LockLocation
    push       eax               ; push address of exchange info data buffer

    mov        edi, esi
    add        edi, ApProcedureLocation
    mov        eax, [edi]

    call       eax               ; Invoke C function

    jmp        $                 ; Never reach here
RendezvousFunnelProcEnd:

;-------------------------------------------------------------------------------------
;  AsmRelocateApLoop (MwaitSupport, ApTargetCState, PmCodeSegment, TopOfApStack, CountTofinish);
;-------------------------------------------------------------------------------------
global ASM_PFX(AsmRelocateApLoop)
ASM_PFX(AsmRelocateApLoop):
AsmRelocateApLoopStart:
    mov        eax, esp
    mov        esp, [eax + 16]     ; TopOfApStack
    push       dword [eax]         ; push return address for stack trace
    push       ebp
    mov        ebp, esp
    mov        ebx, [eax + 8]      ; ApTargetCState
    mov        ecx, [eax + 4]      ; MwaitSupport
    mov        eax, [eax + 20]     ; CountTofinish
    lock dec   dword [eax]         ; (*CountTofinish)--
    cmp        cl,  1              ; Check mwait-monitor support
    jnz        HltLoop
MwaitLoop:
    cli
    mov        eax, esp
    xor        ecx, ecx
    xor        edx, edx
    monitor
    mov        eax, ebx            ; Mwait Cx, Target C-State per eax[7:4]
    shl        eax, 4
    mwait
    jmp        MwaitLoop
HltLoop:
    cli
    hlt
    jmp        HltLoop
AsmRelocateApLoopEnd:

;-------------------------------------------------------------------------------------
;  AsmGetAddressMap (&AddressMap);
;-------------------------------------------------------------------------------------
global ASM_PFX(AsmGetAddressMap)
ASM_PFX(AsmGetAddressMap):
    pushad
    mov        ebp,esp

    mov        ebx,  [ebp + 24h]
    mov        dword [ebx], RendezvousFunnelProcStart
    mov        dword [ebx +  4h], Flat32Start - RendezvousFunnelProcStart
    mov        dword [ebx +  8h], RendezvousFunnelProcEnd - RendezvousFunnelProcStart
    mov        dword [ebx + 0Ch], AsmRelocateApLoopStart
    mov        dword [ebx + 10h], AsmRelocateApLoopEnd - AsmRelocateApLoopStart
    mov        dword [ebx + 14h], Flat32Start - RendezvousFunnelProcStart

    popad
    ret

;-------------------------------------------------------------------------------------
;AsmExchangeRole procedure follows. This procedure executed by current BSP, that is
;about to become an AP. It switches it'stack with the current AP.
;AsmExchangeRole (IN   CPU_EXCHANGE_INFO    *MyInfo, IN   CPU_EXCHANGE_INFO    *OthersInfo);
;-------------------------------------------------------------------------------------
global ASM_PFX(AsmExchangeRole)
ASM_PFX(AsmExchangeRole):
    ; DO NOT call other functions in this function, since 2 CPU may use 1 stack
    ; at the same time. If 1 CPU try to call a function, stack will be corrupted.
    pushad
    mov        ebp,esp

    ; esi contains MyInfo pointer
    mov        esi, [ebp + 24h]

    ; edi contains OthersInfo pointer
    mov        edi, [ebp + 28h]

    ;Store EFLAGS, GDTR and IDTR register to stack
    pushfd
    mov        eax, cr4
    push       eax       ; push cr4 firstly
    mov        eax, cr0
    push       eax

    sgdt       [esi + 8]
    sidt       [esi + 14]

    ; Store the its StackPointer
    mov        [esi + 4],esp

    ; update its switch state to STORED
    mov        byte [esi], CPU_SWITCH_STATE_STORED

WaitForOtherStored:
    ; wait until the other CPU finish storing its state
    cmp        byte [edi], CPU_SWITCH_STATE_STORED
    jz         OtherStored
    pause
    jmp        WaitForOtherStored

OtherStored:
    ; Since another CPU already stored its state, load them
    ; load GDTR value
    lgdt       [edi + 8]

    ; load IDTR value
    lidt       [edi + 14]

    ; load its future StackPointer
    mov        esp, [edi + 4]

    ; update the other CPU's switch state to LOADED
    mov        byte [edi], CPU_SWITCH_STATE_LOADED

WaitForOtherLoaded:
    ; wait until the other CPU finish loading new state,
    ; otherwise the data in stack may corrupt
    cmp        byte [esi], CPU_SWITCH_STATE_LOADED
    jz         OtherLoaded
    pause
    jmp        WaitForOtherLoaded

OtherLoaded:
    ; since the other CPU already get the data it want, leave this procedure
    pop        eax
    mov        cr0, eax
    pop        eax
    mov        cr4, eax
    popfd

    popad
    ret
