#include "domain/commands/shutdownCommand.h"

namespace simple_server
{
    namespace domain
    {
        namespace commands
        {
            std::string ShutdownCommand::execute()
            {
                shutdownFlag_ = true;
                return "Server is shutting down...\n";
            }
        } // namespace commands
    } // namespace domain
} // namespace simple_server