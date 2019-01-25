#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PORT 8080

int main() {

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("Error! Socket creation failed");
        return EXIT_FAILURE;
    }

    int option_value = 1;
    int set_sock_opt = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option_value,
                                  sizeof(option_value));

    if (set_sock_opt == -1) {
        perror("Error! Failed to set socket options");
        return EXIT_FAILURE;
    }

    int bound_socket = bind(server_fd, (const struct sockaddr *) &address, sizeof(address));

    if (bound_socket == -1) {
        perror("Error! Bind failed");
        return EXIT_FAILURE;
    }

    printf("Bound to IP 0x%#08X, %d\n", address.sin_addr.s_addr, address.sin_port);

    int listen_socket = listen(server_fd, 3);

    if (listen_socket == -1) {
        perror("Error! Listen failed");
        return EXIT_FAILURE;
    }

    socklen_t address_size = sizeof(address);

    int handling_socket = accept(server_fd, (struct sockaddr *) &address, &address_size);

    if (handling_socket == -1) {
        perror("Error! Failed to accept");
        return EXIT_FAILURE;
    }

    _Bool recv_error = true;

    while (handling_socket != -1) {

        char buffer[32] = {};

        printf("Accepted connection on %#08X, %d\n", address.sin_addr.s_addr, address.sin_port);
        ssize_t recv_success = recv(handling_socket, buffer, 32, 0);

        if (recv_success == -1) {
            perror("Error! Failed to receive");
            return EXIT_FAILURE;
        }

        printf("%s\n", buffer);

        if (strcmp(buffer, "exit") == 0){
            recv_error = false;
            break;
        }

        handling_socket = accept(server_fd, (struct sockaddr *) &address, &address_size);
    }

    if (!recv_error) {
        printf("Exited gracefully\n");
    }

    return EXIT_SUCCESS;
}