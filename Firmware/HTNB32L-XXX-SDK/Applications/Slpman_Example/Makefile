
AVAILABLE_TARGETS = qcx212_0h00
TOOLCHAIN         = GCC
BINNAME           = HTNB32L-XXX-Slpman_Example
PROJECT_NAME = Slpman_Example

TOP := ../..

BUILD_AT                  = n
BUILD_AT_DEBUG            = n
THIRDPARTY_ERCOAP_ENABLE   = y
THIRDPARTY_LIBCOAP_ENABLE  = n
THIRDPARTY_MQTT_ENABLE     = n
THIRDPARTY_HTTPC_ENABLE    = n
THIRDPARTY_MBEDTLS_ENABLE  = n
THIRDPARTY_TINYDTLS_ENABLE = n
BUILD_PS = y

BUILD_DRIVER_EXAMPLE = n
BUILD_DRIVER_LIB = n

BUILD_STATIC_LIBRARY = n
BUILD_MODULAR_LIBRARY := y

HT_DEFAULT_LINKER_FILE = y
DRIVER_USART_ENABLE	   = y
HT_USART_API_ENABLE := y
HT_LIBRARY_SLPMAN_ENABLE := y
UART_UNILOG_ENABLE = y

CFLAGS_INC        +=  -I Inc

obj-y             += Src/main.o \
                     Src/HT_BSP_Custom.o

include $(TOP)/Applications/$(PROJECT_NAME)/MakeProj.rules                    
include $(TOP)/SDK/PLAT/tools/scripts/Makefile.rules
