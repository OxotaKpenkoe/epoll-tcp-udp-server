#include "domain/commandParser.h"
#include "domain/commands/timeCommand.h"
#include "domain/commands/statsCommand.h"
#include "domain/commands/shutdownCommand.h"
#include "domain/commands/errorCommand.h"

#include <cctype>
#include <algorithm>

namespace simple_server
{
    namespace domain
    {

        std::unique_ptr<commands::iCommand> CommandParser::parse(const std::string &message)
        {
            if (message.empty() || message[0] != '/')
            {
                return nullptr;
            }

            size_t spacePos = message.find(' ');
            std::string command;

            if (spacePos == std::string::npos)
            {
                command = message.substr(1);
            }
            else
            {
                command = message.substr(1, spacePos - 1);
            }

            command.erase(std::find_if(command.rbegin(), command.rend(),
                                       [](unsigned char ch)
                                       { return !std::isspace(ch); })
                              .base(),
                          command.end());
            if (command == "time")
            {
                return std::make_unique<commands::TimeCommand>();
            }
            else if (command == "stats")
            {
                return std::make_unique<commands::StatsCommand>(stats_);
            }
            else if (command == "shutdown")
            {
                return std::make_unique<commands::ShutdownCommand>(shutdownFlag_);
            }
            else
            {
                return std::make_unique<commands::ErrorCommand>("Unknown command: " + command + "\n");
            }
        }

    } // namespace domain
} // namespace simple_server