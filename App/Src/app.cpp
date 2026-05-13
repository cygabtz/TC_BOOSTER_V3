// Headers de C
#include <array>
#include <cstdint>
#include <iostream>

// Headers de CubeMX
#include "main.h"
#include "stm32h723xx.h"
#include "stm32h7xx_hal_adc.h"

// Headers propios
#include "app.hpp"
#include "pwm.hpp"
#include "adcchannel.hpp"
#include "currentsensor.hpp"
#include "pi_converter.hpp"


extern ADC_HandleTypeDef hadc1;

void app_setup() {
    //setup_sensor_test();
    pwm_test();
    read_ADC_test_init();
}

void app_loop() {
    //sensor_test();
    //pi_test();

    read_ADC_test();

    //std::cout << "Hi";
}

// ------------------------------
// Generación de PWM. Testeado.
// --------------------------------

extern TIM_HandleTypeDef htim2;

PWM pwm1(&htim2, TIM_CHANNEL_1); //PA0
PWM pwm2(&htim2, TIM_CHANNEL_2);
PWM pwm3(&htim2, TIM_CHANNEL_3);

void pwm_test() {
    pwm1.start();
    pwm1.setFrequency(10000);
    pwm1.setDutyCycle(70);

    pwm2.start();
    pwm2.setFrequency(1000);
    pwm2.setDutyCycle(80);

    pwm3.start();
    pwm3.setFrequency(1000);
    pwm3.setDutyCycle(90);
}

// --------------------------------
// Test de lectura de ADC con DMA.
// --------------------------------

// CAMBIO: PF11 no se puede usar para leer ADC, así que se han cambiado los canales a:
// Rank 1 ADC1_INP3 --> PA6
// Rank 2 ADC1_INP4 --> PC4
// Rank 3 ADC1_INP5 --> PB1 

#define ADC_CHANNELS 3
uint32_t adc_values[ADC_CHANNELS];

void read_ADC_test_init() {
    HAL_ADC_Start(&hadc1);
    HAL_ADCEx_Calibration_Start(&hadc1, 0, ADC_SINGLE_ENDED); 
    HAL_ADC_Start_DMA(&hadc1, adc_values, ADC_CHANNELS);
}

float sensor_1, sensor_2, sensor_3;

void read_ADC_test() {
    // Corriente de 
    [[maybe_unused]]float sensor_1 = (adc_values[0] * 3.3f) / 65535.0f;


    [[maybe_unused]]float sensor_2 = (adc_values[1] * 3.3f) / 65535.0f;
    [[maybe_unused]]float sensor_3 = (adc_values[2] * 3.3f) / 65535.0f;

    HAL_Delay(100);
}

// --------------------------------
// Lectura de corrientes.
// --------------------------------

// CubeMX configura así los canales por defecto para ADC1
/* ADCChannel adc_A(&hadc1, ADC_CHANNEL_2);
ADCChannel adc_B(&hadc1, ADC_CHANNEL_3);
ADCChannel adc_C(&hadc1, ADC_CHANNEL_4); */

// Parámetros del sensor de efecto Hall
// El offset se calcula experimentalmente promediando el valor que de cuando el campo es nulo.
constexpr float HALL_OFFSET = 0.0f;
// La ganancia se calcula también experimentalmente.
/* constexpr float HALL_GAIN = 0.0f; */

// Número de muestras que se promedian en la media móvil
/* constexpr uint16_t WINDOW = 10; */

/* CurrentSensor sensor_A(adc_A, HALL_OFFSET, HALL_GAIN, WINDOW);
CurrentSensor sensor_B(adc_B, HALL_OFFSET, HALL_GAIN, WINDOW);
CurrentSensor sensor_C(adc_C, HALL_OFFSET, HALL_GAIN, WINDOW); */
uint16_t dmaBuffer[3];
/* void setup_sensor_test() {
    //std::array<uint16_t,3> dmaBuffer{};
    HAL_ADC_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)dmaBuffer, sizeof(dmaBuffer)/sizeof(uint16_t));

    adc_A.setChannelBuffer((uint32_t*)&dmaBuffer[0]);
    adc_B.setChannelBuffer((uint32_t*)&dmaBuffer[1]);
    adc_C.setChannelBuffer((uint32_t*)&dmaBuffer[2]);

}

void sensor_test() {
    sensor_A.update();
    sensor_B.update();
    sensor_C.update();

    [[maybe_unused]] float Ia = sensor_A.getCurrent();
    [[maybe_unused]] float Ib = sensor_A.getCurrent();
    [[maybe_unused]] float Ic = sensor_A.getCurrent();
} */

// Señal de prueba generada con PWM



// --------------------------------
// Generador de señal + Controlador PI
// --------------------------------

// Se buscan las ganancias experimentalmente.
/* constexpr float K_P = 0.0f;
constexpr float K_I = 0.0f; */


// Hay que elegir un valor adecuado entre el periodo de muestreo y el del seno a generar.
// Según el teorema de Nyquist: frecuencia_muestreo > 2 * frecuencia_seno
// Si no se produce aliasing y no se obtiene un seno
// https://es.mathworks.com/discovery/nyquist-theorem.html

// Si se quiere frecuencia_seno = 30Hz -> frecuencia_muestreo > 60Hz
// Entonces periodo muestreo = 1ms <-> frecuencia_muestro = 200Hz

/* float sampling_period = 0.001; // 1 ms
float signal_frequency = 200; // 200 Hz */

// Inicializar convertidores
/* PIconverter pi_A(K_P, K_I, sampling_period);
PIconverter pi_B(K_P, K_I, sampling_period);
PIconverter pi_C(K_P, K_I, sampling_period);
 */
// La amplitud de la señal objetivo dependerá del rango máximo del sensor Hall
/* constexpr float amplitude = 2.5f; */

// Incremento del ángulo para general la señal
/* static float theta = 0.0f;
const float PI = 3.14159265f; */

// Se debe ejecutar según TIM6 genere interrupción en el estado RUN
// TODO: configurar el ARR/PSC de TIM6 según el periodo de muestreo deseado
// TODO: revisar los rangos de valores que currentSensor saca y que PIconverter usan
/* void pi_test() {
    // Corrientes objetivo:
    theta += 2.0f * PI * signal_frequency * sampling_period; 

    float target_A = amplitude * sinf(theta);
    float target_B = amplitude * sinf(theta - (2.0f * PI / 3.0f)); // -120 grados en radiantes
    float target_C = amplitude * sinf(theta - (4.0f * PI / 3.0f)); // -240 grados en radiantes

    // Corrientes reales (test):
    // Hay que conectarlo con currentSensor
    float real_A = 0.0f;
    float real_B = 0.0f;
    float real_C = 0.0f;

    // Pasar las corriente por el control PI

    float result_A = pi_A.convert(target_A, real_A);
    float result_B = pi_A.convert(target_B, real_B);
    float result_C = pi_A.convert(target_C, real_C);

} */