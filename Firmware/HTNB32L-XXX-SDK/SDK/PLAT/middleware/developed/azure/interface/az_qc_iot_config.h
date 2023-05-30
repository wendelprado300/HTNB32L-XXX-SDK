/**
 * @file az_qc_iot_config.h
 * @brief QC IoT specific configuration file
 */

// Get from console
// =================================================
#define QC_HOST           "ssls://eastUSIoTHub-QC.azure-devices.net:8883" ///< Customer specific MQTT HOST. The same will be used for Thing Shadow
#define QC_IOT_MQTT_HOST           "eastUSIoTHub-QC.azure-devices.net" ///< Customer specific MQTT HOST. The same will be used for Thing Shadow
#define QC_IOT_MQTT_PORT               8883                            ///< default port for MQTT/S
#define QC_IOT_MQTT_CLIENT_ID         "testDevice1"                         ///< MQTT client ID should be unique for every device
#define QC_IOT_MQTT_USENAME           "eastUSIoTHub-QC.azure-devices.net/testDevice1//?api-version=2018-06-30"  // Username
#define QC_IOT_MQTT_PASSWORD          "SharedAccessSignature sr=eastUSIoTHub-QC.azure-devices.net%2Fdevices%2FtestDevice1&sig=Ya67gxdV2oXGH1GrjSyHgZxgtRWFXLQyU1zAYrn0QLk%3D&se=1667386021"  //Password
#define QC_IOT_ROOT_CA_FILENAME    "server_ca_cert.pem"             ///< Root CA file name
#define QC_IOT_CERTIFICATE_FILENAME   "device_ec_cert.pem"             ///< device signed certificate file name
#define QC_IOT_PRIVATE_KEY_FILENAME   "device_ec_key.pem"              ///< Device private key filename
#define QC_IOT_TOPIC_NAME             "devices/testDevice1/messages/events" ///< MQTT subscribe/publish Topic name
#define QC_IOT_TOPIC_MSG              "Hello"
#define QC_IOT_CONNECTION_TIMEOUT     60000   						   ///<MQTT Connection Timeout>
// =================================================

// MQTT PubSub
#define QC_IOT_MQTT_TX_BUF_LEN 1024 ///< Any time a message is sent out through the MQTT layer. The message is copied into this buffer anytime a publish is done. This will also be used in the case of Thing Shadow
#define QC_IOT_MQTT_RX_BUF_LEN 1024 ///< Any message that comes into the device should be less than this buffer size. If a received message is bigger than this buffer size the message will be dropped.
#define QC_IOT_MQTT_NUM_SUBSCRIBE_HANDLERS 5 ///< Maximum number of topic filters the MQTT client can handle at any given time. This should be increased appropriately when using Thing Shadow
