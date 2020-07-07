/*
 * Copyright (C) 2020 The LineageOS Project
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

#define LOG_TAG "PowerShareService"

#include "PowerShare.h"
#include <hidl/HidlTransportSupport.h>
#include <fstream>

namespace vendor {
namespace lineage {
namespace powershare {
namespace V1_0 {
namespace implementation {

#define WIRELESS_TX_ENABLE_PATH "/sys/devices/platform/soc/888000.i2c/i2c-5/5-0061/power_supply/rx1619/reverse_chg_mode"
#define WIRELESS_TX_ENABLE_PATH1   "/sys/devices/platform/soc/888000.i2c/i2c-5/5-003b/power_supply/idt/reverse_chg_mode"
#define WIRELESS_TX_DISABLE_PATH "/sys/rx1619/txon"

/*
 * Write value to path and close file.
 */
static void set(std::string path, std::string value) {
    std::ofstream file(path);

    if (!file.is_open()) {
        ALOGW("failed to write %s to %s", value.c_str(), path.c_str());
        return;
    }

    file << value;
}

template <typename T>
static T get(const std::string& path, const T& def) {
    std::ifstream file(path);
    T result;

    file >> result;
    return file.fail() ? def : result;
}

Return<bool> PowerShare::isEnabled() {
    return get<std::string>(WIRELESS_TX_ENABLE_PATH, std::to_string(0)) != std::to_string(0) || get<std::string>(WIRELESS_TX_ENABLE_PATH1, std::to_string(0)) != std::to_string(0) ;
}

Return<bool> PowerShare::setEnabled(bool enable) {
    if(enable){
    set(WIRELESS_TX_ENABLE_PATH, std::to_string(1));
    set(WIRELESS_TX_ENABLE_PATH1, std::to_string(1));
    }else{
        set(WIRELESS_TX_DISABLE_PATH, std::to_string(0));
    }
    return isEnabled();
}

Return<uint32_t> PowerShare::getMinBattery() {
    return 0;
}

Return<uint32_t> PowerShare::setMinBattery(uint32_t) {
    return getMinBattery();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace powershare
}  // namespace lineage
}  // namespace vendor
