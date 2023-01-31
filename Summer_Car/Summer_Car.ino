#include "PS2X_lib.h"
#include "chassis.h"
#include <Servo.h>

#define PS2_DAT 13 // 14
#define PS2_CMD 11 // 15
#define PS2_SEL 10 // 16
#define PS2_CLK 12 // 17

extern Servo servo1;
extern Servo servo2;
extern Servo servo3;
extern Servo servo4;
extern my_step_motor_t step1;
extern my_step_motor_t step2;
extern my_step_motor_t step3;
int init_flag = 5;
/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures true

#define rumble true
extern chassis_t chassis;
extern gimbal_t gimbal;
PS2X ps2x;

int error = 0;
byte type = 0;
byte vibrate = 0;

// Reset func
void (*resetFunc)(void) = 0;

void setup()
{
    servo_init();
    motor_init();
    step_motor_init();
    Serial.begin(115200);
    delay(500);
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    if (error == 0)
    {
        Serial.print("Found Controller, configured successful ");
    }
    else
    {
        Serial.print("error");
        Serial.print(error);
    }
    pinMode(water,OUTPUT);
    water_off();

}

void loop()
{

    if (error == 1)
    {
        resetFunc();
    }
    while (init_flag)  //读取几次数据 但不作处理
    {
        ps2x.read_gamepad(false, vibrate);
        delay(10);
        init_flag--;
    }
    
    ps2x.read_gamepad(false, vibrate); // read controller and set large motor to spin at 'vibrate' speed

    set_chassis_mode();
    chassis_control();

    if (ps2x.NewButtonState())
    { // will be TRUE if any button changes state (on to off, or off to on)
        if (ps2x.Button(PSB_L3))
        {
            if (gimbal.dev1_catch == MODE_ON)
            {
                device1_loose();
            }
            else if(gimbal.dev1_catch == MODE_OFF)
            {
                device1_catch();
            }
            Serial.println("L3 pressed");
        }

        if (ps2x.Button(PSB_R3))
        {
            if (gimbal.dev2_catch == MODE_ON)
            {
                device2_loose();
            }
            else if(gimbal.dev2_catch == MODE_OFF)
            {
                device2_catch();
            }
            Serial.println("R3 pressed");
        }

        if (ps2x.Button(PSB_L2))
        {
            if (step2.mode == STEP_ON)
            {
                step2.mode = STEP_OFF;
            }
            else if (step2.mode == STEP_OFF)
            {
                step2.mode = STEP_ON;
            }
            step2.dir = 0;
        }
        if (ps2x.Button(PSB_R2))
        {
            if (step2.mode == STEP_ON)
            {
                step2.mode = STEP_OFF;
            }
            else if (step2.mode == STEP_OFF)
            {
                step2.mode = STEP_ON;
            }
            step2.dir = 1;
        }
        if (ps2x.Button(PSB_TRIANGLE))
        {
            if (step3.mode == STEP_ON)
            {
                step3.mode = STEP_OFF;
            }
            else if (step3.mode == STEP_OFF)
            {
                step3.mode = STEP_ON;
            }
            step3.dir = 0;
        }
    }
    if (ps2x.ButtonPressed(PSB_CIRCLE)) // will be TRUE if button was JUST pressed
    {
        if (step1.mode == STEP_ON)
        {
            step1.mode = STEP_OFF;
        }
        else if (step1.mode == STEP_OFF)
        {
            step1.mode = STEP_ON;
        }
        step1.dir = 0;
        // Serial.println("Circle just pressed");
    }
    if (ps2x.NewButtonState(PSB_CROSS)) // will be TRUE if button was JUST pressed OR released
    {
        if (step3.mode == STEP_ON)
        {
            step3.mode = STEP_OFF;
        }
        else if (step3.mode == STEP_OFF)
        {
            step3.mode = STEP_ON;
        }
        step3.dir = 1;

    }

    if (ps2x.ButtonReleased(PSB_SQUARE)) // will be TRUE if button was JUST released
    {

        Serial.println(chassis.mode_zuan);
        if (chassis.mode_zuan == ZUAN_OFF)
        {
            zuan_on();
            chassis.mode_zuan = ZUAN_ON;
        }
        else if (chassis.mode_zuan == ZUAN_ON)
        {
            zuan_off();
            chassis.mode_zuan = ZUAN_OFF;
        }

    }

    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1))
    {
        if (ps2x.Analog(PSS_LY) < 5)
        {
            device1_rollout();
        }
        if (ps2x.Analog(PSS_LY) > 245)
        {
            device1_return();
        }
        if (ps2x.Analog(PSS_RX) < 5)
        {
            device2_horizen();
        }
        if (ps2x.Analog(PSS_RX) > 245)
        {
            device2_vertical();
        }


    }
    if(ps2x.Button(PSB_SELECT))
    {
        water_on();
    }
    else 
    {
        water_off();
    }

    if (step1.mode == STEP_ON)
    {
        Serial.println(step1.mode);
        step_motor_single(step_motor1_dir, step_motor1_pul, step1.dir,500);
    }
    else if (step2.mode == STEP_ON)
    {
        Serial.println(step2.mode);
        step_motor_single(step_motor2_dir, step_motor2_pul, step2.dir,300);
    }
    else if (step3.mode == STEP_ON)
    {
        Serial.println("step3");
        Serial.println(step3.mode);
        step_motor_single(step_motor3_dir, step_motor3_pul, step3.dir,150);
    }

    delay(10);
}
