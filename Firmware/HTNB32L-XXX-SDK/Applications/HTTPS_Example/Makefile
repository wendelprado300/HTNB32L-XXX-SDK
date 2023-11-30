AVAILABLE_TARGETS = qcx212_0h00
TOOLCHAIN         = GCC
PROJECT_NAME = HTTPS_Example
BINNAME           = HTNB32L-XXX-HTTPS_Example

TOP := ../..

BUILD_AT                  = n
BUILD_AT_DEBUG            = n
THIRDPARTY_HTTPC_ENABLE   = y
THIRDPARTY_MBEDTLS_ENABLE = y
MBEDTLS_WITH_CA           = y

HT_LIBRARY_HTTPS_ENABLE = y
DRIVER_USART_ENABLE = y
UART_UNILOG_ENABLE = y

CFLAGS_INC        +=  -I Inc

obj-y             += Src/main.o \
                     Src/HT_BSP_Custom.o

include $(TOP)/SDK/PLAT/tools/scripts/Makefile.rules
