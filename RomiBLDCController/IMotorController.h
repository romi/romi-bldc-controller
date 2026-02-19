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

#include <stdint.h>

class IMotorController
{
public:
        virtual ~IMotorController() = default;
        
        virtual bool setup() = 0;
        virtual bool configure() = 0;
        virtual bool calibrate() = 0;
        virtual bool enable() = 0;
        virtual bool disable() = 0;
        virtual void stop() = 0;
        
        // Set the new angular position, in radians.
        virtual bool moveto(float angle_radians) = 0;
        
        // Returns true if an update was performed, false otherwise. 
        virtual bool update() = 0;

        virtual void get_position(float& radians, uint32_t& time) = 0;
};

