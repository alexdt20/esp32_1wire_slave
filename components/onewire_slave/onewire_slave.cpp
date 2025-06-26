#include "onewire_slave.h"

namespace esphome {
namespace onewire_slave {

static const char *const TAG = "onewire.slave";

void OneWireSlave::setup() {
  ESP_LOGCONFIG(TAG, "Setting up 1-Wire Slave on pin %d", this->pin_->get_pin());
  this->pin_->setup();
}

void OneWireSlave::loop() {
  // Здесь будет реализация 1-Wire протокола
}

}  // namespace onewire_slave
}  // namespace esphome
