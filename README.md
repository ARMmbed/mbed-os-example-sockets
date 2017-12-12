### Getting started with the network-socket API ###

This is a quick example of a simple HTTP client program using the
[network socket API](https://os.mbed.com/docs/latest/reference/network-socket.html) that [Mbed OS](https://github.com/ARMmbed/mbed-os) provides.

The program brings up an underlying network interface and uses it to perform an HTTP
transaction over a TCPSocket.

**Note:** The current example is limited to the ethernet interface on supported devices.
To use the example with a different interface, you need to modify `main.cpp` and
replace the EthernetInterface class with the appropriate interface.

**Note:** The default serial port baud rate is 9600 bit/s.

### Expected output ###

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

You can find more information on the network socket API in the [API reference documentation](https://os.mbed.com/docs/latest/reference/network-socket.html).

## Troubleshooting

If you have problems, you can review the [documentation](https://os.mbed.com/docs/latest/tutorials/debugging.html) for suggestions on what could be wrong and how to fix it.
