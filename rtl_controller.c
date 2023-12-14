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
    fprintf(stdout, "rtl_tcp server: %s %d\n", rtl->address, rtl->port);

    /*
     * Open TCP connection
     */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    hostinfo = gethostbyname(rtl->address);
    if (hostinfo == NULL) {

        fprintf(stderr, "Unknown host %s.\n", rtl->address);
        close(sockfd);
        return -1;
    }

    memset(&sin, 0, sizeof(struct sockaddr_in));
    memcpy(&sin.sin_addr.s_addr, hostinfo->h_addr, hostinfo->h_length);
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
        rtl_send_command(sockfd, COMMAND_SET_SAMLING_RATE, rtl->sampling_rate_hz);
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
