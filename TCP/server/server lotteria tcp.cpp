#include <iostream>
#include <time.h>
#include <WinSock2.h>

#pragma comment (lib,"Ws2_32.lib")

using namespace std;
WSADATA wsaData;
SOCKET request_socket_id; // socket richieste di connessione
SOCKET communication_socket_id; // socket comunicazione con client
struct sockaddr_in server_add; // struttura per indirizzo server	
struct sockaddr_in client_add; // struttura per indirizzo client
int numeri[6], vincenti[6], premio[7]={0, 10, 50, 500, 10000, 400000, 32000000}, cont;

unsigned long WINAPI client_service (void *arg)
{
	SOCKET sock=*(SOCKET*)arg;
	cont=0;
	while(recv(sock, (char*)numeri, sizeof(numeri), 0)<0); //ricezione numeri giocati dal client

	cout<<"richiesta numeri vincenti" << endl;
	if(send(sock, (char*)vincenti, sizeof(vincenti), 0)<0) //invio numeri vincenti al client
		cout<<"numeri vincenti non inviati"<<endl;
	else
		cout <<"invio numeri vincenti"<< endl;			

	for (int i = 0; i<6; i++) 
		for (int j = 0; j<6; j++)
			if (numeri[i] == vincenti[j]) //controllo numeri vincenti
				cont++;
		
	send(sock, (char*)&cont, sizeof(cont), 0); //invio contatore numeri vincenti
	cont=0; //azzeramento contatore indispensabile per il corretto invio dei dati ai successivi thread
	send(sock, (char*)premio, sizeof(premio), 0); //invio lista premi
	closesocket(sock);
	cout<<"Utente disconnesso"<<endl;
	ExitThread(0);
}
void main()
{
	
	srand(time(NULL));

	if (WSAStartup(0x0202, &wsaData) != 0) // inizializzazione WinSock (versione 2.2)
	{
		cout<<"Errore inizializzazione WinSock!"<<endl;
		return;
	}

	if ((request_socket_id = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) // apertura del socket
	{
		cout<<"Errore apertura socket!"<<endl;
		WSACleanup();
		return;
	}	 

	memset(&server_add, 0, sizeof(server_add)); // azzeramento struttura
	server_add.sin_family = AF_INET; // dominio indirizzi IP
	server_add.sin_addr.s_addr = 0; // indirizzo IP locale
	server_add.sin_port = htons(2015); // numero di porta server

	if (bind (request_socket_id, (struct sockaddr*)&server_add, sizeof(server_add))==SOCKET_ERROR) // associazione del numero di porta al socket
	{
		cout<<"Errore associazione socket!"<<endl; 
		closesocket(request_socket_id);
		WSACleanup();
		return;
	}

	if (listen(request_socket_id, 0) == SOCKET_ERROR) // impostazione del socket per ricevere le richieste di connessione
	{
		cout<<"Errore impostazione socket!"<<endl;
		closesocket(request_socket_id);
		WSACleanup();
		return;
	} 
	
	for (int i = 0; i<6; i++)
		vincenti[i] = rand()%90+1;
	cout<<"Server in attesa..."<<endl<<endl;
	while (1)
	{
		int client_add_size = sizeof(client_add); //conversione della dimensione di client_add necessaria per la funzione accept
		if ((communication_socket_id = accept (request_socket_id, (struct sockaddr *)&client_add, (int*)&client_add_size))!= INVALID_SOCKET) //accettazione del client
		{	
			cout<<"Nuovo utente connesso"<<endl;
			CreateThread(NULL, 4096, &client_service, &communication_socket_id, 0, NULL);
		}
	}
	
	closesocket(request_socket_id); //chiusura socket richiesta
	WSACleanup();
	system("pause");
}