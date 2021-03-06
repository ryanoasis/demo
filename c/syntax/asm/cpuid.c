#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define TRUE  1
#define FALSE 0

static inline void cpuid_vendor_00(char * vendor) {
    int ebx = 0, ecx = 0, edx = 0;

    __asm__ volatile("cpuid" \
            : "=b"(ebx), \
            "=c"(ecx), \
            "=d"(edx) \
            : "a"(0x00));
    sprintf(vendor  , "%c%c%c%c", ebx, (ebx >> 8), (ebx >> 16), (ebx >> 24));
    sprintf(vendor+4, "%c%c%c%c", edx, (edx >> 8), (edx >> 16), (edx >> 24));
    sprintf(vendor+8, "%c%c%c%c", ecx, (ecx >> 8), (ecx >> 16), (ecx >> 24));
    vendor[12] = 0x00;
}

static inline void cpuid_hv_vendor_00(char * vendor) {
    int ebx = 0, ecx = 0, edx = 0;

    __asm__ volatile("cpuid" \
            : "=b"(ebx), \
            "=c"(ecx), \
            "=d"(edx) \
            : "a"(0x40000000));
    sprintf(vendor  , "%c%c%c%c", ebx, (ebx >> 8), (ebx >> 16), (ebx >> 24));
    sprintf(vendor+4, "%c%c%c%c", ecx, (ecx >> 8), (ecx >> 16), (ecx >> 24));
    sprintf(vendor+8, "%c%c%c%c", edx, (edx >> 8), (edx >> 16), (edx >> 24));
    vendor[12] = 0x00;
}

static inline void cpuid_brand(char * brand, uint32_t eax_value) {
    int eax = 0, ebx = 0, ecx = 0, edx = 0;

    __asm__ volatile("cpuid" \
            : "=a"(eax), \
            "=b"(ebx), \
            "=c"(ecx), \
            "=d"(edx) \
            : "a"(eax_value));
    sprintf(brand   , "%c%c%c%c", eax, (eax >> 8), (eax >> 16), (eax >> 24));
    sprintf(brand+4 , "%c%c%c%c", ebx, (ebx >> 8), (ebx >> 16), (ebx >> 24));
    sprintf(brand+8 , "%c%c%c%c", ecx, (ecx >> 8), (ecx >> 16), (ecx >> 24));
    sprintf(brand+12, "%c%c%c%c", edx, (edx >> 8), (edx >> 16), (edx >> 24));
}

static inline int cpuid_hv_bit() {
    int ecx;
    __asm__ volatile("cpuid" \
            : "=c"(ecx) \
            : "a"(0x01));
    return (ecx >> 31) & 0x1;
}

void cpu_write_vendor(char * vendor) {
    cpuid_vendor_00(vendor);
}

void cpu_write_hv_vendor(char * vendor) {
    cpuid_hv_vendor_00(vendor);
}

void cpu_write_brand(char * brand) {
    int eax;
    /* Check if Processor Brand String is supported */
    __asm__ volatile(".intel_syntax noprefix;"
            "mov eax, 0x80000000;"
            "cpuid;"
            "cmp eax, 0x80000004;"
            "xor eax, eax;"
            "setge al;"
            ".att_syntax;"
            : "=a"(eax)
            );
    /* It's supported, so fill char * brand */
    if (eax) {
        cpuid_brand(brand,    0x80000002);
        cpuid_brand(brand+16, 0x80000003);
        cpuid_brand(brand+32, 0x80000004);
        brand[48] = 0x00;
    }
}

int cpu_hv() {
    return cpuid_hv_bit() ? TRUE : FALSE;
}

int main()
{
    char cpu_vendor[13], cpu_hv_vendor[13], cpu_brand[49];
    cpu_write_vendor(cpu_vendor);
    cpu_write_hv_vendor(cpu_hv_vendor);
    cpu_write_brand(cpu_brand);

    printf("[*] CPU: %s\n", cpu_vendor);
    printf("    Hypervisor: %s\n", cpu_hv_vendor);
    printf("    CPU brand: %s\n", cpu_brand);
    printf("    CPU Hypervisor Bit: %d\n", cpu_hv());

    return 0;
}
