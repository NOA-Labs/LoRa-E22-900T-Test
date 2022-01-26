#pragma once

#include <Arduino.h>

#define JOYSTICK_TOTAL_ADC_CHANNEL                      2

#define ADC_BUFF_SIZE                                   10

typedef struct{
uint8_t  index;
uint16_t adc_array[ADC_BUFF_SIZE];
uint16_t (*read)();
}joystick_param;

class joystick_drv{
public:
    void begin();
    uint32_t read_ave_val(joystick_param &p);
    uint32_t read_left_x_ave_val();
    uint32_t read_left_y_ave_val();
    uint32_t read_right_x_ave_val();
    uint32_t read_right_y_ave_val();
private:
    joystick_param joystick_left_p[JOYSTICK_TOTAL_ADC_CHANNEL];
    joystick_param joystick_right_p[JOYSTICK_TOTAL_ADC_CHANNEL];
};