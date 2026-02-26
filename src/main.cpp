#include "LoadBalancer.h"
#include <iostream>

using namespace std;

int main(){
    //test LoadBalancer with 5 servers and capacity of 100
    LoadBalancer test(5, 100);

    cout << "Queue size: " << test.get_queue_size() << endl;
    cout << "Server count: " << test.get_server_count() << endl;
    return 0;
}