
#include <fakix/capabilities.h>
#include <fakix/init/ramcaps.h>
#include <fakix/ram.h>

#include "mm.h"

/**
 *  argv[0] = "/sbin/init"
 * 
 *  RAM caps start at CAP_INIT_RAM_BASE.
 **/
int main(int argc, char *argv[])
{
    mm_init();
}