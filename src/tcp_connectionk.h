#pragma once

#include <SDL.h>
#include <SDL_net.h>
#include <string>

#define BUFFER_SIZE 4000


class TcpConnection
{
protected:
	SDLNet_SocketSet mSocketSet;
	std::string mHost;
	
	void sendMessage(TCPsocket &socket, const char *arg_message);
	void setHost(const char *arg_host);
	bool getHostIPAddress(IPaddress &out, int port);


public:
	TcpConnection();
	~TcpConnection();
	
	virtual void FetchNewMessages() = 0;

	
	static char *IPToString(IPaddress *ip) {
		char *tmp = new char[4000];
		Uint8 *arr = (Uint8*)&ip->host;
		sprintf_s(tmp, 4000, "%i.%i.%i.%i", arr[0], arr[1], arr[2], arr[3]);
		return tmp;
	}

};