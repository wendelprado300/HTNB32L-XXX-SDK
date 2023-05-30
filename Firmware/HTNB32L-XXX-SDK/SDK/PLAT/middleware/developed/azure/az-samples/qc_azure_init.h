/****************************************************************************
 *
 * Copy right:   2017-, Copyrigths of Qualcomm Ltd.
 * File name:    app.h
 * Description:  QCX212 azure demo entry header file
 * History:      Rev1.0   2018-07-12
 *
 ****************************************************************************/

#define QMSG_ID_BASE               (0x160) 
#define QMSG_ID_NW_IPV4_READY      (QMSG_ID_BASE)
#define QMSG_ID_NW_IPV6_READY      (QMSG_ID_BASE + 1)
#define QMSG_ID_NW_IPV4_6_READY    (QMSG_ID_BASE + 2)
#define QMSG_ID_NW_DISCONNECT      (QMSG_ID_BASE + 3)
#define QMSG_ID_SOCK_SENDPKG       (QMSG_ID_BASE + 4)
#define QMSG_ID_SOCK_RECVPKG       (QMSG_ID_BASE + 5)

/*
#define MQTTCLIENT_SUCCESS 0
#define MQTTCLIENT_FAILURE -1
#define MQTTCLIENT_PERSISTENCE_ERROR -2
#define MQTTCLIENT_DISCONNECTED -3
#define MQTTCLIENT_MAX_MESSAGES_INFLIGHT -4
#define MQTTCLIENT_BAD_UTF8_STRING -5
#define MQTTCLIENT_NULL_PARAMETER -6
#define MQTTCLIENT_TOPICNAME_TRUNCATED -7
#define MQTTCLIENT_BAD_STRUCTURE -8
#define MQTTCLIENT_BAD_QOS -9
#define MQTTCLIENT_PERSISTENCE_DEFAULT 0
#define MQTTCLIENT_PERSISTENCE_NONE 1
#define MQTTCLIENT_PERSISTENCE_USER 2
#define MQTTCLIENT_PERSISTENCE_ERROR -2
*/
typedef enum
{
    APP_INIT_STATE,
    APP_DEACTIVE_STATE,
    APP_IPREADY_STATE,
    APP_REPORT_STATE,
    APP_IDLE_STATE
} appRunningState_t;


void qc_init_azure(void *arg);
