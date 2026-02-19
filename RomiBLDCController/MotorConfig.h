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

static const int kNumPolesPairs = 7;
static const int kVoltageSupply = 24;
static const constexpr float kPhaseResistance = 0.039;     
static const constexpr float kPhaseInductance = 0.000024;  
static const int kVoltageLimit = 20;            
static const int kCurrentLimit = 15;           
static const int kVelocityLimit = 15;  // in what units?
static const int kVoltageSensorAlign = 6; // ???

