# 1 "c:\\Users\\dell\\Desktop\\Mega2560\\Summer_Car\\Summer_Car.ino"
# 2 "c:\\Users\\dell\\Desktop\\Mega2560\\Summer_Car\\Summer_Car.ino" 2
# 3 "c:\\Users\\dell\\Desktop\\Mega2560\\Summer_Car\\Summer_Car.ino" 2
# 4 "c:\\Users\\dell\\Desktop\\Mega2560\\Summer_Car\\Summer_Car.ino" 2






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
# 27 "c:\\Users\\dell\\Desktop\\Mega2560\\Summer_Car\\Summer_Car.ino"
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
    error = ps2x.config_gamepad(12 /* 17*/, 11 /* 15*/, 10 /* 16*/, 13 /* 14*/, true, true);
    if (error == 0)
    {
        Serial.print("Found Controller, configured successful ");
    }
    else
    {
        Serial.print("error");
        Serial.print(error);
    }
    pinMode(21,0x1);
    water_off();

}

void loop()
{

    if (error == 1)
    {
        resetFunc();
    }
    while (init_flag) //读取几次数据 但不作处理
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
        if (ps2x.Button(0x0002))
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

        if (ps2x.Button(0x0004))
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

        if (ps2x.Button(0x0100))
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
        if (ps2x.Button(0x0200))
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
        if (ps2x.Button(0x1000))
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
    if (ps2x.ButtonPressed(0x2000)) // will be TRUE if button was JUST pressed
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
    if (ps2x.NewButtonState(0x4000)) // will be TRUE if button was JUST pressed OR released
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

    if (ps2x.ButtonReleased(0x8000)) // will be TRUE if button was JUST released
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

    if (ps2x.Button(0x0400) || ps2x.Button(0x0800))
    {
        if (ps2x.Analog(8) < 5)
        {
            device1_rollout();
        }
        if (ps2x.Analog(8) > 245)
        {
            device1_return();
        }
        if (ps2x.Analog(5) < 5)
        {
            device2_horizen();
        }
        if (ps2x.Analog(5) > 245)
        {
            device2_vertical();
        }


    }
    if(ps2x.Button(0x0001))
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
        step_motor_single(14 /*分苗*/, 15, step1.dir,500);
    }
    else if (step2.mode == STEP_ON)
    {
        Serial.println(step2.mode);
        step_motor_single(16 /*左右*/, 17, step2.dir,300);
    }
    else if (step3.mode == STEP_ON)
    {
        Serial.println("step3");
        Serial.println(step3.mode);
        step_motor_single(18 /*上下*/, 19, step3.dir,150);
    }

    delay(10);
}
