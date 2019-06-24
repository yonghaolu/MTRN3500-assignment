#include "DAC06.h"
#include "MSIP404.h"
#include "PCM3718.h"
#include "EmbeddedOperations.h"


int main(){
    EmbeddedOperations eops;
    EmbeddedDevice::PCM3718 pcm( &eops, 0x300);
    EmbeddedDevice::MSIP404 msi( &eops, 0x200);
    EmbeddedDevice::DAC06 dac( &eops, 0x320);
    EmbeddedDevice::encoder e;

    msi.resetChannel(0);
    e.i = msi.readChannel(0);
    msi.readIndex(0);  
    std::cout << e.i <<std::endl;

    pcm.digitalInput();
    pcm.digitalByteInput(1);

    pcm.setRange(0);
    pcm.analogInput(0);

    return 0;
} 