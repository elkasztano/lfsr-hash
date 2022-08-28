// experimental hash generator based on 128 bit lfsr
// input from stdin is used as seed
// for demonstration purposes only
// do not use for cryptographic applications

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

// declare array for 128 bit lfsr state
static uint64_t state128[2];

// 128 bit lfsr with taps at 128, 127, 126, 121
int lfsr128(void)
{
	uint64_t bit;
	bit = ((state128[0] >> 0) ^ (state128[0] >> 1) ^ (state128[0] >> 2) ^ (state128[0] >> 7)) & 1ULL;
	state128[0] = (state128[0] >> 1) | (state128[1] << 63);
	state128[1] = (state128[1] >> 1) | (bit << 63);
}

int main(int argc, char *argv[])
{
	int opt;
	int count=0;
	int n=8; // default output length: 8 bytes = 64 bits
	uint8_t col = 0;

	// get command line arguments
	while((opt = getopt(argc, argv, ":hcn:")) != -1)
	{
		switch(opt)
		{
			case 'h':
				printf("usage: lfsr_hash [options]\n options:\n\t-h\tshow help and quit\n\t-n output length in bytes (must be a positive integer)\n\t-c\toutput color pattern\n");
				return 0;
			case 'n':
				n = atoi(optarg);				
				break;
			case 'c':
				col = 1;
				break;
			case ':':
				printf("option needs a value\n");
				return 1;
			case '?':
				printf("unknown option: %c\n", optopt);
				return 1;
		}
	}

	// define lfsr start state, can be anything except zero
	state128[0] = 0x7fe1105ab10c5e4fULL;
	state128[1] = 0x61a27f1a2009503fULL;

	int d;
	char c, temp;

	// perform XOR-operation repeatedly on the above defined start state using bytes from stdin
	while ((d = fgetc(stdin)) != EOF)
	{
		if (!(count%16)) count = 0;
		c = (char)d;
		state128[count/8] ^= (uint64_t)c << count%8;
		++count;
	}

	// exit in case the result is zero, which is extremely unlikely but could theoretically occur
	// a state of zero is not allowed in the lfsr, because then the output will consist of zeroes only
	if (!(state128[0]+state128[1]))
	{
		printf("Start state zero!\n");
		return 1;
	}

	// let the lfsr run a few times before actually using it
	// the lfsr should run least 128 times, because otherwise we just get bits from the initial state
	for (int i = 1; i <= 3333; ++i) lfsr128();
	
	// allocate memory for output
	unsigned char *hash;

	hash = (unsigned char*)calloc(n, sizeof(unsigned char));

	// fill the output array with bits from the lfsr
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			lfsr128();
			hash[i] |= (state128[0] & 1ULL) << j;
		}
	}

	// ANSI escape sequences are used for color mode
	// 256 colors are used, so 1 character corresponds to 1 byte
	if (col == 1)
	{
		for (int i = n-1; i>= 0; --i) printf("\033[38;5;%dm\u258a", hash[i]);
	}
	
	// hexadecimal output in normal mode, so 2 characters correspond to 1 byte
	else
	{
		for (int i = n-1; i >= 0; --i) printf("%02x", hash[i]);
	}

	// reset colors to default and write newline character
	printf("\033[39m\n");

	return 0;
}
