#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

class WebServer{
    private:
        Request* current_request = nullptr;
        int remaining_time = 0;
    public:
        WebServer();
        ~WebServer();

        void assign_request(const Request& req);
        void process_request();
        bool is_busy() const;
};

#endif