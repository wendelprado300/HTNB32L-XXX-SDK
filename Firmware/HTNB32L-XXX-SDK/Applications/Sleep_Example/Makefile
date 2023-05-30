
AVAILABLE_TARGETS = qcx212_0h00
TOOLCHAIN         = GCC
BINNAME           = HTNB32L-XXX-Sleep_Example

TOP := ../..

BUILD_AT                  = n
BUILD_AT_DEBUG            = n
THIRDPARTY_ROHC_ENABLE    = n

CFLAGS_INC        +=  -I Inc

HT_USART_API_ENABLE := y
DRIVER_USART_ENABLE         = y
HT_DEFAULT_LINKER_FILE = y

HT_USART_API_ENABLE := y
HT_SPI_API_ENABLE := n
HT_I2C_API_ENABLE := n

obj-y             += Src/main.o \
                     Src/HT_BSP_Custom.o \
                     Src/HT_Sleep_Example.o
                     
include $(TOP)/SDK/PLAT/tools/scripts/Makefile.rules

CFLAGS += -DSLEEP_EXAMPLE_TEST

