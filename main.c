/*
Seminario de embebidos D10
Profesor Jesus Ramos
Angel Emmanuel Suarez Torres
217542842
Proyecto final
28 de octubre del 2023
*/

// LCD module connections
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;
// End LCD module connections

// Keypad module connections
char  keypadPort at PORTD;
// End Keypad module connections

// #include <stdlib.h>
// #include <stdio.h>
#include <stdint.h>
// #include <string.h>
#include <stdbool.h>

#define BUZZER PORTC.F4

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
bool gameOver;

uint8_t BYTE;
uint8_t BIT;
uint8_t MASK;
/**********/

/* FUNCTIONS */
void loop();
bool getBit(uint8_t x, uint8_t y);
void setBit(uint8_t x, uint8_t y, bool value);
void newPiece();
bool updatePiece();
void movePiece(bool right);
void rotatePiece(bool right);

void printBoard();
void getKey();

void config();
void printLCD(char f, char c, const char txt[]);
void printUART(const char txt[]);
/*************/

void main()
{
	memset(board, 0x00, 25);
	memset(oldPiece, 0x00, sizeof(Piece) * 4);
	random = 0;
	score = 0;
	key = 0;
	gameOver = false;

	config();
	srand(12345);
	loop();
}

void loop()
{
	// Initial piece
	random = rand() % 7;
	newPiece();

	// LCD text
	printLCD(1, 1, "SCORE:");
	printLCD(2, 1, "NEXT:");

	while (!gameOver)
	{
		printBoard();

		// Score
		Lcd_Chr(1, 8, (score/100)+48);
		Lcd_Chr(1, 9, ((score/10)%10)+48);
		Lcd_Chr(1, 10, (score%10)+48);

		// Next piece
		switch (random)
		{
			case 0: BYTE = 'T'; break;
			case 1: BYTE = 'L'; break;
			case 2: BYTE = 'J'; break;
			case 3: BYTE = 'I'; break;
			case 4: BYTE = 'S'; break;
			case 5: BYTE = 'Z'; break;
			case 6: BYTE = 'O'; break;
		}
		Lcd_Chr(2, 7, BYTE);

		delay_ms(500);

		getKey();
		switch (key)
		{
			case 'L': movePiece(false); break;
			case 'R': movePiece(true); break;
			case 'D': {while (!updatePiece());} break;
			case 'A': rotatePiece(false); break;
			case 'B': rotatePiece(true); break;
		}

		if (key != 'D')
			updatePiece();
	}

	printLCD(2, 1, "GAME OVER  ");
	printUART("--GAME OVER--");
}

bool getBit(uint8_t x, uint8_t y)
{
	BYTE = ((y * 10) + x) / 8;
	BIT = ((y * 10) + x) % 8;

	MASK = 1 << (7 - BIT);
	return MASK & board[BYTE];
}

void setBit(uint8_t x, uint8_t y, bool value)
{
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

bool updatePiece()
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
					// Sound
					BUZZER = 1;
					delay_ms(50);
					BUZZER = 0;
					//

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
			return true;
		}
	}

	// Update Y position
	for (i = 0; i < 4; i++)
	{
		currentPiece[i].y++;
		setBit(currentPiece[i].x, currentPiece[i].y, true);
	}

	return false;
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
	for (i = 0; i < 4; i++)
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

void printBoard()
{
	uint8_t x, y;

	for (y = 0; y < 20; y++)
	{
		for (x = 0; x < 10; x++)
			UART1_WRITE(getBit(x, y) ? 219 : 32);
		printUART("\n\r");
	}
}

void getKey()
{
	key = Keypad_Key_Click();

	switch (key)
	{
		case  1: key = 0;   break; // 1
		case  2: key = 0;   break; // 2
		case  3: key = 0;   break; // 3
		case  4: key = 'B'; break; // A
		case  5: key = 0;   break; // 4
		case  6: key = 0;   break; // 5
		case  7: key = 0;   break; // 6
		case  8: key = 'A'; break; // B
		case  9: key = 0;   break; // 7
		case 10: key = 0;   break; // 8
		case 11: key = 0;   break; // 9
		case 12: key = 0;   break; // C
		case 13: key = 0;   break; // *
		case 14: key = 'L'; break; // 0
		case 15: key = 'D'; break; // #
		case 16: key = 'R'; break; // D
	}
}

void config()
{
	ANSEL = 0x00;
	ANSELH = 0x00;
	C1ON_bit = 0x00;
	C2ON_bit = 0x00;

	TRISA = 0x00;
	TRISB = 0x00;
	TRISC = 0x00;
	TRISD = 0x00;
	TRISE = 0x00;

	PORTA = 0x00;
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	PORTE = 0x00;

	// TX
	TRISC.F6 = 0;
	PORTC.F6 = 0;

	// RX
	TRISC.F7 = 1; // C7 Entrada

	// BUZZER
	TRISC.F4 = 0;
	PORTC.F4 = 0;

	UART1_Init(9600); // Iniciar UART1
	delay_ms(200);

	Lcd_Init();
	Lcd_Cmd(_LCD_CLEAR);
	Lcd_Cmd(_LCD_CURSOR_OFF);

	Keypad_Init();
}

void printLCD(char f, char c, const char txt[])
{
	uint8_t i = 0;
	while (txt[i] != '\0')
	{
		Lcd_Chr(f, c+i, txt[i]);
		++i;
	}
}

void printUART(const char txt[])
{
	uint16_t i = 0;
	while (txt[i] != '\0')
	{
		UART1_WRITE(txt[i]);
		i++;
	}
}
