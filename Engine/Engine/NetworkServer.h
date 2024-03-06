#pragma once

#include "NetworkCommon.h"

class NetworkServer final
{
public:
	enum NetworkServerState
	{
		DISABLED,
		CREATE_SERVER,
		RUNNING
	};

	inline static NetworkServer& Instance()
	{
		static NetworkServer instance;
		return instance;
	}

	void SendPacket(RakNet::BitStream& bs);

protected:
	void Initialize();
	void Update();

private:
	int port;
	bool initialized = false;
	NetworkServerState state;

	RakNet::RakPeerInterface* rakInterface;
	std::vector<RakNet::RakNetGUID> clientConnections;

private:
	void LoadSettings();
	void SetupServer();
	void _Update();

private:
	NetworkServer() = default;
	~NetworkServer() = default;
	NetworkServer(NetworkServer const&) = delete;
	friend class Engine;
};