/*
 * error.h - error handling file 
 */

#ifndef ERROR_H
#define ERROR_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

inline void error_event_handler(const char* context, int err)
{
    Serial.printf("[ERROR] %s (err = %d)\n", context, err);

    while (true) {
        delay(1);
    }
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* ERROR_H */
