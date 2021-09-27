#include "general.h"

const UINT8 _gDriverUnloadImageCount = 1;

const UINT32 _gUefiDriverRevision = 0x200;
const UINT32 _gDxeRevision = 0x200;

CHAR8* gEfiCallerBaseName = "SecureFake";

EFI_STATUS EFIAPI UefiUnload(EFI_HANDLE ImageHandle)
{
    return EFI_ACCESS_DENIED;
}

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    return PrepaireSystem();
}