#pragma once

#include "iCommand.h"

namespace simple_server
{
    namespace domain
    {
        namespace commands
        {
            class ShutdownCommand : public iCommand
            {
            public:
                explicit ShutdownCommand(bool &shutdownFlag): shutdownFlag_(shutdownFlag) {};

                std::string execute() override;

            private:
                bool &shutdownFlag_;
            };

        } // namespace commands
    } // namespace domain
} // namespace simple_server