#include "app/server.h"
#include <iostream>
#include <csignal>
#include <cstdlib>

simple_server::application::Server *p_server = nullptr;

void signalHandler(int signal)
{
    std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
    if (p_server)
    {
        p_server->stop();
    }
}

int main(int argc, char *argv[])
{
    uint32_t tcpPort = 38080;
    uint32_t udpPort = 38081;

    if (argc >= 2) {
        tcpPort = static_cast<uint16_t>(std::atoi(argv[1]));
    }
    if (argc >= 3) {
        udpPort = static_cast<uint16_t>(std::atoi(argv[2]));
    }

    try
    {
        simple_server::application::Server server(tcpPort, udpPort);
        p_server = &server;

        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        server.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}