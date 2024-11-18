#pragma once

#ifdef USE_ESP32

#include <vector>
#include "esphome/core/component.h"
#include "binary_sensor.h"
#include "button.h"
#include "client.h"
#include "lock.h"
#include "number.h"
#include "select.h"
#include "sensor.h"
#include "switch.h"
#include "text_sensor.h"

namespace esphome {
namespace kingsong_euc {

#define PUBLISH_STATE(entity, state) \
  if (entity != nullptr) \
    entity->publish_state(state);

#define REGISTER_BMS_SENSORS(bms) \
  REGISTER_SENSOR(bms_##bms##_current) \
  REGISTER_SENSOR(bms_##bms##_factory_capacity) \
  REGISTER_TEXT_SENSOR(bms_##bms##_firmware) \
  REGISTER_SENSOR(bms_##bms##_full_cycles) \
  REGISTER_TEXT_SENSOR(bms_##bms##_manufacture_date) \
  REGISTER_TEXT_SENSOR(bms_##bms##_serial) \
  REGISTER_SENSOR(bms_##bms##_soc) \
  REGISTER_SENSOR(bms_##bms##_remaining_capacity) \
  REGISTER_SENSOR(bms_##bms##_voltage) \
  REGISTER_SENSOR(bms_##bms##_temperature_1) \
  REGISTER_SENSOR(bms_##bms##_temperature_2) \
  REGISTER_SENSOR(bms_##bms##_temperature_3) \
  REGISTER_SENSOR(bms_##bms##_temperature_4) \
  REGISTER_SENSOR(bms_##bms##_temperature_5) \
  REGISTER_SENSOR(bms_##bms##_temperature_6) \
  REGISTER_SENSOR(bms_##bms##_mosfet_temperature)

#define REGISTER_BMS_CELL_VOLTAGE_SENSORS_1_16(bms) \
  REGISTER_SENSOR(bms_##bms##_cell_1_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_2_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_3_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_4_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_5_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_6_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_7_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_8_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_9_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_10_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_11_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_12_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_13_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_14_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_15_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_16_voltage)

#define REGISTER_BMS_CELL_VOLTAGE_SENSORS_17_20(bms) \
  REGISTER_SENSOR(bms_##bms##_cell_17_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_18_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_19_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_20_voltage)

#define REGISTER_BMS_CELL_VOLTAGE_SENSORS_21_30(bms) \
  REGISTER_SENSOR(bms_##bms##_cell_21_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_22_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_23_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_24_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_25_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_26_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_27_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_28_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_29_voltage) \
  REGISTER_SENSOR(bms_##bms##_cell_30_voltage)

class KingSongEUC : public KingSongEUCClient, public Component {
 public:
  void dump_config() override;
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  void setup() override { this->codec_ = make_unique<KingSongEUCCodec>(); }

  REGISTER_BINARY_SENSOR(charging)
  REGISTER_BINARY_SENSOR(fan)
  REGISTER_BINARY_SENSOR(gyroscope_error)
  REGISTER_BINARY_SENSOR(hall_sensor_error)
  REGISTER_BINARY_SENSOR(phase_short_circuit)

  REGISTER_BUTTON(beep)
  REGISTER_BUTTON(horn)
  REGISTER_BUTTON(power_off)

  REGISTER_LOCK(lock)

  REGISTER_NUMBER(alarm_1)
  REGISTER_NUMBER(alarm_2)
  REGISTER_NUMBER(alarm_3)
  REGISTER_NUMBER(command)
  REGISTER_NUMBER(standby_delay)
  REGISTER_NUMBER(tilt_back)

  REGISTER_SELECT(magic_light_mode)
  REGISTER_SELECT(main_light_mode)
  REGISTER_SELECT(ride_mode)
  REGISTER_SELECT(spectrum_light_mode)
  REGISTER_SELECT(voice_language)

  REGISTER_SENSOR(current)
  REGISTER_SENSOR(cpu_load)
  REGISTER_SENSOR(error_code)
  REGISTER_SENSOR(mosfet_temperature)
  REGISTER_SENSOR(motor_temperature)
  REGISTER_SENSOR(odometer)
  REGISTER_SENSOR(power)
  REGISTER_SENSOR(pwm)
  REGISTER_SENSOR(ride_time)
  REGISTER_SENSOR(speed)
  REGISTER_SENSOR(speed_limit)
  REGISTER_SENSOR(trip_distance)
  REGISTER_SENSOR(trip_max_speed)
  REGISTER_SENSOR(uptime)
  REGISTER_SENSOR(voltage)

  REGISTER_SWITCH(circle_light)
  REGISTER_SWITCH(lift_sensor)
  REGISTER_SWITCH(music_bluetooth)
  REGISTER_SWITCH(spectrum_light)
  REGISTER_SWITCH(strobe)
  REGISTER_SWITCH(voice)

  // REGISTER_TEXT_SENSOR(bluetooth_password)
  REGISTER_TEXT_SENSOR(charging_status)
  REGISTER_TEXT_SENSOR(error_description)
  REGISTER_TEXT_SENSOR(model)
  REGISTER_TEXT_SENSOR(serial)

  REGISTER_BMS_SENSORS(1)
  REGISTER_BMS_CELL_VOLTAGE_SENSORS_1_16(1)
#if KINGSONG_EUC_CELL_COUNT > 16
  REGISTER_BMS_CELL_VOLTAGE_SENSORS_17_20(1)
#endif
#if KINGSONG_EUC_CELL_COUNT > 20
  REGISTER_BMS_CELL_VOLTAGE_SENSORS_21_30(1)
#endif

#if KINGSONG_EUC_BMS_COUNT > 1
  REGISTER_BMS_SENSORS(2)
  REGISTER_BMS_CELL_VOLTAGE_SENSORS_1_16(2)
#if KINGSONG_EUC_CELL_COUNT > 16
  REGISTER_BMS_CELL_VOLTAGE_SENSORS_17_20(2)
#endif
#if KINGSONG_EUC_CELL_COUNT > 20
  REGISTER_BMS_CELL_VOLTAGE_SENSORS_21_30(2)
#endif
#endif

 protected:
  std::vector<KingSongEUCBinarySensor *> binary_sensors_;
  std::vector<KingSongEUCButton *> buttons_;
  std::vector<KingSongEUCLock *> locks_;
  std::vector<KingSongEUCNumber *> numbers_;
  std::vector<KingSongEUCSelect *> selects_;
  std::vector<KingSongEUCSensor *> sensors_;
  std::vector<KingSongEUCSwitch *> switches_;
  std::vector<KingSongEUCTextSensor *> text_sensors_;
};

}  // namespace kingsong_euc
}  // namespace esphome

#endif
