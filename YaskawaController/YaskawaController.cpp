/*
Jack Woline 
YASKAWA Controller Network Communicator

Will validate connection establishment in person. Compilation is done for Windows, so testing 
must be done on a Windows machine with access to the switched network.
In the case that this program succeeds in connection, migration to C++ and using the YMConnect 
library will be more effective than socket programming with Python.
*/

#include <iostream>
#include "YMConnect.h"

constexpr int ARG_REQ = 2;

const char* RequiredArguments[ARG_REQ] =
{
    "IPv4 address for the controller", 
    "Singal to send (stop/start)"
};

int main(int argc, char** argv)
{
    // Validate arguments
    for (int i = argc-1; i < ARG_REQ; i++)
    {
        std::cerr << "Missing argument: " << RequiredArguments[i] << std::endl;
    }

    if (argc - 1 < ARG_REQ)
    {
        
        return 1;
    }

    // Struct for storing status of the connection
    StatusInfo status{};

    // Establish connection
    MotomanController* controller = YMConnect::OpenConnection(argv[1], status);

    std::cout << status << std::endl;
    std::cin.get();

    if (status.StatusCode != 0)
    {
        return status.StatusCode;
    }

    YMConnect::CloseConnection(controller);

    return 0;
}