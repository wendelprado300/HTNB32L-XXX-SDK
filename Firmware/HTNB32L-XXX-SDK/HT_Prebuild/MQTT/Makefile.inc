
MQTT_DIR      := $(TOP)/PLAT/HT_Libraries/MQTT

CFLAGS_INC    += -I $(TOP)/PLAT/os/freertos/inc               \
                  -I $(TOP)/PLAT/os/freertos/CMSIS/inc        \
                  -I $(TOP)/PLAT/middleware/thirdparty/cjson  \
                  -I $(MQTT_DIR)/MQTTPacket/Inc               \
                  -I $(MQTT_DIR)/FreeRTOS/Inc    \
                  -I $(MQTT_DIR)/MQTTClient/Inc \
                  -I $(TOP)/PLAT/os/freertos/portable/gcc

CFLAGS += -DFEATURE_MQTT_ENABLE

MQTT_SRC_DIRS += $(MQTT_DIR)/MQTTPacket/Src            \
                 $(MQTT_DIR)/FreeRTOS/Src              \
                 $(MQTT_DIR)/MQTTClient/Src

MQTT_EXCLUDE_FILES :=

MQTT_CSRC = $(foreach dir, $(MQTT_SRC_DIRS), $(wildcard $(dir)/*.c))
MQTT_CFILES = $(filter-out $(MQTT_EXCLUDE_FILES), $(MQTT_CSRC))
MQTT_COBJSTEMP := $(patsubst %.c, %.o, $(MQTT_CFILES))
MQTT_COBJSTEMP :=$(subst $(MQTT_DIR),PLAT/HT_Libraries/MQTT,$(MQTT_COBJSTEMP))
MQTT_COBJS := $(addprefix $(BUILDDIR)/, $(MQTT_COBJSTEMP))

-include $(MQTT_COBJS:.o=.d)

ifeq ($(MQTT_EXAMPLE),y)
CFLAGS += -DMQTT_EXAMPLE
endif

ht_modular_lib-y += libHTMqtt.a

$(BUILDDIR)/lib/libHTMqtt.a: $(MQTT_COBJS)
	@mkdir -p $(dir $@)
	$(ECHO) AR $@
	$(Q)$(AR) -cr $@ $^
