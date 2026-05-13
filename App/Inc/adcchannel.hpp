#ifndef ADCCHANNEL_HPP
#define ADCCHANNEL_HPP

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_adc.h"
#include "stm32h7xx_hal_tim.h"
#include <cstdint>

class ADCChannel {
private:
    ADC_HandleTypeDef* hadc;
    uint32_t channel;
    uint32_t* channelBuffer;
public:    
    ADCChannel(ADC_HandleTypeDef* adcHandler, uint32_t adcChannel);

    void setChannelBuffer(uint32_t* bufferPtr);
    uint32_t getRaw() const;
    float toVoltage() const;
    
};

#endif