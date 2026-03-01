#include "LoadBalancer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

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

    //LoadBalancer with a capacity of 100 //UNUSED -- FIX
    LoadBalancer test(num_servers, min_per, max_per, delay, rate, pmin, pmax);

    cout << "Loaded config: " << num_servers << " servers, " << run_time << " cycles" << endl;

    //Block an IP range
    test.block_ip("10.0");

    // cout << "Initial state:" << endl;
    // cout << "Queue size: " << test.get_queue_size() << endl;
    // cout << "Server count: " << test.get_server_count() << endl;
    
    //Fill queue with random requests
    for(int i = 0; i < initial_queue; i++){
        test.add_random_request();
    }

    // cout << "After adding 500 random requests:" << endl;
    // cout << "Queue size: " << test.get_queue_size() << endl;

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
    //cout << "Requests processed: " << 100 - test.get_queue_size() << endl;

    return 0;
}