#ifndef __SPIUART_H__
#define __SPIUART_H__

#include "application.h"

class SPIUart : public Stream {
public:
    void begin();
    int available();
    int read();
    size_t write(byte value);
    size_t write(const char* str, size_t size);

using Print::write;
    
    void flush();
    int peek() { return 0; };
    
private:
    void configure();
    void writeRegister(byte reg, byte val);
    byte readRegister(byte reg);
    
private:
    int chipSelectPin;
};

#endif
