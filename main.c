#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/ptrace.h>
#include "color_utils.h"
#include "questions_exports.h"

#define MESSAGE_MAX_SIZE (1024)

typedef enum {
    UNINITIALIZED=-1,
    SUCCESS,
    PTRACE_NOT_ATTACHED,
} rc_t;

typedef bool (* question_function_t)(void);

typedef struct {
    question_function_t question_function;
    char success_message[MESSAGE_MAX_SIZE];
    char failed_message[MESSAGE_MAX_SIZE];
} question_t;

question_t questions[] = {
                            {what_is_the_value_in_the_address, "Not bad, Moving on to the next question.\n", "Wrong! try 'help print'\n"},
                            {set_the_address_to_value, "Nice, Moving on to the next question.\n", "Wrong! try 'help set'\n"},
                            {set_eax_register_value, "Great, Moving on to the next question.\n", "Wrong! try 'help set'\n"},
                            {set_breakpoint_at_address, "Exactly, Moving on to the next question.\n", "Wrong! try 'help break'\n"},
                            {jump_to_address, "That's what i am talking about, Moving on to the next question.\n", "Wrong! try 'help jump'\n"},
                            {return_from_function, "Attaboy, Moving on to the next question.\n", "Wrong! try 'help return'\n"},
                            {disassemble_me, "Impressive you finished this tutorial\n", "Wrong! try 'help display'\n"},
                         };

int main(int argc, char *argv[])
{
    rc_t rc = UNINITIALIZED;
    uint8_t index = 0;

    if (0 == ptrace(PTRACE_TRACEME, 0, 1, 0)) {
        printf(RED "In order to start the tutorial please run using gdb.\n" RESET);
        rc = PTRACE_NOT_ATTACHED;
        goto Cleanup;
    }

    printf(YELLOW "Welcome to the gdb tutorial\n" RESET);
    printf(YELLOW "This program will transfer you to the GDB after every question, "
                  "when you found the answer just 'continue' or 'c' in the gdb.\n" RESET);

    for (index = 0; index < sizeof(questions) / sizeof(*questions); index++) {
        while (!questions[index].question_function()) {
            printf(RED "%s" RESET, questions[index].failed_message);
            fflush(stdout);
        }

        printf(GREEN "%s" RESET, questions[index].success_message);
        fflush(stdout);
    }

    rc = SUCCESS;
Cleanup:
    return rc;
}
