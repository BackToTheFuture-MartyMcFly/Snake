// Snake.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.


// лежит на GitHub !!!
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <ctime>

#define Xmax 45
#define Ymax 15
#define default_long 10
#define real_long 9

int FlagEndGame = 0;

HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD eat_coord, begin_coord, score_coord, lenght_coord;

enum { up_ = 72, down_ = 80, left_ = 75, right_ = 77, extended_code = 224, esc = 27, pause = 'p', up_2 = 'w', down_2 = 's', left_2 = 'a', right_2 = 'd', enter = 13};


void create_arena() {
	printf("+");
	for (int i = 0; i < Xmax - 1; i++) {
		printf("=");
	}
	printf("+");
	for (int i = 0; i < Ymax; i++) {
		printf("\n");
		printf("|");
		for (int j = 0; j < Xmax - 1; j++) {
			printf(" ");
		}
		printf("|");
	}
	printf("\n");
	printf("+");
	for (int i = 0; i < Xmax - 1; i++) {
		printf("=");
	}
	printf("+");
	printf("\n\n");
}


struct Snake {
	COORD *body;
	int tmp_lenght = default_long, lenght = real_long;

	void alloc() {
		tmp_lenght += default_long;
		COORD* tmp = new COORD[tmp_lenght];
		for (int i = 0; i < tmp_lenght; i++) {
			tmp[i] = body[i];
		}
		body = new COORD[tmp_lenght];
		for (int i = 0; i < lenght; i++) {
			body[i] = tmp[i];
		}
		delete[] tmp;
	}

	void delete_snake() {
		/*for (int i = 1; i < lenght; i++) {
			SetConsoleCursorPosition(Console, body[i]);
			printf("@");
		}*/
		lenght = real_long;
		tmp_lenght = default_long;
		delete[] body;
	}
};

void Init_snake(Snake & snake) {
	snake.body = new COORD[snake.tmp_lenght] ;
	snake.body[0].X = begin_coord.X;
	snake.body[0].Y = begin_coord.Y;
	SetConsoleCursorPosition(Console, begin_coord);
	printf("+");
	for (int i = 1; i < snake.lenght; i++) {
		snake.body[i].X = begin_coord.X - i;
		snake.body[i].Y = begin_coord.Y;
		SetConsoleCursorPosition(Console, snake.body[i]);
		printf("@");
	}
}

int this_is_snake_for_end(Snake& snake, COORD& object) {							
	for (int i = 1; i < snake.lenght; i++) {									 										
		if (object.X == snake.body[i].X && object.Y == snake.body[i].Y)            
			return 1;
		else
			continue;
	}
	return 0;
}

int this_is_snake_for_eat(Snake& snake, COORD& object) {                  
	if (object.X == snake.body[0].X && object.Y == snake.body[0].Y)           
		return 1;
	return this_is_snake_for_end(snake, object);
}


void create_eat(Snake& snake) {
	srand(time(NULL));
	eat_coord.X = rand() % (Xmax - 1) + 1;
	eat_coord.Y = rand() % (Ymax - 1) + 1;
	while (this_is_snake_for_eat(snake, eat_coord)) {
		eat_coord.X = rand() % (Xmax - 1) + 1;
		eat_coord.Y = rand() % (Ymax - 1) + 1;
	}
	SetConsoleCursorPosition(Console, eat_coord);
	printf("*");
}

void snake_growth(Snake& snake) {
	snake.lenght++;
	if (snake.lenght >= snake.tmp_lenght) {
		snake.alloc();
	}
	snake.body[snake.lenght - 1] = snake.body[snake.lenght - 2];
}

void move_snake(Snake& snake, COORD& direct) {
	while(!_kbhit()) {
		SetConsoleCursorPosition(Console, score_coord);
		printf("Score: %d", snake.lenght - 3);
		SetConsoleCursorPosition(Console, lenght_coord);
		printf("Snake length: %d", snake.lenght);
		if (snake.body[0].X == eat_coord.X) {
			if (snake.body[0].Y == eat_coord.Y) {
				snake_growth(snake);
				create_eat(snake);
			}
		}
		SetConsoleCursorPosition(Console, snake.body[snake.lenght - 1]);
		printf(" ");
		for (int i = snake.lenght - 1; i > 0; i--) {
			snake.body[i] = snake.body[i - 1];
		}	
		snake.body[0].X += direct.X;
		snake.body[0].Y += direct.Y;
		if (this_is_snake_for_end(snake, snake.body[0])) {
			system("cls");
			SetConsoleCursorPosition(Console, begin_coord);
			printf("YOU LOSE! GAME OVER!\n Press 'Enter' to restart game or 'Esc' to exit");
			FlagEndGame = 1;
			break;
		}
		if (snake.body[0].X == Xmax)
			snake.body[0].X = 1;
		if (snake.body[0].X == 0)
			snake.body[0].X = Xmax - 1;
		if (snake.body[0].Y == Ymax + 1)
			snake.body[0].Y = 1;
		if (snake.body[0].Y == 0)
			snake.body[0].Y = Ymax;
		SetConsoleCursorPosition(Console, snake.body[0]);
		printf("+");
		for (int i = 1; i < snake.lenght; i++) {
			SetConsoleCursorPosition(Console, snake.body[i]);
			printf("@");
		}
		Sleep(111);	
	}
}


int main()
{
	score_coord.X = Xmax + 2; score_coord.Y = 2;
	lenght_coord.X = Xmax + 2; lenght_coord.Y = 3;
	begin_coord.X = int(Xmax / 2);
	begin_coord.Y = int(Ymax / 2);
	COORD direct;
	direct.X = 1; direct.Y = 0;
	Snake snake;
	create_arena();
	Init_snake(snake);
	COORD tmp;
	create_eat(snake);
	for (;;) {
		int cod;
		move_snake(snake, direct);
		if (FlagEndGame == 1) {
			snake.delete_snake();
			system("cls");
			SetConsoleCursorPosition(Console, begin_coord);
			printf("YOU LOSE! GAME OVER!\n Press 'Enter' to restart game or 'Esc' to exit");
			while (1) {
				cod = _getch();
				if (cod == enter) {
					Snake snake;
					direct.X = 1; direct.Y = 0;
					system("cls");
					create_arena();
					Init_snake(snake);
					create_eat(snake);
					move_snake(snake, direct);
					break;
				}
			}
		}
		else
		tmp = direct;
		cod = _getch();
		if (cod == esc) 
			break;
		if (cod == pause) {
			
			while (cod) {
				while (!_kbhit()) {
					SetConsoleCursorPosition(Console, begin_coord);
					printf("PAUSE");
					Sleep(200);
					SetConsoleCursorPosition(Console, begin_coord);
					printf("     ");
					Sleep(200);
				}
				cod = _getch();
				if (cod == pause) {
					SetConsoleCursorPosition(Console, begin_coord);
					printf("     ");
					break;
				}	
			}
		}
		switch (cod) {
			case up_2:
				direct.X = 0; direct.Y = -1;
				break;
			case down_2:
				direct.X = 0; direct.Y = 1;
				break;
			case left_2:
				direct.X = -1; direct.Y = 0;
				break;
			case right_2:
				direct.X = 1; direct.Y = 0;
				break;
			}
			if (snake.body[0].X + direct.X == snake.body[1].X && snake.body[0].Y + direct.Y == snake.body[1].Y) {
				direct.X = tmp.X;
				direct.Y = tmp.Y;
			}
		if (cod == extended_code) {
			int key = _getch();
			switch (key) {
			case up_:
				direct.X = 0; direct.Y = -1;
				break;
			case down_:
				direct.X = 0; direct.Y = 1;
				break;
			case left_:
				direct.X = -1; direct.Y = 0;
				break;
			case right_:
				direct.X = 1; direct.Y = 0;
				break;
			}
			if (snake.body[0].X + direct.X == snake.body[1].X && snake.body[0].Y + direct.Y == snake.body[1].Y) {
				direct.X = tmp.X;
				direct.Y = tmp.Y;
			}
		}
	}
	
	snake.delete_snake();
	system("cls");
	printf("Thanks for game!\n");
}


