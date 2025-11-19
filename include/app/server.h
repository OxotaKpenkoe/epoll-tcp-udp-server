#pragma once

#include <memory>
#include <unordered_map>
#include <cstdint>

#include "infrastructure/epollManager.h"
#include "infrastructure/tcpListener.h"
#include "infrastructure/udpSocket.h"
#include "infrastructure/clientConnection.h"
#include "domain/commandParser.h"
#include "domain/statistics.h"

namespace simple_server
{
    namespace application
    {
        class Server
        {
        public:
            Server(uint16_t tcpPort, uint16_t udpPort);
            ~Server();

            void run();

            void stop();

        private:
            void handleNewConnection();
            void handleClientData(int clientFd);
            void handleUdpData();

            std::string processMessage(const std::string &message);

            void closeClient(int clientFd);

        private:
            infrastructure::EpollManager epollManager_;
            infrastructure::TcpListener tcpListener_;
            infrastructure::UdpSocket udpSocket_;

            std::unordered_map<int, std::unique_ptr<infrastructure::ClientConnection>> clients_;

            domain::Statistics statistics_;
            domain::CommandParser commandParser_;

            bool shutdownFlag_;
            bool running_;
        };

    } // namespace application
} // namespace simple_server