#include <string.h>
#include <tonc.h>
#include "tileset.h"

#define TRANSPARENT 0
#define CURSOR 1
#define WHITE 2
#define BLACK 3
#define DOT 4
#define BOTTOM_EDGE 5
#define RIGHT_EDGE 6
#define BOTTOM_RIGHT_CORNER 7
#define OUTSIDE 8
#define NUMBER(n) ((n) + 8)

void set_tile(int charblock, int r, int c, int tile) {
  se_mem[charblock][r * 2 * 32 +      c * 2    ] = tile * 4    ;
  se_mem[charblock][r * 2 * 32 +      c * 2 + 1] = tile * 4 + 1;
  se_mem[charblock][r * 2 * 32 + 32 + c * 2    ] = tile * 4 + 2;
  se_mem[charblock][r * 2 * 32 + 32 + c * 2 + 1] = tile * 4 + 3;
}



#define NUR_ROWS 10
#define NUR_COLS 10

// Returns true if all squares are filled in with something.
bool checkFull(int puzzle[NUR_ROWS][NUR_COLS]) {
  for (int r = 0; r < NUR_ROWS - 1; r++) {
    for (int c = 0; c < NUR_COLS - 1; c++) {
      if (puzzle[r][c] == WHITE) return false;
    }
  }
  return true;
}

// Returns true if there are no 2x2 pools of black squares.
bool checkPools(int puzzle[NUR_ROWS][NUR_COLS]) {
  for (int r = 0; r < NUR_ROWS - 1; r++) {
    for (int c = 0; c < NUR_COLS - 1; c++) {
      int r1 = r + 1;
      int c1 = c + 1;
      if ( puzzle[r ][c ] == BLACK
        && puzzle[r1][c ] == BLACK
        && puzzle[r ][c1] == BLACK
        && puzzle[r1][c1] == BLACK
      ) {
        return false;
      }
    }
  }
  return true;
}

// Returns true if all the black squares are connected.
bool checkBlack(int puzzle[NUR_ROWS][NUR_COLS]) {
  return false;
}

// Returns true if each number n connects to n-1 dots, and no other numbers.
bool checkNumbers(int puzzle[NUR_ROWS][NUR_COLS]) {
  return false;
}

int main() {

  int puzzle[NUR_ROWS][NUR_COLS];
  puzzle[0][0] = NUMBER(2);
puzzle[0][1] = WHITE;
puzzle[0][2] = NUMBER(4);
puzzle[0][3] = WHITE;
puzzle[0][4] = NUMBER(2);
puzzle[0][5] = WHITE;
puzzle[0][6] = WHITE;
puzzle[0][7] = NUMBER(2);
puzzle[0][8] = WHITE;
puzzle[0][9] = WHITE;
puzzle[1][0] = WHITE;
puzzle[1][1] = WHITE;
puzzle[1][2] = WHITE;
puzzle[1][3] = WHITE;
puzzle[1][4] = WHITE;
puzzle[1][5] = WHITE;
puzzle[1][6] = WHITE;
puzzle[1][7] = WHITE;
puzzle[1][8] = WHITE;
puzzle[1][9] = WHITE;
puzzle[2][0] = WHITE;
puzzle[2][1] = WHITE;
puzzle[2][2] = WHITE;
puzzle[2][3] = WHITE;
puzzle[2][4] = WHITE;
puzzle[2][5] = NUMBER(2);
puzzle[2][6] = WHITE;
puzzle[2][7] = WHITE;
puzzle[2][8] = WHITE;
puzzle[2][9] = WHITE;
puzzle[3][0] = NUMBER(3);
puzzle[3][1] = WHITE;
puzzle[3][2] = WHITE;
puzzle[3][3] = WHITE;
puzzle[3][4] = WHITE;
puzzle[3][5] = WHITE;
puzzle[3][6] = WHITE;
puzzle[3][7] = WHITE;
puzzle[3][8] = NUMBER(3);
puzzle[3][9] = WHITE;
puzzle[4][0] = WHITE;
puzzle[4][1] = WHITE;
puzzle[4][2] = WHITE;
puzzle[4][3] = NUMBER(2);
puzzle[4][4] = WHITE;
puzzle[4][5] = WHITE;
puzzle[4][6] = NUMBER(3);
puzzle[4][7] = WHITE;
puzzle[4][8] = WHITE;
puzzle[4][9] = NUMBER(3);
puzzle[5][0] = WHITE;
puzzle[5][1] = WHITE;
puzzle[5][2] = WHITE;
puzzle[5][3] = WHITE;
puzzle[5][4] = WHITE;
puzzle[5][5] = WHITE;
puzzle[5][6] = WHITE;
puzzle[5][7] = WHITE;
puzzle[5][8] = WHITE;
puzzle[5][9] = WHITE;
puzzle[6][0] = NUMBER(3);
puzzle[6][1] = WHITE;
puzzle[6][2] = WHITE;
puzzle[6][3] = WHITE;
puzzle[6][4] = WHITE;
puzzle[6][5] = WHITE;
puzzle[6][6] = NUMBER(1);
puzzle[6][7] = WHITE;
puzzle[6][8] = WHITE;
puzzle[6][9] = WHITE;
puzzle[7][0] = WHITE;
puzzle[7][1] = WHITE;
puzzle[7][2] = WHITE;
puzzle[7][3] = NUMBER(3);
puzzle[7][4] = WHITE;
puzzle[7][5] = WHITE;
puzzle[7][6] = WHITE;
puzzle[7][7] = WHITE;
puzzle[7][8] = WHITE;
puzzle[7][9] = WHITE;
puzzle[8][0] = NUMBER(2);
puzzle[8][1] = WHITE;
puzzle[8][2] = WHITE;
puzzle[8][3] = WHITE;
puzzle[8][4] = WHITE;
puzzle[8][5] = WHITE;
puzzle[8][6] = NUMBER(3);
puzzle[8][7] = WHITE;
puzzle[8][8] = WHITE;
puzzle[8][9] = NUMBER(2);
puzzle[9][0] = WHITE;
puzzle[9][1] = WHITE;
puzzle[9][2] = WHITE;
puzzle[9][3] = NUMBER(2);
puzzle[9][4] = WHITE;
puzzle[9][5] = WHITE;
puzzle[9][6] = WHITE;
puzzle[9][7] = WHITE;
puzzle[9][8] = WHITE;
puzzle[9][9] = WHITE;

  int cursor_r = 0;
  int cursor_c = 0;

  // Load palette
  memcpy(pal_bg_mem, tilesetPal, tilesetPalLen);

  // Load tiles into CBB 0
  memcpy(tile_mem, tilesetTiles, tilesetTilesLen);

  // Load map into SBB 30
  for (int r = 0; r < 32; r++) {
    for (int c = 0; c < 32; c++) {
      set_tile(30, r, c, OUTSIDE);
    }
  }
  for (int c = 0; c < NUR_COLS; c++) set_tile(30, NUR_ROWS, c, BOTTOM_EDGE);
  for (int r = 0; r < NUR_ROWS; r++) set_tile(30, r, NUR_COLS, RIGHT_EDGE);
  set_tile(30, NUR_ROWS, NUR_COLS, BOTTOM_RIGHT_CORNER);
  for (int r = 0; r < NUR_ROWS; r++) {
    for (int c = 0; c < NUR_COLS; c++) {
      set_tile(30, r, c, puzzle[r][c]);
    }
  }

  // Load map into SBB 31
  for (int r = 0; r < 32; r++) {
    for (int c = 0; c < 32; c++) {
      set_tile(31, r, c, TRANSPARENT);
    }
  }
  set_tile(31, cursor_r, cursor_c, CURSOR);

  // set up BG0 for a 4bpp 64x32t map, using charblock 0 and screenblock 31 (cursor)
  REG_BG0CNT = BG_CBB(0) | BG_SBB(31) | BG_4BPP | BG_REG_64x32;
  // set up BG1 for a 4bpp 64x32t map, using charblock 0 and screenblock 30 (puzzle squares)
  REG_BG1CNT = BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;
  REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;

  irq_init(NULL);
  irq_add(II_VBLANK, NULL);

  int key_repeat = 0;
  bool clearing = false;
  while (1) {
    VBlankIntrWait();
    key_poll();
    set_tile(31, cursor_r, cursor_c, TRANSPARENT); // remove the cursor
    if (key_hit(1 << KI_LEFT | 1 << KI_RIGHT | 1 << KI_UP | 1 << KI_DOWN)) {
      key_repeat = 0; // reset the key repeat timer
    }
#define START_REPEAT 20
#define REPEAT_SPEED 2
    if (key_is_down(1 << KI_LEFT | 1 << KI_RIGHT | 1 << KI_UP | 1 << KI_DOWN)) {
      if (key_repeat < START_REPEAT) key_repeat++;
      else key_repeat = START_REPEAT - REPEAT_SPEED;
    }
    bool virtual_left  = key_hit(1 << KI_LEFT ) || (key_is_down(1 << KI_LEFT ) && key_repeat == START_REPEAT);
    bool virtual_right = key_hit(1 << KI_RIGHT) || (key_is_down(1 << KI_RIGHT) && key_repeat == START_REPEAT);
    bool virtual_up    = key_hit(1 << KI_UP   ) || (key_is_down(1 << KI_UP   ) && key_repeat == START_REPEAT);
    bool virtual_down  = key_hit(1 << KI_DOWN ) || (key_is_down(1 << KI_DOWN ) && key_repeat == START_REPEAT);
    bool moved_cursor = false;
    if (virtual_left  && cursor_c > 0           ) { cursor_c--; moved_cursor = true; }
    if (virtual_right && cursor_c < NUR_COLS - 1) { cursor_c++; moved_cursor = true; }
    if (virtual_up    && cursor_r > 0           ) { cursor_r--; moved_cursor = true; }
    if (virtual_down  && cursor_r < NUR_ROWS - 1) { cursor_r++; moved_cursor = true; }

    if (key_hit(1 << KI_A)) {
      switch (puzzle[cursor_r][cursor_c]) {
        case WHITE:
        case BLACK:
          puzzle[cursor_r][cursor_c] = DOT;
          set_tile(30, cursor_r, cursor_c, DOT);
          clearing = false;
          break;
        case DOT:
          puzzle[cursor_r][cursor_c] = WHITE;
          set_tile(30, cursor_r, cursor_c, WHITE);
          clearing = true;
          break;
        default:
          clearing = false;
          break;
      }
    }
    else if (key_is_down(1 << KI_A) && moved_cursor) {
      switch (puzzle[cursor_r][cursor_c]) {
        case WHITE:
        case BLACK:
        case DOT:
          if (clearing) {
            puzzle[cursor_r][cursor_c] = WHITE;
            set_tile(30, cursor_r, cursor_c, WHITE);
          }
          else {
            puzzle[cursor_r][cursor_c] = DOT;
            set_tile(30, cursor_r, cursor_c, DOT);
          }
          break;
      }
    }

    if (key_hit(1 << KI_B)) {
      switch (puzzle[cursor_r][cursor_c]) {
        case WHITE:
        case DOT:
          puzzle[cursor_r][cursor_c] = BLACK;
          set_tile(30, cursor_r, cursor_c, BLACK);
          clearing = false;
          break;
        case BLACK:
          puzzle[cursor_r][cursor_c] = WHITE;
          set_tile(30, cursor_r, cursor_c, WHITE);
          clearing = true;
          break;
        default:
          clearing = false;
          break;
      }
    }
    else if (key_is_down(1 << KI_B) && moved_cursor) {
      switch (puzzle[cursor_r][cursor_c]) {
        case WHITE:
        case BLACK:
        case DOT:
          if (clearing) {
            puzzle[cursor_r][cursor_c] = WHITE;
            set_tile(30, cursor_r, cursor_c, WHITE);
          }
          else {
            puzzle[cursor_r][cursor_c] = BLACK;
            set_tile(30, cursor_r, cursor_c, BLACK);
          }
          break;
      }
    }

    set_tile(31, cursor_r, cursor_c, CURSOR); // readd the cursor
  }
}
