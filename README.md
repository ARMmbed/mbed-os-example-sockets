### Getting started with the network-socket API ###

This is a quick example of a simple HTTP client program using the
network-socket API that is provided as a part of [mbed-os](github.com/armmbed/mbed-os).

The program brings up an underlying network interface, and uses it to perform an HTTP
transaction over a TCPSocket.

**Note:** The current example is limited to the ethernet interface on supported devices.
To use the example with a different interface, you will need to modify main.cpp and
replace the EthernetInterface class with the appropriate interface.

### Documentation ###

More information on the network-socket API can be found in the [mbed handbook](https://docs.mbed.com/docs/mbed-os-api-reference/en/5.1/APIs/communication/network_sockets/).

### Known issues

- ARCH_PRO runtime fails for all toolchains - issue [here](https://github.com/ARMmbed/mbed-os-example-sockets/issues/17)
