#include "general.h"

static EFI_GET_VARIABLE oGetVariable = NULL;

static EFI_EVENT NotifyEvent = NULL;
static EFI_EVENT ExitEvent = NULL;
static BOOLEAN Virtual = FALSE;
static BOOLEAN Runtime = FALSE;

#define CHECK_NAME(x, y) (StrnCmp(x, y, sizeof(y) / sizeof(CHAR16)) == 0)

EFI_STATUS EFIAPI HookedGetVariable(CHAR16* variableName, EFI_GUID* vendorGuid, UINT32* attributes, UINTN* dataSize, void* data)
{
    if (CHECK_NAME(variableName, EFI_SECURE_BOOT_ENABLE_NAME))
    {
        if (*dataSize < sizeof(UINT8))
            return EFI_BUFFER_TOO_SMALL;

        *(UINT8*)data = SECURE_BOOT_ENABLE;
        return EFI_SUCCESS;
    }

    if (CHECK_NAME(variableName, EFI_CUSTOM_MODE_NAME))
    {
        if (*dataSize < sizeof(UINT8))
            return EFI_BUFFER_TOO_SMALL;

        *(UINT8*)data = STANDARD_SECURE_BOOT_MODE;
        return EFI_SUCCESS;
    }

    if (CHECK_NAME(variableName, EFI_VENDOR_KEYS_NV_VARIABLE_NAME))
    {
        if (*dataSize < sizeof(UINT8))
            return EFI_BUFFER_TOO_SMALL;

        *(UINT8*)data = VENDOR_KEYS_VALID;
        return EFI_SUCCESS;
    }

    if (CHECK_NAME(variableName, EFI_SECURE_BOOT_MODE_NAME))
    {
        if (*dataSize < sizeof(UINT8))
            return EFI_BUFFER_TOO_SMALL;

        *(UINT8*)data = SECURE_BOOT_ENABLE;
        return EFI_SUCCESS;
    }

    if (CHECK_NAME(variableName, EFI_VENDOR_KEYS_VARIABLE_NAME))
    {
        if (*dataSize < sizeof(UINT8))
            return EFI_BUFFER_TOO_SMALL;

        *(UINT8*)data = VENDOR_KEYS_VALID;
        return EFI_SUCCESS;
    }

    return oGetVariable(variableName, vendorGuid, attributes, dataSize, data);
}

void EFIAPI SetVirtualAddressMapEvent(EFI_EVENT Event, void* Context)
{
    gRT->ConvertPointer(0, (void**)&oGetVariable);

    NotifyEvent = NULL;
    Virtual = TRUE;
}

void EFIAPI ExitBootServicesEvent(EFI_EVENT Event, void* Context)
{
    gBS->CloseEvent(ExitEvent);
    ExitEvent = NULL;
    gBS = NULL;
    Runtime = TRUE;
}

void* SetServicePointer(EFI_TABLE_HEADER* ServiceTableHeader, void** ServiceTableFunction, void* NewFunction)
{
    if (ServiceTableFunction == NULL || NewFunction == NULL || *ServiceTableFunction == NULL)
        return NULL;

    ASSERT(gBS != NULL);
    ASSERT(gBS->CalculateCrc32 != NULL);

    const EFI_TPL tpl = gBS->RaiseTPL(TPL_HIGH_LEVEL);

    void* originalFunction = *ServiceTableFunction;
    *ServiceTableFunction = NewFunction;

    ServiceTableHeader->CRC32 = 0;
    gBS->CalculateCrc32((UINT8*)ServiceTableHeader, ServiceTableHeader->HeaderSize, &ServiceTableHeader->CRC32);

    gBS->RestoreTPL(tpl);

    return originalFunction;
}

EFI_STATUS PrepaireSystem()
{
    EFI_STATUS status = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL,
        TPL_NOTIFY,
        SetVirtualAddressMapEvent,
        NULL,
        &gEfiEventVirtualAddressChangeGuid,
        &NotifyEvent);
    if (EFI_ERROR(status))
    {
        Print(L"Can't create event (SetVirtualAddressMapEvent): %d\n", status);
        return status;
    }
    Print(L"SetVirtualAddressMapEvent -> 0x%p\n", SetVirtualAddressMapEvent);

    status = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL,
        TPL_NOTIFY,
        ExitBootServicesEvent,
        NULL,
        &gEfiEventExitBootServicesGuid,
        &ExitEvent);
    if (EFI_ERROR(status))
    {
        Print(L"Can't create event (ExitBootServicesEvent): %d\n", status);
        return status;
    }
    Print(L"ExitBootServicesEvent -> 0x%p\n", ExitBootServicesEvent);

    oGetVariable = (EFI_GET_VARIABLE)SetServicePointer(&gRT->Hdr, (void**)&gRT->GetVariable, (void**)&HookedGetVariable);
    Print(L"GetVariable -> 0x%p\n", HookedGetVariable);

    return EFI_SUCCESS;
}