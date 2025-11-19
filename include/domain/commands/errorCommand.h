#pragma once

#include "iCommand.h"

namespace simple_server
{
    namespace domain
    {
        namespace commands
        {
            class ErrorCommand : public iCommand
            {
            public:
                explicit ErrorCommand(const std::string &errorMessage) : errorMessage_(std::move(errorMessage)) {};

                std::string execute() override;

            private:
                const std::string errorMessage_;
            };

        } // namespace commands
    } // namespace domain
} // namespace simple_server