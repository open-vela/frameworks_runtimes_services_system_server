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
#include <utils/Log.h>
#include <utils/String8.h>

#include "WindowManagerService.h"

using namespace android;
using android::binder::Status;

extern "C" int main(int argc, char **argv) {
    // create ProcessState
    sp<ProcessState> proc(ProcessState::self());

    // obtain service manager
    sp<IServiceManager> sm(defaultServiceManager());
    ALOGI("systemd: defaultServiceManager(): %p", sm.get());

    // TODO: for ams/wms
#ifdef CONFIG_SYSTEM_WINDOW_SERVICE
    sp<os::wm::WindowManagerService> wms = sp<os::wm::WindowManagerService>::make();
    auto status = sm->addService(String16(os::wm::WindowManagerService::name()), wms);
    if (status != 0) {
        ALOGE("Failed to add service window");
        return -1;
    }
#endif

    // start worker thread
    ProcessState::self()->startThreadPool();

    // join the main thread
    IPCThreadState::self()->joinThreadPool();
    return 0;
}
