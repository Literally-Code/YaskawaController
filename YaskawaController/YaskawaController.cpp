/*
Jack Woline 
YASKAWA Controller Network Communicator

Will validate connection establishment in person. Compilation is done for Windows, so testing 
must be done on a Windows machine with access to the switched network.
In the case that this program succeeds in connection, migration to C++ and using the YMConnect 
library will be more effective than socket programming with Python.
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include "YMConnect.h"

constexpr int ARG_REQ = 2;

const char* RequiredArguments[ARG_REQ] =
{
    "IPv4 address for the controller", 
    "Singal to send (stop/start)"
};

int check_args(int argc, char** argv)
{
    for (int i = argc - 1; i < ARG_REQ; i++)
    {
        std::cerr << "Missing argument: " << RequiredArguments[i] << std::endl;
    }

    if (argc - 1 < ARG_REQ)
    {
        return 1;
    }
}

int main(int argc, char** argv)
{
    // Validate arguments
    int args_status = check_args(argc, argv);
    if (args_status != 0)
    {
        return args_status;
    }

    char waitForUserToPressEnter;
    std::string signal = argv[2];
    StatusInfo status;
    MotomanController* c = YMConnect::OpenConnection(argv[1], status);

    if (status.StatusCode != 0)
    {
        std::cout << status << std::endl;
        std::cin >> waitForUserToPressEnter;
        return status.StatusCode;
    }

    status = c->ControlCommands->DisplayStringToPendant("Hello from YMConnect");
    
    /*
        // Probably not ideal, stops the motion itself without stopping the job
        c->MotionManager->MotionStop()
        c->MotionManager->MotionStart()
    
        // Likely makes the process actually hold
    */

    std::cout << status << std::endl;

    if (signal == "stop")
    {
        c->ControlCommands->SetHold(SignalStatus::ON);
        std::cout << "Sent signal to stop" << std::endl;
        std::cout << status << std::endl;
    }

    YMConnect::CloseConnection(c);

    std::cin >> waitForUserToPressEnter;
    system("pause"); // Delet this

    return status.StatusCode;
}