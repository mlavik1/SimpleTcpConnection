#include "client_connection.h"

ClientConnection::ClientConnection()
{
	mSocketSet = SDLNet_AllocSocketSet(1);
}

ClientConnection::~ClientConnection()
{
	SDLNet_TCP_Close(mServerSocket);
}

void ClientConnection::FetchNewMessages()
{
	if (SDLNet_CheckSockets(mSocketSet, 0) > 0 && SDLNet_SocketReady(mServerSocket))
	{
		char text[BUFFER_SIZE];
		if (SDLNet_TCP_Recv(mServerSocket, text, 100))
		{
			if (mMessageCallback)
				mMessageCallback(text);
		}
		else
		{
			if (mServerDisconnectedCallback)
				mServerDisconnectedCallback();
		}
	}
}

void ClientConnection::SendMessage(const char * arg_message)
{
	TcpConnection::sendMessage(mServerSocket, arg_message);
}

bool ClientConnection::Connect(const char *arg_host, int port)
{
	setHost(arg_host);

	IPaddress ip;
	if (!getHostIPAddress(ip, port))
		return false;

	mServerSocket = SDLNet_TCP_Open(&ip);

	if (!mServerSocket)
	{
		return false;
	}

	// Add server socket to socket set
	SDLNet_TCP_AddSocket(mSocketSet, mServerSocket);

	return true;
}
