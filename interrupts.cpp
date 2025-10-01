/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include<stdio.h>
#include"interrupts.hpp"

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    std::string current_activity = ""; // Current activity (main or ISR)
    std::string current_isr = ""; // Current ISR being executed
    float current_time = 0; // Current time in the simulation
    int device_number = -1; // Device number for the current interrupt
    float device_duration = 0; // Duration of the current interrupt
    int hex_address = 0; // Hex address of the current ISR
    bool isr_executing = false; // Flag to indicate if an ISR is currently executing


    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        //execute in format: time of the event, Duratin, activity.
        if (activity == "CPU"){
            //Simulate CPU activity
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", " + activity + "\n";
            write_output(execution);
            current_time += duration_intr; // Update current time

        }
        else if (activity == "SYSCALL"){


        }
        else if (activity == "END_IO"){


        }

        else {
            std::cerr << "Error: Unknown activity type: " << activity << std::endl;
            continue; // Skip to the next line
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
