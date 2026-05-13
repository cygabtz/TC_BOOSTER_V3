#ifndef PWM_HPP
#define PWM_HPP

#include "stm32h7xx_hal.h"
#include <cstdint>

class PWM {
private:
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint32_t frequency;
    float duty;
public:
    PWM(TIM_HandleTypeDef* timerHandle, uint32_t timChannel);
    void start();
    void stop();
    void setDutyCycle(float duty);
    float getDutyCycle() const;
    void setFrequency(uint32_t); 
    uint32_t getFrequency();
};

#endif