#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <cinttypes>


class Linesensor {
private:
    const uint8_t pin;
    
public:
    Linesensor(uint8_t pin);

    bool isLineDetected();

};


#endif