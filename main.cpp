#include "mbed.h"

// Network interface
NetworkInterface *net;

// Socket demo
int main() {
    int remaining;
    int rcount;
    char *p;
    char buffer[256];
    nsapi_size_or_error_t result;

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

    result = net->connect();
    if (result != 0) {
        printf("Error! net->connect() returned: %d\n", result);
        return result;
    }

    // Show the network address
    SocketAddress a;
    net->get_ip_address(&a);
    printf("IP address: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");
    net->get_netmask(&a);
    printf("Netmask: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");
    net->get_gateway(&a);
    printf("Gateway: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");

    // Open a socket on the network interface, and create a TCP connection to ifconfig.io
    TCPSocket socket;
    // Send a simple http request
    char hostname[] = "ifconfig.io";
    char sbuffer[] = "GET / HTTP/1.1\r\nHost: ifconfig.io\r\nConnection: close\r\n\r\n";
    nsapi_size_t size = strlen(sbuffer);

    result = socket.open(net);
    if (result != 0) {
        printf("Error! socket.open() returned: %d\n", result);
        goto DISCONNECT;
    }

    // Get the host address
    printf("\nResolve hostname %s\n", hostname);
    result = net->gethostbyname(hostname, &a);
    if (result != 0) {
        printf("Error! gethostbyname(%s) returned: %d\n", hostname, result);
        net->disconnect();
        exit(-1);
    }
    printf("%s address is %s\n", hostname, (a.get_ip_address() ? a.get_ip_address() : "None") );

    a.set_port(80);
    result = socket.connect(a);
    if (result != 0) {
        printf("Error! socket.connect() returned: %d\n", result);
        net->disconnect();
        exit(-2);
    }

    // Loop until whole request sent
    while (size) {
        result = socket.send(sbuffer+result, size);
        if (result < 0) {
            printf("Error! socket.send() returned: %d\n", result);
            goto DISCONNECT;
        }
        size -= result;
        printf("sent %d [%.*s]\n", result, strstr(sbuffer, "\r\n")-sbuffer, sbuffer);
    }

    // Receieve an HTTP response and print out the response line
    remaining = 256;
    rcount = 0;
    p = buffer;
    while (remaining > 0 && 0 < (result = socket.recv(p, remaining))) {
        p += result;
        rcount += result;
        remaining -= result;
    }
    if (result < 0) {
        printf("Error! socket.recv() returned: %d\n", result);
        goto DISCONNECT;
    }
	// the HTTP response code
    printf("recv %d [%.*s]\n", rcount, strstr(buffer, "\r\n")-buffer, buffer);

DISCONNECT:

    // Close the socket to return its memory and bring down the network interface
    socket.close();

    // Bring down the ethernet interface
    net->disconnect();
    printf("Done\n");
}
