
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <MQTTClient.h>

#if !							defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"

#else

#include MBEDTLS_CONFIG_FILE
#endif


#include "qc_iot_sample_common.h"
#include "../qc-samples/mqtt_transport.h"

#include "../az-sdk/az_result.h"
#include "../az-sdk/az_span.h"
#include "../az-sdk/az_iot_hub_client.h"
#include "../az-sdk/az_json.h"


#define MAX_TELEMETRY_MESSAGE_COUNT 2
#define MQTT_TIMEOUT_DISCONNECT_MS (10 * 1000)

#define SAMPLE_TYPE 			PAHO_IOT_HUB
#define SAMPLE_NAME 			PAHO_IOT_HUB_TELEMETRY_SAMPLE
#define GETLOWER4BYTES			0x0FFFFFFFF


typedef enum {
	QAPI_FAILURE = -14, 
	QAPI_CLIENT_NOT_CONNECTED, 
	QAPI_TLSCONNECTION_FAILED, 
	QAPI_MQTTCONNECTION_FAILED, 
	QAPI_SENDPUBLISH_FAILED, 
	QAPI_SENDSUBSCRIBE_FAILED, 
	QAPI_SENDUNSUBSCRIBE_FAILED, 
	QAPI_DISCONNECT_FAILED, 
	QAPI_RECEIVEMESSAGE_FAILED, 
	QAPI_CLIENT_NOT_FOUND, 
	QAPI_REMOVE_HDLR_FAILED, 
	QAPI_MAX_CONN_REACHED, 
	QAPI_NO_MEMORY, 
	QAPI_STARTTASK_FAILED, 
	QAPI_SUCCESS = 0
} qapi_Status_t;


typedef struct QcClientConnect {
	xSocket_t		socket;
	int 			timeout_s;
	int 			timeout_r;
	BOOL			isSSLs;
	char *			pRootCALocationPath;			///< Pointer to string containing the filename (including path) of the root CA file.
	char *			pDeviceCertLocationPath;		///< Pointer to string containing the filename (including path) of the device certificate.
	char *			pDevicePrivateKeyLocationPath;	///< Pointer to string containing the filename (including path) of the device private key file.
	char *			pDestinationURL;				///< Pointer to string containing the endpoint of the MQTT service.
	int 			DestinationPort;				///< Integer defining the connection port of the MQTT service.
	unsigned int	timeout_ms; 					///< Unsigned integer defining the TLS handshake timeout value in milliseconds.
	unsigned char	ServerVerificationFlag; 		///< Boolean.  True = perform server certificate hostname validation.  False = skip validation \b NOT recommended.
	int 			caCertLen;
	int 			clientCertLen;
	int 			clientPkLen;
	int 			tls_cipher_suites[8];
} QcIotNetConnect;


typedef struct {
	/** The eyecatcher for this structure.	must be MQTC. */
	char			struct_id[4];

	/** The version number of this structure.  Must be 0 */
	int 			struct_version;

	/** Version of MQTT to be used.  3 = 3.1 4 = 3.1.1
	  */
	unsigned char	MQTTVersion;
	MQTTString		clientID;
	unsigned short	keepAliveInterval;
	unsigned char	cleansession;
	unsigned char	willFlag;
	MQTTPacket_willOptions will;
	MQTTString		username;
	MQTTString		password;
} QcIotNetPacket_connectData;




#define QcIot_connectData_initializer MQTTPacket_connectData_initializer

typedef struct {
	char			struct_id[4];
	int 			struct_version;
	char *			topicName;
	char *			message;
	unsigned char	retained;
	char			qos;
} QcIot_Mqtt_willOptions;


typedef struct {
	char			struct_id[4];
	int 			struct_version;
	unsigned char	MQTTVersion;
	char *			clienID;						//MQTTString clientID;
	unsigned short	keepAliveInterval;
	unsigned char	cleansession;
	unsigned char	willFlag;
	QcIot_Mqtt_willOptions will;					//MQTTPacket_willOptions will;
	char *			username;						//MQTTString username;
	char *			password;						//MQTTString password;
} QcIotPkt_ConnectData;


typedef struct {
	QcIotNetConnect netparams;
	QcIotPkt_ConnectData connectparams;
	QcIot_Mqtt_willOptions willoptions;
} IotConfigParams;


typedef struct {
	int 			app_id; 						//context_id
	MQTTClient *	mqtt_client;
	void(*callback) ();
	int 			sock_id;
} qapi_iot_app_context_t;


typedef struct {
	enum QoS qos;
	unsigned char	retained;
	unsigned char	dup;
	unsigned short	id;
	void *			payload;
	size_t			payloadlen;
} qapi_publish;


#define MAX_NUM_CLIENTS 		1

extern int mqtt_mbedTLSConn(Network * qc_network, TLSConnectParams * params);
qapi_Status_t qapi_Net_MQTT_New(int * app_id);
MQTTClient * qapi_get_mqtt_handler(int app_id);
qapi_Status_t qapi_remove_app_handler(int app_id);
qapi_Status_t qapi_Create_Configure_MQTT_Client(int app_id, QcIotNetConnect * connectparams);
qapi_Status_t qapi_Net_MQTT_Connect_Client_to_Iot_Hub(int app_id, QcIotNetPacket_connectData Iotconnectoptions,
	 void(*default_message_handler) ());
qapi_Status_t qapi_Net_MQTT_Publish(int app_id, char * topic, qapi_publish message);
qapi_Status_t qapi_Net_MQTT_Subscribe(int app_id, void(*message_handler) (), char * prov_subTopic, int QOS);
qapi_Status_t qapi_Net_MQTT_Unsubscribe(int app_id, char * topic);
qapi_Status_t qapi_Net_MQTT_Disconnect(int app_id);
qapi_Status_t qapi_Net_MQTT_Error_Handling(int app_id);
int qapi_Is_Connected(int app_id);








