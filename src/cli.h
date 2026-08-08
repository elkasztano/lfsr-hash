#ifndef CLI_H
#define CLI_H
#include <stdint.h>

#define LH_FLAGS_COLOR (1 << 0)

typedef struct {
	int n;
	uint8_t flags;
} lh_cli_t;

lh_cli_t lh_cli(int argc, char **argv);

#endif

