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
  BMS_CELL_VOLTAGE,
  BMS_CURRENT,
  BMS_FACTORY_CAPACITY,
  BMS_FULL_CYCLES,
  BMS_MOSFET_TEMPERATURE,
  BMS_REMAINING_CAPACITY,
  BMS_SOC,
  BMS_TEMPERATURE_1,
  BMS_TEMPERATURE_2,
  BMS_TEMPERATURE_3,
  BMS_TEMPERATURE_4,
  BMS_TEMPERATURE_5,
  BMS_TEMPERATURE_6,
  BMS_VOLTAGE,
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
      case KingSongEUCSensorType::BMS_CELL_VOLTAGE:
      case KingSongEUCSensorType::BMS_CURRENT:
      case KingSongEUCSensorType::BMS_FACTORY_CAPACITY:
      case KingSongEUCSensorType::BMS_FULL_CYCLES:
      case KingSongEUCSensorType::BMS_MOSFET_TEMPERATURE:
      case KingSongEUCSensorType::BMS_REMAINING_CAPACITY:
      case KingSongEUCSensorType::BMS_SOC:
      case KingSongEUCSensorType::BMS_TEMPERATURE_1:
      case KingSongEUCSensorType::BMS_TEMPERATURE_2:
      case KingSongEUCSensorType::BMS_TEMPERATURE_3:
      case KingSongEUCSensorType::BMS_TEMPERATURE_4:
      case KingSongEUCSensorType::BMS_TEMPERATURE_5:
      case KingSongEUCSensorType::BMS_TEMPERATURE_6:
      case KingSongEUCSensorType::BMS_VOLTAGE:
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
