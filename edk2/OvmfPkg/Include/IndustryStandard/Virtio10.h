/** @file
  Definitions from the VirtIo 1.0 specification (csprd05).

  Copyright (C) 2016, Red Hat, Inc.
  Copyright (C) 2017, AMD, Inc.

  This program and the accompanying materials are licensed and made available
  under the terms and conditions of the BSD License which accompanies this
  distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS, WITHOUT
  WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef _VIRTIO_1_0_H_
#define _VIRTIO_1_0_H_

#include <IndustryStandard/Pci23.h>
#include <IndustryStandard/Virtio095.h>

//
// Subsystem Device IDs (to be) introduced in VirtIo 1.0
//
#define VIRTIO_SUBSYSTEM_GPU_DEVICE         16

//
// Structures for parsing the VirtIo 1.0 specific PCI capabilities from the
// config space
//
#pragma pack (1)
typedef struct {
  EFI_PCI_CAPABILITY_VENDOR_HDR VendorHdr;
  UINT8  ConfigType; // Identifies the specific VirtIo 1.0 config structure
  UINT8  Bar;        // The BAR that contains the structure
  UINT8  Padding[3];
  UINT32 Offset;     // Offset within Bar until the start of the structure
  UINT32 Length;     // Length of the structure
} VIRTIO_PCI_CAP;
#pragma pack ()

//
// Values for the VIRTIO_PCI_CAP.ConfigType field
//
#define VIRTIO_PCI_CAP_COMMON_CFG 1 // Common configuration
#define VIRTIO_PCI_CAP_NOTIFY_CFG 2 // Notifications
#define VIRTIO_PCI_CAP_DEVICE_CFG 4 // Device specific configuration

//
// Structure pointed-to by Bar and Offset in VIRTIO_PCI_CAP when ConfigType is
// VIRTIO_PCI_CAP_COMMON_CFG
//
#pragma pack (1)
typedef struct {
  UINT32 DeviceFeatureSelect;
  UINT32 DeviceFeature;
  UINT32 DriverFeatureSelect;
  UINT32 DriverFeature;
  UINT16 MsixConfig;
  UINT16 NumQueues;
  UINT8  DeviceStatus;
  UINT8  ConfigGeneration;
  UINT16 QueueSelect;
  UINT16 QueueSize;
  UINT16 QueueMsixVector;
  UINT16 QueueEnable;
  UINT16 QueueNotifyOff;
  UINT64 QueueDesc;
  UINT64 QueueAvail;
  UINT64 QueueUsed;
} VIRTIO_PCI_COMMON_CFG;
#pragma pack ()

//
// VirtIo 1.0 device status bits
//
#define VSTAT_FEATURES_OK BIT3

//
// VirtIo 1.0 reserved (device-independent) feature bits
//
#define VIRTIO_F_VERSION_1      BIT32
#define VIRTIO_F_IOMMU_PLATFORM BIT33

#endif // _VIRTIO_1_0_H_
