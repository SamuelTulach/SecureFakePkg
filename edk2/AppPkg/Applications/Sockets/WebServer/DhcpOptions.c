/** @file
    Display the DHCP options

    Copyright (c) 2011 - 2012, Intel Corporation. All rights reserved.<BR>
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution. The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/
#include <WebServer.h>
#include <Guid/DxeServices.h>
#include <Pi/PiDxeCis.h>

#include <Protocol/Dhcp4.h>
#include <Protocol/ServiceBinding.h>

/**
  Respond with the DHCP options

  @param[in]  SocketFD      The socket's file descriptor to add to the list.
  @param[in]  pPort         The WSDT_PORT structure address
  @param[out] pbDone        Address to receive the request completion status

  @retval EFI_SUCCESS       The request was successfully processed

**/
EFI_STATUS
DhcpOptionsPage (
  IN int SocketFD,
  IN WSDT_PORT * pPort,
  OUT BOOLEAN * pbDone
  )
{
//  EFI_HANDLE Dhcp4Handle;
  EFI_DHCP4_MODE_DATA Dhcp4Mode;
  UINTN HandleCount;
  EFI_DHCP4_PROTOCOL * pDhcp4;
  EFI_DHCP4_PACKET * pDhcp4Packet;
  EFI_HANDLE * pEnd;
  EFI_HANDLE * pHandle;
//  EFI_SERVICE_BINDING_PROTOCOL * pService;
  EFI_STATUS Status;

  DBG_ENTER ( );

  //
  //  Send the DHCP options
  //
  for ( ; ; ) {
    //
    //  Send the page header
    //
    Status = HttpPageHeader ( SocketFD, pPort, L"DHCP Options" );
    if ( EFI_ERROR ( Status )) {
      break;
    }

    //
    //  Build the header
    //
    Status = HttpSendAnsiString ( SocketFD,
                                  pPort,
                                  "<h1>" );
    if ( EFI_ERROR ( Status )) {
      break;
    }
    Status = HttpSendUnicodeString ( SocketFD,
                                     pPort,
                                     L"DHCP Options" );
    if ( EFI_ERROR ( Status )) {
      break;
    }
    Status = HttpSendAnsiString ( SocketFD,
                                  pPort,
                                  "</h1>\r\n" );
    if ( EFI_ERROR ( Status )) {
      break;
    }

    //
    //  Attempt to locate DHCP clients
    //
    Status = gBS->LocateHandleBuffer ( ByProtocol,
//                                       &gEfiDhcp4ServiceBindingProtocolGuid,
                                       &gEfiDhcp4ProtocolGuid,
                                       NULL,
                                       &HandleCount,
                                       &pHandle );
    if ( EFI_ERROR ( Status )) {
      Status = HttpSendAnsiString ( SocketFD,
                                    pPort,
                                    "DHCP not in use" );
      if ( EFI_ERROR ( Status )) {
        break;
      }
    }
    else {
      //
      //  Walk the list of handles
      //
      pEnd = &pHandle [ HandleCount ];
      while ( pEnd > pHandle ) {
/*
        //
        //  Get the DHCP service binding
        //
        Status = gBS->OpenProtocol ( *pHandle,
                                      &gEfiDhcp4ServiceBindingProtocolGuid,
                                      &pService,
                                      NULL,
                                      gImageHandle,
                                      EFI_OPEN_PROTOCOL_GET_PROTOCOL );
        if ( EFI_ERROR ( Status )) {
          Status = HttpSendAnsiString ( SocketFD,
                                        pPort,
                                        "Failed to open gEfiDhcp4ServiceBindingProtocolGuid" );
          break;
        }

        //
        //  Get the DHCP handle
        //
        Status = pService->CreateChild ( pService,
                                         &Dhcp4Handle );
        if ( EFI_ERROR ( Status )) {
          Status = HttpSendAnsiString ( SocketFD,
                                        pPort,
                                        "Failed to create DHCP4 child" );
        }
        else {
*/
          //
          //  Get the DHCP protocol
          //
          Status = gBS->OpenProtocol ( *pHandle,
//                                       Dhcp4Handle,
                                       &gEfiDhcp4ProtocolGuid,
                                       (VOID **)&pDhcp4,
                                       NULL,
                                       gImageHandle,
                                       EFI_OPEN_PROTOCOL_GET_PROTOCOL );
          if ( EFI_ERROR ( Status )) {
            Status = HttpSendAnsiString ( SocketFD,
                                          pPort,
                                          "Failed to open gEfiDhcp4ProtocolGuid" );
          }
          else {
            //
            //  Get the DHCP packet
            //
            Status = pDhcp4->GetModeData ( pDhcp4,
                                           &Dhcp4Mode );
            if ( EFI_ERROR ( Status )) {
              Status = HttpSendAnsiString ( SocketFD,
                                            pPort,
                                            "Failed to get DHCP4 mode" );
            }
            else {
              //
              //  Get the last packet
              //
              pDhcp4Packet = Dhcp4Mode.ReplyPacket;
              if ( NULL == pDhcp4Packet ) {
                Status = HttpSendAnsiString ( SocketFD,
                                              pPort,
                                              "No DHCP reply received!<br/>DHCP Mode:<br/>" );
                if ( EFI_ERROR ( Status )) {
                  break;
                }

                //
                //  Display the DHCP mode data
                //
                Status = HttpSendDump ( SocketFD,
                                        pPort,
                                        sizeof ( Dhcp4Mode ),
                                        (UINT8 *)&Dhcp4Mode );
              }
              else {
                //
                //  Display the DHCP packet
                //
                Status = HttpSendDump ( SocketFD,
                                        pPort,
                                        pDhcp4Packet->Length,
                                        (UINT8 *)&pDhcp4Packet->Dhcp4 );
              }
            }
/*
          }

          //
          //  Done with the DHCP protocol
          //
          pService->DestroyChild ( pService,
                                   Dhcp4Handle );
*/
        }

        //
        //  Set the next service binding
        //
        pHandle += 1;
      }
    }

    //
    //  Send the page trailer
    //
    Status = HttpPageTrailer ( SocketFD, pPort, pbDone );
    break;
  }

  //
  //  Return the operation status
  //
  DBG_EXIT_STATUS ( Status );
  return Status;
}
