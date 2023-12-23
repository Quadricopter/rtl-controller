#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rtl_controller.h"

void    usage(FILE *dst, const char *prg_name)
{
    fprintf(dst, "Usage: %s [-a address] [-p port] OPT\n", prg_name);

    fprintf(dst, "\nOptional option:\n");
    fprintf(dst, "-a address (Default: %s)\n", DEFAULT_RTL_TCP_ADDRESS);
    fprintf(dst, "-p port (Default: %d)\n", DEFAULT_RTL_TCP_PORT);
    fprintf(dst, "-h Display this help\n");

    fprintf(dst, "\nThen requires AT LEAST one of the following option:\n");
    fprintf(dst, "-f frequency (Range: %d to %d Hz)\n", FREQUENCY_HZ_MIN, FREQUENCY_HZ_MAX);
    fprintf(dst, "-s sampling rate (2400000 Hz)\n");
    fprintf(dst, "-g gain (tenth of dB), 0=Auto\n");
    fprintf(dst, "-P PPM correction\n");
    fprintf(dst, "-i IF gain\n");
    fprintf(dst, "-A AGC mode [0-1]\n");
    fprintf(dst, "-o offset tuning [0-1]\n");
}

int main(int argc, char *argv[])
{
    t_rtl_contoller rtl;
    int             opt;

    rtl_init(&rtl);

    /*
     * Read parameters
     */

    while ((opt = getopt(argc, argv, "ha:p:f:s:g:P:i:A:o:")) != -1) {

        switch (opt) {

        case 'h':
            usage(stderr, argv[0]);
            rtl_release(&rtl);
            exit(EXIT_SUCCESS);
            break;

        /* Server options */
        case 'a':
            rtl.address = strdup(optarg);
            break;

        case 'p':
            rtl.port = atoi(optarg);
            break;

        /* rtl_tcp options */
        case 'f':
            rtl.frequency_hz = atoi(optarg);
            rtl.flags |= (1 << FLAG_SET_FREQUENCY);
            break;

        case 's':
            rtl.sampling_rate_hz = atoi(optarg);
            rtl.flags |= (1 << FLAG_SET_SAMPLING_RATE);
            break;

        case 'g':
           rtl.gain = (int)(atof(optarg) * 10);
           rtl.flags |= (1 << FLAG_SET_GAIN);
           break;

        case 'P':
            rtl.ppm = atoi(optarg);
            rtl.flags |= (1 << FLAG_SET_PPM_CORRECTION);
            break;

        case 'i':
            rtl.if_gain = atoi(optarg);
            rtl.flags |= (1 << FLAG_SET_IF_GAIN);
            break;

        case 'A':
            rtl.agc_mode = atoi(optarg);
            rtl.flags |= (1 << FLAG_SET_AGC_MODE);
            break;

        case 'o':
            rtl.offset_tuning = atoi(optarg);
            rtl.flags |= (1 << FLAG_SET_OFFSET_TUNING);
            break;

        default: /* '?' */
            usage(stderr, argv[0]);
            rtl_release(&rtl);
            exit(EXIT_FAILURE);
        }
    }

    if (rtl.flags == 0) {

        fprintf(stdout, "Nothing to do..\n");
        usage(stderr, argv[0]);
        rtl_release(&rtl);
        exit(EXIT_FAILURE);
    }

    /*
     * Send commands
     */

    rtl_set(&rtl);

    /*
     * Clean
     */

    rtl_release(&rtl);

    return EXIT_SUCCESS;
}
