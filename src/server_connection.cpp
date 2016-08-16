#include "server_connection.h"
#include <assert.h>
#include <iostream>

ServerConnection::ServerConnection(int max_clients)
{
	mMaxClients = max_clients;

	for (int i = 0; i < max_clients; i++)
	{
		mClients[i] = 0;
		mClientIsFree[i] = true;
	}

	mSocketSet = SDLNet_AllocSocketSet(max_clients);
}

ServerConnection::~ServerConnection()
{
	SDLNet_TCP_Close(mClients[0]);
}

void ServerConnection::FetchNewMessages()
{

	TCPsocket new_tcpsock;

	new_tcpsock = SDLNet_TCP_Accept(getServerSocket());
	if (new_tcpsock)
	{
		if (!new_tcpsock) {
			std::cout << "server_connection.cpp: error on accepting incoming client connection: " << SDLNet_GetError() << std::endl;
		}
		else {
			IPaddress *ip = SDLNet_TCP_GetPeerAddress(new_tcpsock);
			

			int iFreeClient;
			for (iFreeClient = 1; iFreeClient < mMaxClients; iFreeClient++)
				if (mClientIsFree[iFreeClient])
					break;
			if (iFreeClient >= mMaxClients)
			{
				std::cout << "Error: Too many clients!!" << std::endl;
				goto FetchMessages;
			}

			mClients[iFreeClient] = new_tcpsock;
			mClientIsFree[iFreeClient] = false;

			SDLNet_TCP_AddSocket(mSocketSet, new_tcpsock);

			if (mClientConnectedCallback)
				mClientConnectedCallback(iFreeClient);
		}
	}


FetchMessages:
	for (int i = 1; i < mMaxClients; i++)
	{
		if (SDLNet_CheckSockets(mSocketSet, 0) > 0 && SDLNet_SocketReady(mClients[i]))
		{
			char text[BUFFER_SIZE];
			if (SDLNet_TCP_Recv(mClients[i], text, 100) > 0)
			{
				if (mMessageCallback)
					mMessageCallback(i, text);
			}
			else
			{
				SDLNet_TCP_Close(mClients[i]);
				mClientIsFree[i] = true;
				if (mClientDisconnectedCallback)
					mClientDisconnectedCallback(i);
			}
		}
	}
}

bool ServerConnection::Connect(int port)
{
	setHost("");

	IPaddress ip;
	if (!getHostIPAddress(ip, port))
		return false;


	// Client #0 = server
	mClients[0] = SDLNet_TCP_Open(&ip);
	mClientIsFree[0] = false;

	if (!mClients[0])
	{
		std::cout << "failed to connect to server" << std::endl;
		return false;
	}

	// Add server socket to socket set
	SDLNet_TCP_AddSocket(mSocketSet, mClients[0]);


	return true;
}

void ServerConnection::SendMessage(int socket_index, const char * message)
{
	TcpConnection::sendMessage(mClients[socket_index], message);
}

void ServerConnection::SendMessageToAll(const char * message)
{
	for (int i = 1; i < mMaxClients; i++)
	{
		if (!mClientIsFree[i])
			sendMessage(mClients[i], message);
	}
}

std::string ServerConnection::GetSocketIPAddress(int socket_index)
{
	IPaddress *ip = SDLNet_TCP_GetPeerAddress(mClients[socket_index]);
	return std::string(IPToString(ip));
}
