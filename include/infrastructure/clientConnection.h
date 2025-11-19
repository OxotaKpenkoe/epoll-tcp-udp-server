#pragma once

#include <string>
#include <netinet/in.h>

namespace simple_server
{
    namespace infrastructure
    {
        class ClientConnection
        {
        public:
            ClientConnection(int fd, const sockaddr_in &address): fd_(fd), address_(address){};
            ~ClientConnection(){closeConnection();};

            ClientConnection(const ClientConnection &) = delete;
            ClientConnection &operator=(const ClientConnection &) = delete;

            ssize_t read();

            ssize_t write(const std::string& data);

            std::string extractMessage();

            bool checkIsMessageFull() const;

            std::string getClientInfo() const;

            int getFd() const {return fd_;};

            void closeConnection();

            private:
            int fd_;
            sockaddr_in address_;
            std::string readBuffer_;
        };

    } // namespace infrastructure
} // namespace simple_server