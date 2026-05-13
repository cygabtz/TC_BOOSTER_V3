#include <cstdint>

#include "adcchannel.hpp"
#include "currentsensor.hpp"

CurrentSensor::CurrentSensor(ADCChannel& adcChannel, float offset, float gain, uint16_t window)
    : adc(adcChannel), offset(offset), gain(gain), window(window), filteredCurrent(0.0f)
{}

void CurrentSensor::update() {
    float voltage = adc.toVoltage();

    // Aplicar offset y gain
    float corrected = voltage - offset;
    float current = corrected * gain;

    // Aplicar media móvil
    filteredCurrent += (current - filteredCurrent) / window;
}

float CurrentSensor::getCurrent() const {
    return filteredCurrent;
}

void CurrentSensor::setOffset(float newOffset) {
    offset = newOffset;
}

void CurrentSensor::setGain(float newGain) {
    gain = newGain;
}

void CurrentSensor::setWindow(uint16_t newWindow) {
    window = newWindow;
}


