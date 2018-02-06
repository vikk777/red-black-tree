#include <windows.h>

// форматированный ввод целого числа---------------
char* readInt(){
	char *c = new char;
	char i = 0;
	char flag = 0;
	char b;
	
	do{
	b = getch();
	if ((b == '\b')&&(i>0)){
		printf("\b \b");
		i--;	
	}
	if ((b == '-') && (!flag) && (i == 0)){
		std::cout << b;
		c[i] = b;
		i++;
		c = (char*) realloc(c, sizeof(*c) * (i+1));
		flag = 1;
	}
	if ((b >= '0') && (b <= '9')){
		std::cout << b;
		c[i] = b;
		i++;
		c = (char*) realloc(c, sizeof(*c) * (i+1));
	}
	}while (b != '\r');
	c[i] = '\0';
	
	return c;
}

//Цвета в консоли
enum ConsoleColor
{
        Black         = 0,
        Blue          = 1,
        Green         = 2,
        Cyan          = 3,
        Red           = 4,
        Magenta       = 5,
        Brown         = 6,
        LightGray     = 7,
        DarkGray      = 8,
        LightBlue     = 9,
        LightGreen    = 10,
        LightCyan     = 11,
        LightRed      = 12,
        LightMagenta  = 13,
        Yellow        = 14,
        White         = 15
};
 
void SetColor(ConsoleColor text, ConsoleColor background)
{
	HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleHandle, (WORD)((background << 4) | text));
}

//Координаты в консоли
void gotoxy(int col, int line){
	COORD coord;
	coord.X = col;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), coord);
}
