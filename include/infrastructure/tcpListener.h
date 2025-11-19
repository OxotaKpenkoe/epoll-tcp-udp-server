#pragma once

#include <netinet/in.h>
#include <cstdint>

namespace simple_server
{
    namespace infrastructure
    {
        class TcpListener
        {
        public:
            TcpListener(uint16_t port);
            ~TcpListener();

            TcpListener(const TcpListener &) = delete;
            TcpListener &operator=(const TcpListener &) = delete;

            int accept(sockaddr_in &clientAddress);

            int getFd() const { return listenFd_; };
            bool isValid() const { return listenFd_ >= 0; };

        private:
            int listenFd_;
            uint16_t port_;
        };

    } // namespace infrastructure
} // namespace simple_server