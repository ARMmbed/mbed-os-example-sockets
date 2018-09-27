## Getting started with the network-socket API

This is a quick example of a simple HTTP client program using the
[network-socket API](https://os.mbed.com/docs/latest/reference/network-socket.html) that [Mbed OS](https://github.com/ARMmbed/mbed-os) provides.

The program brings up an underlying network interface, and uses it to perform an HTTP
transaction over a TCPSocket.

### Selecting the network interface

This application is able to use any network inteface it finds. Please see the Mbed OS documentationg for [selecting the default network interface](https://os.mbed.com/docs/v5.10/apis/network-interfaces.html).

For example, building on Ethernet enabled boards, you do not do any configuration.

Building for WiFi boards, you need to provide SSID, password and security settings in `mbed_app.json` as instructed in the documentation. For example, like this:

```
{
    "target_overrides": {
        "*": {
            "platform.stdio-convert-newlines": true,
            "target.network-default-interface-type": "WIFI",
            "nsapi.default-wifi-security": "WPA_WPA2",
            "nsapi.default-wifi-ssid": "\"ssid\"",
            "nsapi.default-wifi-password": "\"password\""
        }
    }
}
```

Building for boards that have more that one network interface, you might need to override `target.network-default-interface-type` variable.

### Building

```
mbed compile -t <toolchain> -m <target>
```

For example, building for K64F using GCC: `mbed compile -t GCC_ARM -m K64F`

### Expected output ###

**Note:** The default serial port baud rate is 9600 bit/s.

```
IP address: 10.118.14.45
Netmask: 255.255.252.0
Gateway: 10.118.12.1
sent 39 [GET / HTTP/1.1]
recv 173 [HTTP/1.1 200 OK]
External IP address: 217.140.111.135
Done
```

### Documentation ###

More information on the network-socket API can be found in the [mbed handbook](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/communication/network_sockets/).

## Troubleshooting

If you have problems, you can review the [documentation](https://os.mbed.com/docs/latest/tutorials/debugging.html) for suggestions on what could be wrong and how to fix it.
