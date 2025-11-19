#pragma once

#include "iCommand.h"
#include "domain/statistics.h"

namespace simple_server
{
    namespace domain
    {
        namespace commands
        {
            class StatsCommand : public iCommand
            {
            public:
                explicit StatsCommand(const domain::Statistics &stats) : stats_(stats) {};

                std::string execute() override;

            private:
                const domain::Statistics &stats_;
            };

        } // namespace commands
    } // namespace domain
} // namespace simple_server