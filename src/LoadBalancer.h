#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "Request.h"
#include "WebServer.h"
#include <queue>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/**
 * @class LoadBalancer
 * @brief Central class for managing the load balancer simulation.
 *
 * This class handles a queue of incoming requests, a dynamic set of web servers,
 * IP range-based blocking for firewall/DOS prevention, random request generation,
 * and automatic server scaling to maintain a balanced queue size.
 *
 * The simulation advances time step-by-step via advance_time(), processing requests,
 * assigning them to idle servers, adding new random requests with configurable probability,
 * and adjusting the number of servers based on queue thresholds.
 */
class LoadBalancer{
    private:
        queue<Request> request_queue;                   ///< FIFO queue of pending requests
        vector<WebServer> web_servers;                  ///< Dynamic list of web server instances
        vector<string> blocked_ips;                     ///< List of IP prefixes blocked by the firewall
        int time;                                       ///< Current simulation clock cycle
        int min_queue_per_server;                       ///< Minimum queue size per server (threshold for removal)
        int max_queue_per_server;                       ///< Maximum queue size per server (threshold for addition)
        int delay_cycles;                               ///< Number of cycles to wait between scaling actions
        double request_probability;                     ///< Probability (0.0 to 1.0) of adding a random request per cycle
        int processing_time_min;                        ///< Minimum processing time for new random requests
        int processing_time_max;                        ///< Maximum processing time for new random requests
        int remove_delay_counter = 0;                   ///< Countdown timer before next server removal is allowed
        int add_delay_counter = 0;                      ///< Countdown timer before next server addition is allowed
    
    public:
        /**
         * @brief Constructor that initializes the load balancer with simulation parameters.
         * @param num_servers Initial number of web servers to create
         * @param min_per_server Minimum queue size per server (used to trigger server removal)
         * @param max_per_server Maximum queue size per server (used to trigger server addition)
         * @param delay_cyc Number of clock cycles to wait between scaling actions
         * @param request_prob Probability (0.0 to 1.0) of generating a random request each cycle
         * @param proc_min Minimum processing time (cycles) for newly generated requests
         * @param proc_max Maximum processing time (cycles) for newly generated requests
         */
        LoadBalancer(int num_servers, int min_per_server, int max_per_server, int delay_cyc, double request_prob, int proc_min, int proc_max);
        
        //Request management functions

        /**
         * @brief Adds a single request to the queue if not blocked by the firewall.
         * @param req The request to add (contains source/destination IP, time, and job type)
         */
        void add_request(Request req);

        /**
         * @brief Generates and adds a single random request to the queue.
         * 
         * Uses current configuration for IP addresses and processing time range.
         * Job type is randomly 'P' (processing) or 'S' (streaming).
         */
        void add_random_request();

        /**
         * @brief Attempts to assign queued requests to idle web servers.
         * 
         * Processes the queue until empty or no idle servers remain.
         * Unassigned requests are re-queued for the next cycle.
         */
        void assign_requests();

        //Server function

        /**
         * @brief Dynamically adjusts the number of web servers based on queue size.
         * 
         * - If queue > max_queue_per_server × current_servers, adds one server (with delay).
         * - If queue < min_queue_per_server × current_servers, removes one server (with delay, min 1 server).
         * Delays are reset after each scaling action to prevent rapid changes.
         */
        void adjust_servers();

        //IP blocking functions

        /**
         * @brief Adds an IP prefix to the blocked list (firewall/DOS prevention).
         * @param ip_prefix The IP prefix to block (e.g., "10.0" blocks all 10.0.x.x addresses)
         */
        void block_ip(const string& ip_prefix);

        /**
         * @brief Checks if an IP address matches any blocked prefix.
         * @param ip The IP address to check
         * @return true if the IP is blocked, false otherwise
         */
        bool is_blocked(const string& ip) const;

        //Time function

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
        void advance_time();

        //Getter functions

        /**
         * @brief Returns the current number of pending requests in the queue.
         * @return Current queue size
         */
        size_t get_queue_size() const;

        /**
         * @brief Returns the current number of active web servers.
         * @return Current server count
         */
        size_t get_server_count() const;

};

#endif