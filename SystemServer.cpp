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
#include <nuttx/config.h>
#include <utils/Log.h>
#include <utils/Looper.h>
#include <utils/String8.h>

#ifdef CONFIG_SYSTEM_WINDOW_SERVICE
#include "WindowManagerService.h"
#endif

#ifdef CONFIG_SYSTEM_PACKAGE_SERVICE
#include "pm/PackageManagerService.h"
#endif

#ifdef CONFIG_SYSTEM_ACTIVITY_SERVICE
#include "am/ActivityManagerService.h"
#include "app/ActivityManager.h"
#endif

using namespace android;
using android::binder::Status;

static int binderCallback(int /*fd*/, int /*events*/, void* /*data*/) {
    IPCThreadState::self()->handlePolledCommands();
    return 1;
}

extern "C" int main(int argc, char** argv) {
    sp<Looper> looper = Looper::prepare(0);

    int fd = -1;
    IPCThreadState::self()->setupPolling(&fd);
    if (fd < 0) {
        ALOGE("Cann't get binder fd!!!");
        return -1;
    }

    looper->addFd(fd, Looper::POLL_CALLBACK, Looper::EVENT_INPUT, binderCallback, nullptr);

    // obtain service manager
    sp<IServiceManager> sm(defaultServiceManager());
    ALOGI("systemd: defaultServiceManager(): %p", sm.get());

#ifdef CONFIG_SYSTEM_PACKAGE_SERVICE
    sp<::os::pm::PackageManagerService> pms = new ::os::pm::PackageManagerService();
    sm->addService(::os::pm::PackageManagerService::name(), pms);
#endif

#ifdef CONFIG_SYSTEM_ACTIVITY_SERVICE
    sp<::os::am::ActivityManagerService> ams = new ::os::am::ActivityManagerService();
    sm->addService(String16(::os::app::ActivityManager::name()), ams);
#endif

#ifdef CONFIG_SYSTEM_WINDOW_SERVICE
    sp<::os::wm::WindowManagerService> wms = sp<::os::wm::WindowManagerService>::make();
    sm->addService(String16(::os::wm::WindowManagerService::name()), wms);
#endif

#ifdef CONFIG_SYSTEM_ACTIVITY_SERVICE
    ams->systemReady();
#endif

    while (true) {
        looper->pollAll(-1);
    }
    return 0;
}
