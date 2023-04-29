/*
 * Copyright (c) 2023, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "motor_DC_custom.h"
#include "motor_sensor_AS5047p.h"

// AS5047 Encoder parameters
#define ENC_RESOLUTION 16384
#define MOTOR_REDUCTION 50
#define ENC_WHEEL_RADIUS 0.035f
SPI spiAS5047p(PA_7, PA_6, PA_5); // mosi, miso, sclk
#define AS5047_CS PA_8
sixtron::MotorSensorAS5047P *my_AS5047_sensor;

// Motor MBED config
#define MOTOR_UPDATE_RATE 1ms
#define MOTOR_FLAG 0x01
#define MOTOR_DIR PH_1
#define MOTOR_PWM PB_7
Ticker MotorUpdateTicker;
EventFlags MotorFlag;
Thread motorThread(osPriorityNormal);
sixtron::MotorDCCustom *my_custom_DC_motor;
bool motor_init_done = false;

void MotorFlagUpdate()
{
    MotorFlag.set(MOTOR_FLAG);
}

void motorThreadMain()
{
    // First, convert the rate of the loop in seconds [float]
    auto f_secs = std::chrono::duration_cast<std::chrono::duration<float>>(MOTOR_UPDATE_RATE);
    float dt_pid = f_secs.count();

    // Create AS5047p sensor
    my_AS5047_sensor = new sixtron::MotorSensorAS5047P(&spiAS5047p,
            AS5047_CS,
            dt_pid,
            ENC_RESOLUTION,
            ENC_RESOLUTION * MOTOR_REDUCTION,
            ENC_WHEEL_RADIUS,
            DIR_INVERTED);

    // Create a motor object
    sixtron::PID_params pid_motor_params;
    pid_motor_params.Kp = 15.0f;
    pid_motor_params.Ki = 40.0f;
    pid_motor_params.Kd = 0.00f;
    pid_motor_params.dt_seconds = dt_pid;
    pid_motor_params.ramp = 1.0f * dt_pid;

    my_custom_DC_motor = new sixtron::MotorDCCustom(
            dt_pid, my_AS5047_sensor, MOTOR_DIR, MOTOR_PWM, pid_motor_params, 0.5f);

    // Init motor (will init sensor as well)
    my_custom_DC_motor->init();
    motor_init_done = true;

    while (true) {
        // wait for the flag trigger
        MotorFlag.wait_any(MOTOR_FLAG);

        // Update motor
        my_custom_DC_motor->update();
    }
}

// Just for the debug
void set_motor_target(float speed_ms)
{
    printf("Applying %2.3f m/s to the motor.\n", speed_ms);
    my_custom_DC_motor->setSpeed(speed_ms);
}

int main()
{
    // Start the thread for motor control
    motorThread.start(motorThreadMain);

    // Setup ticker to update the motor base flag at exactly the defined rate
    MotorUpdateTicker.attach(&MotorFlagUpdate, MOTOR_UPDATE_RATE);

    printf("Waiting for the motor to be setup ...\n");
    while (!motor_init_done)
        ;
    printf("Motor init done, continue with setting targets.\n");

    while (true) {

        set_motor_target(0.2f);
        ThisThread::sleep_for(3s);

        set_motor_target(0.0f);
        ThisThread::sleep_for(3s);

        set_motor_target(-0.2f);
        ThisThread::sleep_for(3s);

        set_motor_target(0.5f);
        ThisThread::sleep_for(3s);

        set_motor_target(-0.5f);
        ThisThread::sleep_for(3s);
    }
}
