
#include "MQTTClient.h"
#include "MQTTConnect.h"
#include "MQTTFreeRTOS.h"
#include "MQTTPacket.h"
#include "../interface/az_qc_iot_config.h"
#include "mqtt_transport_mbedTLS.h"
#include "qc_fs.h"


/**
 * @brief MQTT Version Type
 *
 * Defining an MQTT version type.
 *
 */
typedef enum {
        MQTT_3_1 = 3,   ///< MQTT 3.1   (protocol message byte = 3)
        MQTT_3_1_1 = 4  ///< MQTT 3.1.1 (protocol message byte = 4)
} MQTT_Ver_t;

/**
 * @brief Quality of Service Type
 *
 * Defining a QoS type.
 * @note QoS 2 is \b NOT supported by the AWS IoT Service at the time of this SDK release.
 *
 */
typedef enum {
        QOS_0,  ///< QoS 0 = at most once delivery
        QOS_1,  ///< QoS 1 = at least once delivery
        QOS_2   ///< QoS 2 is NOT supported
} QoSLevel;


/**
 * @brief Last Will and Testament Definition
 *
 * Defining a type for LWT parameters.
 * @note Retained messages are \b NOT supported by the AWS IoT Service at the time of this SDK release.
 *
 */
typedef struct {
        const char *pTopicName; ///< LWT Topic
        const char *pMessage;   ///< Message to be delivered as LWT
        bool isRetained;                ///< NOT supported
        QoSLevel qos;                   ///< QoS of LWT message
} MQTTwillOptions;
extern const MQTTwillOptions MQTTwillOptionsDefault;

/**
 * @brief Disconnect Callback Handler Type
 *
 * Defining a TYPE for definition of disconnect callback function pointers.
 *
 */
typedef void (*iot_disconnect_handler)(void);

//iot_disconnect_handler clientDisconnectHandler;

#if 0
typedef void (*disconnectHander_t)(void);

void setDisconnectHandler(MQTTClient* c, disconnectHander_t disconnectHandler) {
        c->disconnectHandler = disconnectHandler;
}
#endif
//extern char *az_cacert=NULL;
//extern char *az_clicert=NULL;
//extern char *az_clikey=NULL;
//char *az_store_clicert;
/**
 * @brief MQTT Connection Parameters
 *
 * Defining a type for MQTT connection parameters.  Passed into client when establishing a connection.
 *
 */
typedef struct {
        char *pHostURL;                                         ///< Pointer to a string defining the endpoint for the MQTT service
        uint16_t port;                                          ///< MQTT service listening port
        char *pRootCALocation;                          ///< Pointer to a string defining the Root CA file (full file, not path)
        char *pDeviceCertLocation;                      ///< Pointer to a string defining the device identity certificate file (full file, not path)
        char *pDevicePrivateKeyLocation;        ///< Pointer to a string defining the device private key file (full file, not path)
        char *pClientID;                                        ///< Pointer to a string defining the MQTT client ID (this needs to be unique \b per \b device across your AWS account)
        char *pUserName;                                        ///< Not used in the AWS IoT Service
        char *pPassword;                                        ///< Not used in the AWS IoT Service
        MQTT_Ver_t MQTTVersion;                         ///< Desired MQTT version used during connection
        uint16_t KeepAliveInterval_sec;         ///< MQTT keep alive interval in seconds.  Defines inactivity time allowed before determining the connection has been lost.
        bool isCleansession;                            ///< MQTT clean session.  True = this session is to be treated as clean.  Previous server state is cleared and no stated is retained from this connection.
        bool isWillMsgPresent;                          ///< Is there a LWT associated with this connection?
        MQTTwillOptions will;                           ///< MQTT LWT parameters.
        uint32_t mqttCommandTimeout_ms;         ///< Timeout for MQTT blocking calls.  In milliseconds.
        uint32_t tlsHandshakeTimeout_ms;        ///< TLS handshake timeout.  In milliseconds.
        bool isSSLHostnameVerify;                       ///< Client should perform server certificate hostname validation.
        iot_disconnect_handler disconnectHandler;       ///< Callback to be invoked upon connection loss.
	//TLSConnectParams *pTLS;
} MQTTConnectParams;

extern const MQTTConnectParams MQTTConnectParamsDefault;

typedef struct {
        QoSLevel qos;
	bool isRetained;
        bool isDuplicate;
        uint16_t id;
        void *pPayload;
        uint32_t PayloadLen;
} MQTTMessageParams;

extern const MQTTMessageParams MQTTMessageParamsDefault;
//params.pTopicName;
//params.MessageParams.pPayload;
typedef struct {
        char *pTopicName;
        uint16_t TopicNameLen;
        MQTTMessageParams MessageParams;
} MQTTCallbackParams;

extern const MQTTCallbackParams MQTTCallbackParamsDefault;

typedef int32_t (*iot_message_handler)(MQTTCallbackParams params);

typedef struct {
        char *pTopic;
        QoSLevel qos;
        iot_message_handler mHandler;
} MQTTSubscribeParams;

extern const MQTTSubscribeParams MQTTSubscribeParamsDefault;

typedef struct {
        char *pTopic; 
        MQTTMessageParams MessageParams;
} MQTTPublishParams;
extern const MQTTPublishParams MQTTPublishParamsDefault;

int qc_iot_mqtt_connect_azure_hub(MQTTConnectParams *pParams);
int qc_iot_mqtt_publish(MQTTPublishParams *pParams);
int qc_iot_mqtt_subscribe(MQTTSubscribeParams *pParams);
int qc_iot_mqtt_unsubscribe(char *pTopic);
int qc_iot_mqtt_disconnect(void);
int qc_iot_mqtt_yield(int timeout);
bool qc_iot_is_mqtt_connected(void);

typedef int (*pConnectFunc_t)(MQTTConnectParams *pParams);
typedef int (*pPublishFunc_t)(MQTTPublishParams *pParams);
typedef int (*pSubscribeFunc_t)(MQTTSubscribeParams *pParams);
typedef int (*pUnsubscribeFunc_t)(char *pTopic);
typedef int (*pDisconnectFunc_t)(void);
typedef int (*pYieldFunc_t)(int timeout);
typedef bool (*pIsConnectedFunc_t)(void);

typedef struct{
        pConnectFunc_t mqtt_connect;
        pPublishFunc_t mqtt_publish;
        pSubscribeFunc_t mqtt_subscribe;
        pUnsubscribeFunc_t mqtt_unsubscribe;
        pDisconnectFunc_t mqtt_disconnect;
        pYieldFunc_t mqtt_yield;
        pIsConnectedFunc_t mqtt_isConnected;
}MQTTClient_t;

void qc_iot_mqtt_init(MQTTClient_t *pClient);


