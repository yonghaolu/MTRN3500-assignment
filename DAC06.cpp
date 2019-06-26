#include "DAC06.h"
#define DAC_BASE 0x320

EmbeddedDevice::DAC06::DAC06(EmbeddedOperations *eops, uint32_t base_addr){
    
    this->eops =eops;
    this->base = base_addr;
    if(eops ->ioperm(this->base, 12, 1)!=0){
        std::cout<<"fail perm"<<std::endl;
    }
}

EmbeddedDevice::DAC06::~DAC06(){

}

//Output the raw value provided to the specified channel in the DAC
void EmbeddedDevice::DAC06::analogOutputRaw(uint8_t channel, uint16_t value){
    uint16_t RawValue = value;   
    eops -> outb(RawValue&0x00FF,base + (2 * channel) );
    eops -> outb((RawValue >> 8) & 0x0f ,base + (2 * channel + 1) );

}

//Output the desired voltage provided to the specified channel in the DAC
void EmbeddedDevice::DAC06::analogOutputVoltage(uint8_t channel, double desired_voltage){
    uint16_t output = static_cast<uint16_t>((desired_voltage + .5* 10)/10 * 4096);   //range is -5v to 5v
    analogOutputRaw(channel,output);
}

