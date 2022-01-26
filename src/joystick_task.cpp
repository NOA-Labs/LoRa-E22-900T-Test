#include "joystick_task.h"
#include "system_cfg.h"

#define JOYSTICK_TASK_STACK_SIZE                      2048
#define JOYSTICK_TASK_PRIORITY                        7

/**
 * 
 * 
 */
#define DEFAULT_SPEED_GRADE                             50
#define DEFAULT_STOP_AREA_ADC_VAL                       100
#define DEFAULT_FORWARD_DIR_ADC_VAL                     4095
#define DEFAULT_REVERSE_DIR_ADC_VAL                     200
#define DEFAULT_ZERO_POINT_ADC_VAL                      1750

#define JOYSTICK_DIR_COUNT                              4

enum JOYSTICK_DIR_TYPE{
    JOYSTICK_LEFT_X = 0,
    JOYSTICK_LEFT_Y,
    JOYSITCK_RIGHT_X,
    JOYSTICK_RIGHT_Y
};

TaskHandle_t    joystick_task_t = NULL;

joystick_drv    joystick;
joystick_speed  joystickSpeedArray[JOYSTICK_DIR_COUNT];

static void joystickSpeedArrayInit()
{
    joystickSpeedArray[JOYSTICK_LEFT_X].begin();
    joystickSpeedArray[JOYSTICK_LEFT_X].setStopAreaValue(DEFAULT_STOP_AREA_ADC_VAL);
    joystickSpeedArray[JOYSTICK_LEFT_X].setSpeedGrade(DEFAULT_SPEED_GRADE);
    joystickSpeedArray[JOYSTICK_LEFT_X].setZeroPointValue(DEFAULT_ZERO_POINT_ADC_VAL);
    joystickSpeedArray[JOYSTICK_LEFT_X].setMaxForwardValue(DEFAULT_FORWARD_DIR_ADC_VAL);
    joystickSpeedArray[JOYSTICK_LEFT_X].setMinReverseValue(DEFAULT_REVERSE_DIR_ADC_VAL);

    joystickSpeedArray[JOYSTICK_LEFT_Y].begin();
    joystickSpeedArray[JOYSTICK_LEFT_Y].setStopAreaValue(DEFAULT_STOP_AREA_ADC_VAL);
    joystickSpeedArray[JOYSTICK_LEFT_Y].setSpeedGrade(DEFAULT_SPEED_GRADE);
    joystickSpeedArray[JOYSTICK_LEFT_Y].setZeroPointValue(DEFAULT_ZERO_POINT_ADC_VAL);
    joystickSpeedArray[JOYSTICK_LEFT_Y].setMaxForwardValue(DEFAULT_FORWARD_DIR_ADC_VAL);
    joystickSpeedArray[JOYSTICK_LEFT_Y].setMinReverseValue(DEFAULT_REVERSE_DIR_ADC_VAL);

    joystickSpeedArray[JOYSITCK_RIGHT_X].begin();
    joystickSpeedArray[JOYSITCK_RIGHT_X].setStopAreaValue(DEFAULT_STOP_AREA_ADC_VAL);
    joystickSpeedArray[JOYSITCK_RIGHT_X].setSpeedGrade(DEFAULT_SPEED_GRADE);
    joystickSpeedArray[JOYSITCK_RIGHT_X].setZeroPointValue(DEFAULT_ZERO_POINT_ADC_VAL);
    joystickSpeedArray[JOYSITCK_RIGHT_X].setMaxForwardValue(DEFAULT_FORWARD_DIR_ADC_VAL);
    joystickSpeedArray[JOYSITCK_RIGHT_X].setMinReverseValue(DEFAULT_REVERSE_DIR_ADC_VAL);

    joystickSpeedArray[JOYSTICK_RIGHT_Y].begin();
    joystickSpeedArray[JOYSTICK_RIGHT_Y].setStopAreaValue(DEFAULT_STOP_AREA_ADC_VAL);
    joystickSpeedArray[JOYSTICK_RIGHT_Y].setSpeedGrade(DEFAULT_SPEED_GRADE);
    joystickSpeedArray[JOYSTICK_RIGHT_Y].setZeroPointValue(DEFAULT_ZERO_POINT_ADC_VAL);
    joystickSpeedArray[JOYSTICK_RIGHT_Y].setMaxForwardValue(DEFAULT_FORWARD_DIR_ADC_VAL);
    joystickSpeedArray[JOYSTICK_RIGHT_Y].setMinReverseValue(DEFAULT_REVERSE_DIR_ADC_VAL);
}

static uint32_t cal_speed_grade(uint32_t &adc_val, joystick_speed &speed)
{
    uint32_t        speed_grade     = speed.getSpeedGrade();
    uint32_t        stop_area_val   = speed.getStopAreaValue();
    uint32_t        max_forward_val = speed.getMaxForwardValue();
    uint32_t        min_reverse_val = speed.getMinReverseValue();
    uint32_t        zero_point_val  = speed.getZeroPointValue();
    uint32_t        grade = 0;

    if(adc_val >= zero_point_val - stop_area_val && adc_val <= zero_point_val + stop_area_val){
        return 0;
    }
    else if(adc_val < zero_point_val){
        if(adc_val < min_reverse_val){
            adc_val = min_reverse_val;
        }
        grade = (zero_point_val - adc_val) * speed_grade / (zero_point_val - min_reverse_val);
    }
    else{
        if(adc_val > max_forward_val){
            adc_val = max_forward_val;
        }
        grade = (adc_val - zero_point_val) * speed_grade / (max_forward_val - zero_point_val);
        grade += speed_grade;
    }
    
    return grade;
}

static void joystick_task_entry(void *param)
{
    uint32_t grade_array[JOYSTICK_DIR_COUNT] = {0};
    uint32_t adc_val;
    
    joystick.begin();
    joystickSpeedArrayInit();

    while(1){
       delay(10);
       adc_val = joystick.read_left_x_ave_val();
       grade_array[JOYSTICK_LEFT_X] = cal_speed_grade(adc_val, joystickSpeedArray[JOYSTICK_LEFT_X]);
       DBG_PRINTF("1: %d\r\n",grade_array[JOYSTICK_LEFT_X]);
       adc_val = joystick.read_left_y_ave_val();
       grade_array[JOYSTICK_LEFT_Y] = cal_speed_grade(adc_val, joystickSpeedArray[JOYSTICK_LEFT_Y]);
       DBG_PRINTF("2: %d\r\n", grade_array[JOYSTICK_LEFT_Y]);
       adc_val = joystick.read_right_x_ave_val();
       grade_array[JOYSITCK_RIGHT_X] = cal_speed_grade(adc_val, joystickSpeedArray[JOYSITCK_RIGHT_X]);
       DBG_PRINTF("3: %d\r\n", grade_array[JOYSITCK_RIGHT_X]);
       adc_val = joystick.read_right_y_ave_val();
       grade_array[JOYSTICK_RIGHT_Y] = cal_speed_grade(adc_val, joystickSpeedArray[JOYSTICK_RIGHT_Y]);
       DBG_PRINTF("4: %d\r\n", grade_array[JOYSTICK_RIGHT_Y]);
    }
}

int app_joystick_task_create()
{
    xTaskCreateUniversal(joystick_task_entry,
                        "btn handle task",
                        JOYSTICK_TASK_STACK_SIZE,
                        NULL,
                        JOYSTICK_TASK_PRIORITY,
                        &joystick_task_t,
                        CONFIG_ARDUINO_RUNNING_CORE);

    if(joystick_task_t == NULL)return -1;

    DBG_PRINTLN("<button_handle_task.cpp>app button handle task create.");
    return 0;
}

joystick_speed::joystick_speed()
{
    
}

void joystick_speed::begin()
{
    speed_p.speed_grade     = DEFAULT_SPEED_GRADE;
    speed_p.stop_area_val   = DEFAULT_STOP_AREA_ADC_VAL;
    speed_p.max_forward_val = DEFAULT_FORWARD_DIR_ADC_VAL;
    speed_p.min_reverse_val = DEFAULT_REVERSE_DIR_ADC_VAL;
    speed_p.zero_point_val  = DEFAULT_ZERO_POINT_ADC_VAL;
}