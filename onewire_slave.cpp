#include "onewire_slave.h"

namespace esphome {
namespace onewire_slave {

static const char *const TAG = "onewire.slave";

void OneWireSlave::setup() {
  ESP_LOGCONFIG(TAG, "Setting up 1-Wire Slave...");
  this->pin_->setup();
}

}  // namespace onewire_slave
}  // namespace esphome