#pragma once

#include <sys/epoll.h>
#include <vector>

namespace simple_server
{
    namespace infrastructure
    {

        class EpollManager
        {
        public:
            EpollManager();
            ~EpollManager();

            EpollManager(const EpollManager &) = delete;
            EpollManager &operator=(const EpollManager &) = delete;
            bool add(int fd, uint32_t events);

            bool remove(int fd);

            bool modify(int fd, uint32_t events);

            int wait(int timeout = -1);

            const epoll_event *getEvents() const;

            bool isValid() const;

        private:
            int epollFd_;
            std::vector<epoll_event> events_;
            static constexpr int MAX_EVENTS = 64;
        };

    } // namespace infrastructure
} // namespace simple_server