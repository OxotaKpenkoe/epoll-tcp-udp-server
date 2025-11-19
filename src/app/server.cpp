#include "app/server.h"

#include <iostream>
#include <sys/epoll.h>

namespace simple_server
{
    namespace application
    {
        Server::Server(uint16_t tcpPort, uint16_t udpPort) : tcpListener_(tcpPort),
                                                             udpSocket_(udpPort),
                                                             commandParser_(statistics_, shutdownFlag_),
                                                             shutdownFlag_(false),
                                                             running_(false)
        {
            if (!epollManager_.add(tcpListener_.getFd(), EPOLLIN))
            {
                throw std::runtime_error("Failed to add TCP listener to epoll");
            }
            if (!epollManager_.add(udpSocket_.getFd(), EPOLLIN))
            {
                throw std::runtime_error("Failed to add UDP socket to epoll");
            }

            std::cout << "Server initialized on TCP port " << tcpPort
                      << " and UDP port " << udpPort << std::endl;
        }

        Server::~Server()
        {
            stop();
        }

        void Server::stop()
        {
            if (!running_)
                return;

            running_ = false;

            for (auto &[fd, client] : clients_)
            {
                client->closeConnection();
            }
            clients_.clear();

            std::cout << "Server stopped" << std::endl;
        }

        void Server::run()
        {
            running_ = true;
            std::cout << "Server is running..." << std::endl;

            while (running_ && !shutdownFlag_)
            {
                int eventCount = epollManager_.wait(500);
                if (eventCount == -1)
                {
                    std::cerr << "epoll_wait error, stopping server" << std::endl;
                    break;
                }
                if (eventCount == 0)
                {
                    continue;
                }

                const epoll_event *events = epollManager_.getEvents();

                for (int i = 0; i < eventCount; ++i)
                {
                    int fd = events[i].data.fd;

                    if (fd == tcpListener_.getFd())
                    {
                        handleNewConnection();
                    }
                    else if (fd == udpSocket_.getFd())
                    {
                        handleUdpData();
                    }
                    else
                    {
                        handleClientData(fd);
                    }
                }
            }
        }

        void Server::closeClient(int clientFd)
        {
            auto it = clients_.find(clientFd);
            if (it == clients_.end())
            {
                return;
            }

            epollManager_.remove(clientFd);
            it->second->closeConnection();
            clients_.erase(it);
            statistics_.decrimentActiveConnections();
        }

        void Server::handleNewConnection()
        {
            sockaddr_in clientAddr;
            int clientFd = tcpListener_.accept(clientAddr);
            if (clientFd < 0)
            {
                return;
            }
            auto client = std::make_unique<infrastructure::ClientConnection>(clientFd, clientAddr);
            std::cout << "New connection: " << client->getClientInfo() << std::endl;

            if (!epollManager_.add(clientFd, EPOLLIN))
            {
                std::cerr << "Failed to add client to epoll" << std::endl;
                client->closeConnection();
                return;
            }
            clients_[clientFd] = std::move(client);
            statistics_.incrementTotalConnections();
            statistics_.incrementActiveConnections();
        }

        void Server::handleClientData(int clientFd)
        {
            auto it = clients_.find(clientFd);
            if (it != clients_.end())
            {
                auto &client = it->second;
                ssize_t bytesRead = client->read();
                if (bytesRead == 0)
                {
                    std::cout << "Client disconnected: " << client->getClientInfo() << std::endl;
                    closeClient(clientFd);
                    return;
                }
                if (bytesRead < 0)
                {
                    std::cerr << "Read error from client: " << client->getClientInfo() << std::endl;
                    closeClient(clientFd);
                    return;
                }
                while (client->checkIsMessageFull())
                {
                    std::string message = client->extractMessage();
                    std::string response = processMessage(message);
                    client->write(response);
                }
            }
        }

        void Server::handleUdpData()
        {
            char buffer[4096];
            sockaddr_in clientAddr;
            ssize_t bytesRead = udpSocket_.receive(buffer, sizeof(buffer), clientAddr);
            if (bytesRead <= 0)
            {
                return;
            }
            std::string message(buffer, bytesRead);
            if (!message.empty() && message.back() == '\n')
            {
                message.pop_back();
            }
            std::string response = processMessage(message);
            udpSocket_.send(response, clientAddr);
        }

        std::string Server::processMessage(const std::string &message)
        {
            auto command = commandParser_.parse(message);
            if (command)
            {
                return command->execute();
            }
            else
            {
                return message + "\n";
            }
        }

    } // namespace application
} // namespace simple_server