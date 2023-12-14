#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rtl_controller.h"

void    usage(FILE *dst, const char *prg_name)
{
    fprintf(dst, "Usage: %s [-a address] [-p port] OPT\n", prg_name);
    fprintf(dst, "\nThen requires AT LEAST one of the following option:\n");
    fprintf(dst, "-f frequency (Range: %d to %d Hz)\n", FREQUENCY_HZ_MIN, FREQUENCY_HZ_MAX);
    fprintf(dst, "-s sampling rate (2400000 Hz)\n");
}

int main(int argc, char *argv[])
{
    t_rtl_contoller rtl;
    int             opt;

    rtl_init(&rtl);

    /*
     * Read parameters
     */
    

    while ((opt = getopt(argc, argv, "a:p:f:s:g:P:")) != -1) {

        switch (opt) {

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

        default: /* '?' */
            usage(stderr, argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (rtl.flags == 0) {

        fprintf(stdout, "Nothing to do..\n");
        usage(stderr, argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     * Send commands
     */

    rtl_set(&rtl);

    /*
     * Close TCP connection and clean
     */

    if (rtl.address)
        free(rtl.address);

    return EXIT_SUCCESS;
}
