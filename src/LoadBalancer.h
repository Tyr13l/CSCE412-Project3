#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "Request.h"
#include "WebServer.h"
#include <queue>
#include <vector>
#include <string>
//#include <map>
//#include <utility>
#include <iostream>

using namespace std;

class LoadBalancer{
    private:
        queue<Request> request_queue;
        vector<WebServer> web_servers;
        vector<string> blocked_ips;
        int time;
        int min_queue_per_server;
        int max_queue_per_server;
        int delay_cycles;
        double request_probability;
        int processing_time_min;
        int processing_time_max;
        int remove_delay_counter = 0;
        int add_delay_counter = 0;
    
    public:
        LoadBalancer(int num_servers, int min_per_server, int max_per_server, int delay_cyc, double request_prob, int proc_min, int proc_max);
        //Request management functions
        void add_request(Request req);
        void add_random_request();
        void assign_requests();

        //Server function
        void adjust_servers();

        //IP blocking functions
        void block_ip(const string& ip_prefix);
        bool is_blocked(const string& ip) const;

        //Time function
        void advance_time();

        //Getter functions
        size_t get_queue_size() const;
        size_t get_server_count() const;

};

#endif