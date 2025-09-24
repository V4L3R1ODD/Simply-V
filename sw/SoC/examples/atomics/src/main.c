//#include "uninasoc.h"
//#include <stdint.h>
//
//int main()
//{
//
//  // Initialize HAL
//  uninasoc_init();
//
//  // Print
//  printf("Hello World!\n\r");
//
//  // Return to caller
//  return 0;
//
//}


#include "uninasoc.h"
#include <stdint.h>

#define DDR_BASE   0x30000
#define DDR_END    0x3FFFF
#define STEP       0x1000   // It is the step between one address and the next for the testt

int main(int argc, char* argv[]) {

    // Initialize HAL
    uninasoc_init();

    printf("=== DDR ACCESS TEST ===\n\r");
    printf("Range: 0x%x - 0x%x\n\r", DDR_BASE, DDR_END);

    for (uintptr_t addr = DDR_BASE; addr <= DDR_END; addr += STEP) {
        volatile unsigned int* ddr_ptr = (volatile unsigned int*) addr;
        unsigned int test_val = (addr & 0xFFFF);  // address-dependent pseudo-random value 
        unsigned int read_back;

        // --- Normal access ---
        *ddr_ptr = test_val;
        read_back = *ddr_ptr;

        if (read_back == test_val) {
            printf("[NORMAL] Addr 0x%08x: SUCCESS (val=%u)\n\r", addr, read_back);
        } else {
            printf("[NORMAL] Addr 0x%08x: FAILED (read %u, expected %u)\n\r", addr, read_back, test_val);
        }

        // --- Atomic access (LR/SC) ---
        int success;
        unsigned int new_val = test_val + 1234;

        asm volatile (
            "1: lr.w t0, (%1)\n"      // load-reserved
            "   sc.w %0, %2, (%1)\n"  // store-conditional
            : "=r"(success)
            : "r"(ddr_ptr), "r"(new_val)
            : "t0", "memory"
        );

        if (success == 0) {
            unsigned int verify = *ddr_ptr;
            printf("[ATOMIC] Addr 0x%08x: SUCCESS (val=%u)\n\r", addr, verify);
        } else {
            printf("[ATOMIC] Addr 0x%08x: FAILED (sc.w result=%d)\n\r", addr, success);
        }
    }

    printf("=== DDR ACCESS TEST DONE ===\n\r");

    return 0;
}
