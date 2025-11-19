#include <iostream>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>

#include "infrastructure/tcpListener.h"
#include "infrastructure/socketUtils.h"

namespace simple_server
{
    namespace infrastructure
    {
        TcpListener::TcpListener(uint16_t port) : listenFd_(-1), port_(port)
        {
            listenFd_ = socket(AF_INET, SOCK_STREAM, 0);
            if (listenFd_ == -1)
            {
                throw std::runtime_error("socket() failed: " + std::string(strerror(errno)));
            }

            if (!socket_utils::setReuseAddr(listenFd_))
            {
                std::cerr << "Warning: setReuseAddr failed\n";
            }
            if (!socket_utils::setNonBlocking(listenFd_))
            {
                socket_utils::closeSocket(listenFd_);
                throw std::runtime_error("setNonBlocking failed");
            }
            sockaddr_in serverAddr{};
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_addr.s_addr = INADDR_ANY;
            serverAddr.sin_port = htons(port);

            if (bind(listenFd_, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
            {
                socket_utils::closeSocket(listenFd_);
                throw std::runtime_error("bind() failed" + std::string(strerror(errno)));
            }

            if (listen(listenFd_, SOMAXCONN) == -1)
            {
                socket_utils::closeSocket(listenFd_);
                throw std::runtime_error("listen() failed" + std::string(strerror(errno)));
            }
        }

        TcpListener::~TcpListener()
        {
            socket_utils::closeSocket(listenFd_);
        }

        int TcpListener::accept(sockaddr_in &clientAddress)
        {
            socklen_t adrdrLen = sizeof(clientAddress);
            int clientFd = ::accept(listenFd_, (sockaddr *)&clientAddress, &adrdrLen);
            if (clientFd == -1)
            {
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    std::cerr << "ACCEPT failed: " << strerror(errno) << std::endl;
                }
            }
            return clientFd;
        }

    } // namespace infrastructure
} // namespace simple_server