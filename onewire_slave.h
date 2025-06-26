#pragma once
#include "esphome.h"

class OneWireSlave : public Component {
 public:
  void setup() override {}
  void loop() override {}
  float get_setup_priority() const override { return esphome::setup_priority::BUS; }
  
  void set_pin(InternalGPIOPin *pin) { pin_ = pin; }
  
 private:
  InternalGPIOPin *pin_;
};