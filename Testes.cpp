#include <olcNet.h>
#include <net_server.h>

#define PORT 8080
#define DEFAULT_IP "51.38.81.49"

class CustomServer : public net::cmm::server_interface<CustomMsgType>
{
public:
	CustomServer(uint16_t port) :
		server_interface(port)
	{}

protected:
	bool onClientConnect(std::shared_ptr<net::cmm::connection<CustomMsgType>> client)
	{
		return true;
	}
	void onClientDisconnect(std::shared_ptr<net::cmm::connection<CustomMsgType>> client)
	{
		std::cout << "Removing Client [" << client->GetId() << "]." << std::endl;
	}
	void onMessage(std::shared_ptr<net::cmm::connection<CustomMsgType>> client, net::cmm::message<CustomMsgType>& msg)
	{
		std::string strin;
		std::string name;

		switch (msg.header.id)
		{
		case CustomMsgType::MESSAGE_ALL:
			this->messageAllClients(msg, client);
			msg >> name;
			msg >> strin;
			promptMtx.lock();
			std::cout << "Mensagem de " << name << ": " << strin << std::endl;
			promptMtx.unlock();
			break;
		default:
			break;
		}
	}
};

int main()
{
	setlocale(LC_ALL, "");
	CustomServer server(DEFAULT_PORT);
	server.start();

	while (1)
	{
		server.update(-1, true);
	};

	return 0;
};