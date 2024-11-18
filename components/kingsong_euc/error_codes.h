#pragma once

#include <unordered_map>
#include <string>

namespace esphome {
namespace kingsong_euc {

static const std::unordered_map<uint16_t, std::string> ERROR_CODES = {
    {0, "No error"},
    {105, "Be voltage detect need calibration"},
    {202, "Over current or locked rotor"},
    {203,
     "The motor is blocked, please check whether the motor is rotating smoothly or remove obstacles before riding"},
    {205, "Drive circuit is not functioning correctly, please restart or replace drive circuit"},
    {206, "The motherboard output wire has short circuited, please check whether the battery output wire has short "
          "circuited or whether the motherboard MOS is damaged"},
    {207, "Gyroscope failure, please replace motherboard"},
    {208, "The coefficient of battery voltage have no just"},
    {217, "Motor Hall sensor error, please check the hall sensor and repair accordingly"},
    {218, "Overpower, please avoid over-accelerating or climbing steep hills"},
    {219, "Device is outputting at max"},
    {220, "Motherboard output over voltage, or motor is blocked, please ride with caution"},
    {221, "Motor is experiencing high temperature, please allow Motor to cool down before riding again"},
    {222, "MOS is experiencing high temperature, please allow MOS to cool down before riding again"},
    {223, "Charging is over voltage or over current"},
    {224, "The battery reaches the preset charging value"},
    {226, "BMS warning"},
    {227, "BMS get no data"},
    {228, "Serial Number Error"},
    {229, "Low voltage, please charge your device!"},
    {230, "Reserve power is missing, please replace motherboard"},
    {231, "Over voltage, please beware of your safety and avoid riding downhill"},
    {232, "Lift switch is out of order, please release the handlebar or check whether the lift switch sensor has "
          "experienced a short circuit"},
    {1208, "mttool, vol err"},
    {1209, "mttool, over time"},
    {1210, "mttool, block err"},
    {1211, "mttool, speed err"},
    {2235, "Sensor data is reversed, or line fault, this sensor has now been turned off"},
    {2236, "Sensor B is not connected or the sensor is damaged, the sensor has been closed for use"},
    {2237, "Sensor data A is reversed, this sensor is closed"},
    {2238, "Sensor A is not connected or the sensor is damaged, the sensor has been closed for use"},
    {2239, "The voltage is too high, please do not ride downhill for an extended time"},
    {2240, "The voltage is too high, please remove the charger"},
    {2241, "Low battery, please charge"},
    {2242, "Gyroscope error, please contact your seller and replacement motherboard"},
    {2243, "The output current of the main board has exceeded. Please check if the motor is damaged or if the phase "
           "line is shorted"},
    {2244, "Please check if the motor hall line connection and is functioning normally"},
    {2245, "No serial number or serial number error"},
    {2246, "The motor temperature is too high, please stop and ride after the it has cooled down"},
    {2247, "The motherboard temperature is too high, please stop and ride after the it has cooled down"},
    {2248, "The output current is at max, please ride with caution"},
};

}  // namespace kingsong_euc
}  // namespace esphome
