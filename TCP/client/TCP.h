#include <WinSock2.h>

int TCPclient_connect(char ip_address[], unsigned short port_number);

int TCPclient_send(char* buffer, int num);

int TCPclient_receive(char* buffer, int size);

void TCPclient_disconnect();