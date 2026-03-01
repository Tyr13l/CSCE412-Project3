#include "LoadBalancer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

/**
 * @file main.cpp
 * @brief Driver program for the CSCE 412 Load Balancer simulation.
 *
 * This is the entry point of the application. It:
 * - Initializes random seed
 * - Reads configuration from config.txt (or uses defaults)
 * - Creates a LoadBalancer instance with configured parameters
 * - Blocks a small IP range for testing
 * - Populates the initial queue with random requests
 * - Runs the simulation for the specified number of cycles
 * - Prints periodic status and final state
 *
 * Configuration parameters are read from config.txt if present.
 * All simulation logic is delegated to the LoadBalancer class.
 */

 /**
 * @brief Main entry point of the load balancer simulation.
 * @return 0 on successful execution
 */
int main(){

    srand(time(nullptr));

    int num_servers = 10;
    int run_time = 10000;
    int initial_queue = 1000;
    int min_per = 50;
    int max_per = 80;
    int delay = 5;
    double rate = 0.25;
    int pmin = 5;
    int pmax = 30;

    //Read from config.txt
    ifstream config("config.txt");

    if(config.is_open()){
        string line;

        while(getline(config, line)){
            istringstream iss(line);
            string key, value;

            if(getline(iss, key, '=') && getline(iss, value)){
                if(key == "num_servers"){
                    num_servers = stoi(value);
                } else if(key == "run_time"){
                    run_time = stoi(value);
                } else if(key == "initial_queue_size"){
                    initial_queue = stoi(value);
                } else if(key == "min_queue_per_server"){
                    min_per = stoi(value);
                } else if(key == "max_queue_per_server"){
                    max_per = stoi(value);
                } else if(key == "delay_cycles"){
                    delay = stoi(value);
                } else if(key == "request_probability"){
                    rate = stod(value);
                } else if(key == "processing_time_min"){
                    pmin = stoi(value);
                } else if(key == "processing_time_max"){
                    pmax = stoi(value);
                }
            }
        }

        config.close();
    } else {
        cout << "Warning: config.txt not found - using defaults" << endl;
    }

    LoadBalancer test(num_servers, min_per, max_per, delay, rate, pmin, pmax);

    cout << "Loaded config: " << num_servers << " servers, " << run_time << " cycles" << endl;

    //Block an IP range (very small % of random requests are blocked)
    test.block_ip("10.0");
    
    //Fill queue with random requests
    for(int i = 0; i < initial_queue; i++){
        test.add_random_request();
    }

    //Advance time to process requests (1000 cycles)
    for(int i = 0; i < run_time; i++){
        test.advance_time();

        if(i % 1000 == 0){
            cout << "Time: " << i << ", Queue: " << test.get_queue_size() << ", Servers: " << test.get_server_count() << endl;
        }
    }

    cout << "\nFinal state after " << run_time << " cycles:" << endl;
    cout << "Queue size: " << test.get_queue_size() << endl;
    cout << "Server count: " << test.get_server_count() << endl;

    return 0;
}