#pragma once

#include "tcp_connectionk.h"

class ClientConnection : public TcpConnection
{
private:
	TCPsocket mServerSocket;
	void(*mMessageCallback)(const char*);
	void(*mServerDisconnectedCallback)();

public:
	ClientConnection();
	~ClientConnection();
	virtual void FetchNewMessages() override;
	void SendMessage(const char *arg_message);
	bool Connect(const char *arg_host, int port);



// Callback modifiers
	inline void SetMessageCallback(void(*arg_func)(const char*))
	{
		mMessageCallback = arg_func;
	}

	inline void SetServerDisconnectedCallback(void(*arg_func)())
	{
		mServerDisconnectedCallback = arg_func;
	}
};