#include "qc_iot_mqtt_interface.h"
#include "../az-sdk/az_iot_provisioning_client.h"

typedef struct 
{
    enum QoS qos;
    unsigned char retained;
    unsigned char dup;
    unsigned short id;
    void *payload;
    size_t payloadlen;
	int topiclen;
	char* topicdata;

}Qc_Cbk_Msg;

int Qc_Msg_Data_Copy(Qc_Cbk_Msg *msg_data,MessageData * md);
qapi_Status_t Qc_Create_Connect_to_Iot_Hub(void (*messgae_handler)());

// Device Twin API's
qapi_Status_t Qc_Get_Device_Twin_Document(void);
void Qc_Parse_Device_Twin_Message(char * topic, int topic_len, 
	void * payload, int payloadlen, az_iot_hub_client_twin_response * out_twin_response);
void Qc_Handle_Device_Twin_Message(void * payload, int payloadlen,
	az_iot_hub_client_twin_response const * twin_response);
bool Qc_Parse_Desired_Device_Count_Property(az_span message_span, 
	int32_t * out_parsed_device_count);
void Qc_Build_Reported_Property(az_span reported_property_payload, 
	az_span * out_reported_property_payload);
void Qc_Update_Device_Count_Property(int32_t device_count);
qapi_Status_t Qc_Send_Reported_Property(void);

// Direct Method API's

void Qc_Send_Method_Response(az_iot_hub_client_method_request const * request, 
	 az_iot_status status, 
	 az_span response);
az_span Qc_Invoke_Hub_ping(void);
void Qc_Handle_Method_Request(az_iot_hub_client_method_request const * method_request);
void Qc_Parse_Method_Message(char * topic, 
	int topic_len, 
	void * payload, int payloadlen, 
	az_iot_hub_client_method_request * out_method_request);

// DPS API's
qapi_Status_t Qc_Send_Operation_Query_Message(az_iot_provisioning_client_register_response const * register_response);
void Qc_Handle_Device_Registration_Status_Message(az_iot_provisioning_client_register_response const * register_response,
	 
	bool * ref_is_operation_complete);
void Qc_Parse_Device_Registration_Status_Message(char * topic, 
	int topic_len, 
	char * payload, int payloadlen,
	az_iot_provisioning_client_register_response * out_register_response);

qapi_Status_t Qc_Register_Device_With_Provisioning_Service(void);
qapi_Status_t Qc_Dev_Prov_Create_Connect_to_Iot_Hub(void(*message_handler) ());


