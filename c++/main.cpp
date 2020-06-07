// Developer: marcioz98
// email: marciozgaming@gmail.com

#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>

#ifdef WIN32 //if using windows then do windows specific stuff.
#define WIN32_LEAN_AND_MEAN //remove MFC overhead from windows.h which can cause slowness
#define WIN32_EXTRA_LEAN

#include <windows.h>
#endif

#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <ncurses.h>//needed for getch

const int N = 512;
int counter = 0;
int refresh_after = 500;

int board[N][N];
int temp_board[N][N];

void init_board(){
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			board[x][y] = std::rand()%2;
		}
	}
}

void refresh_board(){
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			if (board[x][y] == 0){
				board[x][y] = std::rand()%2;
			}
		}
	}
}

void print_board(){
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			std::cout << board[x][y];
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
}

void copy_board(){
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			temp_board[x][y] = board[x][y];
		}
	}
}

void replace_board(){
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			board[x][y] = temp_board[x][y];
		}
	}
}

void update_board(){

	copy_board();
	int result = 0;
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){

			if(x-1 >= 0 && y-1 >= 0 && board[x-1][y-1] == 1){
				result++;
			}

			if(y-1 >= 0 && board[x][y-1] == 1){
				result++;
			}

			if(x+1 <= N && y-1 >= 0 && board[x+1][y-1] == 1){
				result++;
			}

			if(x-1 >= 0 && board[x-1][y] == 1){
				result++;
			}

			if(x+1 <= N && board[x+1][y] == 1){
				result++;
			}

			if(x-1 >= 0 && y+1 <= N && board[x-1][y+1] == 1){
				result++;
			}

			if(y+1 < N && board[x][y+1] == 1){
				result++;
			}

			if(x+1 <= N && y+1 <= N && board[x+1][y+1] == 1){
				result++;
			}

			// Dead by underpopulation
			if(board[x][y] == 1 && result < 2){
				temp_board[x][y] = 0;
			}

			// Dead by overpopulation
			if(board[x][y] == 1 && result > 3){
				temp_board[x][y] = 0;
			}

			if(board[x][y] == 0 && result == 3){
				temp_board[x][y] = 1;
			}

			result = 0;
		}
	}

	replace_board();
}

void display(){
	update_board();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);
	
	counter++;
	if(counter > refresh_after){
		refresh_board();
		counter = 0;
	}

	glRectf(-1, -1, 1, 1);

	glColor3f(0.0f, 0.0f, 0.0f);

	int row = 0;
	int col = 0;
	for(float i=-1.0; i<1.0; i=i+(1.0/N)){
		col=0;
		for(float j=-1.0; j<1.0; j=j+(1.0/N)){
			if(board[row][col] == 1){
				glRectf(i, j, i+(1.0/N), j+(1.0/N));
			}
			col++;
		}
		row++;
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void setup() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(int argc, char *argv[]){
	
	std::srand(std::time(NULL));

	init_board();
	copy_board();

	glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
        glutInitWindowSize(768, 768);
        glutCreateWindow("Hello World");

        setup();
        glutFullScreen();
        glutDisplayFunc(display);

	glutMainLoop();

	getch();
	return 0;
}
