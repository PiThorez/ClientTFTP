#include "TFTP.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <host> <port> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *host = argv[1];
    const char *service = argv[2];
    const char *file = argv[3];

    struct addrinfo hints = {0};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;  // Use SOCK_DGRAM for UDP

    struct addrinfo *res;

    int status = getaddrinfo(host, service, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    char addrstr[INET6_ADDRSTRLEN];
    void *addr;

    if (res[0].ai_family == AF_INET) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)res[0].ai_addr;
        addr = &(ipv4->sin_addr);
    } else {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)res[0].ai_addr;
        addr = &(ipv6->sin6_addr);
    }

    inet_ntop(res[0].ai_family, addr, addrstr, sizeof(addrstr));

    printf("Address: %s\n", addrstr);

    int Socket = socket(res[0].ai_family, res[0].ai_socktype, res[0].ai_protocol);
    if (Socket == -1) {
        perror("Erreur lors de la création du socket");
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    char rrqBuffer[TAILLE_BUFFER];
    int rrqLength = snprintf(rrqBuffer, TAILLE_BUFFER, "RRQ %s", file);

    // Use sendto for sending in UDP
    sendto(Socket, rrqBuffer, rrqLength, 0, res[0].ai_addr, res[0].ai_addrlen);

    struct sockaddr_storage clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    char recvBuffer[TAILLE_BUFFER];

    // Use recvfrom for receiving in UDP
    ssize_t bytesRead = recvfrom(Socket, recvBuffer, TAILLE_BUFFER, 0,
                                  (struct sockaddr *)&clientAddr, &clientAddrLen);

    if (bytesRead == -1) {
        perror("Error receiving data packet");
        close(Socket);
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    // Process the received data (you can save it to a file, for example)
    printf("Bytes received: %zd\n", bytesRead);

    // Send acknowledgment (ACK)
    const char *ack = "ACK";

    // Use sendto for sending acknowledgment in UDP
    if (sendto(Socket, ack, strlen(ack), 0, (struct sockaddr *)&clientAddr, clientAddrLen) == -1) {
        perror("Error sending acknowledgment");
        close(Socket);
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    close(Socket);
    freeaddrinfo(res);

    return 0;
}
