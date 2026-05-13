#include <cstdint>
#include "pi_converter.hpp"

PIconverter::PIconverter(float Kp, float Ki, float Ts) 
    : K_p(Kp), K_i(Ki), T_s(Ts) {}

float PIconverter::convert(float target_current, float actual_current) {

    // Cálculo del error e(t)
    float actual_error = target_current - actual_current;

    // Integral del error discretizada
    this->integral_sum += actual_error * T_s;
    
    //Anti wind-up
    if(integral_sum > integral_limit) integral_sum = integral_limit;
    if(integral_sum < -integral_limit) integral_sum = -integral_limit;

    // u(t) = K_p * e(t) + K_i * integral(e(t))
    float result_current = (K_p * actual_error) + (K_i * integral_sum);

    return result_current;
}

void PIconverter::resetIntegralError() {
    this->integral_sum = 0.0f;
}