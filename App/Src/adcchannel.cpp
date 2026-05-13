#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_adc.h"
#include "stm32h7xx_hal_tim.h"
#include <cstdint>
#include "adcchannel.hpp"

// Suponemos que voltaje de referencia real es este y no lo calculamos mediante funciones
// de la HAL dada la complejidad
#define ADC_VREF 3.3f

ADCChannel::ADCChannel(ADC_HandleTypeDef* adcHandler, uint32_t adcChannel)
    : hadc(adcHandler), channel(adcChannel)
{}

void ADCChannel::setChannelBuffer(uint32_t* bufferPtr) {
    this->channelBuffer = bufferPtr;
}

uint32_t ADCChannel::getRaw() const {
    if (this->channelBuffer == nullptr) return 0; 

    return *channelBuffer;
}
float ADCChannel::toVoltage() const {
    if (this->channelBuffer == nullptr) return 0.0f;

    // Consular parámetros en la HAL
    float vref = ADC_VREF;
    uint32_t resolutionBits = 16;

    //Calcular voltaje real con: V_real = V_ADC * V_ref / resolución máxima
    uint32_t maxValue = (1 << resolutionBits) - 1u; // Equivale a 2^resolución - 1
    return (static_cast<float>(*channelBuffer) / maxValue) * vref;
}