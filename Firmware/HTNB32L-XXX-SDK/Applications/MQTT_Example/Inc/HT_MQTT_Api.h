/*

  _    _ _______   __  __ _____ _____ _____   ____  _   _
 | |  | |__   __| |  \/  |_   _/ ____|  __ \ / __ \| \ | |
 | |__| |  | |    | \  / | | || |    | |__) | |  | |  \| |
 |  __  |  | |    | |\/| | | || |    |  _  /| |  | | . ` |
 | |  | |  | |    | |  | |_| || |____| | \ \| |__| | |\  |
 |_|  |_|  |_|    |_|  |_|_____\_____|_|  \_\\____/|_| \_|
 =================== Advanced R&D ========================

 Copyright (c) 2023 HT Micron Semicondutores S.A.
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 http://www.apache.org/licenses/LICENSE-2.0
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

*/

/*!
 * \file HT_MQTT_Api.h
 * \brief MQTT API that implements all MQTT features used in this example. 
  * \author HT Micron Advanced R&D,
 *         Hêndrick Bataglin Gonçalves, Christian Roberto Lehmen,  Matheus da Silva Zorzeto, Felipe Kalinski Ferreira,
 *         Leandro Borges, Mauricio Carlotto Ribeiro, Henrique Kuhn, Cleber Haack, Eduardo Mendel
 *         Gleiser Alvarez Arrojo
 * 
 * \link https://github.com/htmicron
 * \version 0.1
 * \date February 23, 2023
 */

#ifndef __HT_MQTT_API_H__
#define __HT_MQTT_API_H__

#include "stdint.h"
#include "main.h"
#include "MQTTClient.h"
#include "uart_qcx212.h"

#define MQTT_TLS_ENABLE 0

#define MQTT_GENERAL_TIMEOUT 60000

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn uint8_t HT_MQTT_Connect(MQTTClient *mqtt_client, Network *mqtt_network, char *addr, int32_t port, uint32_t send_timeout, uint32_t rcv_timeout, char *clientID, 
                                        char *username, char *password, uint8_t mqtt_version, uint32_t keep_alive_interval)
 * \brief Connect device to a MQTT broker.
 *
 * \param[in] MQTTClient *mqtt_client           MQTT client handle.
 * \param[in] Network *mqtt_network             Network handle.
 * \param[in] char *addr                        MQTT broker host.          
 * \param[in] int32_t port                      MQTT port.
 * \param[in] uint32_t send_timeout             TX timeout.
 * \param[in] int32_t rcv_timeout               RX timeout.
 * \param[in] char *clientID                    MQTT client ID.
 * \param[in] char *username                    MQTT username of this device.
 * \param[in] char *password                    Password to access MQTT topic.
 * \param[in] uint8_t mqtt_version              MQTT version.
 * \param[in] uint32_t keep_alive_interval      MQTT keep alive interval.
 * \param[in] uint32_t sendbuf                  Buffer allocated for TX process.
 * \param[in] uint32_t sendbuf_size             Size of TX buffer.
 * \param[in] uint32_t readbuf                  Buffer allocated for RX process.
 * \param[in] uint32_t readbuf_size             Size of RX buffer.
 * 
 * \retval none
 *******************************************************************/
uint8_t HT_MQTT_Connect(MQTTClient *mqtt_client, Network *mqtt_network, char *addr, int32_t port, uint32_t send_timeout, uint32_t rcv_timeout, char *clientID, 
                                        char *username, char *password, uint8_t mqtt_version, uint32_t keep_alive_interval, uint8_t *sendbuf, 
                                        uint32_t sendbuf_size, uint8_t *readbuf, uint32_t readbuf_size);

/*!******************************************************************
 * \fn void HT_MQTT_Publish(MQTTClient *mqtt_client, char *topic, uint8_t *payload, uint32_t len, enum QoS qos, uint8_t retained, uint16_t id, uint8_t dup)

 * \brief Send an MQTT publish packet and wait for all acks, depending on the QoSs option.
 *
 * \param[in] MQTTClient *mqtt_client           MQTT client handle.
 * \param[in] char *topic                       MQTT topic publish to.
 * \param[in] uint8_t *payload                  Payload that will be sent to the topic.          
 * \param[in] uint32_t len                      Payload length.
 * \param[in] enum QoS qos                      QoS option.
 * \param[in] uint8_t retained                  Retained messages option.
 * \param[in] uint16_t id                       Message ID.
 * \param[in] uint8_t dup                       DUP flag.
 * 
 * 
 * \retval none
 *******************************************************************/
void HT_MQTT_Publish(MQTTClient *mqtt_client, char *topic, uint8_t *payload, uint32_t len, enum QoS qos, uint8_t retained, uint16_t id, uint8_t dup);

/*!******************************************************************
 * \fn void HT_MQTT_SubscribeCallback(MessageData *msg)

 * \brief MQTT subscribe callback function.
 *
 * \param[in] MessageData *msg                  Message received from subscribe.
 * 
 * \retval none
 *******************************************************************/
void HT_MQTT_SubscribeCallback(MessageData *msg);

/*!******************************************************************
 * \fn void HT_MQTT_Subscribe(MQTTClient *mqtt_client, char *topic, enum QoS qos)

 * \brief Subscribe a MQTT topic.
 *
 * \param[in] MQTTClient *mqtt_client           MQTT client handle.
 * \param[in] char *topic                       MQTT topic to subscribe to.
 * \param[in] enum QoS qos                      QoS option.
 *  
 * \retval none
 *******************************************************************/
void HT_MQTT_Subscribe(MQTTClient *mqtt_client, char *topic, enum QoS qos);

#endif /* __HT_MQTT_API_H__ */

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
