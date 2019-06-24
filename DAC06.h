#ifndef DAC06_MODULE_H
#define DAC06_MODULE_H

#include <stdint.h>
//#include <sys/io.h>
#include "EmbeddedOperations.h"

/*
 * For MTRN3500 Students - These methods and their respective signatures must not be changed. If 
 * they are and human intervention is required then marks will be deducted. You are more than
 * welcome to add private member variables and private methods to the provided classes, or create
 * your own underlying classes to provide the requested functionality as long as all of the
 * functionality for the DAC06 can be accessed using the methods in the provided classes.
 */

namespace EmbeddedDevice {
  class DAC06 {
    public:
      DAC06(EmbeddedOperations *eops, uint32_t base_addr);
      ~DAC06();

      void analogOutputRaw(uint8_t channel, uint16_t value);
      void analogOutputVoltage(uint8_t channel, double desired_voltage);

    private:
      // NOTE: All sys/io function calls must be made through the EmbeddedOperations class
      EmbeddedOperations *eops;
      uint32_t base;
      uint16_t RawValue;
      uint16_t output;

  };
}

#endif // DAC06_MODULE_H
