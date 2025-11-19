#include "domain/commands/errorCommand.h"

namespace simple_server
{
    namespace domain
    {
        namespace commands
        {
           
                std::string ErrorCommand::execute() 
                {
                    return errorMessage_;
                }

            

        } // namespace commands
    } // namespace domain
} // namespace simple_server