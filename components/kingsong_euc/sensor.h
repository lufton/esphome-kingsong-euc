#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"
#include "base_entity.h"
#include "const.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_SENSOR(name) \
 protected: \
  KingSongEUCSensor *name##_sensor_{nullptr}; \
\
 public: \
  void set_##name##_sensor(KingSongEUCSensor *sensor) { \
    this->sensors_.push_back(sensor); \
    this->name##_sensor_ = sensor; \
    sensor->set_parent(this); \
  }

enum class KingSongEUCSensorType {
  BMS_1_CELL_1_VOLTAGE,
  BMS_2_CELL_1_VOLTAGE,
  BMS_1_CELL_2_VOLTAGE,
  BMS_2_CELL_2_VOLTAGE,
  BMS_1_CELL_3_VOLTAGE,
  BMS_2_CELL_3_VOLTAGE,
  BMS_1_CELL_4_VOLTAGE,
  BMS_2_CELL_4_VOLTAGE,
  BMS_1_CELL_5_VOLTAGE,
  BMS_2_CELL_5_VOLTAGE,
  BMS_1_CELL_6_VOLTAGE,
  BMS_2_CELL_6_VOLTAGE,
  BMS_1_CELL_7_VOLTAGE,
  BMS_2_CELL_7_VOLTAGE,
  BMS_1_CELL_8_VOLTAGE,
  BMS_2_CELL_8_VOLTAGE,
  BMS_1_CELL_9_VOLTAGE,
  BMS_2_CELL_9_VOLTAGE,
  BMS_1_CELL_10_VOLTAGE,
  BMS_2_CELL_10_VOLTAGE,
  BMS_1_CELL_11_VOLTAGE,
  BMS_2_CELL_11_VOLTAGE,
  BMS_1_CELL_12_VOLTAGE,
  BMS_2_CELL_12_VOLTAGE,
  BMS_1_CELL_13_VOLTAGE,
  BMS_2_CELL_13_VOLTAGE,
  BMS_1_CELL_14_VOLTAGE,
  BMS_2_CELL_14_VOLTAGE,
  BMS_1_CELL_15_VOLTAGE,
  BMS_2_CELL_15_VOLTAGE,
  BMS_1_CELL_16_VOLTAGE,
  BMS_2_CELL_16_VOLTAGE,
  BMS_1_CELL_17_VOLTAGE,
  BMS_2_CELL_17_VOLTAGE,
  BMS_1_CELL_18_VOLTAGE,
  BMS_2_CELL_18_VOLTAGE,
  BMS_1_CELL_19_VOLTAGE,
  BMS_2_CELL_19_VOLTAGE,
  BMS_1_CELL_20_VOLTAGE,
  BMS_2_CELL_20_VOLTAGE,
  BMS_1_CELL_21_VOLTAGE,
  BMS_2_CELL_21_VOLTAGE,
  BMS_1_CELL_22_VOLTAGE,
  BMS_2_CELL_22_VOLTAGE,
  BMS_1_CELL_23_VOLTAGE,
  BMS_2_CELL_23_VOLTAGE,
  BMS_1_CELL_24_VOLTAGE,
  BMS_2_CELL_24_VOLTAGE,
  BMS_1_CELL_25_VOLTAGE,
  BMS_2_CELL_25_VOLTAGE,
  BMS_1_CELL_26_VOLTAGE,
  BMS_2_CELL_26_VOLTAGE,
  BMS_1_CELL_27_VOLTAGE,
  BMS_2_CELL_27_VOLTAGE,
  BMS_1_CELL_28_VOLTAGE,
  BMS_2_CELL_28_VOLTAGE,
  BMS_1_CELL_29_VOLTAGE,
  BMS_2_CELL_29_VOLTAGE,
  BMS_1_CELL_30_VOLTAGE,
  BMS_2_CELL_30_VOLTAGE,
  BMS_1_CURRENT,
  BMS_2_CURRENT,
  BMS_1_FACTORY_CAPACITY,
  BMS_2_FACTORY_CAPACITY,
  BMS_1_FULL_CYCLES,
  BMS_2_FULL_CYCLES,
  BMS_1_MOSFET_TEMPERATURE,
  BMS_2_MOSFET_TEMPERATURE,
  BMS_1_REMAINING_CAPACITY,
  BMS_2_REMAINING_CAPACITY,
  BMS_1_SOC,
  BMS_2_SOC,
  BMS_1_TEMPERATURE_1,
  BMS_2_TEMPERATURE_1,
  BMS_1_TEMPERATURE_2,
  BMS_2_TEMPERATURE_2,
  BMS_1_TEMPERATURE_3,
  BMS_2_TEMPERATURE_3,
  BMS_1_TEMPERATURE_4,
  BMS_2_TEMPERATURE_4,
  BMS_1_TEMPERATURE_5,
  BMS_2_TEMPERATURE_5,
  BMS_1_TEMPERATURE_6,
  BMS_2_TEMPERATURE_6,
  BMS_1_VOLTAGE,
  BMS_2_VOLTAGE,
  CPU_LOAD,
  ERROR_CODE,
  CURRENT,
  MOSFET_TEMPERATURE,
  MOTOR_HALL,
  MOTOR_PHASE_LINE,
  MOTOR_TEMPERATURE,
  ODOMETER,
  POWER,
  PWM,
  RIDE_TIME,
  SPEED,
  SPEED_LIMIT,
  TRIP_DISTANCE,
  TRIP_MAX_SPEED,
  UPTIME,
  VOLTAGE,
};

class KingSongEUCSensor : public sensor::Sensor, public KingSongEUCBaseEntity {
 public:
  KingSongEUCSensor(KingSongEUCSensorType sensor_type, std::string name, uint32_t report_interval, float hysteresis)
      : KingSongEUCBaseEntity(name, report_interval) {
    this->hysteresis_ = hysteresis;
    this->sensor_type_ = sensor_type;
  }

  void dump_config() { LOG_SENSOR("  ", this->type_.c_str(), this); }

  bool has_state() override {
    return this->has_state_ && this->last_updated_ > 0 && KingSongEUCBaseEntity::has_state();
  }

  void publish_state(float state) {
    float prev_state = this->state;
    this->state = state;
    this->has_state_ = true;
    this->just_updated();
    if (prev_state == NAN || std::abs(prev_state - state) >= this->hysteresis_)
      this->report_state();
  }

  void report_state() {
    sensor::Sensor::publish_state(this->state);
    this->just_reported();
  }

  void request_state() override {
    switch (this->sensor_type_) {
      case KingSongEUCSensorType::BMS_1_CELL_1_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_1_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_2_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_2_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_3_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_3_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_4_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_4_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_5_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_5_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_6_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_6_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_7_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_7_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_8_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_8_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_9_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_9_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_10_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_10_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_11_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_11_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_12_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_12_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_13_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_13_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_14_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_14_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_15_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_15_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_16_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_16_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_17_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_17_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_18_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_18_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_19_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_19_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_20_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_20_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_21_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_21_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_22_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_22_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_23_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_23_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_24_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_24_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_25_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_25_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_26_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_26_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_27_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_27_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_28_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_28_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_29_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_29_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CELL_30_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_CELL_30_VOLTAGE:
      case KingSongEUCSensorType::BMS_1_CURRENT:
      case KingSongEUCSensorType::BMS_2_CURRENT:
      case KingSongEUCSensorType::BMS_1_FACTORY_CAPACITY:
      case KingSongEUCSensorType::BMS_2_FACTORY_CAPACITY:
      case KingSongEUCSensorType::BMS_1_FULL_CYCLES:
      case KingSongEUCSensorType::BMS_2_FULL_CYCLES:
      case KingSongEUCSensorType::BMS_1_MOSFET_TEMPERATURE:
      case KingSongEUCSensorType::BMS_2_MOSFET_TEMPERATURE:
      case KingSongEUCSensorType::BMS_1_REMAINING_CAPACITY:
      case KingSongEUCSensorType::BMS_2_REMAINING_CAPACITY:
      case KingSongEUCSensorType::BMS_1_SOC:
      case KingSongEUCSensorType::BMS_2_SOC:
      case KingSongEUCSensorType::BMS_1_TEMPERATURE_1:
      case KingSongEUCSensorType::BMS_2_TEMPERATURE_1:
      case KingSongEUCSensorType::BMS_1_TEMPERATURE_2:
      case KingSongEUCSensorType::BMS_2_TEMPERATURE_2:
      case KingSongEUCSensorType::BMS_1_TEMPERATURE_3:
      case KingSongEUCSensorType::BMS_2_TEMPERATURE_3:
      case KingSongEUCSensorType::BMS_1_TEMPERATURE_4:
      case KingSongEUCSensorType::BMS_2_TEMPERATURE_4:
      case KingSongEUCSensorType::BMS_1_TEMPERATURE_5:
      case KingSongEUCSensorType::BMS_2_TEMPERATURE_5:
      case KingSongEUCSensorType::BMS_1_TEMPERATURE_6:
      case KingSongEUCSensorType::BMS_2_TEMPERATURE_6:
      case KingSongEUCSensorType::BMS_1_VOLTAGE:
      case KingSongEUCSensorType::BMS_2_VOLTAGE:
      case KingSongEUCSensorType::CPU_LOAD:
      case KingSongEUCSensorType::ERROR_CODE:
      case KingSongEUCSensorType::CURRENT:
      case KingSongEUCSensorType::MOSFET_TEMPERATURE:
      case KingSongEUCSensorType::MOTOR_HALL:
      case KingSongEUCSensorType::MOTOR_PHASE_LINE:
      case KingSongEUCSensorType::MOTOR_TEMPERATURE:
      case KingSongEUCSensorType::ODOMETER:
      case KingSongEUCSensorType::POWER:
      case KingSongEUCSensorType::PWM:
      case KingSongEUCSensorType::RIDE_TIME:
      case KingSongEUCSensorType::SPEED:
      case KingSongEUCSensorType::SPEED_LIMIT:
      case KingSongEUCSensorType::TRIP_DISTANCE:
      case KingSongEUCSensorType::TRIP_MAX_SPEED:
      case KingSongEUCSensorType::UPTIME:
      case KingSongEUCSensorType::VOLTAGE:
        break;
    }
  }

 protected:
  KingSongEUCSensorType sensor_type_;
  float hysteresis_ = 0xFFFFFFFF;
};

}  // namespace kingsong_euc
}  // namespace esphome
