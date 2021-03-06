/*
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 */

#ifndef _OS_MODULE_H_
#define _OS_MODULE_H_

#include "artik_error.h"

artik_error os_get_api_version(artik_api_version *version);
artik_module_ops os_request_api_module(const char *name);
artik_error os_release_api_module(const artik_module_ops module);
int os_get_platform(void);
artik_error os_get_platform_name(char *name);
artik_error os_get_available_modules(artik_api_module **modules,
					int *num_modules);
bool os_is_module_available(artik_module_id_t id);
char *os_get_device_info(void);

artik_error os_get_bt_mac_address(char *addr);
artik_error os_get_wifi_mac_address(char *addr);
artik_error os_get_platform_serial_number(char *sn);
artik_error os_get_platform_manufacturer(char *manu);
artik_error os_get_platform_uptime(int64_t *uptime);
artik_error os_get_platform_model_number(char *modelnum);
#endif /* _OS_MODULE_H_ */
