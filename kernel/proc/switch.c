#include <switch.h>
#include <task.h>

void task_switch_to(struct task *prev, struct task *next)
{
    switch_to(prev, next);
}