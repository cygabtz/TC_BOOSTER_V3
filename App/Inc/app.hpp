#ifndef MAIN_HPP
#define MAIN_HPP
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void app_setup(void);
void app_loop(void);

void pwm_test();

void read_ADC_test_init();
void read_ADC_test();

void setup_sensor_test();
void sensor_test();
void pi_test();

#ifdef __cplusplus
}
#endif

#endif