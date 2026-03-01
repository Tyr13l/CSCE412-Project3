#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @class WebServer
 * @brief Represents an individual web server in the load balancer.
 *
 * Each WebServer instance processes one request at a time.
 * It tracks the current request and remaining processing time.
 * Servers become idle when remaining_time reaches zero.
 */
class WebServer{
    private:
        Request* current_request = nullptr;     ///< Pointer to the currently assigned request (owned by this server)
        int remaining_time = 0;                 ///< Remaining clock cycles needed to finish the current request
    public:
        /**
         * @brief Default constructor.
         * Initializes an idle server with no request.
         */
        WebServer();

        /**
         * @brief Destructor.
         * Cleans up any dynamically allocated request.
         */
        ~WebServer();

        /**
         * @brief Assigns a new request to this server.
         * @param req Reference to the request to process (copied internally)
         * 
         * If the server is already busy, prints an error to cerr and ignores the request.
         */
        void assign_request(const Request& req);

        /**
         * @brief Processes one clock cycle of the current request.
         * 
         * Decrements remaining_time. If it reaches zero or below,
         * the request is considered complete and the server becomes idle.
         */
        void process_request();

        /**
         * @brief Checks if the server is currently processing a request.
         * @return true if busy, false if idle
         */
        bool is_busy() const;
};

#endif