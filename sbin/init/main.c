
#include <fakix/capabilities.h>
#include <fakix/init/initcaps.h>
#include <fakix/ram.h>

/**
 *  argv[0] = "/sbin/init"
 * 
 *  RAM caps start at CAP_INIT_RAM_BASE.
 **/
int main(int argc, char *argv[])
{
    
    while (1) {
        asm("");
    }
}