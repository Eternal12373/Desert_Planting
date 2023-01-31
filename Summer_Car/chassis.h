#ifndef CHASSIS_H
#define CHASSIS_H

#include "Arduino.h"
#include "Servo.h"

#define motor_pwm1 7
#define motor_pwm2 6
#define motor_pwm3 23
#define motor_IN1 5
#define motor_IN2 4
#define motor_IN3 3
#define motor_IN4 2
#define water 21

#define step_motor1_dir 14  //分苗
#define step_motor1_pul 15
#define step_motor2_dir 16  //左右
#define step_motor2_pul 17
#define step_motor3_dir 18  //上下
#define step_motor3_pul 19


typedef enum
{
    NO_MOVE = 0, //停止
    MOVE_FORWARD,
    MOVE_BACK,
    TURN_LEFT,
    TURN_RIGHT
} chassis_mode_e;

typedef enum
{
    MODE_IN=0,
    MODE_OUT
} gimbal_pos_mode_e;

typedef enum
{
    MODE_OFF=0,
    MODE_ON
} gimbal_catch_mode_e;

typedef enum
{
    STEP_OFF=0,
    STEP_ON
} gimbal_step_motor_e;
typedef enum
{
    ZUAN_OFF=0,
    ZUAN_ON
} zuantou_mode_e;
typedef struct my_step
{
    gimbal_step_motor_e mode;
    int dir;
} my_step_motor_t;

typedef struct motor__
{
    int Lspeed_set;
    int Rspeed_set;
    chassis_mode_e chassis_mode;
    zuantou_mode_e mode_zuan;
} chassis_t;


typedef struct gimbal__
{
    gimbal_pos_mode_e dev1_pos;
    gimbal_pos_mode_e dev2_pos;
    gimbal_catch_mode_e dev1_catch;
    gimbal_catch_mode_e dev2_catch;
} gimbal_t;

void motor_init();
void SetDirectionAndSpeed(int left_speed, int right_speed);
void set_chassis_mode(void);
void chassis_control(void);
void servo_init();
void device1_rollout(void);
void device1_return(void);
void device1_catch(void);
void device1_loose(void);
void device2_horizen(void);
void device2_vertical(void);
void device2_catch(void);
void device2_loose(void);
void step_motor_init(void);
void step_motor_single(int dir, int pul, int direction,int speed);
void zuan_off(void);
void zuan_on(void);
void water_on(void);
void water_off(void);
#endif