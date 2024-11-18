#include "client.h"

namespace esphome {
namespace kingsong_euc {

KingSongEUCCodec *KingSongEUCClient::get_codec() { return this->codec_.get(); }

uint32_t KingSongEUCClient::get_last_reported() { return this->last_reported_; }

uint32_t KingSongEUCClient::get_last_requested() { return this->last_requested_; }

void KingSongEUCClient::just_reported() { this->last_reported_ = millis(); }

void KingSongEUCClient::just_requested() { this->last_requested_ = millis(); }

void KingSongEUCClient::send_request_(std::unique_ptr<KingSongEUCCommand> request_ptr) {
  auto request = request_ptr.get();
  ESP_LOGD(
      TAG,
      "Request is: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
      request->header & 0xFF, (request->header >> 8) & 0xFF, request->data_byte[0], request->data_byte[1],
      request->data_byte[2], request->data_byte[3], request->data_byte[4], request->data_byte[5], request->data_byte[6],
      request->data_byte[7], request->data_byte[8], request->data_byte[9], request->data_byte[10],
      request->data_byte[11], request->data_byte[12], request->data_byte[13], request->command, request->tail[0],
      request->tail[1], request->tail[2]);
  ESP_LOGD(TAG, "     Index: 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19");
  auto status = esp_ble_gattc_write_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(),
                                         this->char_handle_, sizeof(KingSongEUCCommand), (uint8_t *) request,
                                         ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);
  if (status)
    ESP_LOGW(TAG, "[%s] esp_ble_gattc_write_char failed, status=%d", this->parent_->address_str().c_str(), status);
}

}  // namespace kingsong_euc
}  // namespace esphome
