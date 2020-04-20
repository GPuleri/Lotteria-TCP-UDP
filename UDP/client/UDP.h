#include <WinSock2.h>		

unsigned long IP_to_bin(char ip_add[]);

int UDP_init(char ip_add[], unsigned short port_number);

int UDP_send(char* buffer, int num);

int UDP_receive(char* buffer, int size);

void UDP_close();
