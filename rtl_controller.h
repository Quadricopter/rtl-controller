#ifndef __RTL_CTL_H__
#define __RTL_CTL_H__

#include <stdint.h>

#define DEFAULT_RTL_TCP_ADDRESS "localhost"
#define DEFAULT_RTL_TCP_PORT          1234

#define FREQUENCY_HZ_MIN          52000000
#define FREQUENCY_HZ_MAX        1300000000

enum {

    COMMAND_SET_FREQUENCY = 0x01,
    COMMAND_SET_SAMPLING_RATE = 0x02,
	COMMAND_SET_GAIN_MODE = 0x03,
    COMMAND_SET_GAIN = 0x04,
    COMMAND_SET_PPM_CORRECTION = 0x05,
    COMMAND_SET_IF_GAIN = 0x06,
//    COMMAND_SET_TEST_MODE = 0x07,
    COMMAND_SET_AGC_MODE = 0x08,
//    COMMAND_SET_DIRECT_SAMPLING = 0x09,
    COMMAND_SET_OFFSET_TUNING = 0x0a,
//    COMMAND_SET_RTL_XTAL = 0x0b,
//    COMMAND_SET_TUNER_XTAL = 0x0c,
//    COMMAND_SET_TUNER_GAIN_INDEX = 0x0d,
//    COMMAND_SET_BIAS_TEE = 0x0e
};

enum {

    FLAG_SET_FREQUENCY,
    FLAG_SET_SAMPLING_RATE,
	FLAG_SET_GAIN_MODE,
    FLAG_SET_GAIN,
    FLAG_SET_PPM_CORRECTION,
    FLAG_SET_IF_GAIN,
//    FLAG_SET_TEST_MODE,
    FLAG_SET_AGC_MODE,
//    FLAG_SET_DIRECT_SAMPLING,
    FLAG_SET_OFFSET_TUNING,
//    FLAG_SET_RTL_XTAL,
//    FLAG_SET_TUNER_XTAL,
//    FLAG_SET_TUNER_GAIN_INDEX,
//    FLAG_SET_BIAS_TEE
};

typedef struct s_rtl_ctl {

    /* rtl_tcp server */
    char       *address;
    int         port;

    /* Settings */
    uint32_t    flags;
    uint32_t    frequency_hz;
    uint32_t    sampling_rate_hz;
    uint32_t    gain_mode;
    uint32_t    gain;
    int32_t     ppm;
    uint32_t    if_gain;
    uint32_t    agc_mode;
    uint32_t    offset_tuning;
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
