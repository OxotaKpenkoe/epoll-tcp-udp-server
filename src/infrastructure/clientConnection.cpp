#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cerrno>
#include <iostream>

#include "infrastructure/clientConnection.h"
#include "infrastructure/socketUtils.h"

namespace simple_server
{
    namespace infrastructure
    {

        void ClientConnection::closeConnection()
        {
            if (fd_ >= 0)
            {
                socket_utils::closeSocket(fd_);
                fd_ = -1;
            }
        }

        ssize_t ClientConnection::read()
        {
            ssize_t totalRead = 0;
            while (true)
            {
                char buffer[4096];
                auto bytesRead = recv(fd_, buffer, sizeof(buffer), 0);
                if (bytesRead > 0)
                {
                    readBuffer_.append(buffer, bytesRead);
                    totalRead += bytesRead;
                }
                else if (bytesRead == 0)
                {
                    return 0;
                }
                else
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        break;
                    }
                    else
                    {
                        std::cerr << "TCP connection READ data failed: " << strerror(errno) << std::endl;
                        return -1;
                    }
                }
            }
            return totalRead;
        }

        ssize_t ClientConnection::write(const std::string &data)
        {
            size_t totalSent = 0;
            while (totalSent < data.size())
            {
                auto bytesSent = send(fd_, data.c_str() + totalSent, data.size() - totalSent, 0);
                if (bytesSent > 0)
                {
                    totalSent += bytesSent;
                }
                else if (bytesSent == -1)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        break;
                    }
                    else
                    {
                        std::cerr << "TCP connection SEND data failed: " << strerror(errno) << std::endl;
                        return -1;
                    }
                }
            }
            return totalSent;
        }

        bool ClientConnection::checkIsMessageFull() const
        {
            auto pos = readBuffer_.find('\n');
            if (pos == std::string::npos)
            {
                return false;
            }
            return true;
        }

        std::string ClientConnection::extractMessage()
        {
            auto end_pos = readBuffer_.find('\n');
            if (end_pos == std::string::npos)
            {
                return "";
            }
            auto message = readBuffer_.substr(0, end_pos);
            readBuffer_.erase(0, end_pos + 1);
            return message;
        }

        std::string ClientConnection::getClientInfo() const
        {
            std::string info{inet_ntoa(address_.sin_addr)};
            info += ":";
            info += std::to_string(ntohs(address_.sin_port));
            return info;
        }

    } // namespace infrastructure
} // namespace simple_server