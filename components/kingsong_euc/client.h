#pragma once

#include "esphome/components/ble_client/ble_client.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "codec.h"

namespace esphome {
namespace kingsong_euc {

#define SEND(name) \
 public: \
  void name() { \
    ESP_LOGV(TAG, "Sending request " #name); \
    this->send_request_(this->codec_.get()->get_##name##_request()); \
  }

#define SEND_BOOL_PARAM(name, value) \
 public: \
  void name(bool value) { \
    ESP_LOGV(TAG, "Sending request " #name); \
    this->send_request_(this->codec_.get()->get_##name##_request(value)); \
  }

#define SEND_INT_PARAM(name, value) \
 public: \
  void name(uint16_t value) { \
    ESP_LOGV(TAG, "Sending request " #name); \
    this->send_request_(this->codec_.get()->get_##name##_request(value)); \
  }

class KingSongEUCClient : public ble_client::BLEClientNode {
 public:
  KingSongEUCCodec *get_codec();
  void just_reported();
  void just_requested();

  SEND(beep)
  SEND(get_alarms)
  SEND(get_bluetooth_pass)
  SEND(get_bms_1_firmware)
  SEND(get_bms_1_manufacture_date)
  SEND(get_bms_1_serial)
  SEND(get_bms_2_firmware)
  SEND(get_bms_2_manufacture_date)
  SEND(get_bms_2_serial)
  SEND(get_circle_light)
  SEND(get_lift_sensor)
  uint32_t get_last_reported();
  uint32_t get_last_requested();
  SEND(get_lock)
  SEND(get_magic_light_mode)
  SEND(get_model)
  SEND(get_music_bluetooth)
  SEND(get_serial)
  SEND(get_spectrum_light)
  SEND(get_spectrum_light_mode)
  SEND(get_standby_delay)
  SEND(get_strobe)
  SEND(get_voice)
  SEND(get_voice_language)
  SEND(horn)
  SEND(lock)
  SEND(power_off)
  SEND_INT_PARAM(send_command, command)
  SEND_INT_PARAM(set_alarm_1, alarm_1)
  SEND_INT_PARAM(set_alarm_2, alarm_2)
  SEND_INT_PARAM(set_alarm_3, alarm_3)
  SEND_BOOL_PARAM(set_circle_light, circle_light)
  SEND_BOOL_PARAM(set_lift_sensor, lift_sensor)
  SEND_INT_PARAM(set_magic_light_mode, magic_light_mode)
  SEND_INT_PARAM(set_main_light_mode, main_light_mode)
  SEND_BOOL_PARAM(set_music_bluetooth, music_bluetooth)
  SEND_INT_PARAM(set_ride_mode, ride_mode)
  SEND_BOOL_PARAM(set_spectrum_light, spectrum_light)
  SEND_INT_PARAM(set_spectrum_light_mode, spectrum_light_mode)
  SEND_INT_PARAM(set_standby_delay, standby_delay)
  SEND_BOOL_PARAM(set_strobe, strobe)
  SEND_INT_PARAM(set_tilt_back, tilt_back)
  SEND_BOOL_PARAM(set_voice, voice)
  SEND_INT_PARAM(set_voice_language, voice_language)
  SEND(unlock)

 protected:
  uint16_t char_handle_;
  std::unique_ptr<KingSongEUCCodec> codec_;
  uint32_t last_reported_ = 0;
  uint32_t last_requested_ = 0;

  void send_request_(std::unique_ptr<KingSongEUCCommand> request);
};

}  // namespace kingsong_euc
}  // namespace esphome
