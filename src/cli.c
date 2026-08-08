#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>
#include "cli.h"

void print_help_text(char *progname);

lh_cli_t lh_cli(int argc, char **argv) {
	int c, option_index = 0;

	lh_cli_t cli;

	/* default values */
	cli.n = 8;
	cli.flags = 0;

	while( 1 ) {
		static struct option long_options[] = {
			{ "length", required_argument, 0, 'n' },
			{ "colorful", no_argument, 0, 'c' },
			{ "help", no_argument, 0, 'h' },
			{ "version", no_argument, 0, 'V' },
			{ 0, 0, 0, 0 }
		};

		c = getopt_long(argc, argv, "hVn:c", long_options, &option_index);

		if(c == -1)
			break;

		switch(c) {
			case 'n':
				cli.n = atoi(optarg);
				break;
			case 'c':
				cli.flags |= LH_FLAGS_COLOR;
				break;
			case 'h':
				print_help_text(*argv);
				exit(0);
			case 'V':
				printf("lfsr-hash 2.0\n");
				exit(0);
			case '?':
				print_help_text(*argv);
				exit(1);
			default:
				print_help_text(*argv);
				exit(1);
		}
	}

	return(cli);
}

void print_help_text(char *progname) {
	printf( "Usage: %s [options]\n"
			"-n, --length ......... output length of generated data in bytes\n"
			"-c, --colorful ....... trigger colorful command line output\n"
			"-h, --help ........... show help and exit\n"
			"-V, --version ........ show version and exit\n",
			progname);
}

