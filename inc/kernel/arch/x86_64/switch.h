#ifndef FAKIX_ARCH_X86_64_SWITCH_H
#define FAKIX_ARCH_X86_64_SWITCH_H 1

#define __SAVE_CONTEXT__ "pushf\n\tpushq %%rax\n\tpushq %%rdx\n\tpushq %%rcx\n\tpushq %%rbx\n\tpushq %%rsi\n\tpushq %%rdi\n\tpushq %%r8\n\tpushq %%r9\n\tpushq %%r10\n\tpushq %%r11\n\t"
#define __RESTORE_CONTEXT__ "popq %%r11\n\tpopq %%r10\n\tpopq %%r9\n\tpopq %%r8\n\tpopq %%rdi\n\tpopq %%rsi\n\tpopq %%rbx\n\tpopq %%rcx\n\tpopq %%rdx\n\tpopq %%rax\n\tpopf\n\t"

#define switch_to(prev_task, next_task) \
    do {\
        __asm__ __volatile__ (\
            __SAVE_CONTEXT__\
            "movq %%rsp, %P[TASK_RSP](%[PREV])\n\t"\
            "movq %P[TASK_RSP](%[NEXT]), %rsp\n\t"\
            __RESTORE_CONTEXT__\
            :: [PREV] "D" (prev_task), [NEXT] "S" (next_task),\
               [TASK_RSP] offsetof(struct fakix_task, rsp)\
        );\
    } while (0)

#endif