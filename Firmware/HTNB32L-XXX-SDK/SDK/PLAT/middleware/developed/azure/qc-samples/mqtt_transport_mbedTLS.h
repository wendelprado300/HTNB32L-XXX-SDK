#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/platform.h"
#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"
#include "mbedtls/net_sockets.h"
//#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/pk.h"
#include "mbedtls/x509.h"

// MQTT reuirement with TLS
#include "mbedtls/ssl_ciphersuites.h"
#include "MQTTClient.h"
#include "MQTTConnect.h"
#include "MQTTFreeRTOS.h"
#include "MQTTPacket.h"
#include "lwip/api.h"
#include "lwip/sockets.h"



//Maximum cipherlist support
#define SUPPORT_MAX_CIPHER_LIST				8

//maximum ca list support
#define SSL_MAX_CA_LIST					8

#define SSL_PSK_MAX_LEN					32

typedef struct Network Network;

/**
 * @brief TLS Connection Parameters
 *
 * Defines a type containing TLS specific parameters to be passed down to the
 * TLS networking layer to create a TLS secured socket.
 */

typedef enum {
        NONE_ERROR = 0,                   // Success
        GENERIC_ERROR = -1,
        NULL_VALUE_ERROR = -2,
        CONNECTION_ERROR = -3,
        SUBSCRIBE_ERROR = -4,
        PUBLISH_ERROR = -5,
	DISCONNECT_ERROR = -6,
        YIELD_ERROR = -7,
	TCP_CONNECT_ERROR = -8,
	SSL_CONNECT_ERROR = -9,
        TCP_SETUP_ERROR =-10,
        SSL_CONNECT_TIMEOUT_ERROR = -11,
        SSL_WRITE_ERROR = -12,
        SSL_INIT_ERROR = -13,
        SSL_CERT_ERROR= -14,
        UNSUBSCRIBE_ERROR = -15,
        JSON_PARSE_ERROR = -16,
 	WAIT_FOR_PUBLISH = -17,
        SSL_WRITE_TIMEOUT_ERROR = -18,
        SSL_READ_TIMEOUT_ERROR = -19,
        SSL_READ_ERROR = -20,
}TLSResult;

#define SSL_CIPHERSUITE_LIST_DEPTH 8
#define SUPPORT_MAX_CIPHER_LIST    8
typedef struct TLSClientTag
{
	/* mbedtls connection information */
        mbedtls_ssl_context ssl_ctx;
	mbedtls_net_context server_fd;
        mbedtls_ssl_config conf; //  uint16_t cipher[SSL_CIPHERSUITE_LIST_DEPTH] ;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
        //mbedtls_x509_crt_profile crt_profile;
	mbedtls_x509_crt cacert;
	mbedtls_x509_crt clicert;
	mbedtls_pk_context pkey;
	uint16_t cipher[SUPPORT_MAX_CIPHER_LIST];
} TLSClient;

typedef struct TLSClientContextTag
{
        xSocket_t socket;
        int timeout_s;
        int timeout_r;
        BOOL isSSLs;
	TLSClient *ssl;
        char* pRootCALocation;                        ///< Pointer to string containing the filename (including path) of the root CA file.
        char* pDeviceCertLocation;                      ///< Pointer to string containing the filename (including path) of the device certificate.
        char* pDevicePrivateKeyLocation;        ///< Pointer to string containing the filename (including path) of the device private key file.
        char* pDestinationURL;                          ///< Pointer to string containing the endpoint of the MQTT service.
        int DestinationPort;                            ///< Integer defining the connection port of the MQTT service.
        unsigned int timeout_ms;                        ///< Unsigned integer defining the TLS handshake timeout value in milliseconds.
        unsigned char ServerVerificationFlag;   ///< Boolean.  True = perform server certificate hostname validation.  False = skip validation \b NOT recommended.
        int caCertLen;
        int clientCertLen;
        int clientPkLen;
	int tls_cipher_suites[8];
} TLSConnectParams;

typedef unsigned char tlsDataType;
int mbedTLS_tlsFetchData(void *, tlsDataType *, int); /* must return -1 for error, 0 for call again, or the number of bytes read */

TLSResult mbedTLSConn(TLSConnectParams* params, char *hostname);
int mqtt_mbedTLSConn(Network *network, TLSConnectParams* params);
TLSResult TLSConnect(TLSConnectParams* params, const char *url);
int mbedTLS_disconnect(Network *network);
int mbedTLS_read(Network *network, unsigned char * buffer, int len, int timeout_ms);
int mbedTLS_write(Network *network, unsigned char * buffer, int len, int timeout_ms);
//TLSResult mbedTLS_pollableFd(TLSClient *tls_client) { return tls_client->server_fd.fd; }
int mbedTLS_destroy(Network *network);

