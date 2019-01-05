#pragma once

#ifndef DATA_PATH

#define DATA_PATH "E:\\workplace\\data\\INRIAPerson\\"
#define TRAIN_PATH "E:\\workplace\\data\\INRIAPerson\\TrainNew\\"
#define TEST_PATH "E:\\workplace\\data\\INRIAPerson\\TestNew\\"
#endif // !DATA_PATH


#ifndef HOG_PARA

#define HOG_PARA 1
#define BIN_SIZE 20
#define BIN_NVM 9
#define NORM_WIDTH 82
#define NORM_HEIGHT 130
#define CELL_SIZE 8
#define BLOCK_SIZE 2
#define PIC_CELL_WH 50
#define CELL_W_NVM  ((NORM_WIDTH-2) / CELL_SIZE)
#define CELL_H_NVM  ((NORM_HEIGHT-2) / CELL_SIZE)
#define BLOCK_W_NVM  (CELL_W_NVM - BLOCK_SIZE + 1)
#define BLOCK_H_NVM  (CELL_H_NVM - BLOCK_SIZE + 1)
#define CELL_NVM (CELL_W_NVM * CELL_H_NVM)
#define BLOCK_NVM (BLOCK_W_NVM * BLOCK_H_NVM)
#define ARRAY_ALL (BLOCK_W_NVM * BLOCK_H_NVM * BLOCK_SIZE * BLOCK_SIZE * BIN_NVM)
#define random(x) (rand()%x)

#endif // !HOG_PARA
