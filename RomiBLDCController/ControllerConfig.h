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

// DRV8302 pins
static const int kPinPhaseA = 9;
static const int kPinPhaseB = 10;
static const int kPinPhaseC = 11;
static const int kEnablePin = 8;
static const int M_PWM = 6; 
static const int M_OC = 5;
static const int OC_ADJ = 7;

