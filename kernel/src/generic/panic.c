/// Copyright (C) StrawberryHacker

#include "panic.h"
#include "debug.h"
#include "sections.h"
#include "memory.h"
#include "cpu.h"

extern __bootsig__ u8 boot_signature[32];

/// If the program enters an undefined state this function will print the 
/// reason and terminate the execution by asserting a breakpoint
void panic_handler(const char* file_name, u32 line_number,
    const char* reason) {
    
    debug_print("Panic! %s \n", reason);
    debug_print("File: %s \n", file_name);
    debug_print("Line: %d \n", line_number);
    debug_flush();

    // Go to the bootloader. Alternatively all interrupts except the serial 0
    // handler must be disabled
    memory_copy("StayInBootloader", boot_signature, 16);
    dmb();
    // Perform a soft reset
    cpsid_i();
    *((u32 *)0x400E1800) = 0xA5000000 | 0b1;

    while (1);
}