#include "WebServer.h"
#include <iostream>

using namespace std;

/**
 * @brief Default constructor for WebServer.
 * 
 * Initializes an idle server with no current request and zero remaining time.
 */
WebServer::WebServer(){

}

/**
 * @brief Destructor for WebServer.
 * 
 * Deletes the dynamically allocated current_request if it exists,
 * preventing memory leaks.
 */
WebServer::~WebServer(){
    if(current_request != nullptr){
        delete current_request;
        current_request = nullptr;
    }
}

/**
 * @brief Assigns a new request to this server.
 * @param req Constant reference to the request to process (a copy is made)
 * 
 * If the server is already busy, prints an error to cerr and ignores the request.
 * Otherwise, allocates a new Request copy and sets the remaining time.
 */
void WebServer::assign_request(const Request& req){
    if(current_request != nullptr){
        cerr << "Error: WebServer is already processing a request." << endl;
        return;
    }

    current_request = new Request(req);
    remaining_time = req.time;
}

/**
 * @brief Processes one clock cycle of the current request.
 * 
 * Decrements remaining_time. If it reaches zero or below,
 * clears the current request pointer, making the server idle again.
 */
void WebServer::process_request(){
    if(current_request == nullptr){
        return;
    }

    remaining_time--;

    if(remaining_time <= 0){
        current_request = nullptr;
    }
}

/**
 * @brief Checks if the server is currently processing a request.
 * @return true if a request is active (remaining_time > 0), false if idle
 */
bool WebServer::is_busy() const{
    return current_request != nullptr;
}