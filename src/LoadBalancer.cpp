#include "LoadBalancer.h"
#include "WebServer.h"
#include "Request.h"
#include <iostream>
#include <string>

using namespace std;

LoadBalancer::LoadBalancer(int num_servers, int server_capacity){
    time = 0;

    //Create initial web servers
    for(int i = 0; i < num_servers; i++){
        web_servers.emplace_back();
    }
}

void LoadBalancer::add_request(Request req){
    if(is_blocked(req.ip_in)){
        cout << "Request from " << req.ip_in << " is blocked." << endl;
        return;
    }

    request_queue.push(req);

    cout << "Added request from " << req.ip_in << " to " << req.ip_out << " with job type " << req.job_type << " and time " << req.time << "." << endl;
}

void LoadBalancer::block_ip(const string& ip){
    blocked_ips[ip] = true;
    cout << "Blocked IP: " << ip << endl;
}

bool LoadBalancer::is_blocked(const string& ip){
    return blocked_ips.find(ip) != blocked_ips.end() && blocked_ips[ip];
}

size_t LoadBalancer::get_queue_size() const{
    return request_queue.size();
}

size_t LoadBalancer::get_server_count() const{
    return web_servers.size();
}