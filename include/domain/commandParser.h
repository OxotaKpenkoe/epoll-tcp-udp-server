#pragma once

#include "domain/commands/iCommand.h"
#include "domain/statistics.h"
#include <memory>
#include <string>

namespace simple_server
{
    namespace domain
    {
        class CommandParser
        {

        public:
            CommandParser(Statistics &stats, bool &shutdownFlag) : stats_(stats), shutdownFlag_(shutdownFlag) {};

            std::unique_ptr<commands::iCommand> parse(const std::string &message);

        private:
            Statistics &stats_;
            bool &shutdownFlag_;
        };

    } // namespace domain
} // namespace simple_server