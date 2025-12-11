#include<stdio.h>
#include <stdbool.h>
#include <stdlib.h> // Required for malloc and free

typedef struct{
   char pieceType;
   char color;
   bool isonboard;
}cell;
// Helper to check if path is clear between from and to (excluding endpoints)
bool isPathClear(cell* board, int MAX_NODE, int fromrow, int fromcol, int torow, int tocol) {
    int rowStep = (torow - fromrow) == 0 ? 0 : (torow - fromrow) / abs(torow - fromrow);
    int colStep = (tocol - fromcol) == 0 ? 0 : (tocol - fromcol) / abs(tocol - fromcol);

    int currentRow = fromrow + rowStep;
    int currentCol = fromcol + colStep;

    while (currentRow != torow || currentCol != tocol) {
        int idx = currentRow * MAX_NODE + currentCol;
        if (board[idx].pieceType != '\0') {
            return false; // Path blocked
        }
        currentRow += rowStep;
        currentCol += colStep;
    }
    return true;
}



bool isvalidmove(cell* board, int MAX_NODE, int fromrow, int fromcol, int torow, int tocol) {
    int fromIndex = fromrow * MAX_NODE + fromcol;
    int toIndex = torow * MAX_NODE + tocol;

    cell fromCell = board[fromIndex];
    cell toCell = board[toIndex];

    if (toCell.pieceType != '\0' && toCell.color == fromCell.color) {
      return false;
    }

    char piece = fromCell.pieceType;

    int rowDiff = abs(torow - fromrow);
    int colDiff = abs(tocol - fromcol);

    switch (piece) {
      case 'N': {
          // Knight moves same as before
          if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)) {
              return true;
          }
          return false;
      }
      case 'P': {
          // Pawn logic (unchanged)
          int direction = (fromCell.color == 'W') ? -1 : 1;
          int startRow = (fromCell.color == 'W') ? 7 : 2;
          int rowDiff = torow - fromrow;
          int colDiff = tocol - fromcol;

          if (colDiff == 0 && rowDiff == direction) {
             if (toCell.pieceType == '\0') return true;
             return false;
          }
          if (colDiff == 0 && rowDiff == 2 * direction && fromrow == startRow) {
             int intermediateIndex = (fromrow + direction) * MAX_NODE + fromcol;
             if (board[intermediateIndex].pieceType == '\0' && toCell.pieceType == '\0') return true;
             return false;
          }
          if (abs(colDiff) == 1 && rowDiff == direction) {
             if (toCell.pieceType != '\0' && toCell.color != fromCell.color) return true;
             return false;
          }
          return false;
      }
      case 'R': {
          // Rook moves horizontally or vertically
          if (fromrow == torow || fromcol == tocol) {
              if (isPathClear(board, MAX_NODE, fromrow, fromcol, torow, tocol)) {
                  return true;
              }
          }
          return false;
      }
      case 'B': {
          // Bishop moves diagonally
          if (rowDiff == colDiff) {
              if (isPathClear(board, MAX_NODE, fromrow, fromcol, torow, tocol)) {
                  return true;
              }
          }
          return false;
      }
      case 'Q': {
          // Queen moves like rook or bishop
          if (fromrow == torow || fromcol == tocol || rowDiff == colDiff) {
              if (isPathClear(board, MAX_NODE, fromrow, fromcol, torow, tocol)) {
                  return true;
              }
          }
          return false;
      }
      case 'K': {
          // King moves one square any direction
          if ((rowDiff <= 1) && (colDiff <= 1)) {
              return true;
          }
          return false;
      }
      default:
          return false;
    }
}


void selectpiece(cell* board, char rowLetter, char colDigit, int MAX_NODE) {
    // Convert row letter (e.g., 'A') to index (1-8)
    if (rowLetter < 'A' || rowLetter > 'H' || colDigit < '1' || colDigit > '8') {
        printf("Invalid input: Use rows A-H and columns 1-8.\n");
        return;
    }

    int row = rowLetter - 'A' + 1;  // Row 1 to 8
    int col = colDigit - '0';      // Col 1 to 8

    int index = row * MAX_NODE + col;
    cell c = board[index];

    printf("Selected cell at %c%c: ", rowLetter, colDigit);
        if (c.pieceType == '\0') {
                printf(" -- ");
                return;
            } else {
                printf(" %c%c ", c.color, c.pieceType);
                return;
            }
   

}

void printBoard(cell* board, int MAX_NODE){
   for (int i = 0; i < MAX_NODE; i++) {
         for (int j = 0; j < MAX_NODE; j++) {
               cell c = board[i * MAX_NODE + j];

               if (c.pieceType == '\0') {
                  printf(" -- ");
               } else {
                  printf(" %c%c ", c.color, c.pieceType);
               }
         }
         printf("\n");
   }
}
void movepiecehelper(cell* board, int fromIndex, int toIndex, char rowLetter, char colDigit, int MAX_NODE) { //actually moves the piece
    board[toIndex] = board[fromIndex];
    board[fromIndex].pieceType = '\0';
    board[fromIndex].color = '\0';

   printf("Moved piece to %c%c: %c%c\n", rowLetter, colDigit, board[toIndex].color, board[toIndex].pieceType);

}

bool movepiece(cell* board, char fromRow, char fromCol, char rowLetter, char colDigit, int MAX_NODE) {

   if (fromRow < 'A' || fromRow > 'H' || fromCol < '1' || fromCol > '8' ||
      rowLetter < 'A' || rowLetter > 'H' || colDigit < '1' || colDigit > '8') {
      printf("Invalid input: Use rows A-H and columns 1-8.\n");
      return false;
   }
   int trow = rowLetter - 'A' + 1;  // Row 1 to 8
   int tcol = colDigit - '0';      // Col 1 to 8

   int row = fromRow - 'A' + 1;  // Row 1 to 8
   int col = fromCol - '0';      // Col 1 to 8



   int toIndex = trow * MAX_NODE + tcol;
   int fromIndex = row * MAX_NODE + col;
   //check if there is a piece to move to
   if (board[fromIndex].pieceType == '\0') {
      printf("No piece at the selected position %c%c to move.\n", fromRow, fromCol);
      return false;
   }

   if (!isvalidmove(board, MAX_NODE, row, col, trow, tcol)) {
      printf("Invalid move for piece %c%c from %c%c to %c%c.\n",
             board[fromIndex].color, board[fromIndex].pieceType, 
             fromRow, fromCol, rowLetter, colDigit);
      return false;
   }
   movepiecehelper(board, fromIndex, toIndex, rowLetter, colDigit, MAX_NODE);
   return true;

   

}

void toggle_state(bool *state) {
    *state = !(*state); 
}




void main (void)
 {
    bool whiteturn = true;
    bool viablemove = false;
    int MAX_NODE = 9;
    cell* board = malloc(MAX_NODE * MAX_NODE * sizeof(cell));
    //set board
    for (int i = 0; i < MAX_NODE; i++)
    {
      for (int j = 0; j < MAX_NODE; j++){
         int index = i * MAX_NODE + j;
         if (i == 0 && j > 0){
            board[index].pieceType = '0' + j;
            board[index].color = ' '; 
         }else if (j == 0 && i > 0){
            board[index].pieceType = 'A' + i - 1;
            board[index].color = ' '; 
         }else{
            board[index].pieceType = '\0';
            board[index].color = '\0';
         }
      }
    }

   //set pieces on board
   //black pieces
    int x = 1;
      //rook
      
      board[x * MAX_NODE + 1].pieceType = 'R';
      board[x * MAX_NODE + 1].color = 'B'; 
      //knight
      board[x * MAX_NODE + 2].pieceType = 'N';
      board[x * MAX_NODE + 2].color = 'B'; 
      //bishop
      board[x * MAX_NODE + 3].pieceType = 'B';
      board[x * MAX_NODE + 3].color = 'B'; 
      //queen
      board[x * MAX_NODE + 4].pieceType = 'Q';
      board[x * MAX_NODE + 4].color = 'B'; 
      //king
      board[x * MAX_NODE + 5].pieceType = 'K';
      board[x * MAX_NODE + 5].color = 'B'; 
      //bishop
      board[x * MAX_NODE + 6].pieceType = 'B';
      board[x * MAX_NODE + 6].color = 'B';
      //knight
      board[x * MAX_NODE + 7].pieceType = 'N';
      board[x * MAX_NODE + 7].color = 'B';  
      //rook
      board[x * MAX_NODE + 8].pieceType = 'R';
      board[x * MAX_NODE + 8].color = 'B'; 
      
      //pawns
      x = 2;
      for (int i = 1; i < MAX_NODE; i++){
         board[x * MAX_NODE + i].pieceType = 'P';
         board[x * MAX_NODE + i].color = 'B'; 
      }

   //white pieces
     x = 8;
      
      //rook
      board[x * MAX_NODE + 1].pieceType = 'R';
      board[x * MAX_NODE + 1].color = 'W'; 
      //knight
      board[x * MAX_NODE + 2].pieceType = 'N';
      board[x * MAX_NODE + 2].color = 'W'; 
      //bishop
      board[x * MAX_NODE + 3].pieceType = 'B';
      board[x * MAX_NODE + 3].color = 'W'; 
      //queen
      board[x * MAX_NODE + 4].pieceType = 'Q';
      board[x * MAX_NODE + 4].color = 'W'; 
      //king
      board[x * MAX_NODE + 5].pieceType = 'K';
      board[x * MAX_NODE + 5].color = 'W'; 
      //bishop
      board[x * MAX_NODE + 6].pieceType = 'B';
      board[x * MAX_NODE + 6].color = 'W';
      //knight
      board[x * MAX_NODE + 7].pieceType = 'N';
      board[x * MAX_NODE + 7].color = 'W';  
      //rook
      board[x * MAX_NODE + 8].pieceType = 'R';
      board[x * MAX_NODE + 8].color = 'W'; 
      
      //pawns
      x = 7;
      for (int i = 1; i < MAX_NODE; i++){
         board[x * MAX_NODE + i].pieceType = 'P';
         board[x * MAX_NODE + i].color = 'W'; 
      }

   printBoard(board, MAX_NODE);
   for (int i=1; i < 15; i++){

    if (whiteturn == true){
        printf("White's Turn\n");
    }else{
        printf("Black's Turn\n");
    }

      //selecting a piece
      char input[5];
      char rowInput;
      char colInput;
      char torowInput;
      char tocolInput;

     //loop until current player has made a viable move
     
      while (viablemove == false){
        printf("Select a piece on the board. Then select where to move it. (Use Position: A1, B3, etc..):\n");
        scanf(" %4s", input); 
        rowInput = input[0];
        colInput = input[1];
        torowInput = input[2];
        tocolInput = input[3];
        viablemove = movepiece(board, rowInput, colInput, torowInput, tocolInput, MAX_NODE);
      }
      viablemove = false;
      toggle_state(&whiteturn);
      
      printBoard(board, MAX_NODE);
   }
  




    free(board);
    return;
 }