#include "WebServer.h"
#include <iostream>

using namespace std;

WebServer::WebServer(){

}

WebServer::~WebServer(){

}

void WebServer::assign_request(Request* req){
    if(current_request != nullptr){
        cerr << "Error: WebServer is already processing a request." << endl;
        return;
    }

    current_request = req;
    remaining_time = req->time;
}

void WebServer::process_request(){
    if(current_request == nullptr){
        return;
    }

    remaining_time--;

    if(remaining_time <= 0){
        current_request = nullptr;
    }
}

bool WebServer::is_busy() const{
    return current_request != nullptr;
}