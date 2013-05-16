//
//  main.cpp
//  hw2
//
//  Created by Spencer Moran on 2/25/13.
//  Copyright (c) 2013 Spencer Moran. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <cstdlib>

struct treeNode {
    int manDistance;
    int fOfX;
    std::vector<int> moves;
    int board[3][4];
    
    treeNode() : manDistance(-1) {
        board[0][0] = -1;
        board[0][1] = -1;
        board[0][2] = -1;
        board[0][3] = -1;
        board[1][0] = -1;
        board[1][1] = -1;
        board[1][2] = -1;
        board[1][3] = -1;
        board[2][0] = -1;
        board[2][1] = -1;
        board[2][2] = -1;
        board[2][3] = -1;
    }
};

class compareMD {
public:
    bool operator()(treeNode* a, treeNode* b) {
        return ((a->moves.size() + a->manDistance) > (b->moves.size() + b->manDistance));
    }
};

void createTree(treeNode*);
void getBoard(std::ifstream&, treeNode*);
void setManDistance(treeNode*);
void copyNode(treeNode*, treeNode*);
bool nextToZero(int, int, int, int);
bool notInQueueMD(treeNode*, std::priority_queue<treeNode*, std::vector<treeNode*>, compareMD>);

int main(int argc, const char * argv[])
{
    if (argc != 2) {
        std::cout << "Sorry, wrong number of arguments passed!" << std::endl;
    }
    std::ifstream fin;
    std::ofstream fout;
    fin.open(argv[1]);
    
    treeNode* initialState = new treeNode;
    
    getBoard(fin, initialState);
    setManDistance(initialState);
    createTree(initialState);
    
    
    return 0;
}

void getBoard(std::ifstream& fin, treeNode* initialState) {
    
    int startingBoard[3][4];
    
    fin >> startingBoard[0][0];
    fin >> startingBoard[0][1];
    fin >> startingBoard[0][2];
    fin >> startingBoard[1][0];
    fin >> startingBoard[1][1];
    fin >> startingBoard[1][2];
    fin >> startingBoard[1][3];
    fin >> startingBoard[2][1];
    fin >> startingBoard[2][2];
    fin >> startingBoard[2][3];
    startingBoard[0][3] = -1;
    startingBoard[2][0] = -1;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            initialState->board[i][j] = startingBoard[i][j];
        }
    }
    
}

void createTree(treeNode* initial) {
    
    std::priority_queue<treeNode*, std::vector<treeNode*>, compareMD> pqMD;
    int emptyR = 0, emptyC = 0;
    
    pqMD.push(initial);
    treeNode* current = pqMD.top();
    pqMD.pop();
    
    while (current->manDistance != 0) {
        
        if (!pqMD.empty()) {
            pqMD.pop();
        }
        
        //find the empty space
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                if (current->board[i][j] == 0) {
                    emptyR = i;
                    emptyC = j;
                }
            }
        }
        
        //add all possible swaps to the priority queue
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                if (nextToZero(i, j, emptyR, emptyC) && !(i == 0 && j == 3) && !(i == 2 && j == 0)) {
                    treeNode* addToQueue = new treeNode;
                    copyNode(addToQueue, current);
                    addToQueue->board[emptyR][emptyC] = current->board[i][j];
                    addToQueue->board[i][j] = 0;
                    setManDistance(addToQueue);
                    if (addToQueue->moves.empty()) {
                        addToQueue->moves.push_back(current->board[i][j]);
                        addToQueue->fOfX = int(addToQueue->moves.size()) + addToQueue->manDistance;
                        pqMD.push(addToQueue);
                    }
                    else {
                        if (addToQueue->moves.back() != current->board[i][j]) {
                            addToQueue->moves.push_back(current->board[i][j]);
                            addToQueue->fOfX = int(addToQueue->moves.size()) + addToQueue->manDistance;
                            if (notInQueueMD(addToQueue, pqMD) ) {
                                pqMD.push(addToQueue);
                            }
                        }
                    }
                    
                }
            }
        }
        
        current = pqMD.top();
        
    }
    
    
    std::cout << "Manhattan Distance: " << std::endl;
    std::cout << "Sequence of tiles to be moved : " ;
    for (int i = 0; i < current->moves.size(); i++) {
        std::cout << current->moves[i] << " " ;
    }
    std::cout << std::endl;
    std::cout << "Number of moves: " << current->moves.size() << std::endl << std::endl;

}

void setManDistance(treeNode* current) {
    
    current->manDistance = 0;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            
            if (current->board[i][j] == 1) {
                current->manDistance += std::max(std::max(abs(i-0), abs(j-0)), abs((i-0)-(j-0)));
            }
            
            if (current->board[i][j] == 2) {
                current->manDistance += std::max(std::max(abs(i-0), abs(j-1)), abs((i-0)-(j-1)));
            }
            
            if (current->board[i][j] == 3) {
                current->manDistance += std::max(std::max(abs(i-0), abs(j-2)), abs((i-0)-(j-2)));
            }
            
            if (current->board[i][j] == 4) {
                current->manDistance += std::max(std::max(abs(i-1), abs(j-0)), abs((i-1)-(j-0)));
            }
            
            if (current->board[i][j] == 5) {
                current->manDistance += std::max(std::max(abs(i-1), abs(j-1)), abs((i-1)-(j-1)));
            }
            
            if (current->board[i][j] == 6) {
                current->manDistance += std::max(std::max(abs(i-1), abs(j-2)), abs((i-1)-(j-2)));
            }
            
            if (current->board[i][j] == 7) {
                current->manDistance += std::max(std::max(abs(i-1), abs(j-3)), abs((i-1)-(j-3)));
            }
            
            if (current->board[i][j] == 8) {
                current->manDistance += std::max(std::max(abs(i-2), abs(j-1)), abs((i-2)-(j-1)));
            }
            
            if (current->board[i][j] == 9) {
                current->manDistance += std::max(std::max(abs(i-2), abs(j-2)), abs((i-2)-(j-2)));
            }
        }
    }
}


void copyNode(treeNode* addToQueue, treeNode* current) {
    
    addToQueue->manDistance = current->manDistance;
    addToQueue->fOfX = current->fOfX;
    for (int i =0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            addToQueue->board[i][j] = current->board[i][j];
        }
    }
    for (int i = 0; i < current->moves.size(); i++) {
        addToQueue->moves.push_back(current->moves[i]);
    }
}

bool nextToZero(int r, int c, int eR, int eC) {
    
    
    if (eR - r == 0 && eC - c == 0) {
        return false;
    }
    
    else if (eR - r == 1 && eC - c == 0) {
        return true;
    }
    
    else if (eR - r == 1 && eC - c == 1) {
        return true;
    }
    
    else if (eR - r == 0 && eC - c == 1) {
        return true;
    }
    
    else if (eR - r == -1 && eC - c == 0) {
        return true;
    }
    
    else if (eR - r == -1 && eC - c == -1) {
        return true;
    }
    
    else if (eR - r == 0 && eC - c == -1) {
        return true;
    }
    
    else {
        return false;
    }
}

bool notInQueueMD(treeNode* current, std::priority_queue<treeNode*, std::vector<treeNode*>, compareMD> pqMD) {
    
    std::priority_queue<treeNode*, std::vector<treeNode*>, compareMD> tempQueue;
    
    while (!pqMD.empty()) {
        
        tempQueue.push(pqMD.top());
        treeNode* tempNode = pqMD.top();
        pqMD.pop();
        
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 4; k++) {
                if (current->board[j][k] != tempNode->board[j][k]) {
                    while (!tempQueue.empty()) {
                        pqMD.push(tempQueue.top());
                        tempQueue.pop();
                    }
                    return true;
                }
            }
        }
        if (current->fOfX < tempNode->fOfX) {
            tempQueue.pop();
            while (!tempQueue.empty()){
                pqMD.push(tempQueue.top());
                tempQueue.pop();
            }
            return true;
        }
    }
    while (!tempQueue.empty()) {
        pqMD.push(tempQueue.top());
        tempQueue.pop();
    }
    return false;
}









