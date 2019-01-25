#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080

void send_msg(char *message);

int main() {
    char string[32] = {};
    for (int i = 1; i <= 25; i++) {
        sprintf(string, "%s%d", "Hello Server ", i);
        send_msg(string);
        memset(&string, 0, 32);
    }
    send_msg("exit");
    return EXIT_SUCCESS;
}

void send_msg(char *message) {
    uint16_t server_port = htons(PORT);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("Error! Socket creation failed");
        return;
    }

    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = 0x0100007F;
    server_address.sin_port = server_port;

    printf("Connecting to IP %#08X, %d\n", server_address.sin_addr.s_addr, server_address.sin_port);
    int connected = connect(sock, (const struct sockaddr *) &server_address, sizeof(server_address));

    if (connected == -1) {
        perror("Error! Connection failed");
        return;
    }


    send(sock, message, strlen(message), 0);
    printf("Sent %s\n", message);
    shutdown(connected, SHUT_RDWR);
}