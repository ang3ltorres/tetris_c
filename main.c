#define DEBUG
#define WINDOWS

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef WINDOWS
	#include <synchapi.h>
	#define sleep(ms) Sleep(ms)
#else
	#define sleep(ms) sleep_ms(ms)
#endif

/* GLOBAL */
uint8_t board[25];
/**********/

/* FUNCTIONS */
bool getBit(uint8_t x, uint8_t y);
void setBit(uint8_t x, uint8_t y, bool value);
/*************/

#ifdef WINDOWS
	int
#else
	void
#endif
main()
{
	memset(board, 0x00, 25);

	#ifdef WINDOWS


		printf("[%d][%d]: %d\n", 0, 2, getBit(5, 2));

		return 0;
	#endif
}

bool getBit(uint8_t x, uint8_t y)
{
	uint8_t BYTE;
	uint8_t BIT;
	uint8_t MASK;

	BYTE = ((y * 10) + x) / 8;
	BIT = ((y * 10) + x) % 8;

	MASK = 1 << (7 - BIT);
	return MASK & board[BYTE];
}

void setBit(uint8_t x, uint8_t y, bool value)
{
	uint8_t BYTE;
	uint8_t BIT;
	uint8_t MASK;

	BYTE = ((y * 10) + x) / 8;
	BIT = ((y * 10) + x) % 8;

	MASK = 1 << (7 - BIT);

	if (value)
		board[BYTE] |= MASK;
	else
		board[BYTE] &= ~MASK;
}
