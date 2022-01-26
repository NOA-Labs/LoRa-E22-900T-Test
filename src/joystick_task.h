#pragma once

#include "joystick_drv.h"

class joystick_speed{
public:
    joystick_speed();
    void        begin();
    void        setSpeedGrade(uint32_t speed){speed_p.speed_grade = speed;}
    uint32_t    getSpeedGrade(){return speed_p.speed_grade;}
    void        setStopAreaValue(uint32_t val){speed_p.stop_area_val = val;}
    uint32_t    getStopAreaValue(){return speed_p.stop_area_val;}
    void        setMaxForwardValue(uint32_t val){speed_p.max_forward_val = val;}
    uint32_t    getMaxForwardValue(){return speed_p.max_forward_val;}
    void        setMinReverseValue(uint32_t val){speed_p.min_reverse_val = val;}
    uint32_t    getMinReverseValue(){return speed_p.min_reverse_val;}
    void        setZeroPointValue(uint32_t val){speed_p.zero_point_val = val;}
    uint32_t    getZeroPointValue(){return speed_p.zero_point_val;}
private:
    struct joystick_speed_param{
        uint32_t        speed_grade;
        uint32_t        stop_area_val;
        uint32_t        max_forward_val;
        uint32_t        min_reverse_val;
        uint32_t        zero_point_val;
    };
    struct joystick_speed_param  speed_p;
};

int app_joystick_task_create();