// experimental hash generator based on an alternating step generator (ASG)
// The ASG was patented by C.G. Günther in 1989. According to patents.google.com the patent has already expired.
// input from stdin is used as seed
// for demonstration purposes only
// do not use for cryptographic applications

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

// declare array for 128 bit lfsr state
static uint64_t state128[2];
static uint64_t state127[2];
static uint64_t state126[2];

// 128 bit galois lfsr with taps at 128, 127, 126, 121
int lfsr128(void)
{
	state128[0] = state128[0] >> 1 | state128[1] << 63;
	state128[1] = (-(state128[0] & 1ULL) & 0xe100000000000000ULL) ^ (state128[1] >> 1);
}

// 127 bit galois lfsr with taps at 127, 126, 124, 120
int lfsr127(void)
{
	state127[0] = (state127[0] >> 1) | (state127[1] << 63);
	state127[1] = (-(state127[0] & 1ULL) & 0x6880000000000000ULL) ^ (state127[1] >> 1);
}

// 126 bit galois lfsr with taps at 126, 124, 122, 119
int lfsr126(void)
{
	state126[0] = (state126[0] >> 1) | (state126[1] << 63);
	state126[1] = (-(state126[0] & 1ULL) & 0x2a40000000000000ULL) ^ (state126[1] >> 1);
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
				printf("usage: asg_hash [options]\n options:\n\t-h\tshow help and quit\n\t-n output length in bytes (must be a positive integer)\n\t-c\toutput color pattern\n");
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

	// define lfsr start states, can be anything except zero
	state128[0] = 0x7fe1105ab10c5e4fULL;
	state128[1] = 0x61a27f1a2009503fULL;

	state127[0] = 0x1C93370CADADC36AULL;
	state127[1] = 0x1C90070CADADC200ULL;

	state126[0] = 0x1C7557296E0727FFULL;
	state126[1] = 0x159AABE3C6C72865ULL;

	int d;
	char c, temp;

	// perform XOR-operation repeatedly on the above defined start states using bytes from stdin
	while ((d = fgetc(stdin)) != EOF)
	{
		if (!(count%48)) count = 0;
		c = (char)d;
		if (count/16 == 0) state128[(count/8)%2] ^= ((uint64_t)c << ((count%8)*8));
		if (count/16 == 1) state127[(count/8)%2] ^= ((uint64_t)c << ((count%8)*8));
		if (count/16 == 2) state126[(count/8)%2] ^= ((uint64_t)c << ((count%8)*8));
		++count;
	}

	// exit in case the result is zero, which is extremely unlikely but could theoretically occur
	// a state of zero is not allowed in the lfsr, because then the output will consist of zeroes only
	if (!(state128[0]+state128[1]))
	{
		printf("Start state 128 zero!\n");
		return 1;
	}	
	
	if (!(state127[0]+state127[1]))
	{
		printf("Start state 127 zero!\n");
		return 1;
	}
	
	if (!(state126[0]+state126[1]))
	{
		printf("Start state 126 zero!\n");
		return 1;
	}

	// clear the most significant bit of the 127 bit lfsr state, and the 2 most significant bits of the 126 bit lfsr state
	state127[1] &= 0x7FFFFFFFFFFFFFFFULL;
	state126[1] &= 0x3FFFFFFFFFFFFFFFULL;

	// let the lfsrs run a few times before actually using them
	for (int i = 1; i <= 3333; ++i) lfsr128();
	for (int i = 1; i <= 2667; ++i) lfsr127();
	for (int i = 1; i <= 3451; ++i) lfsr126();
	
	// allocate memory for output
	unsigned char *hash;

	hash = (unsigned char*)calloc(n, sizeof(unsigned char));
	
	// fill the output array with bits from the lfsrs
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			lfsr128();
			if (state128[0]&1ULL)
			{	
				lfsr127();
				hash[i] |= (state127[0] & 1ULL) << j;
			}
			else 
			{
				lfsr126();
				hash[i] |= (state126[0] & 1ULL) << j;
			}
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
