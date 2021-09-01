![](./resources/official_armmbed_example_badge.png)
# Socket Example

This example shows usage of [network-socket API](https://os.mbed.com/docs/mbed-os/latest/apis/network-socket.html).

The program brings up an underlying network interface and if it's Wifi also scans for access points.
It creates a TCPSocket and performs an HTTP transaction targeting the website in the `mbed_app.json` config.

The example can be configured to use a TLSSocket. This works only on devices that support TRNG.

## Selecting the network interface

This application is able to use any network interface it finds.

The interface selections is done through weak functions that are overridden by your selected target or any additional
component that provides a network interface.

If more than one interface is provided the target configuration `target.network-default-interface-type`
selects the type provided as the default one. This is usually the Ethernet so building on Ethernet enabled boards,
you do not need any further configuration.

### Configuring mbedtls

By default the examples uses a TCP socket. To enable TLS edit the mbed_app.json to turn on the `use-tls-socket` option:

```
        "use-tls-socket": {
            "value": true
        }
```

It might be necessary to configure the mbedtls library with appropriate macros in mbed_app.json file. Some boards
(like UBLOX_EVK_ODIN_W2) will work fine without any additional configuration and some of them might require some minimal
adjustment. For example K64F requires at least the following macro added:


```
        "K64F": {
            "target.macros_add" : ["MBEDTLS_SHA1_C"]
        }
```

See
[mbedtls configuration guidelines](https://github.com/ARMmbed/mbed-os/tree/master/connectivity/mbedtls#configuring-mbed-tls-features)
for more details.

Also see the API Documentation [TLSSocket](https://os.mbed.com/docs/mbed-os/latest/apis/tlssocket.html).

### WiFi

If you want to use WiFi you need to provide SSID, password and security settings in `mbed_app.json`.

If your board doesn't provide WiFi as the default interface because it has multiple interfaces you need to specify that
you want WiFi in `mbed_app.json`.

```
{
    "target_overrides": {
        "*": {
            "target.network-default-interface-type": "WIFI",
        }
    }
}
```

For more information about Wi-Fi APIs, please visit the
[Mbed OS Wi-Fi](https://os.mbed.com/docs/mbed-os/latest/apis/wi-fi.html)
documentation.

### Supported WiFi hardware

* All Mbed OS boards with build-in Wi-Fi module such as:
    * [ST DISCO IOT board](https://os.mbed.com/platforms/ST-Discovery-L475E-IOT01A/) with integrated
      [ISM43362 WiFi Inventek module](https://github.com/ARMmbed/wifi-ism43362).
    * [ST DISCO_F413ZH board](https://os.mbed.com/platforms/ST-Discovery-F413H/) with integrated
      [ISM43362 WiFi Inventek module](https://github.com/ARMmbed/wifi-ism43362).
* Boards with external WiFi shields such as:
    * [NUCLEO-F429ZI](https://os.mbed.com/platforms/ST-Nucleo-F429ZI/) with ESP8266-01

## Building and flashing the example

### Mbed OS build tools

#### Mbed CLI 2

Starting with version 6.5, Mbed OS uses Mbed CLI 2. It uses Ninja as a build system, and CMake to generate the build environment and manage the build process in a compiler-independent manner. If you are working with Mbed OS version prior to 6.5 then check the section [Mbed CLI 1](#mbed-cli-1).
1. [Install Mbed CLI 2](https://os.mbed.com/docs/mbed-os/latest/build-tools/install-or-upgrade.html).
1. From the command-line, import the example: `mbed-tools import mbed-os-example-sockets`
1. Change the current directory to where the project was imported.

#### Mbed CLI 1
1. [Install Mbed CLI 1](https://os.mbed.com/docs/mbed-os/latest/quick-start/offline-with-mbed-cli.html).
1. From the command-line, import the example: `mbed import mbed-os-example-sockets`
1. Change the current directory to where the project was imported.

### To build the example

1. Connect a USB cable between the USB port on the board and the host computer.
1. Run the following command to build the example project and program the microcontroller flash memory:

    * Mbed CLI 2

    ```bash
    $ mbed-tools compile -m <TARGET> -t <TOOLCHAIN> --flash --sterm
    ```

    * Mbed CLI 1

    ```bash
    $ mbed compile -m <TARGET> -t <TOOLCHAIN> --flash --sterm
    ```

Your PC may take a few minutes to compile your code.

The binary is located at:
* **Mbed CLI 2** - `./cmake_build/<TARGET>/<PROFILE>/<TOOLCHAIN>/mbed-os-example-sockets.bin`</br>
* **Mbed CLI 1** - `./BUILD/<TARGET>/<TOOLCHAIN>/mbed-os-example-sockets.bin`

Alternatively, you can manually copy the binary to the board, which you mount on the host computer over USB.

You can also open a serial terminal separately, rather than using the `--sterm` option, with the following command:

* Mbed CLI 2

```bash
$ mbed-tools sterm
```

* Mbed CLI 1

```bash
$ mbed sterm
```

### Expected output

(Assuming you are using a wifi interface, otherwise the scanning will be skipped)

```
Starting socket demo

2 networks available:
Network: Virgin Media secured: Unknown BSSID: 2A:35:D1:ba:c7:41 RSSI: -79 Ch: 6
Network: VM4392164 secured: WPA2 BSSID: 18:35:D1:ba:c7:41 RSSI: -79 Ch: 6

Connecting to the network...
IP address: 192.168.0.27
Netmask: 255.255.255.0
Gateway: 192.168.0.1

Resolve hostname ifconfig.io
ifconfig.io address is 104.24.122.146

sent 52 bytes: 
GET / HTTP/1.1
Host: ifconfig.io
Connection: close

received 256 bytes:
HTTP/1.1 200 OK

Demo concluded successfully 
```

## License and contributions

The software is provided under Apache-2.0 license. Contributions to this project are accepted under the same license.
Please see [contributing.md](CONTRIBUTING.md) for more info.

This project contains code from other projects. The original license text is included in those source files.
They must comply with our license guide
