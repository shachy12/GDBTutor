#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "color_utils.h"
#include "questions_internal.h"

bool what_is_the_value_in_the_address()
{
    uint32_t value = 0xdeadbeef;
    uint8_t buffer[HEX_VALUE_AS_CHARS_SIZE] = {0 ,};

    printf(CYAN "What is the value in the address %p as hex\n" RESET, &value);
    fflush(stdout);

    TRANSFER_USER_TO_GDB();

    printf(CYAN "Value: " RESET);
    fflush(stdout);
    fgets(buffer, sizeof(buffer), stdin);

    return (value == strtoul(buffer, NULL, 16));
}

bool set_the_address_to_value()
{
    uint32_t wanted_value = 0x1337;
    uint32_t value = 0;
    
    printf(CYAN "Set the value in the address %p to 0x%x\n" RESET, &value, wanted_value);
    fflush(stdout);

    TRANSFER_USER_TO_GDB();

    return (value == wanted_value);
}

bool set_eax_register_value()
{
    register uint32_t eax __asm__("eax");
    uint32_t wanted_value = 0x12345678;

    printf(CYAN "Set the value of the eax register to 0x%x\n" RESET, wanted_value);
    fflush(stdout);

    TRANSFER_USER_TO_GDB();

    return (wanted_value == eax);
}

bool set_breakpoint_at_address()
{
    uint32_t breakpoint = 0;

    printf(CYAN "Set breakpoint at address %p\n" RESET, &breakpoint);
    fflush(stdout);

    TRANSFER_USER_TO_GDB();

    return (0xcc == breakpoint);
}

bool jump_to_address()
{
    uint32_t dummy_var = 0;

    printf(CYAN "Jump to address %p\n" RESET, &&Success);
    fflush(stdout);
    
    TRANSFER_USER_TO_GDB();

    /* Need to do this shit so the compiler won't remove the 'return true' line */
    if (1 == dummy_var) {
        goto Success;
    }

    return false;

Success:
    return true;
}

bool return_from_function()
{
    printf(CYAN "return from this function with the value 1 as the return value\n" RESET);
    fflush(stdout);

    TRANSFER_USER_TO_GDB();

    return false;
}

bool disassemble_me()
{
    char buffer[sizeof("mul")] = {0, };
    
    printf(CYAN "What is the instruction in the third opcode?\n" RESET);
    fflush(stdout);

    TRANSFER_USER_TO_GDB();

    asm __volatile__ ("mov %eax, %ebx;"
                      "add %eax, %ecx;"
                      "mul %eax;");

    printf("Instruction: ");
    fflush(stdout);

    fgets(buffer, sizeof(buffer), stdin);
    return 0 == strcasecmp(buffer, "mul");
}
