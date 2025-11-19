#include "domain/statistics.h"

namespace simple_server
{
    namespace domain
    {
        void Statistics::incrementActiveConnections()
        {
            activeConnections++;
        }

        void Statistics::decrimentActiveConnections()
        {
            if(activeConnections > 0) activeConnections--;
        }

        void Statistics::incrementTotalConnections()
        {
            totalConnections++;
        }

        uint64_t Statistics::getActiveConnections() const
        {
            return activeConnections;
        }

        uint64_t Statistics::getTotalConnections() const 
        {
            return totalConnections;
        }

    } // namespace domain
} // namespace simple_server