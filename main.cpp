#include "mbed.h"
#include "TCPSocket.h"
#include "EthernetInterface.h"


// Socket demo
void http_demo(NetworkInterface *net) {
    TCPSocket socket;

    // Show the network address
    const char *ip = net->get_ip_address();
    printf("IP address is: %s\n", ip ? ip : "No IP");

    // Open a socket on the network interface, and create a TCP connection to mbed.org
    socket.open(net);
    socket.connect("developer.mbed.org", 80);

    // Send a simple http request
    char sbuffer[] = "GET / HTTP/1.1\r\nHost: developer.mbed.org\r\n\r\n";
    int scount = socket.send(sbuffer, sizeof sbuffer);
    printf("sent %d [%.*s]\r\n", scount, strstr(sbuffer, "\r\n")-sbuffer, sbuffer);

    // Recieve a simple http response and print out the response line
    char rbuffer[64];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    printf("recv %d [%.*s]\r\n", rcount, strstr(rbuffer, "\r\n")-rbuffer, rbuffer);

    // Close the socket to return its memory and bring down the network interface
    socket.close();
}


EthernetInterface eth;

int main() {
    // Brings up the ethernet interface
    printf("Ethernet socket example\n");
    eth.connect();

    // Invoke the demo
    http_demo(&eth);

    // Brings down the ethernet interface
    eth.disconnect();

    printf("Done\n");
}

