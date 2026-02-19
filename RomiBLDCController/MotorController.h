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
#pragma once

#include "IMotorController.h"
#include <SimpleFOC.h>

class MotorController : public IMotorController
{
public:
        /*
          States:                  
                      [created]    -(setup)->     [setup] 
                      [setup]      -(configure)-> [configured] 
                      [configured] -(calibrate)-> [calibrated] 
                      [calibrated] -(enable)->    [enabled] 
                      [enabled]    -(disable)->   [disabled]
                      [disabled]   -(enable)->    [enabled]
        */
        enum MotorControllerState {
                kCreated = 0,
                kSetUp,
                kConfigured,
                kCalibrated,
                kEnabled,
                kDisabled,
                kError
        };
        
public:
        BLDCMotor motor_;
        BLDCDriver3PWM driver_;
        Encoder sensor_;
        MotorControllerState state_;

        bool is_created();
        bool is_set_up();
        bool is_configured();
        bool is_calibrated();
        bool is_enabled();
        bool is_disabled();
        bool is_error();
        void do_update();
        
public:
        
        MotorController();
        ~MotorController() override = default;
        
        bool setup() override;
        bool configure() override;
        bool calibrate() override;
        bool enable() override;
        bool disable() override;
        bool moveto(float angle) override;
        void get_position(float& radians, uint32_t& time) override;
        bool update() override;
        void stop() override;
};

