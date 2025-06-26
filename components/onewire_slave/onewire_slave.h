#pragma once
#include "esphome.h"

namespace esphome {
namespace onewire_slave {

class OneWireSlave : public Component {
 public:
  void set_pin(GPIOPin *pin) { pin_ = pin; }
  void setup() override;
  void loop() override;
  
 private:
  GPIOPin *pin_{nullptr};
};

}  // namespace onewire_slave
}  // namespace esphome
