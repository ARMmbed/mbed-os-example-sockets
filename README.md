![](./resources/official_armmbed_example_badge.png)
# Socket Example

This example shows usage of [network-socket API](https://os.mbed.com/docs/latest/reference/network-socket.html).

The program brings up an underlying network interface and if it's Wifi also scans for access points.
It creates a TCPSocket and performs an HTTP transaction targeting the website in the `mbed_app.json` config.

(Note: To see this example in a rendered form you can import into the Arm Mbed Online Compiler,
please see [the documentation](https://os.mbed.com/docs/mbed-os/latest/apis/socket.html#socket-example).)

## Selecting the network interface

This application is able to use any network interface it finds.
Please see the Mbed OS documentation for [selecting the default network interface](https://os.mbed.com/docs/v5.10/apis/network-interfaces.html).

For example, building on Ethernet enabled boards, you do not need any further configuration.

### WiFi

If you want to use WiFi you need to provide SSID, password and security settings in `mbed_app.json`.

If your board doesn't provide WiFi as the default interface because it has multiple interfaces you need to specify that you want WiFi in `mbed_app.json`.

```
{
    "target_overrides": {
        "*": {
            "target.network-default-interface-type": "WIFI",
        }
    }
}
```

For more information about Wi-Fi APIs, please visit the [Mbed OS Wi-Fi](https://os.mbed.com/docs/latest/reference/wi-fi.html) documentation.

### Supported WiFi hardware

* All Mbed OS boards with build-in Wi-Fi module:
    * [ST DISCO IOT board](https://os.mbed.com/platforms/ST-Discovery-L475E-IOT01A/) with integrated [ISM43362 WiFi Inventek module](https://github.com/ARMmbed/wifi-ism43362).
    * [ST DISCO_F413ZH board](https://os.mbed.com/platforms/ST-Discovery-F413H/) with integrated [ISM43362 WiFi Inventek module](https://github.com/ARMmbed/wifi-ism43362).
* Boards with external WiFi shields.
    * [NUCLEO-F429ZI](https://os.mbed.com/platforms/ST-Nucleo-F429ZI/) with ESP8266-01 module using pins D1 and D0.

## Building and flashing the example

### To build the example

Clone the repository containing the collection of examples:

```
git clone https://github.com/ARMmbed/mbed-os-example-sockets.git
cd mbed-os-example-sockets
```

**Tip:** If you don't have git installed, you can [download a zip file](https://github.com/ARMmbed/mbed-os-example-sockets/archive/master.zip) of the repository.

Update the source tree:

```mbed deploy```

Run the build:

```mbed compile -t <ARM | GCC_ARM> -m <YOUR_TARGET>```

### To flash the example onto your board

Connect your mbed board to your computer over USB. It appears as removable storage.

When you run the `mbed compile` command above, mbed cli creates a .bin or a .hex file (depending on your target) in
```BUILD/<target-name>/<toolchain>``` under the example's directory. Drag and drop the file to the removable storage.

## Running the example


When example application is running information about activity is printed over the serial connection.

**Note:** The default serial baudrate has been set to 115200.

Please have a client open and connected to the board. You may use:

- [Tera Term](https://ttssh2.osdn.jp/index.html.en) for windows

- screen or minicom for Linux (example usage: `screen /dev/serial/<your board> 115200`)

- mbed tools have terminal command `mbed term -b 115200`

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
