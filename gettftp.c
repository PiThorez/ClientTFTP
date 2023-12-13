#include "TFTP.h"

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <host> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *host = argv[1];
    const char *service = argv[2];
    const char *file = argv[3];
    

    struct addrinfo hints = {0};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

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

    freeaddrinfo(res);

    return 0;
}