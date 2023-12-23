/*
 * References:
 * https://hz.tools/rtl_tcp/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "rtl_controller.h"

int rtl_init(t_rtl_contoller *rtl)
{
    memset(rtl, 0, sizeof(t_rtl_contoller));
    rtl->port = DEFAULT_RTL_TCP_PORT;

    return 0;
}

int rtl_send_command(int sockfd, uint8_t command, uint32_t value)
{
    char        buff[8];
    uint32_t    data = 0;

    buff[0] = command;
    data = htonl(value);
    memcpy(&buff[1], &data, 4);

    return write(sockfd, &buff, 5);
}

int rtl_set(t_rtl_contoller *rtl)
{
    int                 sockfd = 0;
    struct hostent     *hostinfo = NULL;
    struct sockaddr_in  sin;
    t_dongle_info       dongle_info;

    if (!rtl->address)
        rtl->address = strdup(DEFAULT_RTL_TCP_ADDRESS);
    fprintf(stdout, "rtl_tcp server: %s:%d\n", rtl->address, rtl->port);

    /*
     * Open TCP connection
     */

    hostinfo = gethostbyname(rtl->address);
    if (hostinfo == NULL) {

        fprintf(stderr, "Unknown host %s.\n", rtl->address);
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&sin, 0, sizeof(struct sockaddr_in));
    memcpy(&sin.sin_addr.s_addr, hostinfo->h_addr_list[0], hostinfo->h_length);
    sin.sin_port = htons(rtl->port);
    sin.sin_family = AF_INET;

    if (connect(sockfd, (struct sockaddr*) &sin, sizeof(sin)) != 0) {

        perror("connect()");
        close(sockfd);
        return -1;
    }

    /*
     * Read dongle info
     */

    read(sockfd, &dongle_info, sizeof(t_dongle_info));
    if (strncmp(dongle_info.magic, "RTL0", 4)) {

        fprintf(stderr, "Not talking to a rtl_tcp server!\n");
        close(sockfd);
        return -1;
    }

    /*
     * Send commands
     */

    if (rtl->flags & (1 << FLAG_SET_FREQUENCY)) {

        fprintf(stdout, "Set frequency: %d Hz\n", rtl->frequency_hz);
        rtl_send_command(sockfd, COMMAND_SET_FREQUENCY, rtl->frequency_hz);
    }

    if (rtl->flags & (1 << FLAG_SET_SAMPLING_RATE)) {

        fprintf(stdout, "Set sampling rate: %d Hz\n", rtl->sampling_rate_hz);
        rtl_send_command(sockfd, COMMAND_SET_SAMPLING_RATE, rtl->sampling_rate_hz);
    }

    if (rtl->flags & (1 << FLAG_SET_GAIN_MODE)) {

        fprintf(stdout, "Set gain mode: %d\n", rtl->gain_mode);
        rtl_send_command(sockfd, COMMAND_SET_GAIN_MODE, rtl->gain_mode);
    }

    if (rtl->flags & (1 << FLAG_SET_GAIN)) {

        fprintf(stdout, "Set gain: %d dB\n", rtl->gain/10);

        if (rtl->gain == 0) {
            rtl_send_command(sockfd, COMMAND_SET_GAIN_MODE, 0);
        }
        else {
            rtl_send_command(sockfd, COMMAND_SET_GAIN_MODE, 1);
            rtl_send_command(sockfd, COMMAND_SET_GAIN, rtl->gain);
        }
    }

    if (rtl->flags & (1 << FLAG_SET_PPM_CORRECTION)) {

        fprintf(stdout, "Set freq correction: %d PPM\n", rtl->ppm);
        rtl_send_command(sockfd, COMMAND_SET_PPM_CORRECTION, rtl->ppm);
    }

    if (rtl->flags & (1 << FLAG_SET_IF_GAIN)) {

        fprintf(stdout, "Set IF stage %d gain %d\n", rtl->if_gain >> 16, rtl->if_gain & 0x0000FFFF);
        rtl_send_command(sockfd, COMMAND_SET_IF_GAIN, rtl->if_gain);
    }

    if (rtl->flags & (1 << FLAG_SET_AGC_MODE)) {

        fprintf(stdout, "Set AGC mode: %d\n", rtl->agc_mode);
        rtl_send_command(sockfd, COMMAND_SET_AGC_MODE, rtl->agc_mode);
    }

    if (rtl->flags & (1 << FLAG_SET_OFFSET_TUNING)) {

        fprintf(stdout, "Set offset tuning: %d\n", rtl->offset_tuning);
        rtl_send_command(sockfd, COMMAND_SET_OFFSET_TUNING, rtl->offset_tuning);
    }

    /*
     * Close TCP connection
     */

    close(sockfd);

    return 0;
}

int rtl_release(t_rtl_contoller *rtl)
{
    if (rtl->address) {

        free(rtl->address);
        rtl->address = NULL;
    }

    return 0;
}
