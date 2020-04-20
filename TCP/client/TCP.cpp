#include "TCP.h"

SOCKET socket_id; // identificatore del socket

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

int TCPclient_connect(char ip_address[], unsigned short port_number) 
{
	unsigned long binary_address = IP_to_bin(ip_address); // trasformazione indirizzo ip in binario
	struct sockaddr_in add; // struttura per indirizzo
	unsigned long arg = 1;
	WSADATA wsaData;	
	
	if (WSAStartup(0x0202, &wsaData) != 0) // inizializzazione WinSock (versione 2.2)
		return -1;
	
	memset(&add, 0, sizeof(add)); // azzeramento struttura
	add.sin_family = AF_INET; // dominio indirizzi IP
	add.sin_addr.s_addr = htonl(binary_address); // indirizzo IP server
	add.sin_port = htons(port_number); // numero di porta server
	
	if ((socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==INVALID_SOCKET) // apertura socket TCP
	{
		WSACleanup();
		return -1;
	}
	
	if (connect(socket_id, (struct sockaddr*)&add, sizeof(add)) == SOCKET_ERROR) // connessione al socket del server
	{
		closesocket(socket_id);
		WSACleanup();
		return -1;
	}
	
	if (ioctlsocket(socket_id, FIONBIO, &arg) == SOCKET_ERROR) // impostazione socket non bloccante
	{
		closesocket(socket_id);
		WSACleanup();
		return -1; 
	} 

	return 0; 
}

int TCPclient_receive(char* buffer, int size)
{
	int n;
	if ((n = recv(socket_id, buffer, size, 0))<=0) // ricezione dati dal server
		return -1; 
	return n; 
}

int TCPclient_send(char* buffer, int num)
{
	int n;
	if ((n = send(socket_id, buffer, num, 0))<0) // trasmissione dati al server
		return -1;
	return n; 
}

void TCPclient_disconnect()
{
	closesocket(socket_id); // chiusura socket (disconnessione dal server)
	WSACleanup();
}