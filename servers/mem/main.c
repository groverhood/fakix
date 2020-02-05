#include <cbl.h>
#include <cmd.h>
#include <svr/mem.h>

static memerr_t svr_mem_map_page(void *vaddr, uintptr_t paddr, struct capability cap);

int main(void)
{
	struct command cmd;
	while (1) {
		if (cmd_recv(&cmd) == CMDSTAT_OK) {

		}
	}

	return 0;
}

static memerr_t svr_mem_map_page(void *vaddr, uintptr_t paddr, struct capability cap)
{

}