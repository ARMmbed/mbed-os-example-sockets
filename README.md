### Getting started with the network-socket API ###

This is a quick example of a simple HTTP client program using the
network-socket API that is provided as a part of [mbed-os](github.com/armmbed/mbed-os).

The program brings up an underlying network interface, and uses it to perform an HTTP
transaction over a TCPSocket.

### Ethernet ###

By default, the example should use the ethernet interface available on the board. Simply
compile, flash, and run.

### WiFi ###

To enable WiFi, you will need an [esp8266](https://developer.mbed.org/teams/ESP8266/).
To enable WiFi in the example, you will need to define three defines during compile time.

``` bash
-DMBED_DEMO_WIFI            # enables wifi
-DMBED_DEMO_WIFI_SSID=ssid  # ssid
-DMBED_DEMO_WIFI_PASS=pass  # passphrase
```

### Documentation ###

More information on the network-socket API can be found in the [mbed handbook](https://docs.mbed.com/docs/mbed-os-api-reference/en/5.1/APIs/communication/network_sockets/).
