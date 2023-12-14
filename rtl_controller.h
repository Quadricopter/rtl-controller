#ifndef __RTL_CTL_H__
#define __RTL_CTL_H__

#include <stdint.h>

#define DEFAULT_RTL_TCP_ADDRESS "localhost"
#define DEFAULT_RTL_TCP_PORT          1234

#define FREQUENCY_HZ_MIN          52000000
#define FREQUENCY_HZ_MAX        1300000000

enum {

    COMMAND_SET_FREQUENCY = 0x01,
    COMMAND_SET_SAMLING_RATE = 0x02
};

enum {

    FLAG_SET_FREQUENCY,
    FLAG_SET_PPM_CORRECTION,
    FLAG_SET_SAMPLING_RATE,
    FLAG_SET_GAIN
};

typedef struct s_rtl_ctl {

    /* rtl_tcp server */
    char       *address;
    int         port;

    /* Settings */
    uint32_t    flags;
    uint32_t    frequency_hz;
    uint32_t    sampling_rate_hz;
}   t_rtl_contoller;

typedef struct s_dongle_info {
    char        magic[4];
    uint32_t    tuner_type;
    uint32_t    tuner_gain_type;
}   t_dongle_info;

/*
 *
 */

#ifdef __cplusplus
#extern "C" {
#endif

int rtl_init(t_rtl_contoller*);
int rtl_set(t_rtl_contoller*);
int rtl_release(t_rtl_contoller*);

#ifdef __cplusplus
}
#endif

#endif /* __RTL_CTL_H__ */
