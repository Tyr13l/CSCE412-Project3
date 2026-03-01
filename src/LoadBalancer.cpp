#include "LoadBalancer.h"
#include "WebServer.h"
#include "Request.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/**
 * @brief Constructor that initializes the load balancer with simulation parameters.
 * @param num_servers Initial number of web servers to create
 * @param min_per_server Minimum queue size per server (threshold for removal)
 * @param max_per_server Maximum queue size per server (threshold for addition)
 * @param delay_cyc Number of clock cycles to wait between scaling actions
 * @param request_prob Probability (0.0 to 1.0) of generating a random request per cycle
 * @param proc_min Minimum processing time for new random requests
 * @param proc_max Maximum processing time for new random requests
 */
LoadBalancer::LoadBalancer(int num_servers, int min_per_server, int max_per_server, int delay_cyc, double request_prob, int proc_min, int proc_max){
    time = 0;
    min_queue_per_server = min_per_server;
    max_queue_per_server = max_per_server;
    delay_cycles = delay_cyc;
    request_probability = request_prob;
    processing_time_min = proc_min;
    processing_time_max = proc_max;

    //Create initial web servers
    for(int i = 0; i < num_servers; i++){
        web_servers.emplace_back();
    }
}

/**
 * @brief Adds a single request to the queue if not blocked by the firewall.
 * @param req The request to add (contains source/destination IP, time, and job type)
 */
void LoadBalancer::add_request(Request req){
    if(is_blocked(req.ip_in)){
        //Blocked requests are red
        cout << "\033[31mRequest from " << req.ip_in << " is blocked.\033[0m" << endl;
        return;
    }

    request_queue.push(req);

    //Adding requests are green
    cout << "\033[32mAdded request from " << req.ip_in << " to " << req.ip_out << " with job type " << req.job_type << " and time " << req.time << ".\033[0m" << endl;
}

/**
 * @brief Generates and adds a single random request to the queue.
 * 
 * Uses current configuration for IP addresses and processing time range.
 * Job type is randomly 'P' (processing) or 'S' (streaming).
 */
void LoadBalancer::add_random_request(){
    string ip_in = "10." + to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256);
    string ip_out = "172.16." + to_string(rand() % 256) + "." + to_string(rand() % 256);

    int time = rand() % (processing_time_max - processing_time_min + 1) + processing_time_min;

    if (rand() % 2 == 0){
        add_request({ip_in, ip_out, time, 'P'});
    } else {
        add_request({ip_in, ip_out, time, 'S'});
    }
}

/**
 * @brief Attempts to assign queued requests to idle web servers.
 * 
 * Processes the queue until empty or no idle servers remain.
 * Unassigned requests are re-queued for the next cycle.
 */
void LoadBalancer::assign_requests(){
    bool any_assigned = false;
    queue<Request> unassigned;

    while(!request_queue.empty()){
        Request req = request_queue.front();
        request_queue.pop();

        //Check if request is blocked
        if(is_blocked(req.ip_in)){
            //Blocked requests are red
            cout << "\033[31mRequest from " << req.ip_in << " is blocked.\033[0m" << endl;
            request_queue.pop();
            continue;
        }

        //Find available server
        bool assigned = false;
        for(WebServer& server : web_servers){
            if(!server.is_busy()){
                server.assign_request(req);
                //Assigned requests are blue
                cout << "\033[34mAssigned request from " << req.ip_in << " to " << req.ip_out << " with job type " << req.job_type << " and time " << req.time << ".\033[0m" << endl;
                assigned = true;
                any_assigned = true;
                break;
            }
        }

        //If no available servers, keep request in queue
        if (!assigned){
            unassigned.push(req);
        }
    }

    //Move unassigned requests back to the main queue
    while(!unassigned.empty()){
        request_queue.push(unassigned.front());
        unassigned.pop();
    }

    if(!any_assigned && !request_queue.empty()){
        cout << "All servers are busy. " << request_queue.size() << " requests remain in queue" << endl;
    }
}

/**
 * @brief Dynamically adjusts the number of web servers based on queue size.
 * 
 * - If queue > max_queue_per_server × current_servers, adds one server (with delay).
 * - If queue < min_queue_per_server × current_servers, removes one server (with delay, min 1 server).
 * Delays are reset after each scaling action to prevent rapid changes.
 */
void LoadBalancer::adjust_servers(){
    //Add or remove servers to keep queue balanced
    size_t current_servers = web_servers.size();
    size_t queue_size = request_queue.size();

    //Queue between 50-80 per server
    size_t max_cap = current_servers * max_queue_per_server;
    size_t min_cap;

    while(queue_size > max_cap && add_delay_counter <= 0){
        web_servers.emplace_back();
        add_delay_counter = delay_cycles;
        //Added servers are yellow
        cout << "\033[33mAdded a server. Total servers: " << web_servers.size() << "\033[0m" << endl;

        current_servers++;
        max_cap = current_servers * max_queue_per_server;
        queue_size = request_queue.size();
    }

    min_cap = current_servers * min_queue_per_server;

    if (queue_size < min_cap && current_servers > 1 && remove_delay_counter <= 0){
        web_servers.pop_back();
        remove_delay_counter = delay_cycles;
        //Removed servers are yellow
        cout << "\033[33mRemoved a server. Total servers: " << web_servers.size() << "\033[0m" << endl;
    }

    if(remove_delay_counter > 0){
        remove_delay_counter--;
    }

    if(add_delay_counter > 0){
        add_delay_counter--;
    }
}

/**
 * @brief Adds an IP prefix to the blocked list (firewall/DOS prevention).
 * @param ip_prefix The IP prefix to block (e.g., "10.0" blocks all 10.0.x.x addresses)
 */
void LoadBalancer::block_ip(const string& ip_prefix){
    blocked_ips.emplace_back(ip_prefix);
    cout << "Blocked IP range: " << ip_prefix << endl;
}

/**
 * @brief Checks if an IP address matches any blocked prefix.
 * @param ip The IP address to check
 * @return true if the IP is blocked, false otherwise
 */
bool LoadBalancer::is_blocked(const string& ip) const{
    for(const auto& blocked : blocked_ips){
        //IP prefix matching
        if(ip.substr(0, blocked.length()) == blocked){
            return true;
        }
    }
    return false;
}

/**
 * @brief Advances the simulation by one clock cycle.
 * 
 * In order:
 * 1. Processes active requests on all servers
 * 2. Assigns queued requests to idle servers
 * 3. Adds a random request with configured probability
 * 4. Checks/adjusts server count every 5 cycles
 * 5. Logs current state to simulation.log
 */
void LoadBalancer::advance_time(){
    time++;

    //Process requests on servers
    for(WebServer& server : web_servers){
        server.process_request();
    }

    //Assign requests to idle servers
    assign_requests();

    if((rand() / (double)RAND_MAX) < request_probability){
        add_random_request();
    }

    //Check if servers need to be adjusted every 5 cycles
    static int adjust_counter = 0;
    adjust_counter++;

    if(adjust_counter >= 5){
        adjust_servers();
        adjust_counter = 0;
    }


    ofstream log("simulation.log", ios::app);
    log << "Time: " << time << ", Queue size: " << request_queue.size() << ", Server count: " << web_servers.size() << endl;
    log.close();

}

/**
 * @brief Returns the current number of pending requests in the queue.
 * @return Current queue size
 */
size_t LoadBalancer::get_queue_size() const{
    return request_queue.size();
}

/**
 * @brief Returns the current number of active web servers.
 * @return Current server count
 */
size_t LoadBalancer::get_server_count() const{
    return web_servers.size();
}