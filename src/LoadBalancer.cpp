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

void LoadBalancer::add_random_request(){
    string ip_in = "168.0." + to_string(rand() % 256) + "." + to_string(rand() % 256);
    string ip_out = "208.0." + to_string(rand() % 256) + "." + to_string(rand() % 256);

    int time = rand() % 10 + 1;

    if (rand() % 2 == 0){
        add_request({ip_in, ip_out, time, 'P'});
    } else {
        add_request({ip_in, ip_out, time, 'S'});
    }
}

void LoadBalancer::assign_requests(){
    while(!request_queue.empty()){
        Request& req = request_queue.front();

        //Check if request is blocked
        if(is_blocked(req.ip_in)){
            cout << "Request from " << req.ip_in << " is blocked." << endl;
            request_queue.pop();
            continue;
        }

        //Find available server
        bool assigned = false;
        for(WebServer& server : web_servers){
            if(!server.is_busy()){
                server.assign_request(&req);
                cout << "Assigned request from " << req.ip_in << " to " << req.ip_out << " with job type " << req.job_type << " and time " << req.time << "." << endl;
                request_queue.pop();
                assigned = true;
                break;
            }
        }

        //If no available servers, keep request in queue
        if (!assigned){
            cout << "No available servers for request from " << req.ip_in << endl;
        }
    }
}

void LoadBalancer::adjust_servers(){
    //Add or remove servers to keep queue balanced
    size_t current_servers = web_servers.size();
    size_t queue_size = request_queue.size();

    //Queue between 50-80 per server
    size_t max_cap = current_servers * 80;
    size_t min_cap = current_servers * 50;

    if (queue_size < min_cap && current_servers > 1){
        delete &web_servers.back();
        web_servers.pop_back();
        cout << "Removed a server. Total servers: " << web_servers.size() << endl;
    } else if (queue_size > max_cap){
        web_servers.emplace_back();
        cout << "Added a server. Total servers: " << web_servers.size() << endl;
    }
}

void LoadBalancer::block_ip(const string& ip){
    blocked_ips[ip] = true;
    cout << "Blocked IP: " << ip << endl;
}

bool LoadBalancer::is_blocked(const string& ip){
    return blocked_ips.find(ip) != blocked_ips.end() && blocked_ips[ip];
}

void LoadBalancer::advance_time(){
    time++;

    //Process requests on servers
    for(WebServer& server : web_servers){
        server.process_request();
    }

    //Assign requests to idle servers
    assign_requests();

    if(rand() % 10 == 0){
        add_random_request();
    }

    //Check if servers need to be adjusted every 5 time units
    static int adjust_counter = 0;
    adjust_counter++;

    if(adjust_counter >= 5){
        adjust_servers();
        adjust_counter = 0;
    }

}

size_t LoadBalancer::get_queue_size() const{
    return request_queue.size();
}

size_t LoadBalancer::get_server_count() const{
    return web_servers.size();
}