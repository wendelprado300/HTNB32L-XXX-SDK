
ifeq ($(HT_LIBRARY_MQTT_ENABLE),y)

MQTT_DIR      := $(TOP)/SDK/Thirdparty/MQTT

CFLAGS_INC    += -I $(TOP)/SDK/PLAT/os/freertos/inc \
                  -I $(TOP)/SDK/PLAT/os/freertos/CMSIS/inc \
                  -I $(TOP)/SDK/PLAT/middleware/thirdparty/cjson \
                  -I $(MQTT_DIR)/MQTTPacket/Inc \
                  -I $(MQTT_DIR)/FreeRTOS/Inc \
                  -I $(MQTT_DIR)/MQTTClient/Inc \
                  -I $(TOP)/SDK/PLAT/os/freertos/portable/gcc

CFLAGS += -DFEATURE_MQTT_ENABLE

ht_thirdparty_api-y += $(MQTT_DIR)/MQTTPacket/Src/MQTTConnectClient.o \
						$(MQTT_DIR)/MQTTPacket/Src/MQTTDeserializePublish.o \
						$(MQTT_DIR)/MQTTPacket/Src/MQTTPacket.o \
						$(MQTT_DIR)/MQTTPacket/Src/MQTTSubscribeClient.o \
						$(MQTT_DIR)/MQTTPacket/Src/MQTTUnsubscribeClient.o \
						$(MQTT_DIR)/MQTTPacket/Src/MQTTConnectServer.o \
						$(MQTT_DIR)/MQTTPacket/Src/MQTTFormat.o \
						$(MQTT_DIR)/MQTTPacket/Src/MQTTSerializePublish.o \
						$(MQTT_DIR)/MQTTPacket/Src/MQTTSubscribeServer.o \
						$(MQTT_DIR)/MQTTPacket/Src/MQTTUnsubscribeServer.o \
						$(MQTT_DIR)/FreeRTOS/Src/MQTTFreeRTOS.o \
						$(MQTT_DIR)/MQTTClient/Src/HT_MQTT_Tls.o \
						$(MQTT_DIR)/MQTTClient/Src/MQTTClient.o

endif