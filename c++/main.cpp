// Developer: marcioz98
// email: marciozgaming@gmail.com

#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

const int N = 1024;
int counter = 0;
int refresh_after = 10000;

void init_board(int** board){
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			board[x][y] = std::rand()%2;
		}
	}
}

void refresh_board(int** board){
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			if (board[x][y] == 0){
				board[x][y] = std::rand()%2;
			}
		}
	}
}

void print_board(int** board){
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			std::cout << board[x][y];
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
}

void copy_board(int** board, int** temp_board){
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			temp_board[x][y] = board[x][y];
		}
	}
}

void replace_board(int** board, int**temp_board){
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			board[x][y] = temp_board[x][y];
		}
	}
}

void update_board(int** board, int** temp_board){

	copy_board(board, temp_board);
	int result = 0;
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){

			if(x-1 >= 0 && y-1 >= 0 && board[x-1][y-1] == 1){
				result++;
			}

			if(y-1 >= 0 && board[x][y-1] == 1){
				result++;
			}

			if(x+1 < N && y-1 >= 0 && board[x+1][y-1] == 1){
				result++;
			}

			if(x-1 >= 0 && board[x-1][y] == 1){
				result++;
			}

			if(x+1 < N && board[x+1][y] == 1){
				result++;
			}

			if(x-1 >= 0 && y+1 < N && board[x-1][y+1] == 1){
				result++;
			}

			if(y+1 < N && board[x][y+1] == 1){
				result++;
			}

			if(x+1 < N && y+1 < N && board[x+1][y+1] == 1){
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

	replace_board(board, temp_board);
}

void display(GLFWwindow* window, int** board, int**temp_board){
	counter++;
	if(counter > refresh_after){
		refresh_board(board);
		counter = 0;
	}

	glColor3f(0.0f, 0.0f, 0.0f);

	glRectf(-1, -1, 1, 1);

	glColor3f(0.0f, 1.0f, 0.0f);

	int row = 0;
	int col = 0;
	double slope = 2.0 / N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if(board[i][j] == 1){
				// Mapping i and j to -1;+1 
				double i_new = -1.0 + slope * (float) i;
				double j_new = -1.0 + slope * (float) j;
				glRectf(i_new, j_new, i_new + slope, j_new + slope);
			}
		}
	}
}

void setup(GLFWwindow* window) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char *argv[]){
	int** board;
	board = new int*[N];
	int** temp_board;
	temp_board = new int*[N];

    for (int i = 0; i < N; ++i) {
        board[i] = new int[N];
		temp_board[i] = new int[N];
    }
	
	std::srand(std::time(NULL));

	init_board(board);
	copy_board(board, temp_board);

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		std::cout << "Initialization failed.";
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* window = glfwCreateWindow(1024, 1024, "Hello World!", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "OpenGL context or window creation failed.";
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	setup(window);
	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update_board(board, temp_board);
		display(window, board, temp_board);

		glfwSwapBuffers(window);
        glfwPollEvents();
	}

	glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
