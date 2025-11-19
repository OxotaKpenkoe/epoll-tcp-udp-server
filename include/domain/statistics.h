#pragma once

#include <cstdint>

namespace simple_server
{
    namespace domain
    {

        class Statistics
        {
        public:
            Statistics(): totalConnections(0), activeConnections(0){};
            
            void incrementActiveConnections();
            void decrimentActiveConnections();
            void incrementTotalConnections();

            uint64_t getActiveConnections() const;
            uint64_t getTotalConnections() const;

            private:
            uint64_t totalConnections;
            uint64_t activeConnections;
        };



    } // namespace domain
} // namespace simple_server
