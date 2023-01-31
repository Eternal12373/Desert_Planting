#line 1 "c:\\Users\\dell\\Desktop\\Mega2560\\Summer_Car\\chassis.cpp"
#include "chassis.h"
#include "PS2X_lib.h"

extern PS2X ps2x;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

chassis_t chassis;
gimbal_t gimbal;

my_step_motor_t step1;
my_step_motor_t step2;
my_step_motor_t step3;

void set_chassis_mode()
{
    if (ps2x.Button(PSB_PAD_UP))
    { // will be TRUE as long as button is pressed
        chassis.chassis_mode = MOVE_FORWARD;
    }
    else if (ps2x.Button(PSB_PAD_RIGHT))
    {
        chassis.chassis_mode = TURN_RIGHT;
    }
    else if (ps2x.Button(PSB_PAD_LEFT))
    {
        chassis.chassis_mode = TURN_LEFT;
    }
    else if (ps2x.Button(PSB_PAD_DOWN))
    {
        chassis.chassis_mode = MOVE_BACK;
    }
    else
    {
        chassis.chassis_mode = NO_MOVE;
    }
}
void chassis_control()
{
    switch (chassis.chassis_mode)
    {
    case NO_MOVE:
        digitalWrite(motor_IN1, LOW);
        digitalWrite(motor_IN2, LOW);
        digitalWrite(motor_IN3, LOW);
        digitalWrite(motor_IN4, LOW);
        SetDirectionAndSpeed(0, 0);
        break;
    case MOVE_FORWARD:
        SetDirectionAndSpeed(128, 128);
        break;
    case MOVE_BACK:
        SetDirectionAndSpeed(-128, -128);
        break;
    case TURN_LEFT:
        SetDirectionAndSpeed(-128, 128);
        break;
    case TURN_RIGHT:
        SetDirectionAndSpeed(128, -128);
        break;
    }
}
void SetDirectionAndSpeed(int left_speed, int right_speed)
{
    if (left_speed < 0)
    {
        left_speed *= -1;
        digitalWrite(motor_IN1, HIGH);
        digitalWrite(motor_IN2, LOW);
        analogWrite(motor_pwm1, left_speed);
    }
    else
    {
        digitalWrite(motor_IN1, LOW);
        digitalWrite(motor_IN2, HIGH);
        analogWrite(motor_pwm1, left_speed);
    }
    if (right_speed < 0)
    {
        right_speed *= -1;
        digitalWrite(motor_IN3, HIGH);
        digitalWrite(motor_IN4, LOW);
        analogWrite(motor_pwm2, right_speed);
    }
    else
    {
        digitalWrite(motor_IN3, LOW);
        digitalWrite(motor_IN4, HIGH);
        analogWrite(motor_pwm2, right_speed);
    }
}

void motor_init()
{
    pinMode(motor_pwm1, OUTPUT);
    pinMode(motor_pwm2, OUTPUT);
    pinMode(motor_pwm3,OUTPUT);
    digitalWrite(motor_pwm3,LOW);
    pinMode(motor_IN1, OUTPUT);
    pinMode(motor_IN2, OUTPUT);
    pinMode(motor_IN3, OUTPUT);
    pinMode(motor_IN4, OUTPUT);

    digitalWrite(motor_IN1, LOW);
    digitalWrite(motor_IN2, LOW);
    digitalWrite(motor_IN3, LOW);
    digitalWrite(motor_IN4, LOW);
    


}
void servo_init()
{

    servo1.attach(54);
    servo2.attach(55);
    servo3.attach(56);
    servo4.attach(57);

    device1_return();
    delay(100);
    device1_loose();
    delay(100);
    device2_loose();
    delay(100);
    device2_vertical();
    delay(100);
}

void device1_rollout(void)
{
    servo1.write(60);
    gimbal.dev1_pos = MODE_OUT;
}
void device1_return(void)
{
    servo1.write(180);
    gimbal.dev1_pos = MODE_IN;
}

void device1_catch(void)
{
    servo2.write(30);
    gimbal.dev1_catch = MODE_ON;
}
void device1_loose(void)
{
    servo2.write(180);
    gimbal.dev1_catch = MODE_OFF;
}
void device2_horizen(void)
{
    servo3.write(45);
    gimbal.dev2_pos = MODE_OUT;
}
void device2_vertical(void)
{
    servo3.write(150);
    gimbal.dev2_pos = MODE_IN;
}
void device2_catch(void)
{
    servo4.write(0);
    gimbal.dev2_catch = MODE_ON;
}
void device2_loose(void)
{
    servo4.write(105);
    gimbal.dev2_catch = MODE_OFF;
}
void step_motor_init(void)
{
    pinMode(step_motor1_dir, OUTPUT);
    pinMode(step_motor1_pul, OUTPUT);
    pinMode(step_motor2_dir, OUTPUT);
    pinMode(step_motor2_pul, OUTPUT);
    pinMode(step_motor3_dir, OUTPUT);
    pinMode(step_motor3_pul, OUTPUT);
    step1.mode = STEP_OFF;
    step2.mode = STEP_OFF;
    step3.mode = STEP_OFF;
}
void step_motor_single(int dir, int pul, int direction,int speed)
{
    if (direction != 0)
    {
        digitalWrite(dir, HIGH);
    }
    else
    {
        digitalWrite(dir, LOW);
    }
    for (int x = 0; x < 100; x++)
    {
        digitalWrite(pul, HIGH);
        delayMicroseconds(speed);
        digitalWrite(pul, LOW);
        delayMicroseconds(speed);
    }
}
void zuan_on(void)
{
    digitalWrite(motor_pwm3,HIGH);
}
void zuan_off(void)
{
    digitalWrite(motor_pwm3,LOW);
}
void water_on(void)
{
    digitalWrite(water,HIGH);
}
void water_off(void)
{
    digitalWrite(water,LOW);
}