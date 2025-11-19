#include <sys/socket.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <iostream>

#include "infrastructure/udpSocket.h"
#include "infrastructure/socketUtils.h"

namespace simple_server
{
    namespace infrastructure
    {
        UdpSocket::UdpSocket(uint16_t port) :  udpFd_(-1), port_(port)
        {
            udpFd_ = socket(AF_INET, SOCK_DGRAM, 0);
            if (udpFd_ == -1)
            {
                throw std::runtime_error("socket() failed: " + std::string(strerror(errno)));
            }

            sockaddr_in serverAddress{};
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = INADDR_ANY;
            serverAddress.sin_port = htons(port);

            if (bind(udpFd_, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
            {
                socket_utils::closeSocket(udpFd_);
                throw std::runtime_error("bind() failed" + std::string(strerror(errno)));
            }

            if (!socket_utils::setNonBlocking(udpFd_))
            {
                socket_utils::closeSocket(udpFd_);
                throw std::runtime_error("setNonBlocking failed");
            }
        }

        UdpSocket::~UdpSocket()
        {
            socket_utils::closeSocket(udpFd_);
        }       

        ssize_t UdpSocket::receive(char *buffer, size_t bufferSize, sockaddr_in &clientAddress)
        {
            socklen_t addrLen = sizeof(clientAddress);
            ssize_t bytesRead = recvfrom(
                udpFd_, buffer, bufferSize, 0,
                (sockaddr *)&clientAddress, &addrLen);

            if (bytesRead < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    return 0;
                }

                std::cerr << "UDP READ data failed: " << strerror(errno) << std::endl;
                return -1;
            }
            return bytesRead;
        }

        ssize_t UdpSocket::send(const std::string &data, const sockaddr_in &clientAddress)
        {
            ssize_t bytesSent = ::sendto(udpFd_, data.c_str(), data.size(), 0,
                                         (sockaddr *)&clientAddress, sizeof(clientAddress));

            if (bytesSent == -1)
            {
                std::cerr << "UDP SEND data failed: " << strerror(errno) << std::endl;
                return -1;
            }
            return bytesSent;
        }

    } // namespace infrastructure
} // namespace simple_server
