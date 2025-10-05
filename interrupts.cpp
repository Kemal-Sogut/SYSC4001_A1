/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include "interrupts.hpp"

int main(int argc, char** argv) { 

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    
    std::string execution_temp; // Temporary variable to store execution line
    int t_save = 10; // Context save/restore time
    int t_isr_step = 100; // ISR activity time
    std::string current_isr = ""; // Current ISR being executed
    float current_time = 0; // Current time in the simulation
    int device_number = -1; // Device number for the current interrupt
    float device_duration = 0; // Duration of the current interrupt
    int hex_address = 0; // Hex address of the current ISR
    bool isr_executing = false; // Flag to indicate if an ISR is currently executing
    int delay_remain; // Value of delay corresponding with inputted device number, which will be subtracted from for each ISR step


    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        //execute in format: time of the event, Duratin, activity.
        if (activity == "CPU"){
            //Simulate CPU activity
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", BuRST" + activity + "\n";
            current_time += duration_intr; // Update current time

        }
        
        else if (activity == "SYSCALL"){
            delay_remain = delays.at(duration_intr);
            // Simulate activity
            std::tie(execution_temp, current_time) = intr_boilerplate(current_time, duration_intr, t_save, vectors);
            execution += execution_temp + std::to_string(current_time) + ", " + std::to_string(t_isr_step) + ", Read" + "\n";
            current_time += t_isr_step; // Update current time
            // Check if there is still time left in delay
            if ((delay_remain - t_isr_step) > 0) { // Subtract ISR activity time from remaining delay time if difference is more than 0
                delay_remain -= t_isr_step;
            } else { // Remaining delay time = 0
                delay_remain = 0;
            }
            
            execution += std::to_string(current_time) + ", " + std::to_string(t_isr_step) + ", Transfer" + "\n";
            current_time += t_isr_step; // Update current time
            // Check if there is still time left in delay
            if ((delay_remain - t_isr_step) > 0) { // Subtract ISR activity time from remaining delay time if difference is more than 0
                delay_remain -= t_isr_step;
            } else { // Remaining delay time = 0
                delay_remain = 0;
            }

            if (delay_remain > 0) { // Add remaining delay time to current time if there is still any so it adds up to delay time
                execution += std::to_string(current_time) + ", " + std::to_string(delay_remain) + ", Checking errors" + "\n";
                current_time += delay_remain; 
            } else { // Add ISR activity time
                execution += std::to_string(current_time) + ", " + std::to_string(t_isr_step) + ", Checking errors" + "\n";
                current_time += t_isr_step;
            }

            execution += std::to_string(current_time) + ", 1" + ", IRET\n";
            current_time++; // Update current time
        }

        else if (activity == "END_IO"){
            // Simulate END_IO activity
            delay_remain = delays.at(duration_intr);
            execution += std::to_string(current_time) + ", " + std::to_string(t_isr_step) + ", ENDIO: run the ISR (device driver)" + "\n";
            current_time += t_isr_step; // Update current time
            
            // Check if there is still time left in delay
            if ((delay_remain - t_isr_step) > 0) { // Subtract ISR activity time from remaining delay time if difference is more than 0
                delay_remain -= t_isr_step;
            } else { // Remaining delay time = 0
                delay_remain = 0;
            }

            if (delay_remain > 0) { // Add remaining delay time to current time if there is still any so it adds up to delay time
                execution += std::to_string(current_time) + ", " + std::to_string(delay_remain) + ", check device status" + "\n";
                current_time += delay_remain; 
            } else { // Add ISR activity time
                execution += std::to_string(current_time) + ", " + std::to_string(t_isr_step) + ", check device status" + "\n";
                current_time += t_isr_step;
            }
            execution += std::to_string(current_time) + ", 1" + ", IRET\n";
            current_time++; // Update current time

        }

        else {
             //Output error message
            std::cerr << "Error: Unknown activity type: " << activity << std::endl;
        }
        /************************************************************************/



    }

    input_file.close();

    write_output(execution);

    return 0;
}