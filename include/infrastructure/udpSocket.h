#pragma once

#include <netinet/in.h>
#include <cstdint>
#include <string>

namespace simple_server
{
    namespace infrastructure
    {
        class UdpSocket
        {
        public:
            UdpSocket(uint16_t port);
            ~UdpSocket();

            UdpSocket(const UdpSocket &) = delete;
            UdpSocket &operator=(const UdpSocket &) = delete;

            ssize_t receive(char* buffer, size_t bufferSize, sockaddr_in& clientAddress);

            ssize_t send(const std::string& data, const sockaddr_in& clientAddress);

            int getFd() const {return udpFd_;};
            bool isValidFd() const { return udpFd_ >= 0; };

        private:
            int udpFd_;
            uint16_t port_;
        };
        
        
    } // namespace infrastructure

} // namespace simple_server