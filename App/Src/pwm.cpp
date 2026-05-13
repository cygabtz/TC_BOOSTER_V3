#include "main.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_tim.h"
#include <cstdint>
#include "pwm.hpp"

PWM::PWM(TIM_HandleTypeDef* timerHandle, uint32_t timChannel)
    : htim(timerHandle), channel(timChannel), frequency(0), duty(0.0f)
{}

void PWM::start() {
    HAL_TIM_PWM_Start(htim, channel);
}

void PWM::stop() {
    HAL_TIM_PWM_Stop(htim, channel);
}

void PWM::setDutyCycle(float dutyValue) {
    if (dutyValue > 100.0f) {
        dutyValue = 100.0f;
    }
    if (dutyValue < 0.0f) {
        dutyValue = 0.0f;
    }

    this->duty = dutyValue;

    uint32_t ccr = 0;
    if (frequency != 0) {
        ccr = (uint32_t)((duty / 100.0f) * htim->Instance->ARR);
    }

    __HAL_TIM_SET_COMPARE(htim, channel, ccr);
}

void PWM::setFrequency(uint32_t freq) {
    uint32_t clock_freq = HAL_RCC_GetHCLKFreq();
    uint32_t psc = htim->Instance->PSC;

    uint32_t arr = (clock_freq / (freq * (psc + 1))) - 1;

    __HAL_TIM_SET_AUTORELOAD(htim, arr);

    setDutyCycle(this->duty);

    this->frequency = freq;
}

float PWM::getDutyCycle() const {
    return duty;
}