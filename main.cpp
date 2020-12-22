/* Sockets Example
 * Copyright (c) 2016-2020 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"

class SocketDemo {
    static const size_t MAX_NUMBER_OF_ACCESS_POINTS = 10;
    static const size_t MAX_MESSAGE_RECEIVED_LENGTH = 100;

public:
    SocketDemo() : _net(NetworkInterface::get_default_instance())
    {

    }

    ~SocketDemo()
    {
        _net->disconnect();
    }

    void run()
    {
        if (!_net) {
            printf("Error! No network interface found.\r\n");
            return;
        }

        /* if we're using a wifi interface run a quick scan */
        if (_net->wifiInterface()) {
            /* the scan is not required to connect and only serves to show visible access points */
            wifi_scan();

            /* in this example we use credentials configured at compile time which are used by
             * NetworkInterface::connect() but it's possible to do this at runtime by using the
             * WiFiInterface::connect() which takes these parameters as arguments */
        }

        /* connect will perform the action appropriate to the interface type to connect to the network */

        printf("Connecting to the network...\r\n");

        nsapi_size_or_error_t result = _net->connect();
        if (result != 0) {
            printf("Error! _net->connect() returned: %d\r\n", result);
            return;
        }

        print_network_info();

        /* opening the socket only allocates resources */
        result = socket.open(_net);
        if (result != 0) {
            printf("Error! socket.open() returned: %d\r\n", result);
            return;
        }

        /* now we have to find where to connect */
        SocketAddress address;

        if (!resolve_hostname(address)) {
            return;
        }

        /* set standard HTTP port */
        address.set_port(80);

        /* finally connect */
        result = socket.connect(address);
        if (result != 0) {
            printf("Error! socket.connect() returned: %d\r\n", result);
            return;
        }

        /* exchange an HTTP request and response */

        if (!send_http_request()) {
            return;
        }

        if (!receive_http_response()) {
            return;
        }

        printf("Demo concluded successfully \r\n");
    }

private:
    bool resolve_hostname(SocketAddress &address)
    {
        const char hostname[] = MBED_CONF_APP_HOSTNAME;

        /* get the host address */
        printf("\nResolve hostname %s\r\n", hostname);
        nsapi_size_or_error_t result = _net->gethostbyname(hostname, &address);
        if (result != 0) {
            printf("Error! gethostbyname(%s) returned: %d\r\n", hostname, result);
            return false;
        }

        printf("%s address is %s\r\n", hostname, (address.get_ip_address() ? address.get_ip_address() : "None") );

        return true;
    }

    bool send_http_request()
    {
        /* loop until whole request sent */
        const char buffer[] = R"(GET / HTTP/1.1
Host: ifconfig.io
Connection: close

)";

        nsapi_size_t bytes_to_send = strlen(buffer);

        while (bytes_to_send) {
            nsapi_size_or_error_t result = socket.send(buffer + result, bytes_to_send);
            if (result < 0) {
                printf("Error! socket.send() returned: %d\r\n", result);
                return false;
            }

            bytes_to_send -= result;
        }

        printf("\r\nsent %d bytes: \r\n%s", strlen(buffer), buffer);

        return true;
    }

    bool receive_http_response()
    {
        char buffer[MAX_MESSAGE_RECEIVED_LENGTH];
        int remaining_bytes = MAX_MESSAGE_RECEIVED_LENGTH;
        int received_bytes = 0;

        /* loop until there is nothing received or we've ran out of buffer space */
        while (remaining_bytes > 0) {
            nsapi_size_or_error_t result = socket.recv(buffer + received_bytes, remaining_bytes);
            if (result < 0) {
                printf("Error! socket.recv() returned: %d\r\n", result);
                return false;
            }

            received_bytes += result;
            remaining_bytes -= result;
        }

        /* the message is likely larger but we only want the HTTP response code */

        printf("received %d bytes:\r\n%.*s\r\n\r\n", received_bytes, strstr(buffer, "\n") - buffer, buffer);

        return true;
    }

    void wifi_scan()
    {
        WiFiInterface *wifi = _net->wifiInterface();

        WiFiAccessPoint ap[MAX_NUMBER_OF_ACCESS_POINTS];

        /* scan call returns number of access points found */
        int result = wifi->scan(ap, MAX_NUMBER_OF_ACCESS_POINTS);

        if (result <= 0) {
            printf("WiFiInterface::scan() failed with return value: %d\r\n", result);
            return;
        }

        printf("%d networks available:\r\n", result);

        for (int i = 0; i < result; i++) {
            printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\r\n",
                   ap[i].get_ssid(), get_security_string(ap[i].get_security()),
                   ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
                   ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5],
                   ap[i].get_rssi(), ap[i].get_channel());
        }
        printf("\r\n");
    }

    void print_network_info()
    {
        /* print the network info */
        SocketAddress a;
        _net->get_ip_address(&a);
        printf("IP address: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
        _net->get_netmask(&a);
        printf("Netmask: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
        _net->get_gateway(&a);
        printf("Gateway: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
    }

    static const char *get_security_string(nsapi_security_t sec)
    {
        switch (sec) {
            case NSAPI_SECURITY_NONE:
                return "None";
            case NSAPI_SECURITY_WEP:
                return "WEP";
            case NSAPI_SECURITY_WPA:
                return "WPA";
            case NSAPI_SECURITY_WPA2:
                return "WPA2";
            case NSAPI_SECURITY_WPA_WPA2:
                return "WPA/WPA2";
            case NSAPI_SECURITY_UNKNOWN:
            default:
                return "Unknown";
        }
    }

private:
    NetworkInterface *_net;
    TCPSocket socket;
};

int main() {
    SocketDemo example;

    printf("\r\nStarting socket demo\r\n\r\n");

    example.run();
}
