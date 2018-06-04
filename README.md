### Getting started with the network-socket API ###

This is a quick example of a simple HTTP client program using the
[network-socket API](https://os.mbed.com/docs/latest/reference/network-socket.html) that [Mbed OS](https://github.com/ARMmbed/mbed-os) provides.

The program brings up an underlying network interface, and uses it to perform an HTTP
transaction over a TCPSocket.

This example uses [easy-connect](https://github.com/ARMmbed/easy-connect/) as the network bearer abstraction model. Please configure the `mbed_app.json` accordingly to use the network stack you want to use.

### Building

```
mbed compile -t <toolchain> -m <target>
```

For example, building for K64F using GCC: `mbed compile -t GCC_ARM -m K64F`

### Expected output ###

**Note:** The default serial port baud rate is 9600 bit/s.

```
[EasyConnect] IPv4 mode
[EasyConnect] Using Ethernet
[EasyConnect] Connected to Network successfully
[EasyConnect] MAC address 02:46:38:b7:e9:a8
[EasyConnect] IP address 10.45.0.44
IP address: 10.45.0.44
Netmask: 255.255.254.0
Gateway: 10.45.0.1
sent 58 [GET / HTTP/1.1]https://github.com/ARMmbed/easy-connect/
recv 181 [HTTP/1.1 200 OK]
External IP address: 217.140.96.140
Done
```

### Documentation ###

More information on the network-socket API can be found in the [mbed handbook](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/communication/network_sockets/).

## Troubleshooting

If you have problems, you can review the [documentation](https://os.mbed.com/docs/latest/tutorials/debugging.html) for suggestions on what could be wrong and how to fix it.
