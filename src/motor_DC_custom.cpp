/*
 * Copyright (c) 2023, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */

// ============ THIS FILE IS APPLICATION SPECIFIC ========================

#include "motor_DC_custom.h"

namespace sixtron {

void MotorDCCustom::initHardware()
{
    // Init Motor PWM
    _dir.write(0);
    _pwm.period_us(50); // 20kHz
    _pwm.write(0.0f);

    // Init AS5047p sensor
    _sensor->init();
}

float MotorDCCustom::getSensorSpeed()
{
    _sensor->update(); // should be elsewhere ?
    return _sensor->getSpeed();
}

void MotorDCCustom::setPWM(float pwm)
{
    // update hardware motor PWM
    if (pwm >= 0.0f) {
        _dir.write(0);
    } else {
        pwm = -pwm;
        _dir.write(1);
    }

    _pwm.write(pwm);
}

} // namespace sixtron
