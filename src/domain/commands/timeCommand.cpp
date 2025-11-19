#include <chrono>
#include <sstream>
#include <iomanip>

#include "domain/commands/timeCommand.h"

namespace simple_server
{
    namespace domain
    {
        namespace commands
        {            
                std::string TimeCommand::execute()
                {
                    auto now = std::chrono::system_clock::now();
                    auto time_t_now = std::chrono::system_clock::to_time_t(now);

                    std::stringstream ss;
                    ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");
                    return ss.str() + "\n";
                }
            

        } // namespace commands
    } // namespace domain
} // namespace simple_server