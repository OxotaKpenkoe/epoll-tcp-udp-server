#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <iostream>

#include "infrastructure/socketUtils.h"

namespace simple_server
{
    namespace infrastructure
    {
        namespace socket_utils
        {
            bool setNonBlocking(int sockfd)
            {
                int flags = fcntl(sockfd, F_GETFL, 0);
                if (flags == -1)
                {
                    std::cerr << "fcntl(F_GETFL) failed: " << strerror(errno) << '\n';
                    return false;
                }

                if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
                {
                    std::cerr << "fcntl(F_SETFL, O_NONBLOCK) failed: " << strerror(errno) << '\n';
                    return false;
                }

                return true;
            }

            bool setReuseAddr(int sockfd)
            {
                int opt = 1;
                if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
                {
                    std::cerr << "setsockopt(SO_REUSEADDR) failed: " << strerror(errno) << '\n';
                    return false;
                }
                return true;
            }

            void closeSocket(int sockfd)
            {
                if (sockfd >= 0)
                {
                    close(sockfd);
                }
            }
        } // namespace socket_utils

    } // namespace infrastructure

} // namespace simple_server