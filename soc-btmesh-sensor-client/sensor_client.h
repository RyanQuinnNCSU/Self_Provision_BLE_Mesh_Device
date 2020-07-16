/***************************************************************************//**
 * @file  sensor_client.h
 * @brief Sensor client header file
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

#ifndef SENSOR_CLIENT_H
#define SENSOR_CLIENT_H

/***************************************************************************//**
 * @defgroup SensorClient Sensor Client Module
 * @brief Sensor Clinet Module Implementation
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup SensorClient
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * It changes currently displayed property ID.
 ******************************************************************************/
void sensor_client_change_property(void);

/***************************************************************************//**
 * Publishing of sensor client get descriptor request for currently displayed
 * property id. It also resets the registered devices counter.
 ******************************************************************************/
void sensor_client_publish_get_descriptor_request(void);

/***************************************************************************//**
 * Publishing of sensor client get request for currently displayed property id.
 ******************************************************************************/
void sensor_client_publish_get_request(void);

/***************************************************************************//**
 * Handling of mesh sensor server events.
 * It handles:
 *  - sensor_client_descriptor_status
 *  - sensor_client_status
 *
 * @param[in] pEvt  Pointer to incoming sensor server event.
 ******************************************************************************/
void handle_sensor_client_events(struct gecko_cmd_packet *pEvt);

/** @} (end addtogroup Sensor) */

#endif /* SENSOR_CLIENT_H */
