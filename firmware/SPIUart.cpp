#include "SPIUart.h"

#define WRITE       0B00000000
#define READ        0B10000000

//Register Mapping
#define XHR         0x00
#define FCR         0x02
#define LCR         0x03
#define MCR         0x04
#define LSR         0x05
#define MSR         0x06
#define TXLVL       0x8
#define RXLVL       0x9
#define DLL         0x00
#define DLH         0x01

void SPIUart::begin(){
    chipSelectPin = SS;
    
    SPI.begin();
    
    //Data is transmitted and received MSB first
    SPI.setBitOrder(MSBFIRST);  
    
    //SPI interface will run at 1MHz if 8MHz chip or 2Mhz if 16Mhz
    SPI.setClockDivider(SPI_CLOCK_DIV8); 
    
    //Data is clocked on the rising edge and clock is low when inactive
    SPI.setDataMode(SPI_MODE0);
    
    delay(5000);

    Serial.println("Starting....");

    // initalize chip select pin:
    pinMode(chipSelectPin, OUTPUT);  
    //Clear Chip Select
    digitalWrite(chipSelectPin,HIGH);
    
    configure();
}

int SPIUart::available(){
    return readRegister(RXLVL);
}

int SPIUart::read(){
    return readRegister(XHR);
}

size_t SPIUart::write(byte value){
    writeRegister(XHR,value);
    
    return 1;
}

size_t SPIUart::write(const char* str, size_t size){
    for(int i=0; i<size; ++i){
        writeRegister(XHR,str[i]);
    }
    
    return size;
}

void SPIUart::flush(){
    while(available() > 0) {
        read();
    }
}

void SPIUart::configure(){
    //Line Control Register: Enable Writing DLH & DLL
    //& set no Parity, 1 stop bit, and 8 bit word length
    writeRegister(LCR,0B10000011);
    
    //Division registers DLL & DLH
    // Write '96' to get 9600 baud rate
    //Assumes you have the version with the ~14MHz crystal
    // (16x9600 = 153600 = 14.7456Mhz/96)
    writeRegister(DLL,96);
    writeRegister(DLH,00); 
    
    //Line Control Register: Disnable Writing DLH & DLL
    //Same setup 
    writeRegister(LCR,0B00000011);
    
    //Modem Control Register
    //Normal Operating Mode
    writeRegister(MCR,0B00000000);
    
    //FIFO Control Register: Enable the FIFO and no other features
    writeRegister(FCR,0B00000111);  
}

void SPIUart::writeRegister(byte reg, byte val){
    // SC16IS740 expects a R/W  bit first, followed by the 4 bit
    // register address of the byte.
    // So shift the bits left by three bits:
    reg = reg << 3;
    // now combine the register address and the command into one byte:
    byte dataToSend = reg | WRITE;
    
    // take the chip select low to select the device:
    digitalWrite(chipSelectPin, LOW);
    
    SPI.transfer(dataToSend); //Send register location
    SPI.transfer(val);  //Send value to record into register
    
    // take the chip select high to de-select:
    digitalWrite(chipSelectPin, HIGH);
}

byte SPIUart::readRegister(byte reg){
    byte readback;
    
    // SC16IS740 expects a R/W  bit first, followed by the 4 bit
    // register address of the byte.
    // So shift the bits left by three bits:
    reg = reg << 3;
    // now combine the register address and the command into one byte:
    byte dataToSend = reg | READ;
    
    // take the chip select low to select the device:
    digitalWrite(chipSelectPin, LOW);
    
    SPI.transfer(dataToSend); //Send register location
    readback = SPI.transfer(0);  //Get Value from register
    
    digitalWrite(chipSelectPin, HIGH);
    
    return(readback);
}