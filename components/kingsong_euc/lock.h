#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/components/lock/lock.h"
#include "base_entity.h"
#include "const.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_LOCK(name) \
 protected: \
  KingSongEUCLock *name##_lock_{nullptr}; \
\
 public: \
  void set_##name##_lock(KingSongEUCLock *lock) { \
    this->locks_.push_back(lock); \
    this->name##_lock_ = lock; \
    lock->set_parent(this); \
  }

enum class KingSongEUCLockType {
  LOCK,
};

class KingSongEUCLock : public lock::Lock, public KingSongEUCBaseEntity {
 public:
  KingSongEUCLock(KingSongEUCLockType lock_type, std::string name, uint32_t report_interval)
      : KingSongEUCBaseEntity(name, report_interval) {
    this->lock_type_ = lock_type;
  }

  void dump_config() { LOG_LOCK("  ", this->type_.c_str(), this); }

  void publish_state(lock::LockState state) {
    lock::LockState prev_state = this->state;
    this->state = state;
    this->just_updated();
    if (state != prev_state)
      this->report_state();
  }

  void report_state() override {
    lock::Lock::publish_state(this->state);
    this->just_reported();
  }

  void request_state() override {
    switch (this->lock_type_) {
      case KingSongEUCLockType::LOCK:
        this->get_parent()->get_lock();
        break;
    }
  }

 protected:
  KingSongEUCLockType lock_type_;

  void control(const lock::LockCall &call) {
    if (!this->get_parent()->parent()->connected())
      return;
    lock::LockState state = *call.get_state();
    this->publish_state(state);
    if (state == lock::LOCK_STATE_LOCKED) {
      switch (this->lock_type_) {
        case KingSongEUCLockType::LOCK:
          return this->get_parent()->lock();
      }
    } else if (state == lock::LOCK_STATE_UNLOCKED) {
      switch (this->lock_type_) {
        case KingSongEUCLockType::LOCK:
          return this->get_parent()->unlock();
      }
    }
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
