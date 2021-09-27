#pragma once

//
// Defines copied from EDK2 source tree
// Not included directly since I want this to be
// compilable using gnu-efi with just minimal changes
//

///
/// "SecureBootEnable" variable for the Secure Boot feature enable/disable.
/// This variable is used for allowing a physically present user to disable
/// Secure Boot via firmware setup without the possession of PKpriv.
///
/// GUID: gEfiSecureBootEnableDisableGuid
///
/// Format: UINT8
///
#define EFI_SECURE_BOOT_ENABLE_NAME      L"SecureBootEnable"
#define SECURE_BOOT_ENABLE               1
#define SECURE_BOOT_DISABLE              0

///
///  "CustomMode" variable for two Secure Boot modes feature: "Custom" and "Standard".
///  Standard Secure Boot mode is the default mode as UEFI Spec's description.
///  Custom Secure Boot mode allows for more flexibility as specified in the following:
///    Can enroll or delete PK without existing PK's private key.
///    Can enroll or delete KEK without existing PK's private key.
///    Can enroll or delete signature from DB/DBX without KEK's private key.
///
///  GUID: gEfiCustomModeEnableGuid
///
/// Format: UINT8
///
#define EFI_CUSTOM_MODE_NAME          L"CustomMode"
#define CUSTOM_SECURE_BOOT_MODE       1
#define STANDARD_SECURE_BOOT_MODE     0

///
///  "VendorKeysNv" variable to record the out of band secure boot keys modification.
///  This variable is a read-only NV variable that indicates whether someone other than
///  the platform vendor has used a mechanism not defined by the UEFI Specification to
///  transition the system to setup mode or to update secure boot keys.
///
///  GUID: gEfiVendorKeysNvGuid
///
/// Format: UINT8
///
#define EFI_VENDOR_KEYS_NV_VARIABLE_NAME       L"VendorKeysNv"
#define VENDOR_KEYS_VALID             1
#define VENDOR_KEYS_MODIFIED          0