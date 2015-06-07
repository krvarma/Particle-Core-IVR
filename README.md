Interactive Voice Response system using Particle Core, GSM Breakout Board, Emic2 and DTMF Decoder
------------------------------------------------------------------------

My previous project using was a Voice Messaging system based on Particle Core, GSM Breakout Board and Emic2 TTS Module. Actually this was the project I first started but because of the unavailability of the DTMF Breakout Board, I released the Voice Messaging project.

**Overview**

This project is an IVR System. Following hardware components are using for this project.

1. Particle Core as main processor and for Internet connectivity
2. A GSM Breakout Board for Cellular connectivity
3. [SparkFun I2C/SPI-to-UART Breakout Board](https://www.sparkfun.com/products/9981) for SPI to UART
4. [Emic2 Text to Speech module](https://www.sparkfun.com/products/11711) for TTS
5. [DTMF Breakout Board](http://www.seeedstudio.com/depot/DTMFDual-Tone-Multi-Frequency-Shield-p-1839.html)

Here is the overview of the different hardware components and how they interact.

![enter image description here](https://raw.githubusercontent.com/krvarma/Particle-Core-IVR/master/images/IVR%20Overview.png)

This project is almost similar to my previous Voice Messaging project except it has an additional DTMF Breakout Board. This is used to decode the DTMF signals. This board is used to detect which number user has pressed on other end. The Audio Out of the GSM Breakout Board is connected to the Audio Input of the DTMF Decoder and the pins DR, D0, D1, D2 and D3 are connected to Particle Core pins D2, D3, D4, D5 and D6 respectively. The Data Ready (DR) pin of the DTMF Board will be high when the boards detects the DTMF tone and it is decoded correctly.  When this pin is high, we read the data pins D0...D3, this will be give us the number pressed on the other end of the telephone line.

In this sample the GSM Board is configured to answer the incoming call after 2 rings, so when a call is received it will be automatically answer after 2 rings. When the call is answered, application will speak out a welcome message and prompts the user to enter a 4 digit PIN. The default PIN is `9999`, but you can change it whatever you like. When the correct PIN is entered, the application will ask you to enter 7, 8 or 9. Pressing 7 will speak out the current temperature, pressing 8 will speak out current humidity and pressing 9 will hang up the call. The sample use DHT Sensor to read temperature and humidity. This sensor is connected to the pin D0 of Particle Core.

**Wiring**

*Particle Core and I2C/SPI-to-UART*

 - Core MOSI to I2C/SPI-to-UART MOSI
 - Core MISO to I2C/SPI-to-UART MISO
 - Core SCK to I2C/SPI-to-UART SCK
 - Core SS to I2C/SPI-to-UART SS

*I2C/SPI-to-UART and GSM Shield*

 - I2C/SPI-to-UART Vcc to 3.3v 
 - I2C/SPI-to-UART GND to GND
 - I2C/SPI-to-UART Tx to GSM Board Rx 
 - I2C/SPI-to-UART Rx to GSM Board Tx
 - GSM Board 5v to 5v 
 - GSM Board GND to GND

*Particle Core and Emic2 TTS Module*

 - Core Tx to Emic2 Rx 
 - Core Rx to Emic2 Tx 
 - Emic2 5v to 5v 
 - Emic2 GND to GND

*DTMF Board and GSM Breakout Board*

 - DTMF Board Audio In to GSM Board Audio Out

*DTMF Board to Particle Core*

 - DTMF Board 5v to 5v
 - DTMF GND to GND
 - DTMF DR to Particle Core D2
 - DTMF D0 to Particle Core D3
 - DTMF D1 to Particle Core D4
 - DTMF D2 to Particle Core D5
 - DTMF D3 to Particle Core D6

**Screenshots**

![enter image description here](https://raw.githubusercontent.com/krvarma/Particle-Core-IVR/master/images/wiring.jpg)

**Demo Video**

https://www.youtube.com/watch?v=sPZ4FBCrNjo