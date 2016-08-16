#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <vector>
#include <string>

#include "server_connection.h"
#include "client_connection.h"


#define ID_SERVER 1
#define ID_CLIENT 0

#undef main


char *IPToString(IPaddress *ip) {
	char *tmp = new char[4000];
	Uint8 *arr = (Uint8*)&ip->host;
	sprintf_s(tmp, 4000, "%i.%i.%i.%i", arr[0], arr[1], arr[2], arr[3]);
	return tmp;
}

int main(int args, char **argv)
{
	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	std::cout << "Type " << ID_SERVER << " if you wish to create a Server" << std::endl;
	std::cout << "..or  " << ID_CLIENT << " if you wish to create a Client" << std::endl;
	int type = 0;
	std::cin >> type;
	if (type == ID_SERVER)
		std::cout << "Creating server" << std::endl;
	else
		std::cout << "Creating client" << std::endl;


	// Server loop
	if (type == ID_SERVER)
	{
		ServerConnection conn(10);
		conn.SetMessageCallback([](int socket_index, const char* msg) -> void { std::cout << msg << std::endl; });
		conn.SetClientConnectedCallback([](int socket_index) -> void { std::cout << "Client connected: " << std::endl; });
		conn.SetClientDisconnectedCallback([](int socket_index) -> void { std::cout << "Client disconnected" << std::endl; });
		
		conn.Connect(1234);

		while (true)
		{
			conn.FetchNewMessages();
		}
	}
	else // Client loop
	{
		ClientConnection conn;
		conn.SetMessageCallback([](const char* msg) -> void { std::cout << msg << std::endl; });

		const char *host;
		std::cout << "enter IP (or just write 'localhost')" << std::endl;
		std::string strtmp;
		std::cin >> strtmp;
		host = strtmp.c_str();

		conn.Connect(host, 1234);

		while (true)
		{
			conn.FetchNewMessages();

			char buf[1000];
			std::cin >> buf;
			conn.SendMessage(buf);
		}
	}

	// ***** TODO *****
	// 1. strlen() er for dyrt!!


	SDLNet_Quit();


	return 0;
}