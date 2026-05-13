#ifndef CURRENTSENSOR_HPP
#define CURRENTSENSOR_HPP

#include "adcchannel.hpp"
#include <cstdint>

class CurrentSensor {
private:
    ADCChannel& adc;
    float offset;
    float gain;
    uint16_t window; 
    float filteredCurrent;
public:    
    CurrentSensor(ADCChannel& adcChannel, float offset = 0.0f, float gain = 1.0f, uint16_t window = 10);

    void update();
    float getCurrent() const;

    void setOffset(float newOffset);
    void setGain(float newGain);
    void setWindow(uint16_t newWindow);
    
};

#endif