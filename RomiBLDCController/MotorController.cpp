/*
  Romi motor controller for brushless motors

  Copyright (C) 2026 Sony Computer Science Laboratories
  Author(s) Ayman Laaroussi, Peter Hanappe

  MotorController is free software: you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */

#include "ControllerConfig.h"
#include "MotorConfig.h"
#include "EncoderConfig.h"
#include "MotorController.h"

MotorController::MotorController()
        : motor_(kNumPolesPairs),
          driver_(kPinPhaseA, kPinPhaseB, kPinPhaseC, kEnablePin),
          sensor_(kEncoderPinA, kEncoderPinB, kEncoderResolution),
          state_(kCreated)
{
}

bool MotorController::is_created()
{
        return state_ == kCreated;
}

bool MotorController::is_set_up()
{
        return state_ == kSetUp;
}

bool MotorController::is_configured()
{
        return state_ == kConfigured;
}

bool MotorController::is_calibrated()
{
        return state_ == kCalibrated;
}

bool MotorController::is_enabled()
{
        return state_ == kEnabled;
}

bool MotorController::is_disabled()
{
        return state_ == kDisabled;
}

static Encoder *g_encoder_ = nullptr; 

void doA()
{
        if (g_encoder_)
                g_encoder_->handleA();
}

void doB()
{
        if (g_encoder_)
                g_encoder_->handleB();
}

bool MotorController::setup()
{
        bool success = false;
        if (is_created()) {
                sensor_.init();
                g_encoder_ = &sensor_;
                sensor_.enableInterrupts(doA, doB);
                motor_.linkSensor(&sensor_);
        
                // DRV8302 configuration
                pinMode(M_OC, OUTPUT);
                digitalWrite(M_OC, LOW);
                pinMode(M_PWM, OUTPUT);
                digitalWrite(M_PWM, HIGH);
                pinMode(OC_ADJ, OUTPUT);
                digitalWrite(OC_ADJ, HIGH);
                pinMode(kEnablePin, OUTPUT);
                digitalWrite(kEnablePin, HIGH);
  
                state_ = kSetUp;
                success = true;
        }
        return success;
}

bool MotorController::configure()
{
        bool success = false;
        if (is_set_up()) {
        
                // Driver
                driver_.voltage_power_supply = kVoltageSupply;
                driver_.init();
                motor_.linkDriver(&driver_);
  
                // Motor config
                motor_.phase_resistance = 0.039;     
                motor_.phase_inductance = 0.000024;  
                motor_.voltage_limit = 20;            
                motor_.current_limit = 15;           
                motor_.velocity_limit = 15;
                motor_.voltage_sensor_align = 6;
  
                // Default to position control
                motor_.controller = MotionControlType::angle;
                motor_.P_angle.P = 15;
                motor_.P_angle.D = 0.1;
                // For velocity control
                motor_.PID_velocity.P = 0.1;
                motor_.PID_velocity.I = 5;
                motor_.LPF_velocity.Tf = 0.02;
                motor_.foc_modulation = FOCModulationType::SpaceVectorPWM;
  
                motor_.init();
                motor_.target = 0;

                motor_.disable();  // Start disabled, wait for ENABLE command

                state_ = kConfigured;
                success = true;
        }
                
        return success;
}

bool MotorController::calibrate()
{
        bool success = false;
        if (is_configured()) {
                motor_.initFOC();
                state_ = kCalibrated;
        }
        return success;
}

bool MotorController::enable()
{
        bool success = false;
        if (is_calibrated() || is_disabled()) {
                stop();
                motor_.enable();
                state_ = kEnabled;
                success = true;
        }
        return success;
}

bool MotorController::disable()
{
        bool success = true;
        stop();
        if (is_enabled()) {
                motor_.disable();
                state_ = kDisabled;
                success = true;
        }
        return success;
}

bool MotorController::update()
{
        bool updated = false;
        if (is_enabled()) {
                sensor_.update();
                motor_.loopFOC();
                motor_.move();
                updated = true;
        }
        return updated;
}

bool MotorController::moveto(float angle)
{
        motor_.target = angle;
        return true;
}

void MotorController::get_position(float& angle, uint32_t& time)
{
        // TODO
        angle = 0.0f;
}

void MotorController::stop()
{
        // TODO
}
