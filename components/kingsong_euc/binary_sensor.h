#pragma once

#include "esphome/core/log.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "base_entity.h"
#include "const.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_BINARY_SENSOR(name) \
 protected: \
  KingSongEUCBinarySensor *name##_binary_sensor_{nullptr}; \
\
 public: \
  void set_##name##_binary_sensor(KingSongEUCBinarySensor *binary_sensor) { \
    this->binary_sensors_.push_back(binary_sensor); \
    this->name##_binary_sensor_ = binary_sensor; \
    binary_sensor->set_parent(this); \
  }

enum class KingSongEUCBinarySensorType {
  CHARGING,
  FAN,
  GYROSCOPE_ERROR,
  HALL_SENSOR_ERROR,
  PHASE_SHORT_CIRCUIT,
};

class KingSongEUCBinarySensor : public binary_sensor::BinarySensor, public KingSongEUCBaseEntity {
 public:
  KingSongEUCBinarySensor(KingSongEUCBinarySensorType binary_sensor_type, std::string name, uint32_t report_interval)
      : KingSongEUCBaseEntity(name, report_interval) {
    this->binary_sensor_type_ = binary_sensor_type;
  }

  void dump_config() { LOG_BINARY_SENSOR("  ", this->type_.c_str(), this); }

  bool has_state() override {
    return this->has_state_ && this->last_updated_ > 0 && KingSongEUCBaseEntity::has_state();
  }

  void publish_state(bool state) {
    bool prev_state = this->state;
    this->state = state;
    this->has_state_ = true;
    this->just_updated();
    if (state != prev_state)
      this->report_state();
  }

  void report_state() override {
    binary_sensor::BinarySensor::publish_state(this->state);
    this->just_reported();
  }

  void request_state() override {
    switch (this->binary_sensor_type_) {
      case KingSongEUCBinarySensorType::CHARGING:
      case KingSongEUCBinarySensorType::FAN:
      case KingSongEUCBinarySensorType::GYROSCOPE_ERROR:
      case KingSongEUCBinarySensorType::HALL_SENSOR_ERROR:
      case KingSongEUCBinarySensorType::PHASE_SHORT_CIRCUIT:
        break;
    }
  }

 protected:
  KingSongEUCBinarySensorType binary_sensor_type_;
};

}  // namespace kingsong_euc
}  // namespace esphome
