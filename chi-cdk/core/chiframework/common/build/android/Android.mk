ifeq ($(CAMX_CHICDK_CORE_PATH),)
LOCAL_PATH := $(abspath $(call my-dir)/../../..)
CAMX_CHICDK_CORE_PATH := $(abspath $(LOCAL_PATH)/..)
else
LOCAL_PATH := $(CAMX_CHICDK_CORE_PATH)/chiframework
endif

include $(CLEAR_VARS)


# Module supports function call tracing via ENABLE_FUNCTION_CALL_TRACE
# Required before including common.mk
SUPPORT_FUNCTION_CALL_TRACE := 1

# Get definitions common to the CAMX project here
include $(CAMX_CHICDK_PATH)/core/build/android/common.mk

# UsecaseComposer definitions
COMPOSER_INPUT         := $(CAMX_CHICDK_TOPOLOGY_PATH)/$(TARGET_BOARD_PLATFORM)/$(TARGET_BOARD_PLATFORM)_usecase.xml
COMPOSER_OUTPUT        := $(CAMX_CHICDK_TOPOLOGY_PATH)/$(TARGET_BOARD_PLATFORM)/g_$(TARGET_BOARD_PLATFORM)_usecase.xml
USECASE_COMPONENTS_DIR := $(CAMX_CHICDK_TOPOLOGY_PATH)/usecase-components/
COPYRIGHT_HEADER       := $(CAMX_CHICDK_PATH)/tools/usecaseconverter/xmlheader.txt
COMPOSER_CMD           := python $(CAMX_CHICDK_PATH)/tools/usecaseconverter/usecasecomposer.py -i $(COMPOSER_INPUT) -o $(COMPOSER_OUTPUT) -t $(TARGET_BOARD_PLATFORM) -d $(USECASE_COMPONENTS_DIR) -H $(COPYRIGHT_HEADER)

# Run UsecaseComposer to generate g_<target>_usecase.xml
COMPOSER_STATUS := $(shell $(COMPOSER_CMD)|| echo [UsecaseComposerFailure])
$(info $(COMPOSER_STATUS))
ifeq ($(lastword $(COMPOSER_STATUS)),[UsecaseComposerFailure])
    # Stop the build when error status code detected
    $(error $(COMPOSER_OUTPUT) was not generated)
endif

LOCAL_INC_FILES :=                      \
    chxextensionmodule.h                \
    chxfeature.h                        \
    chxmulticamcontroller.h             \
    chxmultifovzoomratiocontroller.h    \
    chxpipeline.h                       \
    chxsensorselectmode.h               \
    chxsession.h                        \
    chxusecase.h                        \
    chxzoomtranslator.h

LOCAL_SRC_FILES :=                                \
    chxextensioninterface.cpp                     \
    chxextensionmodule.cpp                        \
    chxfeature.cpp                                \
    chxmulticamcontroller.cpp                     \
    chxmultifovzoomratiocontroller.cpp            \
    chxpipeline.cpp                               \
    chxsensorselectmode.cpp                       \
    chxsession.cpp                                \
    chxusecase.cpp                                \
    chxzoomtranslator.cpp

LOCAL_C_LIBS := $(CAMX_C_LIBS)

LOCAL_C_INCLUDES :=                                                    \
    $(CAMX_C_INCLUDES)                                                 \
    $(CAMX_CHICDK_PATH)/oem/qcom/feature2/chifeature2graphselector

# Compiler flags
LOCAL_CFLAGS := $(CAMX_CFLAGS)
LOCAL_CPPFLAGS := $(CAMX_CPPFLAGS)

LOCAL_SHARED_LIBRARIES :=                                   \
    libchilog                                               \
    libcutils                                               \
    liblog                                                  \
    libsync                                                 \
    vendor.qti.hardware.camera.postproc@1.0-service-impl    \

LOCAL_LDFLAGS :=
LOCAL_LDLIBS  :=

LOCAL_MODULE  := libchiframework

include $(BUILD_STATIC_LIBRARY)

-include $(CAMX_CHECK_WHINER)