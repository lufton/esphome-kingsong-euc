#pragma once
#include <cstdint>

namespace esphome {
namespace kingsong_euc {

static const uint16_t DELAY_BEFORE_REQUEST = 100;
static const char *const TAG = "kingsong_euc";
static const uint16_t SERVICE_UUID = 0xFFE0;
static const uint16_t CHARACTERISTIC_UUID = 0xFFE1;

}  // namespace kingsong_euc
}  // namespace esphome
