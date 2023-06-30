#include <stdio.h>
#include <stdbool.h>

bool isVirtualMachine() {
    bool isVM = false;

    // Check for common virtualization indicators

    // Hypervisor vendor string
    FILE* fp = fopen("/sys/class/dmi/id/sys_vendor", "r");
    if (fp != NULL) {
        char vendor[100];
        if (fgets(vendor, sizeof(vendor), fp) != NULL) {
            if (strstr(vendor, "VMware") || strstr(vendor, "VirtualBox") ||
                strstr(vendor, "Microsoft")) {
                isVM = true;
            }
        }
        fclose(fp);
    }

    // CPUID instruction
    asm volatile (
        "pushf\n"
        "pop %%rax\n"
        "mov %%rax, %%rbx\n"
        "xor $0x200000, %%eax\n"
        "push %%rax\n"
        "popf\n"
        "pushf\n"
        "pop %%rax\n"
        "xor %%eax, %%ebx\n"
        "and $0x200000, %%ebx\n"
        "mov $0, %%eax\n"
        : "=a"(isVM)
        :
        : "rbx"
    );

    return isVM;
}

int main() {
    bool vm = isVirtualMachine();

    if (vm) {
        printf("This machine is running on a virtual machine.\n");
    } else {
        printf("This machine is running on physical hardware.\n");
    }

    return 0;
}
