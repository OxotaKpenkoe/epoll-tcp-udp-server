#pragma once

namespace simple_server
{
    namespace infrastructure
    {
        namespace socket_utils
        {
            bool setNonBlocking(int fd);

            bool setReuseAddr(int fd);

            void closeSocket(int fd);
        } // namespace socket_utils

    } // namespace infrastructure

} // namespace simple_server