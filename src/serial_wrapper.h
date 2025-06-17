/*
 * serial_wrapper.h
 */

#ifndef ERROR_H
#define ERROR_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

void s_printf(const char* format, ...);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* ERROR_H */
