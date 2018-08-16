# To get this working:

- Replace the dummy value of `WIFI_PASSWORD` with the actual password
- Install the NeoPixelBus library: In your `~/Documents/Arduino/Libraries` folder, `git clone https://github.com/Makuna/NeoPixelBus`
- Install the AWS_IOT library: `git clone git@github.com:ExploreEmbedded/Hornbill-Examples.git` and put the `arduino-esp32/AWS_IOT` folder into your `~/Documents/Arduino/Libraries` folder.
- In `~/Documents/Arduino/Libraries/AWS_IOT/src/aws_iot_certficates.c`, replace the dummy values. `aws_root_ca_pem` comes from https://www.symantec.com/content/en/us/enterprise/verisign/roots/VeriSign-Class%203-Public-Primary-Certification-Authority-G5.pem . `certificate_pem_crt` and `private_pem_key` are the certificate and private key that you registered for your Thing in the AWS IoT console.

# To use it:

- When power is connected, the blue LED on the board will light up. After a few seconds, when the device has successfully subscribed to an MQTT topic, the LED will turn off. If this doesn't happen, the serial console may help you debug.
- Once it's subscribed to an MQTT topic, it expects messages which are a sequence of binary 5-byte records: 2-byte LED number (MSB first), then 3-byte colour (RGB).
