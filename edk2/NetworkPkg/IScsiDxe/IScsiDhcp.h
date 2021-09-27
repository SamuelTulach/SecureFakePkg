/** @file
  The head file of iSCSI DHCP4 related configuration routines.

Copyright (c) 2004 - 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _ISCSI_DHCP_H_
#define _ISCSI_DHCP_H_

#define ISCSI_ROOT_PATH_ID              "iscsi:"
#define ISCSI_ROOT_PATH_FIELD_DELIMITER ':'

#define RP_FIELD_IDX_SERVERNAME         0
#define RP_FIELD_IDX_PROTOCOL           1
#define RP_FIELD_IDX_PORT               2
#define RP_FIELD_IDX_LUN                3
#define RP_FIELD_IDX_TARGETNAME         4
#define RP_FIELD_IDX_MAX                5

typedef struct _ISCSI_ATTEMPT_CONFIG_NVDATA ISCSI_ATTEMPT_CONFIG_NVDATA;

typedef struct _ISCSI_ROOT_PATH_FIELD {
  CHAR8 *Str;
  UINT8 Len;
} ISCSI_ROOT_PATH_FIELD;

/**
  Parse the DHCP ACK to get the address configuration and DNS information.

  @param[in]       Image            The handle of the driver image.
  @param[in]       Controller       The handle of the controller.
  @param[in, out]  ConfigData       The attempt configuration data.

  @retval EFI_SUCCESS           The DNS information is got from the DHCP ACK.
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate memory.
  @retval EFI_NO_MEDIA          There was a media error.
  @retval Others                Other errors as indicated.

**/
EFI_STATUS
IScsiDoDhcp (
  IN     EFI_HANDLE                  Image,
  IN     EFI_HANDLE                  Controller,
  IN OUT ISCSI_ATTEMPT_CONFIG_NVDATA *ConfigData
  );

#endif
