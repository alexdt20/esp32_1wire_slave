#include "esphome.h"

class OneWireSlave : public Component {
 public:
  void setup() override {
    pin_ = new OneWirePin(ESP32_GPIO_NUM_4);
    pin_->setup();
    
    // Инициализация 20 датчиков
    for (int i = 0; i < 20; i++) {
      sensors_[i].address = generate_rom(i);
      sensors_[i].temperature = 25.0;
    }
  }

  void loop() override {
    if (pin_->reset()) {
      uint8_t cmd = pin_->read();
      if (cmd == 0x55) {  // Match ROM
        uint8_t rom[8];
        for (int i = 0; i < 8; i++) rom[i] = pin_->read();
        
        for (auto &sensor : sensors_) {
          if (memcmp(rom, sensor.address, 8) == 0) {
            current_sensor_ = &sensor;
            break;
          }
        }
      }
      else if (current_sensor_ && cmd == 0x44) {  // Convert T
        delay(750);  // Эмуляция измерения
      }
      else if (current_sensor_ && cmd == 0xBE) {  // Read Scratchpad
        uint16_t temp = static_cast<uint16_t>(current_sensor_->temperature * 16);
        uint8_t data[9] = {
          0x28, 0x00, 0x00, 0x00,
          static_cast<uint8_t>(temp & 0xFF),
          static_cast<uint8_t>(temp >> 8),
          0x10, 0x00  // CRC будет рассчитан ниже
        };
        data[8] = crc8(data, 8);
        for (uint8_t b : data) pin_->write(b);
      }
    }
  }

  void set_temperature(int index, float temp) {
    if (index >= 0 && index < 20) {
      sensors_[index].temperature = temp;
    }
  }

  float get_temperature(int index) {
    return (index >= 0 && index < 20) ? sensors_[index].temperature : NAN;
  }

  std::string get_status() {
    return "Active sensors: 20";
  }

 private:
  struct Sensor {
    uint8_t address[8];
    float temperature;
  };

  Sensor sensors_[20];
  Sensor* current_sensor_ = nullptr;
  OneWirePin* pin_;

  uint8_t crc8(const uint8_t *addr, uint8_t len) {
    uint8_t crc = 0;
    while (len--) {
      uint8_t inbyte = *addr++;
      for (uint8_t i = 8; i; i--) {
        uint8_t mix = (crc ^ inbyte) & 0x01;
        crc >>= 1;
        if (mix) crc ^= 0x8C;
        inbyte >>= 1;
      }
    }
    return crc;
  }

  void generate_rom(int index, uint8_t* rom) {
    rom[0] = 0x28;  // DS18B20 family code
    for (int i = 1; i < 7; i++) rom[i] = 0xAA + index;
    rom[7] = crc8(rom, 7);
  }
};
