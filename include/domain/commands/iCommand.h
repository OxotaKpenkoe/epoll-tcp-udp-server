#pragma once

#include <string>

namespace simple_server
{
    namespace domain
    {
        namespace commands
        {

            class iCommand
            {
            public:
                virtual ~iCommand() = default;
                virtual std::string execute() = 0;
            };

        } // namespace commands
    } // namespace domain
} // namespace simple_server