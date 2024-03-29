#include "PCM3718.h"
#include <iomanip>
#include <iostream>
#define PCM_BASE 0x300

EmbeddedDevice::PCM3718::PCM3718(EmbeddedOperations *eops, uint32_t base_addr){
    this->eops = eops;
    this->base = base_addr;
    this->range = 0;// Default analog range is +- 5V
    
    if(eops->ioperm(base, 16, 1)!=0){
        std::cout<<"fail perm"<<std::endl;
    } 
}

EmbeddedDevice::PCM3718::PCM3718(EmbeddedOperations *eops, uint32_t base_addr, uint8_t analog_range){
    this->eops = eops;
    this->base = base_addr;
    this->range = analog_range;

    if(eops->ioperm(base, 16, 1)!=0){
        std::cout<<"fail perm"<<std::endl;
    } 
}

EmbeddedDevice::PCM3718::~PCM3718(){

}

uint16_t EmbeddedDevice::PCM3718::digitalInput(){
    uint8_t lowByte;
    uint8_t highByte;

    lowByte = eops->inb(base + 0x03);
    highByte = eops->inb(base + 0x0b);

    return (highByte<<8)|(lowByte & 0xff);
}

uint8_t EmbeddedDevice::PCM3718::digitalByteInput(bool high_byte){//(1 = high byte, 0 = low byte)
    if(high_byte){
        return eops ->inb(base + 0x0b);
    }
    else{
        return eops ->inb(base + 0x03);
    }
}

//Return the value of the bit as determined by the input argument (value between 0-15)
bool EmbeddedDevice::PCM3718::digitalBitInput(uint8_t bit){
    uint8_t ByteInput;
    if(bit >15){
        std::cout<<"Fail, Bit value not between 0-15"<<std::endl;
        exit(0);
    }
    
    if (bit > 7) {
		ByteInput = digitalByteInput(1); //read high byte
		
        return ((ByteInput & (1 << (bit - 8))) != 0);
	}
	else {
		ByteInput = digitalByteInput(0); //read low byte

		return ((ByteInput & (1 << bit)) != 0);
		
	}
}


void EmbeddedDevice::PCM3718::digitalOutput(uint16_t value){
    eops -> outb( value & 0xFF , this->base +0x03);
	eops -> outb((value >> 8) & 0x00FF , this->base +0x0B);
}

void EmbeddedDevice::PCM3718::digitalByteOutput(bool high_byte, uint8_t value){
    if(high_byte){ //1 = high_byte 
        eops -> outb(value,this->base + 0x0b);
    }
    else{          //0 = low_byte
        eops -> outb(value, this->base + 0x03);
    }

}

void EmbeddedDevice::PCM3718::setRange(uint8_t new_analog_range){
    this->range = new_analog_range;
}


//Receive the input in the analog channel provided, 
//convert it to a voltage (determined by the setRange) and return it
double EmbeddedDevice::PCM3718::analogInput(uint8_t channel) const{
    int low,high;
    uint16_t rawVoltage;
    double outVoltage; 
    //set range (use low 4 bits)
    eops->outb((range&0x0f),base + 1);
    
    //select channel
    eops -> outb (channel |(channel << 4),base + 2); //chaneel 0-15
    usleep(1000);
    
    //clear interrupts
    eops -> outb (0x00,base + 0x08);

    //trigger conversion
    eops -> outb (0x00,base + 0x0);

    //wait for conversion to complete
    while (eops -> inb(base + 0x08) & 0x80 );

    //read data
    low  = eops-> inb(base + 0x00);
    high = eops-> inb(base + 0x01);

    //return converted data
    rawVoltage = (low>>4)|(high <<4);

    switch (range)
    {
    case 0:
        outVoltage = (10.0/4095.0)*rawVoltage - 0.5*10.0;
        break;
    case 1:
         outVoltage = (1.0/4095.0)*rawVoltage - 0.5*1.0;
        break;
    case 2:
        outVoltage = (0.1/4095.0)*rawVoltage - 0.5*0.1;
        break;
    case 3:
        outVoltage = (0.01/4095.0)*rawVoltage - 0.5*0.01;
        break;
    case 4:
        outVoltage = (rawVoltage/4095.0)*10.0;
        break;
    case 5:
        outVoltage = (rawVoltage/4095.0)*1.0;
        break;
    case 6:
        outVoltage = (rawVoltage/4095.0)*0.1;
        break; 
    case 7:
        outVoltage = (rawVoltage/4095.0)*0.01;
        break; 
    case 8:
        outVoltage = (20.0/4095.0)*rawVoltage - 0.5*20.0;
        break;  
    case 9:
        outVoltage = (2.0/4095.0)*rawVoltage - 0.5*2.0;
        break;
    case 10:
        outVoltage = (0.2/4095.0)*rawVoltage - 0.5*0.2;
        break;
    case 11:
        outVoltage = (0.02/4095.0)*rawVoltage - 0.5 *0.02;
        break;    
    default:
        outVoltage = (10.0/4095.0)*rawVoltage - 0.5*10.0;
    }
    return outVoltage;
}

std::ostream &EmbeddedDevice::operator <<(std::ostream& output, const PCM3718& pcm){
    std::cout<<std::fixed<<"channel 0: <"<<std::setprecision(2)<<pcm.analogInput(0)<<">\t"<<"channel 1: <"<<std::setprecision(2)<<pcm.analogInput(1)<<">\n";
    return output;
}


