#include "MSIP404.h"
#define MSI_BASE 0x200
#define pulse_base 0x800
#define pulseBit_7 0x80
#define pulseBit_5 0x20
EmbeddedDevice::MSIP404::MSIP404(EmbeddedOperations* eops, uint32_t base_addr){
    this->eops = eops;
    this->base = base_addr;
    

	if(eops -> ioperm(base, 16, 1)!=0){
		std::cout<<"faill perm"<<std::endl;
	} 

	if(eops->ioperm(pulse_base,2,1)!=0){
		std::cout<<"faill perm"<<std::endl;
	}
}

EmbeddedDevice::MSIP404::~MSIP404(){
}


//Reset the channel determined by the input value
void EmbeddedDevice::MSIP404::resetChannel(uint8_t channel)
{
    if(channel>7){
		std::cout<<"Fail reset, channel range is 0-7"<<std::endl;
		exit(0);
	}
	else{
		eops -> outb(0 ,base + 1 * channel);
	}	
}

//Read the channel determined by the input value
int32_t EmbeddedDevice::MSIP404::readChannel(uint8_t channel)
{
    if(channel>7){
		std::cout<<"Fail read, channel range is 0-7"<<std::endl;
		exit(0);
	}
	
	
	union encoder{
    	int i;
    	char c[4];
	};
	encoder en;
    int x;
	for (x = 0; x < 4; x++) {
		en.c[x] = eops -> inb(base + x + 4 * channel);
    }
	return en.i;
}

//Read the index bit for the channel determined by the input value
bool EmbeddedDevice::MSIP404::readIndex(uint8_t channel)
{
	if(channel>2){
		std::cout<<"Fail read Index, channel range is 0-2"<<std::endl;
		exit(0);
	}

	if (channel == 0) {
		return (eops->inb(pulse_base) & pulseBit_7); //channel 0, bit 7
	}

	else if (channel == 1) {
		return (eops->inb(pulse_base) & pulseBit_5);// channel 1, bit 5
	}

	else {				//(channel == 2) 
		return (eops->inb(pulse_base + 1) & pulseBit_7);// channel 2, bit 7
	}
	

}

bool EmbeddedDevice::MSIP404::operator!()
{
	for(int i=0;i<8;i++){
		eops->outb(0,base + 1);
	}
	
	return true;
}
