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
Piece oldPiece[4];
uint8_t random;
uint16_t score;
uint8_t key;
uint16_t speed;
bool gameOver;
/**********/

/* FUNCTIONS */
void loop();
bool getBit(uint8_t x, uint8_t y);
void setBit(uint8_t x, uint8_t y, bool value);
void newPiece();
void updatePiece();
void movePiece(bool right);
void rotatePiece(bool right);

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
	memset(oldPiece, 0x00, sizeof(Piece) * 4);
	random = 0;
	score = 0;
	key = 0;
	speed = 1;
	gameOver = false;

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
	// initial piece
	random = rand() % 7;
	newPiece();

	while (!gameOver)
	{
		cleanScreen();
		printBoard();

		sleep(30);

		key = getKey();
		switch (key)
		{
			case 'L': movePiece(false); break;
			case 'R': movePiece(true); break;
			case 'D': speed = 1; break;
			case 'A': rotatePiece(false); break;
			case 'B': rotatePiece(true); break;
		}

		speed--;
		if (speed == 0)
		{
			updatePiece();
			speed = 1;
		}
	}

	cleanScreen();
	printf("--GAME OVER--");
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

	switch (random)
	{
		// T
		case 0:
			currentPiece[0].x = 0;
			currentPiece[0].y = 0;

			currentPiece[1].x = 1;
			currentPiece[1].y = 0;

			currentPiece[2].x = 2;
			currentPiece[2].y = 0;

			currentPiece[3].x = 1;
			currentPiece[3].y = 1;
			break;

		// L
		case 1:
			currentPiece[0].x = 0;
			currentPiece[0].y = 0;

			currentPiece[1].x = 0;
			currentPiece[1].y = 1;

			currentPiece[2].x = 0;
			currentPiece[2].y = 2;

			currentPiece[3].x = 1;
			currentPiece[3].y = 2;
			break;

		// J
		case 2:
			currentPiece[0].x = 1;
			currentPiece[0].y = 0;

			currentPiece[1].x = 1;
			currentPiece[1].y = 1;

			currentPiece[2].x = 1;
			currentPiece[2].y = 2;

			currentPiece[3].x = 0;
			currentPiece[3].y = 2;
			break;

		// I
		case 3:
			currentPiece[0].x = 0;
			currentPiece[0].y = 0;

			currentPiece[1].x = 0;
			currentPiece[1].y = 1;

			currentPiece[2].x = 0;
			currentPiece[2].y = 2;

			currentPiece[3].x = 0;
			currentPiece[3].y = 3;
			break;

		// S
		case 4:
			currentPiece[0].x = 0;
			currentPiece[0].y = 0;

			currentPiece[1].x = 1;
			currentPiece[1].y = 0;

			currentPiece[2].x = 0;
			currentPiece[2].y = 1;

			currentPiece[3].x = 1;
			currentPiece[3].y = 1;
			break;

		// Z
		case 5:
			currentPiece[0].x = 0;
			currentPiece[0].y = 0;

			currentPiece[1].x = 1;
			currentPiece[1].y = 0;

			currentPiece[2].x = 1;
			currentPiece[2].y = 1;

			currentPiece[3].x = 2;
			currentPiece[3].y = 1;
			break;

		// O
		case 6:
			currentPiece[0].x = 0;
			currentPiece[0].y = 0;

			currentPiece[1].x = 1;
			currentPiece[1].y = 0;

			currentPiece[2].x = 0;
			currentPiece[2].y = 1;

			currentPiece[3].x = 1;
			currentPiece[3].y = 1;
			break;
	}

	random = rand() % 7;

	for (i = 0; i < 4; i++)
	{
		currentPiece[i].x += 4;

		if (getBit(currentPiece[i].x, currentPiece[i].y))
			gameOver = true;

		setBit(currentPiece[i].x, currentPiece[i].y, true);
	}
}

void updatePiece()
{
	int8_t i;
	int8_t x, y, y_aux;
	bool erase;

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

			// Check line
			for (y = 19; y >= 0; y--)
			{
				erase = true;

				for (x = 0; x < 10; x++)
					if (!getBit(x, y)) {erase = false; break;}

				if (erase)
				{
					score++;
					y++;

					// Copy each row
					for (y_aux = y; y_aux > 0; y_aux--)
					{
						for (x = 0; x < 10; x++)
							setBit(x, y_aux, getBit(x, y_aux - 1));
					}
					
					// Clean first row
					for (x = 0; x < 10; x++)
						setBit(x, 0, false);
				}
			}

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

void movePiece(bool right)
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

void rotatePiece(bool right)
{
	uint8_t i;
	Piece temp;
	
	/* Subtract point of rotation for each point */
	uint8_t rotationPointX = currentPiece[0].x;
	uint8_t rotationPointY = currentPiece[0].y;

	// Prevent self collisions
	for (i = 0; i < 4; i++)
		setBit(currentPiece[i].x, currentPiece[i].y, false);

	/* Save old pos */
	memcpy(oldPiece, currentPiece, sizeof(Piece) * 4);

	/* Put on corner */
	for (i = 0; i < 4; i++)
	{
		currentPiece[i].x -= rotationPointX;
		currentPiece[i].y -= rotationPointY;
	}

	/* Do rotation */
	if (right)
	{
		for (i = 0; i < 4; i++)
		{
			temp = currentPiece[i];

			currentPiece[i].x = temp.y * -1;
			currentPiece[i].y = temp.x;
		}
	}
	else
	{
		for (i = 0; i < 4; i++)
		{
			temp = currentPiece[i];

			currentPiece[i].x = temp.y;
			currentPiece[i].y = temp.x * -1;
		}
	}

	/* Recover original position */
	for (int i = 0; i < 4; i++)
	{
		currentPiece[i].x += rotationPointX;
		currentPiece[i].y += rotationPointY;
	}

	// Check collision
	for (i = 0; i < 4; i++)
	{
		if (currentPiece[i].x >= 10 || currentPiece[i].y >= 20 || getBit(currentPiece[i].x, currentPiece[i].y))
		{
			memcpy(currentPiece, oldPiece, sizeof(Piece) * 4);
			break;
		}
	}

	for (i = 0; i < 4; i++)
		setBit(currentPiece[i].x, currentPiece[i].y, true);
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
}

void cleanScreen()
{
	system("cls");
}

uint8_t getKey()
{
	if (GetAsyncKeyState('A') & 0x8000) return 'L';
	if (GetAsyncKeyState('D') & 0x8000) return 'R';
	if (GetAsyncKeyState('S') & 0x8000) return 'D';
	if (GetAsyncKeyState('Q') & 0x8000) return 'A';
	if (GetAsyncKeyState('E') & 0x8000) return 'B';
	
	return 0;
}

#endif
