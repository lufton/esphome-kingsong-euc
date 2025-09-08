#pragma once

#include <cstring>
#include <map>
#include <stdint.h>
#include "esphome/components/lock/lock.h"
#include "esphome/core/log.h"
#include "base.h"
#include "const.h"
#include "error_codes.h"

namespace esphome {
namespace kingsong_euc {

#define CMD_REQUEST(name, command) \
 public: \
  std::unique_ptr<KingSongEUCCommand> get_##name##_request() { return this->get_request_(command); }

#define CMD_REQUEST_EMPTY_TAIL(name, command) \
 public: \
  std::unique_ptr<KingSongEUCCommand> get_##name##_request() { \
    std::unique_ptr<KingSongEUCCommand> request = this->get_request_(command); \
    request.get()->tail[0] = 0x00; \
    request.get()->tail[1] = 0x00; \
    request.get()->tail[2] = 0x00; \
    return request; \
  }

#define CMD_REQUEST_BOOL_PARAM(name, command) \
 public: \
  std::unique_ptr<KingSongEUCCommand> get_##name##_request(bool value) { \
    return this->get_request_(command, value ? 1 : 0); \
  }

#define CMD_REQUEST_INT_PARAM(name, command) \
 public: \
  std::unique_ptr<KingSongEUCCommand> get_##name##_request(uint16_t value) { \
    return this->get_request_(command, value); \
  }

#define CMD_REQUEST_INT_VALUE(name, command, value) \
 public: \
  std::unique_ptr<KingSongEUCCommand> get_##name##_request() { return this->get_request_(command, value); }

#define GETTER_FIELD(type, name) \
 private: \
  void set_##name(type value) { this->name##_ = value; } \
\
 protected: \
  type name##_{}; \
\
 public: \
  type get_##name() { return name##_; }

#define GETTER_FIELD_BMS(bms) \
  GETTER_FIELD(float, bms_##bms##_current) \
  GETTER_FIELD(uint32_t, bms_##bms##_factory_capacity) \
  GETTER_FIELD(std::string, bms_##bms##_firmware) \
  GETTER_FIELD(uint16_t, bms_##bms##_full_cycles) \
  GETTER_FIELD(std::string, bms_##bms##_manufacture_date) \
  GETTER_FIELD(float, bms_##bms##_mosfet_temperature) \
  GETTER_FIELD(float, bms_##bms##_soc) \
  GETTER_FIELD(uint32_t, bms_##bms##_remaining_capacity) \
  GETTER_FIELD(std::string, bms_##bms##_serial_number) \
  GETTER_FIELD(float, bms_##bms##_temperature_1) \
  GETTER_FIELD(float, bms_##bms##_temperature_2) \
  GETTER_FIELD(float, bms_##bms##_temperature_3) \
  GETTER_FIELD(float, bms_##bms##_temperature_4) \
  GETTER_FIELD(float, bms_##bms##_temperature_5) \
  GETTER_FIELD(float, bms_##bms##_temperature_6) \
  GETTER_FIELD(float, bms_##bms##_voltage)

#define GETTER_FIELD_BMS_CELL_VOLTAGE_1_16(bms) \
  GETTER_FIELD(float, bms_##bms##_cell_1_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_2_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_3_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_4_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_5_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_6_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_7_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_8_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_9_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_10_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_11_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_12_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_13_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_14_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_15_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_16_voltage)

#define GETTER_FIELD_BMS_CELL_VOLTAGE_17_20(bms) \
  GETTER_FIELD(float, bms_##bms##_cell_17_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_18_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_19_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_20_voltage)

#define GETTER_FIELD_BMS_CELL_VOLTAGE_21_30(bms) \
  GETTER_FIELD(float, bms_##bms##_cell_21_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_22_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_23_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_24_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_25_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_26_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_27_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_28_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_29_voltage) \
  GETTER_FIELD(float, bms_##bms##_cell_30_voltage)

class KingSongEUCCodec {
 public:
  CMD_REQUEST(beep, CMD_BEEP)
  CMD_REQUEST(get_alarms_pass, CMD_GET_ALARMS_PASS)
  CMD_REQUEST_EMPTY_TAIL(get_bms_1_firmware, CMD_GET_BMS_1_FIRMWARE)
  CMD_REQUEST_EMPTY_TAIL(get_bms_1_manufacture_date, CMD_GET_BMS_1_MANUFACTURE_DATE)
  CMD_REQUEST_EMPTY_TAIL(get_bms_1_serial_number, CMD_GET_BMS_1_SERIAL_NUMBER)
  CMD_REQUEST_EMPTY_TAIL(get_bms_2_firmware, CMD_GET_BMS_2_FIRMWARE)
  CMD_REQUEST_EMPTY_TAIL(get_bms_2_manufacture_date, CMD_GET_BMS_2_MANUFACTURE_DATE)
  CMD_REQUEST_EMPTY_TAIL(get_bms_2_serial_number, CMD_GET_BMS_2_SERIAL_NUMBER)
  CMD_REQUEST(get_alarms, CMD_GET_ALARMS)
  CMD_REQUEST(get_bluetooth_pass, CMD_GET_PASS)
  CMD_REQUEST(get_circle_light, CMD_GET_CIRCLE_LIGHT)
  std::unique_ptr<KingSongEUCCommand> get_send_command_request(uint16_t command);
  CMD_REQUEST(get_lift_sensor, CMD_GET_LIFT_SENSOR)
  CMD_REQUEST(get_lock, CMD_GET_LOCK)
  CMD_REQUEST(get_model, CMD_GET_MODEL)
  CMD_REQUEST(get_serial_number, CMD_GET_SERIAL_NUMBER)
  CMD_REQUEST(get_standby_delay, CMD_GET_STANDBY_DELAY)
  CMD_REQUEST(get_magic_light_mode, CMD_GET_MAGIC_LIGHT_MODE)
  CMD_REQUEST(get_music_bluetooth, CMD_GET_MUSIC_BLUETOOTH)
  CMD_REQUEST(get_spectrum_light, CMD_GET_SPECTRUM_LIGHT)
  CMD_REQUEST(get_spectrum_light_mode, CMD_GET_SPECTRUM_LIGHT_MODE)
  CMD_REQUEST(get_strobe, CMD_GET_STROBE)
  CMD_REQUEST(get_voice, CMD_GET_VOICE)
  CMD_REQUEST(get_voice_language, CMD_GET_VOICE_LANGUAGE)
  CMD_REQUEST(horn, CMD_HORN)
  CMD_REQUEST_INT_VALUE(lock, CMD_LOCK, 0x0001)
  CMD_REQUEST(power_off, CMD_POWER_OFF)
  std::unique_ptr<KingSongEUCCommand> get_set_alarms_request(uint8_t alarm_1, uint8_t alarm_2, uint8_t alarm_3,
                                                             uint8_t tilt_back);
  std::unique_ptr<KingSongEUCCommand> get_set_alarm_1_request(uint8_t alarm_1);
  std::unique_ptr<KingSongEUCCommand> get_set_alarm_2_request(uint8_t alarm_2);
  std::unique_ptr<KingSongEUCCommand> get_set_alarm_3_request(uint8_t alarm_3);
  CMD_REQUEST_BOOL_PARAM(set_circle_light, CMD_SET_CIRCLE_LIGHT)
  std::unique_ptr<KingSongEUCCommand> get_set_main_light_mode_request(uint8_t main_light_mode);
  CMD_REQUEST_BOOL_PARAM(set_music_bluetooth, CMD_SET_MUSIC_BT)
  std::unique_ptr<KingSongEUCCommand> get_set_ride_mode_request(uint8_t ride_mode);
  std::unique_ptr<KingSongEUCCommand> get_set_standby_delay_request(uint16_t standby_delay);
  CMD_REQUEST_BOOL_PARAM(set_lift_sensor, CMD_SET_LIFT_SENSOR)
  CMD_REQUEST_INT_PARAM(set_magic_light_mode, CMD_SET_MAGIC_LIGHT_MODE)
  CMD_REQUEST_BOOL_PARAM(set_spectrum_light, CMD_SET_SPECTRUM_LIGHT)
  CMD_REQUEST_INT_PARAM(set_spectrum_light_mode, CMD_SET_SPECTRUM_LIGHT_MODE)
  CMD_REQUEST_BOOL_PARAM(set_strobe, CMD_SET_STROBE)
  std::unique_ptr<KingSongEUCCommand> get_set_tilt_back_request(uint8_t tilt_back);
  std::unique_ptr<KingSongEUCCommand> get_set_voice_request(bool voice);
  CMD_REQUEST_INT_PARAM(set_voice_language, CMD_SET_VOICE_LANGUAGE)

  std::unique_ptr<KingSongEUCCommand> get_unlock_request();
  void save_buffer(uint8_t *buffer);
  KingSongEUCPkt get_packet() { return this->buffer_.packet; }
  std::string get_string(uint8_t length);
  std::string get_string();
  KingSongEUCBMSPkt get_bms_packet() { return (KingSongEUCBMSPkt) this->buffer_.tail[0]; }
  uint16_t get_value() { return this->get_word(2); }
  int8_t get_byte(uint8_t index) { return this->buffer_.data_byte[index - 2]; }
  int16_t get_word(uint8_t index) {
    return this->buffer_.data_byte[index - 1] << 8 | this->buffer_.data_byte[index - 2];
  }
  int32_t get_dword(uint8_t index) {
    return this->buffer_.data_byte[index - 1] << 24 | this->buffer_.data_byte[index - 2] << 16 |
           this->buffer_.data_byte[index + 1] << 8 | this->buffer_.data_byte[index];
  }
  void log_buffer();

 protected:
  KingSongEUCPacket buffer_;
  struct lock_pin {
    uint8_t a;
    uint8_t b;
    uint8_t c;
  } lock_pin_{};
  GETTER_FIELD(uint16_t, alarm_1)
  GETTER_FIELD(uint16_t, alarm_2)
  GETTER_FIELD(uint16_t, alarm_3)
  GETTER_FIELD(std::string, alarms_pass)
  GETTER_FIELD(uint8_t, charging)
  GETTER_FIELD(bool, circle_light)
  GETTER_FIELD(uint16_t, cpu_load)
  GETTER_FIELD(float, current)
  GETTER_FIELD(uint16_t, error_code)
  GETTER_FIELD(std::string, error_description)
  GETTER_FIELD(bool, fan)
  GETTER_FIELD(bool, gyroscope_error)
  GETTER_FIELD(bool, hall_sensor_error)
  GETTER_FIELD(bool, lift_sensor)
  GETTER_FIELD(lock::LockState, lock)
  GETTER_FIELD(uint8_t, magic_light_mode)
  GETTER_FIELD(uint8_t, main_light_mode)
  GETTER_FIELD(std::string, model)
  GETTER_FIELD(float, mosfet_temperature)
  GETTER_FIELD(float, motor_temperature)
  GETTER_FIELD(bool, music_bluetooth)
  GETTER_FIELD(float, odometer)
  GETTER_FIELD(bool, phase_short_circuit)
  GETTER_FIELD(float, power)
  GETTER_FIELD(uint16_t, pwm)
  GETTER_FIELD(uint8_t, ride_mode)
  GETTER_FIELD(uint16_t, ride_time)
  GETTER_FIELD(std::string, serial_number)
  GETTER_FIELD(bool, spectrum_light)
  GETTER_FIELD(uint8_t, spectrum_light_mode)
  GETTER_FIELD(float, speed)
  GETTER_FIELD(float, speed_limit)
  GETTER_FIELD(uint16_t, tilt_back)
  GETTER_FIELD(float, trip_distance)
  GETTER_FIELD(float, trip_max_speed)
  GETTER_FIELD(uint16_t, standby_delay)
  GETTER_FIELD(bool, strobe)
  GETTER_FIELD(bool, voice)
  GETTER_FIELD(uint8_t, voice_language)
  GETTER_FIELD(uint16_t, uptime)
  GETTER_FIELD(float, voltage)
  GETTER_FIELD_BMS(1)
  GETTER_FIELD_BMS_CELL_VOLTAGE_1_16(1)
#if KINGSONG_EUC_CELL_COUNT > 16
  GETTER_FIELD_BMS_CELL_VOLTAGE_17_20(1)
#endif
#if KINGSONG_EUC_CELL_COUNT > 20
  GETTER_FIELD_BMS_CELL_VOLTAGE_21_30(1)
#endif

#if KINGSONG_EUC_BMS_COUNT > 1
  GETTER_FIELD_BMS(2)
  GETTER_FIELD_BMS_CELL_VOLTAGE_1_16(2)
#if KINGSONG_EUC_CELL_COUNT > 16
  GETTER_FIELD_BMS_CELL_VOLTAGE_17_20(2)
#endif
#if KINGSONG_EUC_CELL_COUNT > 20
  GETTER_FIELD_BMS_CELL_VOLTAGE_21_30(2)
#endif
#endif

  inline std::string get_error_description_(uint16_t error_code);
  std::unique_ptr<KingSongEUCCommand> get_request_(KingSongEUCCmd type, std::map<uint8_t, uint8_t> bytes);
  std::unique_ptr<KingSongEUCCommand> get_request_(KingSongEUCCmd type);
  std::unique_ptr<KingSongEUCCommand> get_request_(KingSongEUCCmd type, uint16_t value);
  std::unique_ptr<KingSongEUCCommand> get_request_(KingSongEUCCmd type, uint16_t value,
                                                   std::map<uint8_t, uint8_t> bytes);
};

}  // namespace kingsong_euc
}  // namespace esphome
