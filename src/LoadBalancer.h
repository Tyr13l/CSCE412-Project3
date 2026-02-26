#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "Request.h"
#include "WebServer.h"
#include <queue>
#include <vector>
#include <string>
#include <map>
#include <iostream>

using namespace std;

class LoadBalancer{
    private:
        queue<Request> request_queue;
        vector<WebServer> web_servers;
        map<string, bool> blocked_ips;
        int time;
    
    public:
        LoadBalancer(int num_servers, int server_capacity);
        void add_request(Request req);
        void block_ip(const string& ip);
        bool is_blocked(const string& ip);
        size_t get_queue_size() const;
        size_t get_server_count() const;

};

#endif