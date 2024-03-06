#pragma once

#include "NetworkCommon.h"

class NetworkClient final
{
public:
	enum NetworkClientState
	{
		DISABLED,
		WAITING_TO_CONNECT,
		WAITING_FOR_FIRST_PACKET,
		CONNECTED,
		NETWORK_ERROR
	};

	inline static NetworkClient& Instance()
	{
		static NetworkClient instance;
		return instance;
	}

	bool IsInitialized() { return initialized; }

protected:
	void Initialize();
	void Update();

private:
	int port;
	bool initialized = false;
	std::string ipAddress;
	NetworkClientState state;

	RakNet::RakPeerInterface* rakInterface;
	RakNet::RakNetGUID serverGUID;

private:
	void LoadSettings();
	void WaitingForFirstPacket();
	void _Update();

private:
	NetworkClient() = default;
	~NetworkClient() = default;
	NetworkClient(NetworkClient const&) = delete;
	friend class Engine;
};
