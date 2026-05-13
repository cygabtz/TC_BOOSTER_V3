#ifndef PI_CONVERTER_HPP
#define PI_CONVERTER_HPP
#include "stdint.h"

// Aplica control proporcional-integral tal que la corriente de cada
// fase siga la corriente objetivo

// Se aplica un control independiente que no tiene en cuenta la
// interconexión entre las fases pero que simplifica los cálculos.


// Realmente aplicamos un control de lazo cerrado de conmutación
// sinusoidal, documentado en:
// https://www.mathworks.com/help/mcb/gs/open-loop-and-closed-loop-control.html
// https://www.renesas.cn/zh/key-technologies/motor-control/motor-algorithms#universal_dc_motor_control_algorithms

class PIconverter {
private:
    float integral_sum = 0.0f;
    float integral_limit = 1.0f;

    float K_p; // Ganancia proporcional
    float K_i; // Ganancia integral
    float T_s; // Tiempo entre iteraciones
public:    
    PIconverter(float Kp, float Ki, float Ts);

    float convert(float target_current, float actual_current);
    void resetIntegralError();
    // Anti wind-up, valor simétrico
    void setIntegralLimit(float limit);
};

#endif