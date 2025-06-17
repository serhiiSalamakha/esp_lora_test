/*
 * serial_wrapper.cpp
 */

#include <Arduino.h>

extern "C" void s_printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    Serial.printf(format, args);
    va_end(args);
}
