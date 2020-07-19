/***************************************************************************//**
 * @file  app.h
 * @brief Application header file
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef APP_H
#define APP_H

#include <gecko_configuration.h>

extern uint16_t Unicast;

//Model IDs
#define GENERIC_LEVEL_SERVER 0x1002
#define GENERIC_LEVEL_CLIENT 0x1003

//Group Addresses for models
#define CLIENT_PUB_ADD 0xC005
#define CLIENT_SUB_ADD 0xC010
#define SERVER_PUB_ADD 0xC010
#define SERVER_SUB_ADD 0xC005

//Message Info
#define ELEM_0 0x0000
#define APP_KEY_INDEX_0 0x0000
#define TRANS 0
#define CLIENT_FLAGS 0
#define MESH_GENERIC_CLIENT_request_level 2
#define DATA_LENGHT_CLIENT 2
#define DATA_LENGHT_SERVER 4
#define DELAY 0
#define SERVER_FLAGS 0xFFFFFFFF


/***************************************************************************//**
 * @defgroup app Application Code
 * @brief Sample Application Implementation
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Application
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup app
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * Main application code.
 * @param[in] pConfig  Pointer to stack configuration.
 ******************************************************************************/
void appMain(gecko_configuration_t *pConfig);

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */

#endif /* APP_H */
