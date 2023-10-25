#define DEBUG
#define WINDOWS

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef WINDOWS
	#include <time.h>
	#include <windows.h>

	#define sleep(ms) Sleep(ms)
#else
	#define sleep(ms) delay_ms(ms)
#endif

struct Piece
{
	uint8_t x;
	uint8_t y;
};
typedef struct Piece Piece;

/* GLOBAL */
uint8_t board[25];
Piece currentPiece[4];
uint16_t score;
uint8_t key;
/**********/

/* FUNCTIONS */
void loop();
bool getBit(uint8_t x, uint8_t y);
void setBit(uint8_t x, uint8_t y, bool value);
void newPiece();
void updateCurrentPiece();
void moveCurrentPiece(bool right);

#ifdef WINDOWS

void printBoard();
void cleanScreen();
uint8_t getKey();

#endif
/*************/

#ifdef WINDOWS
	int
#else
	void
#endif
main()
{
	memset(board, 0x00, 25);
	score = 0;
	key = 0;

	#ifdef WINDOWS
		HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
		SetConsoleMode(hInput, mode);

		srand(time(NULL));
		loop();
		return 0;
	#else
		srand(12345);
	#endif
}

void loop()
{
	newPiece();
	setBit(9, 19, true);
	setBit(9, 0, true);

	while (true)
	{
		cleanScreen();
		printBoard();

		sleep(400);

		key = getKey();
		if (key == 1) moveCurrentPiece(false);
		else if (key == 2) moveCurrentPiece(true);

		updateCurrentPiece();
	}
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

void newPiece()
{
	uint8_t i;
	uint8_t random = rand() % 7;

	switch (random)
	{
		// T
		case 0:
			currentPiece[0].x = 3;
			currentPiece[0].y = 0;

			currentPiece[1].x = 4;
			currentPiece[1].y = 0;

			currentPiece[2].x = 5;
			currentPiece[2].y = 0;

			currentPiece[3].x = 4;
			currentPiece[3].y = 1;
			break;

		// L
		case 1:
			currentPiece[0].x = 3;
			currentPiece[0].y = 0;

			currentPiece[1].x = 3;
			currentPiece[1].y = 1;

			currentPiece[2].x = 3;
			currentPiece[2].y = 2;

			currentPiece[3].x = 4;
			currentPiece[3].y = 2;
			break;

		// J
		case 2:
			currentPiece[0].x = 4;
			currentPiece[0].y = 0;

			currentPiece[1].x = 4;
			currentPiece[1].y = 1;

			currentPiece[2].x = 4;
			currentPiece[2].y = 2;

			currentPiece[3].x = 3;
			currentPiece[3].y = 2;
			break;

		// I
		case 3:
			currentPiece[0].x = 4;
			currentPiece[0].y = 0;

			currentPiece[1].x = 4;
			currentPiece[1].y = 1;

			currentPiece[2].x = 4;
			currentPiece[2].y = 2;

			currentPiece[3].x = 4;
			currentPiece[3].y = 3;
			break;

		// S
		case 4:
			currentPiece[0].x = 4;
			currentPiece[0].y = 0;

			currentPiece[1].x = 5;
			currentPiece[1].y = 0;

			currentPiece[2].x = 3;
			currentPiece[2].y = 1;

			currentPiece[3].x = 4;
			currentPiece[3].y = 1;
			break;

		// Z
		case 5:
			currentPiece[0].x = 3;
			currentPiece[0].y = 0;

			currentPiece[1].x = 4;
			currentPiece[1].y = 0;

			currentPiece[2].x = 4;
			currentPiece[2].y = 1;

			currentPiece[3].x = 5;
			currentPiece[3].y = 1;
			break;

		// O
		case 6:
			currentPiece[0].x = 4;
			currentPiece[0].y = 0;

			currentPiece[1].x = 5;
			currentPiece[1].y = 0;

			currentPiece[2].x = 4;
			currentPiece[2].y = 1;

			currentPiece[3].x = 5;
			currentPiece[3].y = 1;
			break;
	}

	for (i = 0; i < 4; i++)
		setBit(currentPiece[i].x, currentPiece[i].y, true);
}

void updateCurrentPiece()
{
	uint8_t i;

	// Prevent self collisions
	for (i = 0; i < 4; i++)
		setBit(currentPiece[i].x, currentPiece[i].y, false);

	// Check collision
	for (i = 0; i < 4; i++)
	{
		if ((currentPiece[i].y + 1 >= 20) || (getBit(currentPiece[i].x, currentPiece[i].y + 1)))
		{
			for (i = 0; i < 4; i++)
				setBit(currentPiece[i].x, currentPiece[i].y, true);

			newPiece();
			return;
		}
	}

	// Update Y position
	for (i = 0; i < 4; i++)
	{
		currentPiece[i].y++;
		setBit(currentPiece[i].x, currentPiece[i].y, true);
	}
}

void moveCurrentPiece(bool right)
{
	uint8_t i;
	int8_t dir = right ? 1 : -1;

	// Prevent self collisions
	for (i = 0; i < 4; i++)
		setBit(currentPiece[i].x, currentPiece[i].y, false);

	// Check collision
	for (i = 0; i < 4; i++)
	{
		if ((!right && currentPiece[i].x == 0) || (currentPiece[i].x + dir >= 10) || (getBit(currentPiece[i].x + dir, currentPiece[i].y)))
		{
			for (i = 0; i < 4; i++)
				setBit(currentPiece[i].x, currentPiece[i].y, true);

			return;
		}
	}

	// Update X position
	for (i = 0; i < 4; i++)
	{
		currentPiece[i].x += dir;
		setBit(currentPiece[i].x, currentPiece[i].y, true);
	}
}

#ifdef WINDOWS

void printBoard()
{
	uint8_t x, y;

	for (y = 0; y < 20; y++)
	{
		for (x = 0; x < 10; x++)
			printf("%c", (getBit(x, y) ? 219 : 32));
		printf("\n");
	}
	printf("\n%u", getKey());
}

void cleanScreen()
{
	system("cls");
}

uint8_t getKey()
{
	if (GetAsyncKeyState('A') & 0x8000) return 1;
	if (GetAsyncKeyState('D') & 0x8000) return 2;
	
	return 0;
}

#endif
