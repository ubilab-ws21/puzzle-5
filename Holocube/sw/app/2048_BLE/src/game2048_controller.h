#ifndef GAME2048_H
#define GAME2048_H
//#include <stdio.h>
#include <iostream>
using namespace std;
#include <Arduino.h>
#define SIZE 4 
#define WIN_SCORE 2048

class GAME2048 
{
private:
    int board[4][4];
    int previous[4][4];
    string Location[4][4];
    int moveRecord[4][4];


public:
    void init() {
        for (int i = 0; i < SIZE * SIZE; i++) {
            this->board[i / 4][i % 4] = 0;
            this->previous[i / 4][i % 4] = 0;
            this->moveRecord[i / 4][i % 4] = 0;
        }
    };
    int  addRandom();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    int  judge();
    void recordLocation(int direction);
    void countMoveRecord(int direction);
    void recordBoard() {
        for (int i = 0; i < SIZE * SIZE; i++) {
            this->previous[i / 4][i % 4] = this->board[i / 4][i % 4];
        }
    };



    int  comparePre() { //compare if the two values are equal or not
        int x = 0;
        for (int i = 0; i < SIZE * SIZE; i++) {
            if (this->board[i / 4][i % 4] == this->previous[i / 4][i % 4])
                x++;
        }
        if (x >= 16) {
            return 1;
        }
        return 0;
    };
    int* getBoard() {
        return &this->board[0][0];
    };
    string* getLocation() {
        return &this->Location[0][0];
    };
    int* getMoveRecord() {
        return &this->moveRecord[0][0];
    };
};

#endif

