#include "domain/commands/statsCommand.h"

#include <iostream>


namespace simple_server
{
    namespace domain
    {
        namespace commands
        { 
                std::string StatsCommand::execute()
                {
                    return "Total connections: " + std::to_string(stats_.getTotalConnections()) + "\n" 
                         + "Active connections: " + std::to_string(stats_.getActiveConnections()) + "\n";
                }           

        } // namespace commands
    } // namespace domain
} // namespace simple_server