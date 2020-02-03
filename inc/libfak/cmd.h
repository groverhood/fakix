/**
 *  === cmd.h ===
 * 
 *  This header exposes functions for users to exploit IPC, which is abstracted
 *  in the form of commands.
 **/

#ifndef FAKIX_LIBFAK_CMD_H
#define FAKIX_LIBFAK_CMD_H 1

#include <sys/types.h>
#include <stdint.h>

/* These two data types are left to be defined by the processes that are
   communicating. */

/* Enumeration for a command. */
typedef int cmdval_t;

/* Descriptor of a command. */
typedef int cmdheader_t;

/* Secret to identify commands against to ensure there is no malicious IPC. */
typedef uint64_t cmdsecret_t;

/* Fixed-definition status code returned by cmd system calls. */
typedef int cmdstat_t;

#define CMDVAL(n) ((cmdval_t)(n))
#define CMDSTAT(n) ((cmdstat_t)(n))

#define CMDSTAT_OK CMDSTAT(0x00)
#define CMDSTAT_ERR CMDSTAT(0x01)

struct command {
	cmdheader_t hdr;
	cmdval_t cmd;
	cmdsecret_t secret;
};

cmdstat_t cmd_send(pid_t proc, const struct command *cmd);
cmdstat_t cmd_recv(struct command *cmd);

#endif