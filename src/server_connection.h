#pragma once

#include "tcp_connectionk.h"

class ServerConnection : public TcpConnection
{
private:
	TCPsocket mClients[100];
	bool mClientIsFree[100];
	int mMaxClients;

	// Callbacks:
	void(*mMessageCallback)(int, const char*);
	void(*mClientConnectedCallback)(int);
	void(*mClientDisconnectedCallback)(int);

	inline TCPsocket getServerSocket() { return mClients[0]; }

public:
	ServerConnection(int max_clients);
	~ServerConnection();

	bool Connect(int port);
	void SendMessage(int socket_index, const char *message);
	void SendMessageToAll(const char *message);

	std::string GetSocketIPAddress(int socket_index);

	// Virtual functions:
	virtual void FetchNewMessages() override;


	// Callback modifiers
	inline void SetMessageCallback(void (*arg_func)(int, const char*))
	{
		mMessageCallback = arg_func;
	}

	inline void SetClientConnectedCallback(void(*arg_func)(int))
	{
		mClientConnectedCallback = arg_func;
	}

	inline void SetClientDisconnectedCallback(void(*arg_func)(int))
	{
		mClientDisconnectedCallback = arg_func;
	}

};