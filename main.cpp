#include "mbed.h"
#include "EthernetInterface.h"

// Network interface
EthernetInterface net;

// Socket demo
int main() {
    // Bring up the ethernet interface
    printf("Ethernet socket example\n");

#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif

    net.connect();

    // Show the network address
    const char *ip = net.get_ip_address();
    const char *netmask = net.get_netmask();
    const char *gateway = net.get_gateway();
    printf("IP address: %s\n", ip ? ip : "None");
    printf("Netmask: %s\n", netmask ? netmask : "None");
    printf("Gateway: %s\n", gateway ? gateway : "None");

    // Open a socket on the network interface, and create a TCP connection to mbed.org
    TCPSocket socket;
    socket.open(&net);
    socket.connect("api.ipify.org", 80);
    char *buffer = new char[256];

    // Send an HTTP request
    strcpy(buffer, "GET / HTTP/1.1\r\nHost: api.ipify.org\r\nConnection: close\r\n\r\n");
    int scount = socket.send(buffer, strlen(buffer));
    printf("sent %d [%.*s]\n", scount, strstr(buffer, "\r\n")-buffer, buffer);

    // Recieve an HTTP response and print out the response line
    int received = 0;
    int remaining = 256;
    int rcount = 0;
    char *p = buffer;
    while (0 != (received = socket.recv(p, remaining))) {
        p += received;
        rcount += received;
        remaining -= received;
    }
    printf("recv %d [%.*s]\n", rcount, strstr(buffer, "\r\n")-buffer, buffer);

    // The api.ipify.org service also gives us the device's external IP address
    const char *payload = strstr(buffer, "\r\n\r\n")+4;
    printf("External IP address: %.*s\n", rcount-(payload-buffer), payload);

    // Close the socket to return its memory and bring down the network interface
    socket.close();
    delete[] buffer;

    // Bring down the ethernet interface
    net.disconnect();
    printf("Done\n");
}
