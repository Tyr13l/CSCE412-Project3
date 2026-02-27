#include "LoadBalancer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(){

    srand(time(nullptr));

    //LoadBalancer with 5 servers and capacity of 100
    LoadBalancer test(10, 100);

    cout << "Initial state:" << endl;
    cout << "Queue size: " << test.get_queue_size() << endl;
    cout << "Server count: " << test.get_server_count() << endl;
    
    //Fill queue with random requests
    for(int i = 0; i < 500; i++){
        test.add_random_request();
    }

    cout << "After adding 500 random requests:" << endl;
    cout << "Queue size: " << test.get_queue_size() << endl;

    //Advance time to process requests (1000 cycles)
    for(int i = 0; i < 1000; i++){
        test.advance_time();

        if(i % 10 == 0){
            cout << "Time: " << i << endl;
            cout << "Queue size: " << test.get_queue_size() << endl;
            cout << "Server count: " << test.get_server_count() << endl;
        }
    }

    cout << "Final state after 50 time units:" << endl;
    cout << "Queue size: " << test.get_queue_size() << endl;
    cout << "Server count: " << test.get_server_count() << endl;
    cout << "Requests processed: " << 100 - test.get_queue_size() << endl;

    return 0;
}