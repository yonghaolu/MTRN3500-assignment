#ifndef EMBEDDED_OPERATIONS_H
#define EMBEDDED_OPERATIONS_H

/*
* uncomment the following line when testing outside the labs.
* When DEBUG is on, inb, outb and ioperm will print strings out into the console. 
* When DEBUG is on, both ioperm and outb will always return 0.
*/
//#define DEBUG

#ifdef DEBUG
#include <iostream>
#else
#include <sys/io.h>
#endif

/*
* For MTRN3500 Students - These methods and their respective signatures must not be changed. If
* they are and human intervention is required then marks will be deducted. You are more than
* welcome to add private member variables and private methods to the provided classes, or create
* your own underlying classes to provide the requested functionality as long as all of the
* functionality for the interfaces can be accessed using the methods in the provided classes.
*/

class EmbeddedOperations {
public:
  EmbeddedOperations() {}
  ~EmbeddedOperations() {}

  virtual unsigned char inb(unsigned short int port) {
#ifdef DEBUG
    std::cout << "inb(0x" << std::hex << port << ")" << std::endl;
    return 0;
#else
    return ::inb(port);
#endif
  }

  virtual void outb(unsigned char value, unsigned short int port) {
#ifdef DEBUG
    std::cout << "outb(0x" << std::hex << (int)value << ", 0x" << port << ")" << std::endl;
#else
    ::outb(value, port);
#endif
  }

  virtual int ioperm(unsigned long from, unsigned long num, int turn_on) {
#ifdef DEBUG
    std::cout << "ioperm(0x" << std::hex << from << ", " << std::dec << num << ", " << turn_on << ")" << std::endl;
    return 0;
#else
    return ::ioperm(from, num, turn_on);
#endif
  }
};

#endif // EMBEDDED_OPERATIONS_H
