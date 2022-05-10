/** @file
*  Enables Platform EHCI and SDHCI devices in QEMU virt-rt machine
*  For Windows RT
*
*  Copyright (c) 2021, GH Cao. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>

EFI_STATUS
EFIAPI
InitializeVirtWinRtPlatformDxe (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                     Status;
  UINTN                          EhciSize;
  VOID*                          EhciBase;
  UINTN                          SdHciSize;
  VOID*                          SdHciBase;

  DEBUG ((DEBUG_INFO, "%a: InitializeVirtWinRtPlatformDxe called\n", __FUNCTION__));

  EhciBase = (VOID*)(UINTN)PcdGet32 (PcdPlatformEhciBase);
  ASSERT (EhciBase != NULL);
  EhciSize = (UINTN)PcdGet32 (PcdPlatformEhciSize);
  ASSERT (EhciSize != 0);

  DEBUG ((DEBUG_INFO, "%a: Got platform EHCI %p %u\n",
          __FUNCTION__, EhciBase, EhciSize));

  Status = RegisterNonDiscoverableMmioDevice (
                   NonDiscoverableDeviceTypeEhci,
                   NonDiscoverableDeviceDmaTypeCoherent,
                   NULL,
                   NULL,
                   1,
                   EhciBase, EhciSize);

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: NonDiscoverable: Cannot install EHCI device @%p (Staus == %r)\n",
            __FUNCTION__, EhciBase, Status));
    return Status;
  }

  SdHciBase = (VOID*)(UINTN)PcdGet32 (PcdPlatformSdHciBase);
  ASSERT (SdHciBase != NULL);
  SdHciSize = (UINTN)PcdGet32 (PcdPlatformSdHciSize);
  ASSERT (SdHciSize != 0);

  DEBUG ((DEBUG_INFO, "%a: Got platform SDHCI %p %u\n",
          __FUNCTION__, SdHciBase, SdHciSize));

  Status = RegisterNonDiscoverableMmioDevice (
                   NonDiscoverableDeviceTypeSdhci,
                   NonDiscoverableDeviceDmaTypeCoherent,
                   NULL,
                   NULL,
                   1,
                   SdHciBase, SdHciSize);

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: NonDiscoverable: Cannot install SDHCI device @%p (Staus == %r)\n",
            __FUNCTION__, SdHciBase, Status));
    return Status;
  }

  return EFI_SUCCESS;
}
