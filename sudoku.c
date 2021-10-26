#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Struct para guardar o valor da linha, coluna, e grade
typedef struct {
	int col;
	int row;
	int(* board)[9];
} parameters;

// Aloca a memória para a struct e passa os parâmetros
parameters* createParameters(int c, int r, int(*b)[9]) {
	parameters* params = (parameters*) malloc(sizeof(parameters));

	params->col = c;
	params->row = r;
	params->board = b;

	return params;
}

// Checa as linhas
void* check_rows(void* params) {
	parameters* param = (parameters*) params;

	int startRow = param->row;
	int startCol = param->col;

	for (int i = startRow; i < 9; i++) {
		int row[10] = {0};

		for (int j = startCol; j < 9; j++) {
			int square = param->board[i][j];

			if (row[square] != 0) {
				return (void*) 0;
			} else {
				row[square] = 1;
			}
		}
	}

	return (void*) 1;
}

// Checa as colunas
void* check_cols(void* params) {
	parameters* param = (parameters*) params;

	int startRow = param->row;
	int startCol = param->col;

	for (int i = startCol; i < 9; i++) {
		int col[10] = {0};

		for (int j = startRow; j < 9; j++) {
			int square = param->board[j][i];

			if (col[square] != 0) {
				return (void*) 0;
			} else {
				col[square] = 1;
			}
		}
	}

	return (void*) 1;
}

// Checa as subgrades
void* check_grid(void* params) {
	parameters* param = (parameters*) params;

	int startRow = param->row;
	int startCol = param->col;

	int saved[10] = {0};

	for (int i = startRow; i < startRow + 3; i++) {
		for (int j = startCol; j < startCol + 3; j++) {
			int square = param->board[i][j];

			if (saved[square] != 0) {
				return (void*) 0;
			} else {
				saved[square] = 1;
			}
		}
	}

	return (void*) 1;
}

int main() {
	// Grade principal
	int board[9][9] = {
		{6, 2, 4, 5, 3, 9, 1, 8, 7},
		{5, 1, 9, 7, 2, 8, 6, 3, 4},
		{8, 3, 7, 6, 1, 4, 2, 9, 5},
		{1, 4, 3, 8, 6, 5, 7, 2, 9},
		{9, 5, 8, 2, 4, 7, 3, 6, 1},
		{7, 6, 2, 3, 9, 1, 4, 5, 8},
		{3, 7, 1, 9, 5, 6, 8, 4, 2},
		{4, 9, 6, 1, 8, 2, 5, 7, 3},
		{2, 8, 5, 4, 7, 3, 9, 1, 6}
	};

	// Threads para as linhas, colunas e as nove subgrades
	pthread_t thread_rows, thread_cols;
	pthread_t threads_grids[9];

	// Inicialização das subgrades
	for (int i = 0; i < 9; i++) {
		pthread_t thread;
		threads_grids[i] = thread;
	}

	// Variáveis para guardar o resultado das threads
	void* result_rows;
	void* result_cols;
	void* result_grids[9];


	// Criação do parâmetro e threads para as linhas e colunas
	parameters* params = createParameters(0, 0, board);

	if (pthread_create(&thread_rows, NULL, check_rows, (void*) params)) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&thread_cols, NULL, check_cols, (void*) params)) {
		perror("pthread_create");
		exit(1);
	}

	// Criação dos parâmetros, threads e joins para as subgrades;
	int x = 0, y = 0, z = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {

			parameters* params = createParameters(x, y, board);

			if (pthread_create(&threads_grids[z], NULL, check_grid, (void*) params)) {
				perror("pthread_create");
				exit(1);
			}

			if (pthread_join(threads_grids[z], &result_grids[z])) {
				perror("pthread_join");
				exit(1);
			}

			// Libera a área de memória alocada pelo parâmetro
			free(params);

			y += 3;

			z++;
		}

		y = 0;

		x += 3;
	}

	// Join das threads das linhas e das colunas

	if (pthread_join(thread_rows, &result_rows)) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(thread_cols, &result_cols)){
		perror("pthread_join");
		exit(1);
	}

	// Libera a área de memória alocada pelo parâmetro
	free(params);

	// Checa se a solução do Sudoku é válida
	if ( (int) result_rows == 1 && (int) result_cols == 1) {
		for (int i = 0; i < 9; i++) {
			if ( (int) result_grids[i] != 1) {
				printf("A solucao para o Sudoku esta incorreta\n");
			} 	
		}

		printf("A solucao para o Sudoku esta correta\n");
	} else {
		printf("A solucao para o Sudoku esta incorreta\n");
	}

	// Termino da execução da thread pai
	pthread_exit(NULL);

	return 0;	
}