#ifndef REQUEST_H
#define REQUEST_H

#include <string>
using namespace std;

/**
 * @struct Request
 * @brief Represents a single request/job in the load balancer simulation.
 *
 * This lightweight struct holds the essential information for each incoming request:
 * source and destination IP addresses, estimated processing time, and job type.
 */
struct Request{
    string ip_in;      ///< Source IP address of the request
    string ip_out;     ///< Destination IP address of the request

    int time;          ///< Estimated processing time in clock cycles
    char job_type;     ///< Job type: 'P' for Processing, 'S' for Streaming
};

#endif