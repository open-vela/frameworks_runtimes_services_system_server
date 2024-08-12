/*
 * Copyright (C) 2023 Xiaomi Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "systemd"
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <nuttx/arch.h>
#include <nuttx/config.h>
#include <utils/Log.h>
#include <utils/Looper.h>
#include <utils/String8.h>
#include <uv.h>

#ifdef CONFIG_SYSTEM_WINDOW_SERVICE
#include "wm/WMService.h"
#endif

#ifdef CONFIG_SYSTEM_PACKAGE_SERVICE
#include "pm/PackageManagerService.h"
#endif

#ifdef CONFIG_SYSTEM_ACTIVITY_SERVICE
#include "am/ActivityManagerService.h"
#include "app/ActivityManager.h"
#endif

#ifdef CONFIG_SYSTEM_BRIGHTNESS_SERVICE
#include <BrightnessService.h>
#endif

using namespace android;
using android::binder::Status;

static void binderPollCallback(uv_poll_t* /*handle*/, int /*status*/, int /*events*/) {
    IPCThreadState::self()->handlePolledCommands();
}

extern "C" int main(int argc, char** argv) {
#ifdef CONFIG_SYSTEM_SERVER_USE_PROFILER
    up_perf_init((void*)up_perf_getfreq());
#endif

    uv_loop_t uvLooper;
    uv_poll_t binderPoll;
    int binderFd = -1;

    uv_loop_init(&uvLooper);
    IPCThreadState::self()->setupPolling(&binderFd);
    if (binderFd < 0) {
        ALOGE("Cann't get binder fd!!!");
        uv_loop_close(&uvLooper);
        return -1;
    }
    uv_poll_init(&uvLooper, &binderPoll, binderFd);
    uv_poll_start(&binderPoll, UV_READABLE, binderPollCallback);

    std::shared_ptr<::os::app::UvLoop> mainLooper = std::make_shared<::os::app::UvLoop>(&uvLooper);

    // obtain service manager
    sp<IServiceManager> sm(defaultServiceManager());
    ALOGI("systemd: defaultServiceManager(): %p", sm.get());

#ifdef CONFIG_SYSTEM_WINDOW_SERVICE
    sp<::os::wm::IWindowManager> wms = startWMService(sm, mainLooper);
    if (!wms) {
        ALOGE("Failed to start window manager service");
        uv_poll_stop(&binderPoll);
        uv_loop_close(mainLooper->get());
        return -1;
    }
#endif

#ifdef CONFIG_SYSTEM_BRIGHTNESS_SERVICE
    sp<::os::brightness::BrightnessService> brightness =
            sp<::os::brightness::BrightnessService>::make(&uvLooper);
    sm->addService(::os::brightness::BrightnessService::name(), brightness);
#endif

#ifdef CONFIG_SYSTEM_PACKAGE_SERVICE
    sp<::os::pm::PackageManagerService> pms = new ::os::pm::PackageManagerService();
    sm->addService(::os::pm::PackageManagerService::name(), pms);
#endif

#ifdef CONFIG_SYSTEM_ACTIVITY_SERVICE
    sp<::os::am::ActivityManagerService> ams = new ::os::am::ActivityManagerService(&uvLooper);
    sm->addService(String16(::os::app::ActivityManager::name()), ams);
#endif

#ifdef CONFIG_SYSTEM_ACTIVITY_SERVICE
    ams->setWindowManager(wms);
    ams->systemReady();
#endif

    uv_run(&uvLooper, UV_RUN_DEFAULT);
    return 0;
}
