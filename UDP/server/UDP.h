#include <WinSock2.h>		

unsigned long IP_to_bin(char* ip_address);

int UDP_init(int port_number);

int UDP_send(unsigned long ip, unsigned short port_number, char* buffer, int byte);

int UDP_receive(unsigned long ip, unsigned short port_number, char* buffer, int size);

void UDP_close();
