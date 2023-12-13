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

    int Socket = socket(res[0].ai_family, res[0].ai_socktype, res[0].ai_protocol);
    if (Socket == -1) {
        perror("Erreur lors de la création du socket");
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    if (connect(Socket, res[0].ai_addr, res[0].ai_addrlen) == -1) {
        perror("Erreur lors de la connexion au serveur");
        close(socket);
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }
    
    struct RRQPacket rrq_packet;
    rrq_packet.opcode = htons(OPCODE_RRQ);
    strncpy(rrq_packet.filename, file, RRQ_MAX_FILENAME);
    strncpy(rrq_packet.mode, "octet", RRQ_MAX_MODE);
	
	//alternative a trouver au size of du socket
    //sendto(socket, &rrq_packet,sizeof (rrq_packet), 0, res[0].ai_addr, res[0].ai_addrlen);

    close(Socket);
    freeaddrinfo(res);

    return 0;
}
