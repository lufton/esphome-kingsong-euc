#include "codec.h"

namespace esphome {
namespace kingsong_euc {

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_send_command_request(uint16_t command) {
  return this->get_request_((KingSongEUCCmd) command);
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_set_alarms_request(uint8_t alarm_1, uint8_t alarm_2,
                                                                             uint8_t alarm_3, uint8_t tilt_back) {
  std::string alarms_password = this->get_alarms_pass();
  return this->get_request_(CMD_SET_ALARMS, {{2, alarm_1},
                                             {4, alarm_2},
                                             {6, alarm_3},
                                             {8, tilt_back},
                                             {10, alarms_password.length() == 6 ? alarms_password[0] : 0x00},
                                             {11, alarms_password.length() == 6 ? alarms_password[1] : 0x00},
                                             {12, alarms_password.length() == 6 ? alarms_password[2] : 0x00},
                                             {13, alarms_password.length() == 6 ? alarms_password[3] : 0x00},
                                             {14, alarms_password.length() == 6 ? alarms_password[4] : 0x00},
                                             {15, alarms_password.length() == 6 ? alarms_password[5] : 0x00}});
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_set_alarm_1_request(uint8_t alarm_1) {
  return this->get_set_alarms_request(alarm_1, this->get_alarm_2(), this->get_alarm_3(), this->get_tilt_back());
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_set_alarm_2_request(uint8_t alarm_2) {
  return this->get_set_alarms_request(this->get_alarm_1(), alarm_2, this->get_alarm_3(), this->get_tilt_back());
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_set_alarm_3_request(uint8_t alarm_3) {
  return this->get_set_alarms_request(this->get_alarm_1(), this->get_alarm_2(), alarm_3, this->get_tilt_back());
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_set_main_light_mode_request(uint8_t main_light_mode) {
  return this->get_request_(CMD_SET_MAIN_LIGHT_MODE, this->voice_ << 8 | main_light_mode);
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_set_ride_mode_request(uint8_t ride_mode) {
  return this->get_request_(CMD_SET_RIDE_MODE, 0xE000 | ride_mode);
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_set_standby_delay_request(uint16_t standby_delay) {
  return this->get_request_(CMD_SET_STANDBY_DELAY, 0x0001,
                            {{4, standby_delay & 0xFF}, {5, (standby_delay >> 8) & 0xFF}});
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_set_tilt_back_request(uint8_t tilt_back) {
  return this->get_set_alarms_request(this->get_alarm_1(), this->get_alarm_2(), this->get_alarm_3(), tilt_back);
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_set_voice_request(bool voice) {
  return this->get_request_(CMD_SET_MAIN_LIGHT_MODE, voice << 8 | this->main_light_mode_);
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_unlock_request() {
  uint8_t x = random_uint32() % 10;
  uint8_t y = random_uint32() % 10;
  uint8_t z = random_uint32() % 10;
  uint8_t a = (x + y + z + this->lock_pin_.a + this->lock_pin_.b + this->lock_pin_.c) % 10;
  uint8_t b = (x + y + z + a + this->lock_pin_.b + this->lock_pin_.c) % 10;
  uint8_t c = (x + y + z + a + b + this->lock_pin_.c) % 10;
  return this->get_request_(CMD_LOCK,
                            {{10, '0' + a}, {11, '0' + x}, {12, '0' + b}, {13, '0' + y}, {14, '0' + c}, {15, '0' + z}});
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_request_(KingSongEUCCmd command,
                                                                   std::map<uint8_t, uint8_t> bytes) {
  std::unique_ptr<KingSongEUCCommand> request_ptr = make_unique<KingSongEUCCommand>();
  KingSongEUCCommand *request = request_ptr.get();
  request->command = command;
  for (const auto &pair : bytes)
    request->data_byte[pair.first - 2] = pair.second;
  return request_ptr;
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_request_(KingSongEUCCmd command) {
  return this->get_request_(command, {});
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_request_(KingSongEUCCmd command, uint16_t value) {
  return this->get_request_(command, value, {});
}

std::unique_ptr<KingSongEUCCommand> KingSongEUCCodec::get_request_(KingSongEUCCmd command, uint16_t value,
                                                                   std::map<uint8_t, uint8_t> bytes) {
  bytes[2] = value & 0xFF;
  bytes[3] = (value >> 8) & 0xFF;
  return this->get_request_(command, bytes);
}

void KingSongEUCCodec::log_buffer() {
  ESP_LOGE(
      TAG,
      "Buffer is: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
      this->buffer_.header & 0xFF, (this->buffer_.header >> 8) & 0xFF, this->buffer_.data_byte[0],
      this->buffer_.data_byte[1], this->buffer_.data_byte[2], this->buffer_.data_byte[3], this->buffer_.data_byte[4],
      this->buffer_.data_byte[5], this->buffer_.data_byte[6], this->buffer_.data_byte[7], this->buffer_.data_byte[8],
      this->buffer_.data_byte[9], this->buffer_.data_byte[10], this->buffer_.data_byte[11], this->buffer_.data_byte[12],
      this->buffer_.data_byte[13], this->buffer_.packet, this->buffer_.tail[0], this->buffer_.tail[1],
      this->buffer_.tail[2]);
  ESP_LOGE(TAG, "    Index: 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19");
}

void KingSongEUCCodec::save_buffer(uint8_t *buffer) {
  memcpy(&this->buffer_, buffer, sizeof(KingSongEUCCommand));
  uint16_t value = this->get_value();
  uint8_t bms_num;
  KingSongEUCPkt packet = this->get_packet();
  switch (packet) {
    case PKT_STANDBY_DELAY:  // 63
      if (this->get_byte(2) + this->get_byte(3) == 0)
        this->set_standby_delay(this->get_word(4));
      break;
    case PKT_SPECTRUM_LIGHT:  // 74
      this->set_spectrum_light(value > 0);
      break;
    case PKT_LIFT_SENSOR:  // 76
      this->set_lift_sensor(value > 0);
      break;
    case PKT_SPECTRUM_LIGHT_MODE:  // 77
      this->set_spectrum_light_mode(value);
      break;
    case PKT_MAGIC_LIGHT_MODE:  // 82
      this->set_magic_light_mode(value);
      break;
    case PKT_STROBE:  // 85
      this->set_strobe(value > 0);
      break;
    case PKT_MUSIC_BT:  // 88
      this->set_music_bluetooth(value > 0);
      break;
    case PKT_LOCK:  // 95
      if (this->get_word(4) + this->get_word(6) + this->get_word(8) > 0) {
        this->lock_pin_.a = this->get_byte(4) - 48;
        this->lock_pin_.b = this->get_byte(6) - 48;
        this->lock_pin_.c = this->get_byte(8) - 48;
      }
      this->set_lock(value > 0 ? lock::LOCK_STATE_LOCKED : lock::LOCK_STATE_UNLOCKED);
      break;
    case PKT_VOICE_LANGUAGE:  // 107
      this->set_voice_language(value);
      break;
    case PKT_CIRCLE_LIGHT:  // 110
      this->set_circle_light(value > 0);
      break;
    case PKT_A9:  // 169
      this->set_voltage(this->get_word(2) / 100.0f);
      this->set_speed(this->get_word(4) / 100.0f);
      this->set_odometer(this->get_dword(6) / 1000.0f);
      this->set_current(this->get_word(10) / 100.0f);
      this->set_mosfet_temperature(this->get_word(12) / 100.0f);
      this->set_ride_mode(this->get_byte(14));
      this->set_power(this->voltage_ * this->current_);
      break;
    case PKT_SERIAL:  // 179
      this->set_serial(this->get_string());
      break;
    case PKT_ALARMS:  // 181
      this->set_alarm_1(this->get_word(4));
      this->set_alarm_2(this->get_word(6));
      this->set_alarm_3(this->get_word(8));
      this->set_tilt_back(this->get_word(10));
      break;
    case PKT_B9:  // 185
      this->set_trip_distance(this->get_dword(2) / 1000.0f);
      this->set_uptime(this->get_word(6));
      this->set_trip_max_speed(this->get_word(8) / 100.0f);
      this->set_main_light_mode(this->get_byte(10));
      this->set_voice(this->get_byte(11) > 0);
      this->set_fan(this->get_byte(12) > 0);
      this->set_charging(this->get_byte(13) > 0);
      this->set_motor_temperature(this->get_word(14) / 100.0f);
      break;
    case PKT_MODEL:  // 187
      this->set_model(this->get_string());
      break;
    case PKT_BMS1_SERIAL:  // 225
      this->set_bms_1_serial(this->get_string());
      break;
    case PKT_BMS2_SERIAL:  // 226
      this->set_bms_2_serial(this->get_string());
      break;
    case PKT_BMS1_MANUFACTURE_DATE:  // 227
      this->set_bms_1_manufacture_date(this->get_string());
      break;
    case PKT_BMS2_MANUFACTURE_DATE:  // 228
      this->set_bms_2_manufacture_date(this->get_string());
      break;
    case PKT_BMS1_FIRMWARE:  // 229
      this->set_bms_1_firmware(this->get_string());
      break;
    case PKT_BMS2_FIRMWARE:  // 230
      this->set_bms_2_firmware(this->get_string());
      break;
    case PKT_ALARMS_PASS:  // 231
      this->set_alarms_pass(this->get_string().length() == 6 ? this->get_string() : "");
      break;
    case PKT_BMS1:  // 241
      switch (this->get_bms_packet()) {
        case GENERAL:
          this->set_bms_1_voltage(this->get_word(2) / 100.0f);
          this->set_bms_1_current(this->get_word(4) / 100.0f);
          this->set_bms_1_remaining_capacity(this->get_word(6) * 10);
          this->set_bms_1_factory_capacity(this->get_word(8) * 10);
          this->set_bms_1_full_cycles(this->get_word(10));
          this->set_bms_1_soc(100.0f * this->get_word(6) / this->get_word(8));
          break;
        case TEMP:
          this->set_bms_1_temperature_1((this->get_word(2) - 2730) / 10.0f);
          this->set_bms_1_temperature_2((this->get_word(4) - 2730) / 10.0f);
          this->set_bms_1_temperature_3((this->get_word(6) - 2730) / 10.0f);
          this->set_bms_1_temperature_4((this->get_word(8) - 2730) / 10.0f);
          this->set_bms_1_temperature_5((this->get_word(10) - 2730) / 10.0f);
          this->set_bms_1_temperature_6((this->get_word(12) - 2730) / 10.0f);
          this->set_bms_1_mosfet_temperature((this->get_word(14) - 2730) / 10.0f);
          break;
        case CELL_GROUP_1:
          this->set_bms_1_cell_1_voltage(this->get_word(2) / 1000.0f);
          this->set_bms_1_cell_2_voltage(this->get_word(4) / 1000.0f);
          this->set_bms_1_cell_3_voltage(this->get_word(6) / 1000.0f);
          this->set_bms_1_cell_4_voltage(this->get_word(8) / 1000.0f);
          this->set_bms_1_cell_5_voltage(this->get_word(10) / 1000.0f);
          this->set_bms_1_cell_6_voltage(this->get_word(12) / 1000.0f);
          this->set_bms_1_cell_7_voltage(this->get_word(14) / 1000.0f);
          break;
        case CELL_GROUP_2:
          this->set_bms_1_cell_8_voltage(this->get_word(2) / 1000.0f);
          this->set_bms_1_cell_9_voltage(this->get_word(4) / 1000.0f);
          this->set_bms_1_cell_10_voltage(this->get_word(6) / 1000.0f);
          this->set_bms_1_cell_11_voltage(this->get_word(8) / 1000.0f);
          this->set_bms_1_cell_12_voltage(this->get_word(10) / 1000.0f);
          this->set_bms_1_cell_13_voltage(this->get_word(12) / 1000.0f);
          this->set_bms_1_cell_14_voltage(this->get_word(14) / 1000.0f);
          break;
        case CELL_GROUP_3:
          this->set_bms_1_cell_15_voltage(this->get_word(2) / 1000.0f);
          this->set_bms_1_cell_16_voltage(this->get_word(4) / 1000.0f);
#if KINGSONG_EUC_CELL_COUNT > 16
          this->set_bms_1_cell_17_voltage(this->get_word(6) / 1000.0f);
          this->set_bms_1_cell_18_voltage(this->get_word(8) / 1000.0f);
          this->set_bms_1_cell_19_voltage(this->get_word(10) / 1000.0f);
          this->set_bms_1_cell_20_voltage(this->get_word(12) / 1000.0f);
#endif
#if KINGSONG_EUC_CELL_COUNT > 20
          this->set_bms_1_cell_21_voltage(this->get_word(14) / 1000.0f);
#endif
          break;
        case CELL_GROUP_4:
#if KINGSONG_EUC_CELL_COUNT > 20
          this->set_bms_1_cell_22_voltage(this->get_word(2) / 1000.0f);
          this->set_bms_1_cell_23_voltage(this->get_word(4) / 1000.0f);
          this->set_bms_1_cell_24_voltage(this->get_word(6) / 1000.0f);
          this->set_bms_1_cell_25_voltage(this->get_word(8) / 1000.0f);
          this->set_bms_1_cell_26_voltage(this->get_word(10) / 1000.0f);
          this->set_bms_1_cell_27_voltage(this->get_word(12) / 1000.0f);
          this->set_bms_1_cell_28_voltage(this->get_word(14) / 1000.0f);
#endif
          break;
        case CELL_GROUP_5:
#if KINGSONG_EUC_CELL_COUNT > 20
          this->set_bms_1_cell_29_voltage(this->get_word(2) / 1000.0f);
          this->set_bms_1_cell_30_voltage(this->get_word(4) / 1000.0f);
#endif
          break;
      }
      break;
    case PKT_BMS2:  // 242
      switch (this->get_bms_packet()) {
        case GENERAL:
          this->set_bms_2_voltage(this->get_word(2) / 100.0f);
          this->set_bms_2_current(this->get_word(4) / 100.0f);
          this->set_bms_2_remaining_capacity(this->get_word(6) * 10);
          this->set_bms_2_factory_capacity(this->get_word(8) * 10);
          this->set_bms_2_full_cycles(this->get_word(10));
          this->set_bms_2_soc(100.0f * this->get_word(6) / this->get_word(8));
          break;
        case TEMP:
          this->set_bms_2_temperature_1((this->get_word(2) - 2730) / 10.0f);
          this->set_bms_2_temperature_2((this->get_word(4) - 2730) / 10.0f);
          this->set_bms_2_temperature_3((this->get_word(6) - 2730) / 10.0f);
          this->set_bms_2_temperature_4((this->get_word(8) - 2730) / 10.0f);
          this->set_bms_2_temperature_5((this->get_word(10) - 2730) / 10.0f);
          this->set_bms_2_temperature_6((this->get_word(12) - 2730) / 10.0f);
          this->set_bms_2_mosfet_temperature((this->get_word(14) - 2730) / 10.0f);
          break;
        case CELL_GROUP_1:
          this->set_bms_2_cell_1_voltage(this->get_word(2) / 1000.0f);
          this->set_bms_2_cell_2_voltage(this->get_word(4) / 1000.0f);
          this->set_bms_2_cell_3_voltage(this->get_word(6) / 1000.0f);
          this->set_bms_2_cell_4_voltage(this->get_word(8) / 1000.0f);
          this->set_bms_2_cell_5_voltage(this->get_word(10) / 1000.0f);
          this->set_bms_2_cell_6_voltage(this->get_word(12) / 1000.0f);
          this->set_bms_2_cell_7_voltage(this->get_word(14) / 1000.0f);
          break;
        case CELL_GROUP_2:
          this->set_bms_2_cell_8_voltage(this->get_word(2) / 1000.0f);
          this->set_bms_2_cell_9_voltage(this->get_word(4) / 1000.0f);
          this->set_bms_2_cell_10_voltage(this->get_word(6) / 1000.0f);
          this->set_bms_2_cell_11_voltage(this->get_word(8) / 1000.0f);
          this->set_bms_2_cell_12_voltage(this->get_word(10) / 1000.0f);
          this->set_bms_2_cell_13_voltage(this->get_word(12) / 1000.0f);
          this->set_bms_2_cell_14_voltage(this->get_word(14) / 1000.0f);
          break;
        case CELL_GROUP_3:
          this->set_bms_2_cell_15_voltage(this->get_word(2) / 1000.0f);
          this->set_bms_2_cell_16_voltage(this->get_word(4) / 1000.0f);
#if KINGSONG_EUC_CELL_COUNT > 16
          this->set_bms_2_cell_17_voltage(this->get_word(6) / 1000.0f);
          this->set_bms_2_cell_18_voltage(this->get_word(8) / 1000.0f);
          this->set_bms_2_cell_19_voltage(this->get_word(10) / 1000.0f);
          this->set_bms_2_cell_20_voltage(this->get_word(12) / 1000.0f);
#endif
#if KINGSONG_EUC_CELL_COUNT > 20
          this->set_bms_2_cell_21_voltage(this->get_word(14) / 1000.0f);
#endif
          break;
        case CELL_GROUP_4:
#if KINGSONG_EUC_CELL_COUNT > 20
          this->set_bms_2_cell_22_voltage(this->get_word(2) / 1000.0f);
          this->set_bms_2_cell_23_voltage(this->get_word(4) / 1000.0f);
          this->set_bms_2_cell_24_voltage(this->get_word(6) / 1000.0f);
          this->set_bms_2_cell_25_voltage(this->get_word(8) / 1000.0f);
          this->set_bms_2_cell_26_voltage(this->get_word(10) / 1000.0f);
          this->set_bms_2_cell_27_voltage(this->get_word(12) / 1000.0f);
          this->set_bms_2_cell_28_voltage(this->get_word(14) / 1000.0f);
#endif
          break;
        case CELL_GROUP_5:
#if KINGSONG_EUC_CELL_COUNT > 20
          this->set_bms_2_cell_29_voltage(this->get_word(2) / 1000.0f);
          this->set_bms_2_cell_30_voltage(this->get_word(4) / 1000.0f);
#endif
          break;
      }
      break;
    case PKT_F5:  // 245
      this->set_phase_short_circuit(this->get_byte(6) > 0);
      this->set_gyroscope_error(this->get_byte(7) > 0);
      this->set_hall_sensor_error(this->get_byte(8) > 0);
      this->set_cpu_load(this->get_byte(14));
      this->set_pwm(this->get_byte(15));
      break;
    case PKT_F6:  // 246
      this->set_speed_limit(this->get_word(2) / 100.0f);
      this->set_ride_time(this->get_word(12));
      this->set_error_code(this->get_word(14));
      this->set_error_description(this->get_error_description_(this->error_code_));
      break;
    case PKT_C9:  // 201
    case PKT_F3:  // 243
    case PKT_F4:  // 244
      break;
    default:
      ESP_LOGE(TAG, "Got unhandled packet type: 0x%2X (%d)", packet, packet);
      this->log_buffer();
      break;
  }
}

std::string KingSongEUCCodec::get_string(uint8_t length) {
  uint8_t *buffer_ = (uint8_t *) &this->buffer_;
  std::string result;

  for (size_t i = 2; result.length() <= length && i < sizeof(KingSongEUCCommand); ++i) {
    if (i == 16)
      continue;
    if (buffer_[i] == 0)
      break;
    result += static_cast<char>(buffer_[i]);
  }

  return result;
}

std::string KingSongEUCCodec::get_string() { return this->get_string(17); }

inline std::string KingSongEUCCodec::get_error_description_(uint16_t error_code) {
  auto pair = ERROR_CODES.find(error_code);
  return pair == ERROR_CODES.end() ? "Unknown error code " + std::to_string(error_code) : pair->second;
}

}  // namespace kingsong_euc
}  // namespace esphome
