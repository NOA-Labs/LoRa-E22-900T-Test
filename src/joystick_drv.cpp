#include "joystick_drv.h"

#define JOYSTICK_LEFT_X_AXIS_ADC_PIN                    32
#define JOYSTICK_LEFT_Y_AXIS_ADC_PIN                    33
#define JOYSTICK_RIGHT_X_AXIS_ADC_PIN                   34
#define JOYSTICK_RIGHT_Y_AXIS_ADC_PIN                   35

static uint16_t read_left_x_adc_val()
{
    // adcAttachPin(JOYSTICK_LEFT_X_AXIS_ADC_PIN);
    return analogRead(JOYSTICK_LEFT_X_AXIS_ADC_PIN);
}
static uint16_t read_left_y_adc_val()
{
    // adcAttachPin(JOYSTICK_LEFT_Y_AXIS_ADC_PIN);
    return analogRead(JOYSTICK_LEFT_Y_AXIS_ADC_PIN);
}
static uint16_t read_right_x_adc_val()
{
    // adcAttachPin(JOYSTICK_RIGHT_X_AXIS_ADC_PIN);
    return analogRead(JOYSTICK_RIGHT_X_AXIS_ADC_PIN);
}
static uint16_t read_right_y_adc_val()
{
    // adcAttachPin(JOYSTICK_RIGHT_Y_AXIS_ADC_PIN);
    return analogRead(JOYSTICK_RIGHT_Y_AXIS_ADC_PIN);
}

void joystick_drv::begin()
{
    analogReadResolution(12);
    analogSetWidth(12);

    memset(this->joystick_left_p[0].adc_array, 0, sizeof(this->joystick_left_p[0]));
    this->joystick_left_p[0].index = 0;
    this->joystick_left_p[0].read  = read_left_x_adc_val;

    memset(this->joystick_left_p[1].adc_array, 0, sizeof(this->joystick_left_p[1]));
    this->joystick_left_p[1].index = 0;
    this->joystick_left_p[1].read  = read_left_y_adc_val;

    memset(this->joystick_right_p[0].adc_array, 0, sizeof(this->joystick_right_p[0]));
    this->joystick_right_p[0].index = 0;
    this->joystick_right_p[0].read  = read_right_x_adc_val;

    memset(this->joystick_right_p[1].adc_array, 0, sizeof(this->joystick_right_p[1]));
    this->joystick_right_p[1].index = 0;
    this->joystick_right_p[1].read  = read_right_y_adc_val;
}

uint32_t joystick_drv::read_ave_val(joystick_param &p)
{
    p.adc_array[p.index] = p.read();
    p.index ++;
    if(p.index == ADC_BUFF_SIZE){
        p.index = 0;
    }

    uint32_t adc_val = 0;

    for(int i = 0; i < ADC_BUFF_SIZE; i++){
        adc_val += p.adc_array[i];
    }

    return adc_val / ADC_BUFF_SIZE;
}
uint32_t joystick_drv::read_left_x_ave_val()
{
    return read_ave_val(this->joystick_left_p[0]);
}

uint32_t joystick_drv::read_left_y_ave_val()
{
    return read_ave_val(this->joystick_left_p[1]);
}
uint32_t joystick_drv::read_right_x_ave_val()
{
    return read_ave_val(this->joystick_right_p[0]);
}

uint32_t joystick_drv::read_right_y_ave_val()
{
    return read_ave_val(this->joystick_right_p[1]);
}