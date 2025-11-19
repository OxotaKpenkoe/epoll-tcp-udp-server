#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cerrno>

#include "infrastructure/epollManager.h"
#include "infrastructure/socketUtils.h"

namespace simple_server
{
    namespace infrastructure
    {
        EpollManager::EpollManager()
        {
            epollFd_ = epoll_create1(0);
            if (epollFd_ == -1)
            {
                throw std::runtime_error("epoll_create1 failed: " + std::string(strerror(errno)));
            }
            events_.resize(MAX_EVENTS);
        }

        EpollManager::~EpollManager()
        {
            if (isValid())
            {
                close(epollFd_);
            }
        }

        bool EpollManager::isValid() const
        {
            return epollFd_ >= 0 ? true : false;
        }

        bool EpollManager::add(int fd, uint32_t events)
        {
            epoll_event ev;
            ev.events = events | EPOLLET;
            ev.data.fd = fd;
            if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &ev) == -1)
            {
                std::cerr << "epoll_ctl ADD failed: " << strerror(errno) << std::endl;
                return false;
            }
            return true;
        }

        bool EpollManager::remove(int fd)
        {
            if (epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, nullptr) == -1)
            {
                std::cerr << "epoll_ctl DEL failed: " << strerror(errno) << std::endl;
                return false;
            }
            return true;
        }

        bool EpollManager::modify(int fd, uint32_t events)
        {
            epoll_event ev;
            ev.events = events | EPOLLET;
            ev.data.fd = fd;
            if (epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &ev) == -1)
            {
                std::cerr << "epoll_ctl MOD failed: " << strerror(errno) << std::endl;
                return false;
            }
            return true;
        }

        int EpollManager::wait(int timeout)
        {
            auto n = epoll_wait(epollFd_, events_.data(), MAX_EVENTS, timeout);
            if (n == -1)
            {
                if (errno == EINTR)
                {
                    return 0;
                }
                else
                {
                    std::cerr << "epoll WAIT failed: " << strerror(errno) << std::endl;
                    return -1;
                }
            }
            return n;
        }

        const epoll_event *EpollManager::getEvents() const
        {
            return events_.data();
        }

    } // namespace infrastructure
} // namespace simple_server