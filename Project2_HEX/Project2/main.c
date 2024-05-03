#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAXSIZE 500
#define MAX_BOARDSIZE 11
#define BOARD 1
#define COMMAND 2
#define STOPLINE 3
#define DIRECTION 6
#define RED_WIN 7
#define BLUE_WIN 8
#define NO_WINNER 9
#define INI 0


typedef struct {
    int x;
    int y;
}Point;

typedef struct {
    int x, y;
} PathNode;

typedef struct {
    int winner;
    bool status;
    PathNode winningPath[MAXSIZE];
    int pathLength;
}Status;

//void printBoard(char** curr_board) {
//    if (curr_board == NULL) {
//        printf("Board is NULL.\n");
//        return;
//    }
//
//    printf("Current Board:\n");
//    for (int i = 0; i < MAXSIZE; i++) {
//        if (curr_board[i] == NULL) {
//            // Assuming NULL marks the end of the meaningful entries in the array
//            break;
//        }
//        printf("%s\n", curr_board[i]);
//    }
//}
//void clearWinningPath(Status* status) {
//    for (int i = 0; i < status->pathLength; i++) {
//        status->winningPath[i].x = 0; // Set default x coordinate
//        status->winningPath[i].y = 0; // Set default y coordinate
//    }
//    status->pathLength = 0; // Reset path length to zero
//}
//
//void clearWinningNode(Status* status, int x, int y) {
//    for (int i = 0; i < status->pathLength; i++) {
//        if (status->winningPath[i].x == x && status->winningPath[i].y == y) {
//            // Mark this node as invalid
//            status->winningPath[i].x = 0;
//            status->winningPath[i].y = 0;
//            return;  // Exit after finding and invalidating the node
//        }
//    }
//}
//void printWinningPath(Status status) {
//    if (status.pathLength == 0) {
//        printf("No winning path recorded.\n");
//        return;
//    }
//    printf("Winning path coordinates:\n");
//    for (int i = 0; i < status.pathLength; i++) {
//        printf("(%d, %d)\n", status.winningPath[i].x, status.winningPath[i].y);
//    }
//}

void recordNode(Status* status, int x, int y) {
    if (status->pathLength < MAXSIZE) { // Ensure there is space in the array
        status->winningPath[status->pathLength].x = x;
        status->winningPath[status->pathLength].y = y;
        status->pathLength++; // Increment the path length after adding the node
    }
}
//empty each cell of 2d array, but the array still exist
void emptyArray(char** array){
    for (int i = 0; i< MAXSIZE; i++) {
        if (array[i]) {
            free(array[i]);
            array[i] = NULL;
        }
    }
}

void freeBoolArray(bool** array,int size){
    for (int i = 0; i<size; i++) {
        if (array[i]) {
            free(array[i]);
        }
    }
    free(array);
}

void freePointArray(Point** array,int size){
    for (int i = 0; i<size; i++) {
        if (array[i]) {
            free(array[i]);
        }
    }
    free(array);
}

int calcuBoard(char** curr_board){
    int max_width = 0;
    int i = 0;
    int row_number = 0;
    while (curr_board[i] &&strcmp(curr_board[i], "") != 0) {
        int width = (int)strlen(curr_board[i]);
        if (width > max_width) {
            max_width = width;
            row_number = i;
        }
        i++;
    }
    //    A cell contains 5 characters,the n-th row must has (n-1)cells and (n-1) '-'
    int board_size = row_number;
    return board_size;
}

int calcuPawn(char** curr_board){
    int i = 0;
    int num_Red = 0,num_Blue = 0;
    char* curr_row = NULL;
    while (curr_board[i] &&strcmp(curr_board[i], "") != 0) {
        curr_row = (char*)malloc(strlen(curr_board[i])+1);
        strcpy(curr_row, curr_board[i]);
        for (int j=0; curr_row[j] != '\0'; j++) {
            if (curr_row[j] == 'r') {
                num_Red++;
            }
            else if (curr_row[j] == 'b'){
                num_Blue++;
            }
        }
        free(curr_row);
        curr_row = NULL;
        i++;
    }
    free(curr_row);
    return (num_Red + num_Blue);
}

bool isRedLastMove(char** curr_board){
    int i = 0;
    int num_Red = 0,num_Blue = 0;
    char* curr_row = NULL;
    while (curr_board[i] &&strcmp(curr_board[i], "") != 0) {
        curr_row = (char*)malloc(strlen(curr_board[i])+1);
        strcpy(curr_row, curr_board[i]);
        for (int j=0; curr_row[j] != '\0'; j++) {
            if (curr_row[j] == 'r') {
                num_Red++;
            }
            else if (curr_row[j] == 'b'){
                num_Blue++;
            }
        }
        free(curr_row);
        curr_row = NULL;
        i++;
    }
    free(curr_row);
    if (num_Red >num_Blue) {
        return true;
    }
    else{
        return false;
    }
}


bool calcuCorrect(char** curr_board){
    int i = 0;
    int num_Red = 0,num_Blue = 0;
    char* curr_row = NULL;
    while (curr_board[i] &&strcmp(curr_board[i], "") != 0) {
        curr_row = (char*)malloc(strlen(curr_board[i])+1);
        strcpy(curr_row, curr_board[i]);
        
        for (int j=0; curr_row[j] != '\0'; j++) {
            if (curr_row[j] == 'r') {
                num_Red++;
            }
            else if (curr_row[j] == 'b'){
                num_Blue++;
            }
        }
        
        free(curr_row);
        curr_row = NULL;
        i++;
    }
    if (((num_Red - num_Blue)== 1) || ((num_Red - num_Blue)== 0)) {
        free(curr_row);
        return true;
    }
    else {
        free(curr_row);
        return false;
    }
}

Point calcuLineBound(char** curr_board,int x, int y){
    int boardSize = calcuBoard(curr_board);
    int baseLenth_y = 0;
    int maxLenth_currline = 0;
    int minLenth_currline = 0;
    Point Bound;
    if (boardSize % 2 == 0) {
        baseLenth_y = boardSize/2 * 6;//boardSize * 6 + 1
    }
    else{
        baseLenth_y = boardSize/2 * 6 + 3;// board/size * 6 + 4 - 1
        
    }

    if (x < boardSize) {
        maxLenth_currline = baseLenth_y + 3*x;//already starts from 0
        minLenth_currline = baseLenth_y - 3*(x+1);
    }
    else if (x == boardSize){
        maxLenth_currline = baseLenth_y + 3*x-2;
        minLenth_currline = 0;
    }
    else{
        maxLenth_currline = baseLenth_y + (2*boardSize - x)*3;// (boardSize - (x - boardSize))*3
        minLenth_currline = baseLenth_y - (2*boardSize - x + 1)*3;
    }
    
    Bound.x = maxLenth_currline;
    Bound.y = minLenth_currline;
    
    return Bound;
    
}

bool isValidMove(char** curr_board,int x, int y){
    Point Bound = calcuLineBound(curr_board, x, y);
    int maxLenth_currline = Bound.x;
    int minLenth_currline = Bound.y;
    int boardSize = calcuBoard(curr_board);
    if (x >=0 && y > minLenth_currline && x< 2*boardSize && y < maxLenth_currline) {
        return true;
    }
    return false;

}

bool isPointOnWinningPath(int x, int y,Status status) {
    for (int i = 0; i < status.pathLength; i++) {
        if (status.winningPath[i].x == x && status.winningPath[i].y == y) {
            return true;  // The point is part of the winning path
        }
    }
    return false;  // The point was not found on the winning path
}

bool searchPath(char** curr_board, bool** visited, int x, int y, char player, Point direction[], int boardSize,Status* status) {
    if (boardSize == 1 && player == 'r') {
        return true;
    }
    if ((player == 'r' && x >= boardSize && (y == calcuLineBound(curr_board, x, y).x - 4 || y == calcuLineBound(curr_board, x, y).x - 2)) ||
        (player == 'b' && x <= boardSize && (y == calcuLineBound(curr_board, x, y).x - 4 || y == calcuLineBound(curr_board, x, y).x - 2)))
    {
        return true;
    }

    visited[x][y] = true;
    for (int i = 0; i < DIRECTION; i++) {
        int new_x = x + direction[i].x;
        int new_y = y + direction[i].y;
        if (isValidMove(curr_board, new_x, new_y) && curr_board[new_x][new_y] == player && !visited[new_x][new_y]) {
            if (searchPath(curr_board, visited, new_x, new_y, player, direction, boardSize,status)) {
                recordNode(status, new_x, new_y);
                return true;
            }
            
        }
    }
    return false;
}

Status isGamerOver(char** curr_board,int boardSize){
    Status gameStatus;
    gameStatus.pathLength = INI;
    gameStatus.winner = INI;
    gameStatus.status = true;
    
    if (calcuCorrect(curr_board)==false) {
        gameStatus.winner = NO_WINNER;
        gameStatus.status = false;//game error,obviously the game is not over

        return gameStatus;
        
    }
    if (calcuPawn(curr_board) == 0 ) {
        gameStatus.winner = NO_WINNER;//NO winner but the board it still working
        return gameStatus;
    }

    int maxRowNum = boardSize*2+1;
    int maxColumnNum = boardSize*6-1;
    //  up, top right corner,bottom right corner,down, bottom left corner,top left corner,
    Point direction[DIRECTION]={{-2,0},{-1,-3},{1,3},{1,-3},{-1,3},{2,0}};
    Point** redPawn_edge = (Point **)malloc(boardSize * sizeof(Point*));
    Point** bluePawn_edge = (Point **)malloc(boardSize * sizeof(Point*));
    // initialize status for all possible pawns
    bool** visited = (bool**)malloc(maxRowNum * sizeof(bool*));
    for (int i = 0; i < maxRowNum; i++) {
        visited[i] = (bool*)malloc(maxColumnNum * sizeof(bool));
        memset(visited[i], 0, maxColumnNum * sizeof(bool));  // Initialize all values to false
    }
    //initialize all possible edge_pawns positions which located on left edges
    for (int i = 0; i<boardSize; i++) {
        redPawn_edge[i] = (Point *)malloc(sizeof(Point));
        bluePawn_edge[i] = (Point *)malloc(sizeof(Point));
        // Coordinates are reversed from natural sense
        redPawn_edge[i]->x = boardSize - i;
        redPawn_edge[i]->y = 2 + 3 * i;
        bluePawn_edge[i]->x = boardSize + i;
        bluePawn_edge[i]->y = 2 + 3 * i;

    }
    
    for (int i = 0; i<boardSize; i++) {
        if (curr_board[redPawn_edge[i]->x][redPawn_edge[i]->y] == 'r' && !visited[redPawn_edge[i]->x][redPawn_edge[i]->y]) {
            
            if (searchPath(curr_board, visited, redPawn_edge[i]->x, redPawn_edge[i]->y, 'r', direction,boardSize,&gameStatus)) {
                recordNode(&gameStatus, redPawn_edge[i]->x, redPawn_edge[i]->y);
                freeBoolArray(visited, maxRowNum);
                freePointArray(bluePawn_edge, boardSize);
                freePointArray(redPawn_edge, boardSize);
                gameStatus.winner = RED_WIN;
                //load the edge point into the path
                return gameStatus;
            }
            
        }
    }

    for (int i = 0; i < maxRowNum; i++) {
        free(visited[i]);
        visited[i] = (bool*)malloc(maxColumnNum * sizeof(bool));
        memset(visited[i], 0, maxColumnNum * sizeof(bool));
    }

    for (int i = 0; i<boardSize; i++) {
        if (curr_board[bluePawn_edge[i]->x][bluePawn_edge[i]->y] == 'b' && !visited[bluePawn_edge[i]->x][bluePawn_edge[i]->y]) {
            if (searchPath(curr_board, visited, bluePawn_edge[i]->x, bluePawn_edge[i]->y, 'b', direction,boardSize,&gameStatus)) {
                recordNode(&gameStatus, bluePawn_edge[i]->x, bluePawn_edge[i]->y);
                freeBoolArray(visited, maxRowNum);
                freePointArray(bluePawn_edge, boardSize);
                freePointArray(redPawn_edge, boardSize);
                gameStatus.winner = BLUE_WIN;
                return gameStatus;
            }
        }

    }
    // No winner
    freeBoolArray(visited, maxRowNum);
    freePointArray(bluePawn_edge, boardSize);
    freePointArray(redPawn_edge, boardSize);
    gameStatus.winner = NO_WINNER;
    return gameStatus;
}

void isBoardPossi(char** curr_board) {
    int boardSize = calcuBoard(curr_board);
    Status currentStatus = isGamerOver(curr_board, boardSize);
    // Check if the board configuration is correct
    if (!currentStatus.status) {
        printf("NO\n");
        return;
    }
    if (currentStatus.winner == NO_WINNER) {
        printf("YES\n");
        return;
    }

    // Determine who made the last move
    bool lastMoveByRed = isRedLastMove(curr_board);
    char lastPlayer = lastMoveByRed ? 'r' : 'b';
    int winAfterRemove = 0;
    int loseAfterRemove = 0;
    for (int i = 0; i < currentStatus.pathLength; i++) {
        int x = currentStatus.winningPath[i].x;
        int y = currentStatus.winningPath[i].y;
        if (curr_board[x][y] == lastPlayer) { // Ensure the node is for the last player
            // Temporarily remove pawn
            char savedPawn = curr_board[x][y];
            curr_board[x][y] = ' ';
            // Check if the game was over before this pawn was added
            Status statusWithoutLastPawn = isGamerOver(curr_board, boardSize);
            if (statusWithoutLastPawn.winner == RED_WIN || statusWithoutLastPawn.winner == BLUE_WIN) {
                winAfterRemove++;
            }
            else loseAfterRemove++;
            // Restore pawn
            curr_board[x][y] = savedPawn;
            }
        }
    
    if (loseAfterRemove ==0) {
        printf("NO\n");
    }
    else printf("YES\n");
}

void runCommands(char** curr_board,char** curr_commands){
    int i = 0;
    while (curr_commands[i] && strcmp(curr_commands[i], "") != 0) {
        if (strcmp(curr_commands[i], "BOARD_SIZE") == 0) {
            printf("%d\n",calcuBoard(curr_board));
        }
        else if (strcmp(curr_commands[i], "PAWNS_NUMBER") == 0){
            printf("%d\n",calcuPawn(curr_board));
        }
        else if(strcmp(curr_commands[i], "IS_BOARD_CORRECT") == 0){
            if(calcuCorrect(curr_board)){
                printf("%s\n","YES");}
            else {
                printf("%s\n","NO");}
        }
        else if(strcmp(curr_commands[i], "IS_GAME_OVER") == 0){
            Status temp = isGamerOver(curr_board,calcuBoard(curr_board));
            if (temp.winner == RED_WIN) {
                printf("YES RED\n");
            }
            else if (temp.winner == BLUE_WIN){
                printf("YES BLUE\n");
            }
            else if (temp.winner == NO_WINNER){
                printf("NO\n");
            }
        }
        else if (strcmp(curr_commands[i], "IS_BOARD_POSSIBLE") == 0){
            isBoardPossi(curr_board);
        }
        i++;
    }
    emptyArray(curr_board);
    emptyArray(curr_commands);
}

bool checkChar(const char string[],char example){
    for (int j=0; string[j] != '\0'; j++) {
        if (string[j] == example) {
            return true;
        }
    }
    return false;
}

int CheckLine(const char line[]) {
    if (strcmp(line, "BOARD_SIZE") == 0 || strcmp(line, "PAWNS_NUMBER") == 0||strcmp(line, "IS_BOARD_CORRECT") == 0 || strcmp(line, "IS_GAME_OVER") == 0||strcmp(line, "IS_BOARD_POSSIBLE") == 0) {
        return COMMAND;
    }
    else if (checkChar(line, '<')||checkChar(line, '-')){
        return BOARD;
    }
    else if (strcmp(line, "") == 0|| strspn(line, " \t\r\n") == strlen(line))
        return STOPLINE;
    
    return 0;
}


void iniArray(char** array){
    for (int i = 0; i < MAXSIZE; i++) {
        array[i] = NULL;
    }
}


int main(void) {
    char curr_line[MAXSIZE];
    char** curr_board = (char**)malloc(MAXSIZE * sizeof(char*)) ;
    char** curr_commands = (char**)malloc(MAXSIZE * sizeof(char*));
    int i = 0, j = 0;
    iniArray(curr_board);
    iniArray(curr_commands);
    
    while(fgets(curr_line, MAXSIZE, stdin) != NULL) {
        curr_line[strcspn(curr_line, "\n")]= 0;//Remove newline character
        if ((CheckLine(curr_line)) == BOARD) {
            curr_board[i] = (char*)malloc(strlen(curr_line)+1);
            strcpy(curr_board[i++], curr_line);
        }
        else if ((CheckLine(curr_line)) == COMMAND){
            curr_commands[j] = (char*)malloc(strlen(curr_line)+1);
            strcpy(curr_commands[j++], curr_line);
            runCommands(curr_board,curr_commands);
            i = 0; j = 0;
        }
        memset(curr_line, 0, sizeof(curr_line));

    }

    free(curr_board);
    free(curr_commands);
    return EXIT_SUCCESS;
}
