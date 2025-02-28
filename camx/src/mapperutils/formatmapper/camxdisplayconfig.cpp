////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 Qualcomm Technologies, Inc.
// All Rights Reserved.
// Confidential and Proprietary - Qualcomm Technologies, Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   camxdisplayconfig.cpp
/// @brief  Utility functions to update Camera Launch status to display module
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "camxcommontypes.h"
#include "camxdebugprint.h"
#include "camxdefs.h"
#include "camxdisplayconfig.h"
#include "camxosutils.h"

#include "IServiceManager.h"
#include "IDisplayConfig.h"

using ::android::hardware::hidl_string;
using ::android::hardware::Return;
using ::android::sp;
using vendor::display::config::V1_9::IDisplayConfig;

CAMX_NAMESPACE_BEGIN

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Namespaces
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Class to implement displayconfig status
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DisplayConfig
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// DisplayConfig
    ///
    /// @brief  Constructor
    ///
    /// @return None
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DisplayConfig() = default;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// ~DisplayConfig
    ///
    /// @brief  Destructor
    ///
    /// @return None
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual ~DisplayConfig();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Initialize
    ///
    /// @brief  Second phase initialization of the display config class
    ///
    /// @return CamxResultSuccess if successful, Errors specified by CamxResults otherwise
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CamxResult Initialize();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// SetCameraStatus
    ///
    /// @brief  API to set Camera Launch status
    ///
    /// @param  status Camera On/Off status
    ///
    /// @return CamxResultSuccess if successful, Errors specified by CamxResults otherwise
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CamxResult SetCameraStatus(
        UINT32 status);

private:
    DisplayConfig(const DisplayConfig& rOther)              = delete;
    DisplayConfig(const DisplayConfig&& rrOther)            = delete;
    DisplayConfig& operator=(const DisplayConfig& rOther)   = delete;
    DisplayConfig& operator=(const DisplayConfig&& rrOther) = delete;

    sp<IDisplayConfig>  mDisplayConfig; ///< Display config pointer
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DisplayConfigInterface::SetCameraStatus
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CamxResult DisplayConfigInterface::SetCameraStatus(
    UINT32 status)
{
    // If Initialize is taking long time, consider to make this variable as Static
    DisplayConfig displayConfig;
    CamxResult    result = displayConfig.Initialize();

    if (CamxResultSuccess == result)
    {
        result = displayConfig.SetCameraStatus(status);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DisplayConfig::Initialize
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CamxResult DisplayConfig::Initialize()
{
    CamxResult result = CamxResultEFailed;

    // This log message will help to determine how long, getService API is taking.
    // If this is taking very long to open, then consider to make this as static.
    CAMX_LOG_VERBOSE(CamxLogGroupFormat, "IDisplayConfig getService called");

    if (mDisplayConfig == NULL)
    {
        mDisplayConfig = IDisplayConfig::getService();
    }

    if (mDisplayConfig == NULL)
    {
        CAMX_LOG_ERROR(CamxLogGroupFormat, "IDisplayConfig service is NULL, version 1.9");
    }
    else
    {
        result = CamxResultSuccess;
        CAMX_LOG_INFO(CamxLogGroupFormat, "IDisplayConfig is loaded, version 1.9");
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  DisplayConfig::~DisplayConfig
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DisplayConfig::~DisplayConfig()
{
    mDisplayConfig.clear();
    mDisplayConfig = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DisplayConfig::SetCameraStatus
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CamxResult DisplayConfig::SetCameraStatus(
    UINT32 status)
{
    CamxResult result = CamxResultSuccess;

    Return<int32_t> ret = mDisplayConfig->setCameraLaunchStatus(status);

    if (false == ret.isOk())
    {
        CAMX_LOG_ERROR(CamxLogGroupFormat, "DisplayConfig Unable to send response. Exception : %s",
                      ret.description().c_str());
        mDisplayConfig.clear();
        mDisplayConfig = NULL;
        result         = CamxResultEFailed;
    }
    else if (0 == ret)
    {
        CAMX_LOG_INFO(CamxLogGroupFormat, "setCameraLaunchStatus returned success, camera status %d",
                      status);
    }
    else
    {
        CAMX_LOG_ERROR(CamxLogGroupFormat, "setCameraLaunchStatus returned failure, camera status %d",
                       status);
    }

    return result;
}

CAMX_NAMESPACE_END
