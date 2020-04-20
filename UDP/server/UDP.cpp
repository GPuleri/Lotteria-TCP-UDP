#include "UDP.h"

SOCKET socket_id;

unsigned long IP_to_bin(char* ip_address)
{
	unsigned long add;
	unsigned char byte;
	char *token;
	char *nextoken;

	if ((token = strtok_s(ip_address, ".", &nextoken)) == NULL)
		return 0x00000000;

	byte = (unsigned char)atoi(token);
	add = (unsigned long)byte * 16777216;

	if ((token = strtok_s(NULL, ".", &nextoken)) == NULL)
		return 0x00000000;

	byte = (unsigned char)atoi(token);
	add += (unsigned long)byte * 65536;

	if ((token = strtok_s(NULL, ".", &nextoken)) == NULL)
		return 0x00000000;

	byte = (unsigned char)atoi(token);
	add += (unsigned long)byte * 256;

	if ((token = strtok_s(NULL, ".", &nextoken)) == NULL)
		return 0x00000000;

	byte = (unsigned char)atoi(token);
	add += (unsigned long)byte * 1;

	return add;
}

int UDP_init(int port_number)
{
	WSADATA wsaData;
	struct sockaddr_in add;	
	unsigned long arg = 1;

	if (WSAStartup(0x0202, &wsaData) != 0)
	{
		socket_id = INVALID_SOCKET;
		return -1;
	}

	if ((socket_id = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		WSACleanup();
		return -1;
	}

	memset(&add, 0, sizeof(add));	
	add.sin_family = AF_INET;		
	add.sin_addr.s_addr = 0;	
	add.sin_port = htons(port_number); 
	if (bind(socket_id, (struct sockaddr*)&add, sizeof(add)) == SOCKET_ERROR)
	{
		closesocket(socket_id);
		WSACleanup();
		socket_id = INVALID_SOCKET;
		return -1;
	}

	if (ioctlsocket(socket_id, FIONBIO, &arg) == SOCKET_ERROR)
	{
		closesocket(socket_id);
		WSACleanup();	
		socket_id = INVALID_SOCKET;
		return -1;
	}


	return 0;

}

int UDP_send(unsigned long ip, unsigned short port_number, char* buffer, int byte)
{
	struct sockaddr_in add;
	int n;

	if (socket_id == INVALID_SOCKET)
		return -1;

	memset(&add, 0, sizeof(add));	
	add.sin_family = AF_INET;	
	add.sin_port = htons(port_number); 
	add.sin_addr.s_addr = htonl(ip);

	if ((n = sendto(socket_id, (char*)buffer, byte, 0, (struct sockaddr*)&add, sizeof(add))) < 0)
		return -2;

	return n;

} 

int UDP_receive(unsigned long ip, unsigned short port_number, char* buffer, int size)
{
	struct sockaddr_in add;
	int dim = sizeof(add);
	int n;

	if (socket_id == INVALID_SOCKET)
		return -1;

	if ((n = recvfrom(socket_id, (char *)buffer, size, 0, (struct sockaddr*)&add, &dim)) <= 0)
		return -2;
	
	

	return n;

} 

void UDP_close()
{
	closesocket(socket_id);
	WSACleanup();
}
