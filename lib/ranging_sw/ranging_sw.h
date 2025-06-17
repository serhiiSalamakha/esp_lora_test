/*
 * ranging_sw.h - software ranging header file
 */

#ifndef RANGING_SW_H
#define RANGING_SW_H

#include <stdint.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RANGING_SW_ERR_NONE          (0)
#define RANGING_SW_ERR_RX_TIMEOUT    (-1)

typedef uint64_t (*ranging_sw_get_time_func)(void);
typedef int (*ranging_sw_send_func)(const uint8_t* data, uint16_t len);
typedef int (*ranging_sw_receive_func)(uint8_t* data, uint16_t max_len);

struct ranging_sw_data {
    ranging_sw_get_time_func get_time_us;
    ranging_sw_send_func send;
    ranging_sw_receive_func receive;
    uint32_t slave_processing_delay_us;
    float speed_of_light;
};

void ranging_sw_init(struct ranging_sw_data* data, ranging_sw_get_time_func get_time_us,
    ranging_sw_send_func send, ranging_sw_receive_func receive);
float ranging_sw_master_get_dist(struct ranging_sw_data* data);
int ranging_sw_slave_handle_request(struct ranging_sw_data* data);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* RANGING_SW_H */
