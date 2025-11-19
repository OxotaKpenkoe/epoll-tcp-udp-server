#pragma once

#include "iCommand.h"

namespace simple_server
{
    namespace domain
    {
        namespace commands
        {
            class TimeCommand : public iCommand
            {
            public:
                std::string execute() override;
            };

        } // namespace commands
    } // namespace domain
} // namespace simple_server