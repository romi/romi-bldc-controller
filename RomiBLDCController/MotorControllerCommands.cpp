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

#include <stdio.h>
#include "MotorControllerCommands.h"
#include "MotorControllerVersion.h"

// FIXME
// #if ARDUINO
// #include "MotorControllerTests.h"
// #endif

using namespace romiserial;

const static MessageHandler handlers[] = {
        { '?', 0, false, send_info },
        { 'P', 0, false, send_position },
        { 'C', 0, false, handle_configure },
        { 'K', 0, false, handle_calibrate },
        { 'E', 1, false, handle_enable },
        { 'm', 2, false, handle_moveto },
        { 'X', 0, false, handle_stop },
};

static IMotorController *controller_ = nullptr;
static IRomiSerial *romi_serial_ = nullptr;
static char reply_buffer[100];

const char *kErrBadState = "Bad state";
const char *kErrNotImplemented = "Not implemented";

void setup_commands(IMotorController *controller, IRomiSerial *romiSerial)
{
        controller_ = controller;
        romi_serial_ = romiSerial;
        if (romi_serial_)
                romi_serial_->set_handlers(handlers, sizeof(handlers) / sizeof(MessageHandler));
}

void handle_commands()
{
        if (controller_ != nullptr
            && romi_serial_ != nullptr)
                romi_serial_->handle_input();
}

void send_info(IRomiSerial *romiSerial, int16_t *, const char *)
{
        romiSerial->send("[0,\"RomiBLDCController\",\"" kMotorControllerVersion "\","
                         "\"" __DATE__ " " __TIME__ "\"]"); 
}

void send_position(IRomiSerial *romiSerial, int16_t *, const char *)
{
        float angle;
        int32_t angle_x1000;
        uint32_t time;
        controller_->get_position(angle, time);
        angle_x1000 = (int32_t) (angle * 1000.0f);
        
#if ARDUINO
        snprintf(reply_buffer, sizeof(reply_buffer),
                 "[0,%ld,%lu]", angle_x1000, time);
#else
        snprintf(reply_buffer, sizeof(reply_buffer),
                 "[0,%d,%u]", angle_x1000, time);
#endif

        romiSerial->send(reply_buffer);                
}

void handle_configure(IRomiSerial *romiSerial, int16_t *, const char *)
{
        bool success;
        // MotorControllerConfiguration config;

        // // Encoders
        // config.encoder_steps = (uint16_t) args[0];
        // config.left_direction = args[1] > 0? 1 : -1;
        // config.right_direction = args[2] > 0? 1 : -1;

        // // Envelope
        // config.max_speed = (double) args[3] / 1000.0f;
        // config.max_acceleration = (double) args[4] / 1000.0f;

        // // PI controller
        // config.kp_numerator = args[5];
        // config.kp_denominator = args[6];
        // config.ki_numerator = args[7];
        // config.ki_denominator = args[8];
        // config.max_amplitude = args[9];
                
        success = controller_->configure();
        if (success) {
                romiSerial->send_ok();  
        } else {
                romiSerial->send_error(1, kErrBadState);  
        }
}

void handle_calibrate(IRomiSerial *romiSerial, int16_t *, const char *)
{
        bool success;
                
        success = controller_->calibrate();
        if (success) {
                romiSerial->send_ok();  
        } else {
                romiSerial->send_error(1, kErrBadState);  
        }
}

void handle_enable(IRomiSerial *romiSerial, int16_t *args, const char *)
{
        bool success;
        if (args[0] == 0) {
                success = controller_->disable();
        } else {
                success = controller_->enable();
        }
        if (success) {
                romiSerial->send_ok();  
        } else {
                romiSerial->send_error(1, kErrBadState);
        }
}

void handle_moveto(IRomiSerial *romiSerial, int16_t *args, const char *)
{
        // Set the new angular position. The angle_radians expresses
        // the position in integral radians, and angle_1000th in
        // 1/1000th of a radian (~ 0.0573°). So the arguments to be
        // transmitted for an angle of -1.2 are (-1, -200), since -1 +
        // (-200/1000) = -1.2.
        int16_t angle_radians = (int16_t) args[0];
        int16_t angle_1000th = (int16_t) args[1];
        float angle = (float) angle_radians + (float) angle_1000th / 1000.0f;
        bool success = controller_->moveto(angle);
        if (success) {
                romiSerial->send_ok();  
        } else {
                romiSerial->send_error(1, kErrBadState);
        }
}

void handle_moveat(IRomiSerial *romiSerial, int16_t *args, const char *)
{
        romiSerial->send_error(2, kErrNotImplemented);
}

void handle_stop(IRomiSerial *romiSerial, int16_t *, const char *)
{
        controller_->stop();
        romiSerial->send_ok();  
}
