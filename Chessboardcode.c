#include<stdio.h>
#include <stdbool.h>
#include <stdlib.h> // Required for malloc and free
#include <string.h> //for unicode version printboard function uses strcpy - tyler
#include <time.h> //for AI random move - tyler

typedef struct{
   char pieceType;
   char color;
   bool isonboard;
}cell;

char whiteCaptured[32] = ""; //added array to store captured white/black pieces
char blackCaptured[32] = "";

//checks if a King is captured and ends game
bool checkWin(cell* board, int MAX_NODE) {
    bool whiteKing = false;
    bool blackKing = false;
    for (int i = 0; i < MAX_NODE; i++) {
        for (int j = 1; j < MAX_NODE; j++) {
            cell c = board[i * MAX_NODE + j];
            if (c.pieceType == 'K' && c.color == 'W') whiteKing = true;
            if (c.pieceType == 'K' && c.color == 'B') blackKing = true;
        }
    }
    if (!whiteKing) {
        system("cls"); //clear console
        printf("WINNER WINNER CHICKEN DINNER! Black wins!\n");
        return true;
    }
    if (!blackKing) {
        system("cls");
        printf("WINNER WINNER CHICKEN DINNER! White wins!\n");
        return true;
    }
    return false;
}

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
          if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)) {
              return true;
          }
          return false;
      }
      case 'P': {
          int direction = (fromCell.color == 'W') ? -1 : 1;
          int startRow = (fromCell.color == 'W') ? 6 : 1;
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
          if (fromrow == torow || fromcol == tocol) {
              if (isPathClear(board, MAX_NODE, fromrow, fromcol, torow, tocol)) {
                  return true;
              }
          }
          return false;
      }
      case 'B': {
          if (rowDiff == colDiff) {
              if (isPathClear(board, MAX_NODE, fromrow, fromcol, torow, tocol)) {
                  return true;
              }
          }
          return false;
      }
      case 'Q': {
          if (fromrow == torow || fromcol == tocol || rowDiff == colDiff) {
              if (isPathClear(board, MAX_NODE, fromrow, fromcol, torow, tocol)) {
                  return true;
              }
          }
          return false;
      }
      case 'K': {
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
    if (rowLetter < 'A' || rowLetter > 'H' || colDigit < '1' || colDigit > '8') {
        printf("Invalid input: Use rows A-H and columns 1-8.\n");
        return;
    }

    int col = rowLetter - 'A' + 1; // Col index 1-8 - updated
    int row = 8 - (colDigit - '0'); // Row index 0-7 (top-left is 0,0) - updated

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
   printf("      A   B   C   D   E   F   G   H\n");
    printf("    +---+---+---+---+---+---+---+---+\n");

    for (int i = 0; i < 8; i++) {
        printf("  %d |", 8 - i);

        for (int j = 1; j <= 8; j++) {
            int index = i * MAX_NODE + j;

            char symbol[4] = " ";  

            if (board[index].pieceType != '\0') {
                char p = board[index].pieceType;
                char c = board[index].color;

                const char* whitePieces[] = { "♙", "♖", "♘", "♗", "♕", "♔" };
                const char* blackPieces[] = { "♟", "♜", "♞", "♝", "♛", "♚" };

                switch (p) {
                    case 'P': strcpy(symbol, c == 'W' ? whitePieces[0] : blackPieces[0]); break;
                    case 'R': strcpy(symbol, c == 'W' ? whitePieces[1] : blackPieces[1]); break;
                    case 'N': strcpy(symbol, c == 'W' ? whitePieces[2] : blackPieces[2]); break;
                    case 'B': strcpy(symbol, c == 'W' ? whitePieces[3] : blackPieces[3]); break;
                    case 'Q': strcpy(symbol, c == 'W' ? whitePieces[4] : blackPieces[4]); break;
                    case 'K': strcpy(symbol, c == 'W' ? whitePieces[5] : blackPieces[5]); break;
                    default: strcpy(symbol, " ");
                }
            }

            printf(" %s |", symbol);
        }

        printf(" %d\n", 8 - i);
        printf("    +---+---+---+---+---+---+---+---+\n");
    }

    printf("      A   B   C   D   E   F   G   H\n");
    //added captured pieces display at bottom
    printf("\nWhite Captured: %s\n", whiteCaptured);
    printf("Black Captured: %s\n", blackCaptured);
}

bool movepiece(cell* board, char fromRow, char fromCol, char rowLetter, char colDigit, int MAX_NODE, char currentTurn) {
   if (fromRow < 'A' || fromRow > 'H' || fromCol < '1' || fromCol > '8' ||
      rowLetter < 'A' || rowLetter > 'H' || colDigit < '1' || colDigit > '8') {
      printf("Invalid input: Use rows A-H and columns 1-8.\n");
      return false;
   }

   int tcol = rowLetter - 'A' + 1; // Col index 1-8
   int trow = 8 - (colDigit - '0'); // Row index 0-7
   int col = fromRow - 'A' + 1; // Col index 1-8
   int row = 8 - (fromCol - '0'); // Row index 0-7

   int toIndex = trow * MAX_NODE + tcol;
   int fromIndex = row * MAX_NODE + col;
   
   if (board[fromIndex].pieceType == '\0') {
      printf("No piece at the selected position %c%c to move.\n", fromRow, fromCol);
      return false;
   }

   if (board[fromIndex].color != currentTurn) {
      printf("It's %s's turn! You can't move %s pieces.\n", 
             currentTurn == 'W' ? "White" : "Black",
             currentTurn == 'W' ? "Black" : "White");
      return false;
   }

   if (!isvalidmove(board, MAX_NODE, row, col, trow, tcol)) {
      printf("Invalid move for piece %c%c from %c%c to %c%c.\n",
             board[fromIndex].color, board[fromIndex].pieceType, 
             fromRow, fromCol, rowLetter, colDigit);
      return false;
   }

   //added captured piece tracking
   if (board[toIndex].pieceType != '\0') {
       char captured = board[toIndex].pieceType;
       if (board[toIndex].color == 'W') {
           strncat(whiteCaptured, &captured, 1);
       } else {
           strncat(blackCaptured, &captured, 1);
       }
   }

   board[toIndex] = board[fromIndex];
   board[fromIndex].pieceType = '\0';
   board[fromIndex].color = '\0';

   printf("Moved piece to %c%c: %c%c\n", rowLetter, colDigit, board[toIndex].color, board[toIndex].pieceType);
   return true;
}

//added function for Black random move
void blackAI(cell* board, int MAX_NODE) {
    int fromRow, fromCol, toRow, toCol;
    bool moved = false;

    while (!moved) {
        fromRow = rand() % 8; 
        fromCol = 1 + rand() % 8; 
        if (board[fromRow * MAX_NODE + fromCol].pieceType != '\0' && board[fromRow * MAX_NODE + fromCol].color == 'B') {
            toRow = rand() % 8;
            toCol = 1 + rand() % 8;
            if (isvalidmove(board, MAX_NODE, fromRow, fromCol, toRow, toCol)) {
                char fR = 'A' + (fromCol - 1);
                char fC = '8' - fromRow;
                char tR = 'A' + (toCol - 1);
                char tC = '8' - toRow;
                movepiece(board, fR, fC, tR, tC, MAX_NODE, 'B');
                moved = true;
            }
        }
    }
}

int main (void)
{
    #ifdef _WIN32
    system("chcp 65001"); //use UTF-8
#endif

    srand(time(NULL)); //initialize random seed for AI

    int MAX_NODE = 9;
    cell* board = malloc(MAX_NODE * MAX_NODE * sizeof(cell));
    char currentTurn = 'W'; // White goes first
    
    for (int i = 0; i < MAX_NODE; i++){
    for (int j = 0; j < MAX_NODE; j++){
        int index = i * MAX_NODE + j;
        board[index].pieceType = '\0';
        board[index].color = '\0';
    }
}

   //set pieces on board
   int x = 0; // black back rank
   board[x * MAX_NODE + 1].pieceType = 'R'; board[x * MAX_NODE + 1].color = 'B'; 
   board[x * MAX_NODE + 2].pieceType = 'N'; board[x * MAX_NODE + 2].color = 'B'; 
   board[x * MAX_NODE + 3].pieceType = 'B'; board[x * MAX_NODE + 3].color = 'B'; 
   board[x * MAX_NODE + 4].pieceType = 'Q'; board[x * MAX_NODE + 4].color = 'B'; 
   board[x * MAX_NODE + 5].pieceType = 'K'; board[x * MAX_NODE + 5].color = 'B'; 
   board[x * MAX_NODE + 6].pieceType = 'B'; board[x * MAX_NODE + 6].color = 'B';
   board[x * MAX_NODE + 7].pieceType = 'N'; board[x * MAX_NODE + 7].color = 'B';  
   board[x * MAX_NODE + 8].pieceType = 'R'; board[x * MAX_NODE + 8].color = 'B'; 
      
   x = 1; // black pawns
   for (int i = 1; i < MAX_NODE; i++){
         board[x * MAX_NODE + i].pieceType = 'P';
         board[x * MAX_NODE + i].color = 'B'; 
   }

   x = 6; // white pawns
   for (int i = 1; i < MAX_NODE; i++){
         board[x * MAX_NODE + i].pieceType = 'P';
         board[x * MAX_NODE + i].color = 'W'; 
   }

   x = 7; // white back rank
   board[x * MAX_NODE + 1].pieceType = 'R'; board[x * MAX_NODE + 1].color = 'W'; 
   board[x * MAX_NODE + 2].pieceType = 'N'; board[x * MAX_NODE + 2].color = 'W'; 
   board[x * MAX_NODE + 3].pieceType = 'B'; board[x * MAX_NODE + 3].color = 'W'; 
   board[x * MAX_NODE + 4].pieceType = 'Q'; board[x * MAX_NODE + 4].color = 'W'; 
   board[x * MAX_NODE + 5].pieceType = 'K'; board[x * MAX_NODE + 5].color = 'W'; 
   board[x * MAX_NODE + 6].pieceType = 'B'; board[x * MAX_NODE + 6].color = 'W';
   board[x * MAX_NODE + 7].pieceType = 'N'; board[x * MAX_NODE + 7].color = 'W';  
   board[x * MAX_NODE + 8].pieceType = 'R'; board[x * MAX_NODE + 8].color = 'W'; 

   printBoard(board, MAX_NODE);
   
   while (1) {
      if (checkWin(board, MAX_NODE)) break; //added win check each turn

      if (currentTurn == 'W') {
          char input[5];
      
          printf("\n--- White's Turn ---\n");
          printf("Select a piece and move it (e.g., G2G4): ");
          scanf(" %4s", input); 
      
          char rowInput = input[0];
          char colInput = input[1];
          char torowInput = input[2];
          char tocolInput = input[3];
      
          bool moveSuccess = movepiece(board, rowInput, colInput, torowInput, tocolInput, MAX_NODE, currentTurn);
      
          if (moveSuccess) {
              currentTurn = 'B'; //switch to AI
          }
      } else {
          printf("\n--- Black's Turn (AI) ---\n");
          blackAI(board, MAX_NODE); //AI makes a random valid move
          currentTurn = 'W'; //switch back to player
      }
      
      printBoard(board, MAX_NODE);
   }

    free(board);
    //wait for user input before exit updated
    while (getchar() != '\n'); // clear input buffer
    getchar(); // wait for Enter
    return 0;
}
