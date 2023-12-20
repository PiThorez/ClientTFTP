#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define TAILLE_BUFFER       512
#define PACKET_SIZE         516
#define RRQ_MAX_FILENAME    255
#define RRQ_MAX_MODE        8

struct RRQPacket{
  short opcode;
  char filename[RRQ_MAX_FILENAME];
  char mode[RRQ_MAX_MODE];
};
