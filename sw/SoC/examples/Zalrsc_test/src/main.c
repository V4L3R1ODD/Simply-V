#include "uninasoc.h"
#include <stdint.h>

extern unsigned int _DDR_start;
extern unsigned int _DDR_end;

int main(int argc, char* argv[]) { 
    
    // Initialize HAL
    uninasoc_init();

    uintptr_t ddr_base = (uintptr_t)&_DDR_start;
    uintptr_t ddr_end  = (uintptr_t)&_DDR_end;
    

    // Usa la prima word della DDR come target per gli AMO
    volatile int *a_ptr = (int *)ddr_base;
    *a_ptr = 0x10;  // Valore iniziale nella DDR
    int old_val;


    // === 1. AMOADD: a = a + 0x5 ===
    int add_val = 0x5;
    __asm__ volatile (
        "amoadd.w %[old], %[val], (%[addr])"
        : [old] "=r"(old_val)
        : [val] "r"(add_val), [addr] "r"(a_ptr)
        : "memory"
    );
    if (*a_ptr  == old_val + add_val) {
        printf("AMOADD SUCCESS!\n\r");
    } else {
        printf("AMOADD FAILED!\n\r");
    }

    // === 2. AMOXOR: a = a ^ 0xA ===
    int xor_val = 0xA;
    __asm__ volatile (
        "amoxor.w %[old], %[val], (%[addr])"
        : [old] "=r"(old_val)
        : [val] "r"(xor_val), [addr] "r"(a_ptr)
        : "memory"
    );
    if (*a_ptr == (old_val ^ xor_val)) {
        printf("AMOXOR SUCCESS!\n\r");
    } else {
        printf("AMOXOR FAILED!\n\r");
    }

    // === 3. AMOAND: a = a & 0xF ===
    int and_val = 0xF;
    __asm__ volatile (
        "amoand.w %[old], %[val], (%[addr])"
        : [old] "=r"(old_val)
        : [val] "r"(and_val), [addr] "r"(a_ptr)
        : "memory"
    );
    if (*a_ptr == (old_val & and_val)) {
        printf("AMOAND SUCCESS!\n\r");
    } else {
        printf("AMOAND FAILED!\n\r");
    }

    // === 4. AMOOR: a = a | 0x80 ===
    int or_val = 0x80;
    __asm__ volatile (
        "amoor.w %[old], %[val], (%[addr])"
        : [old] "=r"(old_val)
        : [val] "r"(or_val), [addr] "r"(a_ptr)
        : "memory"
    );
    if (*a_ptr == (old_val | or_val)) {
        printf("AMOOR SUCCESS!\n\r");
    } else {
        printf("AMOOR FAILED!\n\r");
    }

    while (1) {};
    return 0;
}