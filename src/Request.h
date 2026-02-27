#ifndef REQUEST_H
#define REQUEST_H

#include <string>
using namespace std;

struct Request{
    string ip_in;
    string ip_out;

    int time;
    char job_type;
};

#endif