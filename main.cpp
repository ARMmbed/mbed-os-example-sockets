#include "mbed.h"

// Network interface
NetworkInterface *net;

// Socket demo
int main() {
    int remaining;
    int rcount;
    char *p;
    char *buffer = new char[256];
    nsapi_size_or_error_t r;

    // Bring up the ethernet interface
    printf("Mbed OS Socket example\n");

#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif

    net = NetworkInterface::get_default_instance();

    if (!net) {
        printf("Error! No network inteface found.\n");
        return 0;
    }

    r = net->connect();
    if (r != 0) {
        printf("Error! net->connect() returned: %d\n", r);
    }

    // Show the network address
    const char *ip = net->get_ip_address();
    const char *netmask = net->get_netmask();
    const char *gateway = net->get_gateway();
    printf("IP address: %s\n", ip ? ip : "None");
    printf("Netmask: %s\n", netmask ? netmask : "None");
    printf("Gateway: %s\n", gateway ? gateway : "None");

    // Open a socket on the network interface, and create a TCP connection to mbed.org
    TCPSocket socket;
    r = socket.open(net);
    if (r != 0) {
        printf("Error! socket.open() returned: %d\n", r);
    }

    r = socket.connect("api.ipify.org", 80);
    if (r != 0) {
        printf("Error! socket.connect() returned: %d\n", r);
    }

    // Send a simple http request
    char sbuffer[] = "GET / HTTP/1.1\r\nHost: api.ipify.org\r\nConnection: close\r\n\r\n";
    nsapi_size_t size = strlen(sbuffer);

    // Loop until whole request send
    while(size) {
        r = socket.send(sbuffer+r, size);
        if (r < 0) {
            printf("Error! socket.connect() returned: %d\n", r);
            goto disconnect;
        }
        size -= r;
        printf("sent %d [%.*s]\n", r, strstr(sbuffer, "\r\n")-sbuffer, sbuffer);
    }

    // Receieve an HTTP response and print out the response line
    remaining = 256;
    rcount = 0;
    p = buffer;
    while (0 < (r = socket.recv(p, remaining))) {
        p += r;
        rcount += r;
        remaining -= r;
    }
    if (r < 0) {
        printf("Error! socket.recv() returned: %d\n", r);
        goto disconnect;
    }

    printf("recv %d [%.*s]\n", rcount, strstr(buffer, "\r\n")-buffer, buffer);

    // The api.ipify.org service also gives us the device's external IP address
    p = strstr(buffer, "\r\n\r\n")+4;
    printf("External IP address: %.*s\n", rcount-(p-buffer), p);
    delete[] buffer;

disconnect:
    // Close the socket to return its memory and bring down the network interface
    socket.close();

    // Bring down the ethernet interface
    net->disconnect();
    printf("Done\n");
}
