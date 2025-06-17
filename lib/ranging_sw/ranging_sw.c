/*
 * ranging_sw.h - software ranging functionality
 */

#include <ranging_sw.h>

#define RANGING_SW_REQ_CMD_ID       0x10
#define RANGING_SW_RESP_CMD_ID      0x11

#define RANGING_SW_INVALID_VALUE    (-1.0f)

void ranging_sw_init(struct ranging_sw_data* data, ranging_sw_get_time_func get_time_us,
    ranging_sw_send_func send, ranging_sw_receive_func receive)
{
    data->get_time_us = get_time_us;
    data->send = send;
    data->receive = receive;
}

float ranging_sw_master_get_dist(struct ranging_sw_data* data)
{
    int err;
    uint8_t req_cmd = RANGING_SW_REQ_CMD_ID;
    uint8_t resp_cmd = 0;
    uint64_t tx_time;
    uint64_t rx_time;
    uint64_t rtt_us;

    if (!data || !data->get_time_us || !data->send || !data->receive) {
        return RANGING_SW_INVALID_VALUE;
    }

    tx_time = data->get_time_us();
    err = data->send(&req_cmd, sizeof(req_cmd));
    if (err < 0) {
        return RANGING_SW_INVALID_VALUE;
    }

    err = data->receive(&resp_cmd, sizeof(resp_cmd));
    rx_time = data->get_time_us();
    if (err < 0 || resp_cmd != RANGING_SW_RESP_CMD_ID) {
        return RANGING_SW_INVALID_VALUE;
    }

    rtt_us = rx_time - tx_time;

    /* TODO*/

    /*
    if (rtt_us <= data->slave_processing_delay_us) {
        return -1.0f;
    }

    uint32_t tof_us = (rtt_us - data->slave_processing_delay_us) / 2;

    // d = ToF * c
    float tof_s = tof_us * 1e-6f;
    float distance_m = data->speed_of_light * tof_s;

    return distance_m;
    */
    return 0;
}

int ranging_sw_slave_handle_request(struct ranging_sw_data* data)
{
    int err;
    uint8_t req_cmd = 0;
    uint8_t resp_cmd = RANGING_SW_RESP_CMD_ID;

    err = data->receive(&req_cmd, sizeof(req_cmd));
    if (err < 0) {
        return -ETIMEDOUT;
    }

    if (req_cmd != RANGING_SW_REQ_CMD_ID) {
        return -EINVAL;
    }

    err = data->send(&resp_cmd, sizeof(resp_cmd));
    if (err < 0) {
        return -EIO;
    }

    return 0;
}
