#include "UDP.h"

SOCKET socket_id;
struct sockaddr_in add;

unsigned long IP_to_bin(char ip_add[])
{
	unsigned long add;
	unsigned char byte;
	char *token;
	if ((token = strtok(ip_add,".")) == NULL)
		return 0x00000000;
	byte = (unsigned char)atoi(token);
	add = (unsigned long)byte * 16777216;
	if ((token = strtok(NULL,".")) == NULL)
		return 0x00000000;
	byte = (unsigned char)atoi(token);
	add += (unsigned long)byte * 65536;
	if ((token = strtok(NULL,".")) == NULL)
		return 0x00000000;
	byte = (unsigned char)atoi(token); 
	add += (unsigned long)byte * 256;
	if ((token = strtok(NULL,".")) == NULL)
		return 0x00000000;
	byte = (unsigned char)atoi(token);
	add += (unsigned long)byte * 1;
	return add;
}

int UDP_init(char ip_add[], unsigned short port_number) 
{
	unsigned long binary_address = IP_to_bin(ip_add); // trasformazione indirizzo ip in binario
	unsigned long arg = 1;
	WSADATA wsaData;	
	
	if (WSAStartup(0x0202, &wsaData) != 0) // inizializzazione WinSock (versione 2.2)
		return -1;
	
	memset(&add, 0, sizeof(add)); // azzeramento struttura
	add.sin_family = AF_INET; // dominio indirizzi IP
	add.sin_addr.s_addr = htonl(binary_address); // indirizzo IP server
	add.sin_port = htons(port_number); // numero di porta server
	
	if ((socket_id = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==INVALID_SOCKET) // apertura socket UDP
	{
		WSACleanup();
		return -1;
	}
	
	if (bind(socket_id, (struct sockaddr*)&add, sizeof(add)) == SOCKET_ERROR) //associazione socket a numero di porta
	{
		closesocket(socket_id);
		WSACleanup();
		socket_id = INVALID_SOCKET;
		return -1;
	}

	/*if (ioctlsocket(socket_id, FIONBIO, &arg) == SOCKET_ERROR) // impostazione socket non bloccante
	{
		closesocket(socket_id);
		WSACleanup();	
		socket_id = INVALID_SOCKET;
		return -1;
	}
	*/
	return 0; 
}

int UDP_send(char* buffer, int num)
{
	int n;
	if ((n = sendto(socket_id, (char*)buffer, num, 0, (struct sockaddr*)&add, sizeof(add)))<0) // trasmissione dati al server
		return -1;
	return n; 
} 

int UDP_receive(char* buffer, int size)
{
	int dim=sizeof(add);
	int n;
	
	if ((n = recvfrom(socket_id, buffer, size, 0, (struct sockaddr*)&add, &dim))<=0) // ricezione dati dal server
		return -1; 
	return n; 
}

void UDP_close()
{
	closesocket(socket_id);
	WSACleanup();
}
