QCAZURE_DIR  := $(TOP)/SDK/PLAT/middleware/developed/azure

CFLAGS += -DAZURE_IOT_ENABLE

QCAZURE_SRC_DIRS += $(QCAZURE_DIR)/az-sdk
QCAZURE_SRC_DIRS += $(QCAZURE_DIR)/az-samples
QCAZURE_SRC_DIRS += $(QCAZURE_DIR)/interface
QCAZURE_SRC_DIRS += $(QCAZURE_DIR)/qc-samples

QCAZURE_EXCLUDE_FILES := 
QCAZURE_CSRC = $(foreach dir, $(QCAZURE_SRC_DIRS), $(wildcard $(dir)/*.c))
QCAZURE_CFILES = $(filter-out $(QCAZURE_EXCLUDE_FILES), $(QCAZURE_CSRC))
QCAZURE_COBJSTEMP := $(patsubst %.c, %.o, $(QCAZURE_CFILES))
QCAZURE_COBJSTEMP :=$(subst $(QCAZURE_DIR),PLAT/middleware/developed/azure,$(QCAZURE_COBJSTEMP))
QCAZURE_COBJS := $(addprefix $(BUILDDIR)/, $(QCAZURE_COBJSTEMP))

-include $(QCAZURE_COBJS:.o=.d)

#$(ECHO) Objects QCAZURE_COBJS

ifeq ($(TOOLCHAIN),GCC)

lib-y += libqcazure.a

$(BUILDDIR)/lib/libqcazure.a: $(QCAZURE_COBJS)
	@mkdir -p $(dir $@)
	$(ECHO) AR $@
	$(Q)$(AR) -cr $@ $^

endif

ifeq ($(TOOLCHAIN),ARMCC)

lib-y += libqcazure.lib

$(BUILDDIR)/lib/libqcazure.lib: $(QCAZURE_COBJS)
	@mkdir -p $(dir $@)
	$(ECHO) AR $@
	$(Q)$(AR) $(ARFLAGS) $@ $^

endif

