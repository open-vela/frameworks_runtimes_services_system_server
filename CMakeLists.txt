#
# Copyright (C) 2024 Xiaomi Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.
#

if(CONFIG_SYSTEM_SERVER)
  set(CUR_DEPENDS android_binder)

  if(CONFIG_SYSTEM_WINDOW_SERVICE)
    list(APPEND CUR_DEPENDS window_manager)
  endif()

  if(CONFIG_SYSTEM_PACKAGE_SERVICE)
    list(APPEND CUR_DEPENDS package_manager)
  endif()

  if(CONFIG_SYSTEM_ACTIVITY_SERVICE)
    list(APPEND CUR_DEPENDS activity_manager)
  endif()

  nuttx_add_application(
    NAME
    systemd
    STACKSIZE
    ${CONFIG_SYSTEM_SERVER_STACKSIZE}
    PRIORITY
    ${CONFIG_SYSTEM_SERVER_PRIORITY}
    SRCS
    SystemServer.cpp
    DEPENDS
    ${CUR_DEPENDS})
endif()
