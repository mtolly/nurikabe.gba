#include <string.h>
#include <tonc.h>

#define WHITE 0
#define BLACK 1
#define DOT 2
#define OUTSIDE 3
#define BOTTOM_EDGE 4
#define RIGHT_EDGE 5
#define BOTTOM_RIGHT_CORNER 6
#define TRANSPARENT 7
#define CURSOR 8
#define NUMBER(n) ((n) + 8)

int main() {

  

#define NUR_ROWS 10
#define NUR_COLS 10

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
  pal_bg_mem[0] = CLR_MAG;
pal_bg_mem[1] = CLR_BLACK;
pal_bg_mem[2] = CLR_WHITE;
pal_bg_mem[3] = RGB15(7,10,20);
pal_bg_mem[4] = RGB15(8,8,8);
pal_bg_mem[5] = CLR_RED;

  // Load tiles into CBB 0
  tile_mem[0][WHITE].data[0] = 0x11111111;
tile_mem[0][WHITE].data[1] = 0x22222221;
tile_mem[0][WHITE].data[2] = 0x22222221;
tile_mem[0][WHITE].data[3] = 0x22222221;
tile_mem[0][WHITE].data[4] = 0x22222221;
tile_mem[0][WHITE].data[5] = 0x22222221;
tile_mem[0][WHITE].data[6] = 0x22222221;
tile_mem[0][WHITE].data[7] = 0x22222221;
  tile_mem[0][BLACK].data[0] = 0x11111111;
tile_mem[0][BLACK].data[1] = 0x44444441;
tile_mem[0][BLACK].data[2] = 0x44444441;
tile_mem[0][BLACK].data[3] = 0x44444441;
tile_mem[0][BLACK].data[4] = 0x44444441;
tile_mem[0][BLACK].data[5] = 0x44444441;
tile_mem[0][BLACK].data[6] = 0x44444441;
tile_mem[0][BLACK].data[7] = 0x44444441;
  tile_mem[0][DOT].data[0] = 0x11111111;
tile_mem[0][DOT].data[1] = 0x22222221;
tile_mem[0][DOT].data[2] = 0x22222221;
tile_mem[0][DOT].data[3] = 0x22242221;
tile_mem[0][DOT].data[4] = 0x22414221;
tile_mem[0][DOT].data[5] = 0x22242221;
tile_mem[0][DOT].data[6] = 0x22222221;
tile_mem[0][DOT].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(1)].data[0] = 0x11111111;
tile_mem[0][NUMBER(1)].data[1] = 0x22222221;
tile_mem[0][NUMBER(1)].data[2] = 0x22212221;
tile_mem[0][NUMBER(1)].data[3] = 0x22212221;
tile_mem[0][NUMBER(1)].data[4] = 0x22212221;
tile_mem[0][NUMBER(1)].data[5] = 0x22212221;
tile_mem[0][NUMBER(1)].data[6] = 0x22212221;
tile_mem[0][NUMBER(1)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(2)].data[0] = 0x11111111;
tile_mem[0][NUMBER(2)].data[1] = 0x22222221;
tile_mem[0][NUMBER(2)].data[2] = 0x21111121;
tile_mem[0][NUMBER(2)].data[3] = 0x21222221;
tile_mem[0][NUMBER(2)].data[4] = 0x21111121;
tile_mem[0][NUMBER(2)].data[5] = 0x22222121;
tile_mem[0][NUMBER(2)].data[6] = 0x21111121;
tile_mem[0][NUMBER(2)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(3)].data[0] = 0x11111111;
tile_mem[0][NUMBER(3)].data[1] = 0x22222221;
tile_mem[0][NUMBER(3)].data[2] = 0x21111121;
tile_mem[0][NUMBER(3)].data[3] = 0x21222221;
tile_mem[0][NUMBER(3)].data[4] = 0x21111121;
tile_mem[0][NUMBER(3)].data[5] = 0x21222221;
tile_mem[0][NUMBER(3)].data[6] = 0x21111121;
tile_mem[0][NUMBER(3)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(4)].data[0] = 0x11111111;
tile_mem[0][NUMBER(4)].data[1] = 0x22222221;
tile_mem[0][NUMBER(4)].data[2] = 0x21222121;
tile_mem[0][NUMBER(4)].data[3] = 0x21222121;
tile_mem[0][NUMBER(4)].data[4] = 0x21111121;
tile_mem[0][NUMBER(4)].data[5] = 0x21222221;
tile_mem[0][NUMBER(4)].data[6] = 0x21222221;
tile_mem[0][NUMBER(4)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(5)].data[0] = 0x11111111;
tile_mem[0][NUMBER(5)].data[1] = 0x22222221;
tile_mem[0][NUMBER(5)].data[2] = 0x21111121;
tile_mem[0][NUMBER(5)].data[3] = 0x22222121;
tile_mem[0][NUMBER(5)].data[4] = 0x21111121;
tile_mem[0][NUMBER(5)].data[5] = 0x21222221;
tile_mem[0][NUMBER(5)].data[6] = 0x21111121;
tile_mem[0][NUMBER(5)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(6)].data[0] = 0x11111111;
tile_mem[0][NUMBER(6)].data[1] = 0x22222221;
tile_mem[0][NUMBER(6)].data[2] = 0x21111121;
tile_mem[0][NUMBER(6)].data[3] = 0x22222121;
tile_mem[0][NUMBER(6)].data[4] = 0x21111121;
tile_mem[0][NUMBER(6)].data[5] = 0x21222121;
tile_mem[0][NUMBER(6)].data[6] = 0x21111121;
tile_mem[0][NUMBER(6)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(7)].data[0] = 0x11111111;
tile_mem[0][NUMBER(7)].data[1] = 0x22222221;
tile_mem[0][NUMBER(7)].data[2] = 0x21111121;
tile_mem[0][NUMBER(7)].data[3] = 0x21222221;
tile_mem[0][NUMBER(7)].data[4] = 0x21222221;
tile_mem[0][NUMBER(7)].data[5] = 0x21222221;
tile_mem[0][NUMBER(7)].data[6] = 0x21222221;
tile_mem[0][NUMBER(7)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(8)].data[0] = 0x11111111;
tile_mem[0][NUMBER(8)].data[1] = 0x22222221;
tile_mem[0][NUMBER(8)].data[2] = 0x21111121;
tile_mem[0][NUMBER(8)].data[3] = 0x21222121;
tile_mem[0][NUMBER(8)].data[4] = 0x21111121;
tile_mem[0][NUMBER(8)].data[5] = 0x21222121;
tile_mem[0][NUMBER(8)].data[6] = 0x21111121;
tile_mem[0][NUMBER(8)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(9)].data[0] = 0x11111111;
tile_mem[0][NUMBER(9)].data[1] = 0x22222221;
tile_mem[0][NUMBER(9)].data[2] = 0x21111121;
tile_mem[0][NUMBER(9)].data[3] = 0x21222121;
tile_mem[0][NUMBER(9)].data[4] = 0x21111121;
tile_mem[0][NUMBER(9)].data[5] = 0x21222221;
tile_mem[0][NUMBER(9)].data[6] = 0x21111121;
tile_mem[0][NUMBER(9)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(10)].data[0] = 0x11111111;
tile_mem[0][NUMBER(10)].data[1] = 0x22222221;
tile_mem[0][NUMBER(10)].data[2] = 0x21112121;
tile_mem[0][NUMBER(10)].data[3] = 0x21212121;
tile_mem[0][NUMBER(10)].data[4] = 0x21212121;
tile_mem[0][NUMBER(10)].data[5] = 0x21212121;
tile_mem[0][NUMBER(10)].data[6] = 0x21112121;
tile_mem[0][NUMBER(10)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(11)].data[0] = 0x11111111;
tile_mem[0][NUMBER(11)].data[1] = 0x22222221;
tile_mem[0][NUMBER(11)].data[2] = 0x22121221;
tile_mem[0][NUMBER(11)].data[3] = 0x22121221;
tile_mem[0][NUMBER(11)].data[4] = 0x22121221;
tile_mem[0][NUMBER(11)].data[5] = 0x22121221;
tile_mem[0][NUMBER(11)].data[6] = 0x22121221;
tile_mem[0][NUMBER(11)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(12)].data[0] = 0x11111111;
tile_mem[0][NUMBER(12)].data[1] = 0x22222221;
tile_mem[0][NUMBER(12)].data[2] = 0x21112121;
tile_mem[0][NUMBER(12)].data[3] = 0x21222121;
tile_mem[0][NUMBER(12)].data[4] = 0x21112121;
tile_mem[0][NUMBER(12)].data[5] = 0x22212121;
tile_mem[0][NUMBER(12)].data[6] = 0x21112121;
tile_mem[0][NUMBER(12)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(13)].data[0] = 0x11111111;
tile_mem[0][NUMBER(13)].data[1] = 0x22222221;
tile_mem[0][NUMBER(13)].data[2] = 0x21112121;
tile_mem[0][NUMBER(13)].data[3] = 0x21222121;
tile_mem[0][NUMBER(13)].data[4] = 0x21112121;
tile_mem[0][NUMBER(13)].data[5] = 0x21222121;
tile_mem[0][NUMBER(13)].data[6] = 0x21112121;
tile_mem[0][NUMBER(13)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(14)].data[0] = 0x11111111;
tile_mem[0][NUMBER(14)].data[1] = 0x22222221;
tile_mem[0][NUMBER(14)].data[2] = 0x21212121;
tile_mem[0][NUMBER(14)].data[3] = 0x21212121;
tile_mem[0][NUMBER(14)].data[4] = 0x21112121;
tile_mem[0][NUMBER(14)].data[5] = 0x21222121;
tile_mem[0][NUMBER(14)].data[6] = 0x21222121;
tile_mem[0][NUMBER(14)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(15)].data[0] = 0x11111111;
tile_mem[0][NUMBER(15)].data[1] = 0x22222221;
tile_mem[0][NUMBER(15)].data[2] = 0x21112121;
tile_mem[0][NUMBER(15)].data[3] = 0x22212121;
tile_mem[0][NUMBER(15)].data[4] = 0x21112121;
tile_mem[0][NUMBER(15)].data[5] = 0x21222121;
tile_mem[0][NUMBER(15)].data[6] = 0x21112121;
tile_mem[0][NUMBER(15)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(16)].data[0] = 0x11111111;
tile_mem[0][NUMBER(16)].data[1] = 0x22222221;
tile_mem[0][NUMBER(16)].data[2] = 0x21112121;
tile_mem[0][NUMBER(16)].data[3] = 0x22212121;
tile_mem[0][NUMBER(16)].data[4] = 0x21112121;
tile_mem[0][NUMBER(16)].data[5] = 0x21212121;
tile_mem[0][NUMBER(16)].data[6] = 0x21112121;
tile_mem[0][NUMBER(16)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(17)].data[0] = 0x11111111;
tile_mem[0][NUMBER(17)].data[1] = 0x22222221;
tile_mem[0][NUMBER(17)].data[2] = 0x21112121;
tile_mem[0][NUMBER(17)].data[3] = 0x21222121;
tile_mem[0][NUMBER(17)].data[4] = 0x21222121;
tile_mem[0][NUMBER(17)].data[5] = 0x21222121;
tile_mem[0][NUMBER(17)].data[6] = 0x21222121;
tile_mem[0][NUMBER(17)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(18)].data[0] = 0x11111111;
tile_mem[0][NUMBER(18)].data[1] = 0x22222221;
tile_mem[0][NUMBER(18)].data[2] = 0x21112121;
tile_mem[0][NUMBER(18)].data[3] = 0x21212121;
tile_mem[0][NUMBER(18)].data[4] = 0x21112121;
tile_mem[0][NUMBER(18)].data[5] = 0x21212121;
tile_mem[0][NUMBER(18)].data[6] = 0x21112121;
tile_mem[0][NUMBER(18)].data[7] = 0x22222221;
  
    tile_mem[0][NUMBER(19)].data[0] = 0x11111111;
tile_mem[0][NUMBER(19)].data[1] = 0x22222221;
tile_mem[0][NUMBER(19)].data[2] = 0x21112121;
tile_mem[0][NUMBER(19)].data[3] = 0x21212121;
tile_mem[0][NUMBER(19)].data[4] = 0x21112121;
tile_mem[0][NUMBER(19)].data[5] = 0x21222121;
tile_mem[0][NUMBER(19)].data[6] = 0x21112121;
tile_mem[0][NUMBER(19)].data[7] = 0x22222221;
  

  tile_mem[0][CURSOR].data[0] = 0x00000000;
tile_mem[0][CURSOR].data[1] = 0x55000550;
tile_mem[0][CURSOR].data[2] = 0x50000050;
tile_mem[0][CURSOR].data[3] = 0x00000000;
tile_mem[0][CURSOR].data[4] = 0x00000000;
tile_mem[0][CURSOR].data[5] = 0x00000000;
tile_mem[0][CURSOR].data[6] = 0x50000050;
tile_mem[0][CURSOR].data[7] = 0x55000550;
  tile_mem[0][TRANSPARENT].data[0] = 0x00000000;
tile_mem[0][TRANSPARENT].data[1] = 0x00000000;
tile_mem[0][TRANSPARENT].data[2] = 0x00000000;
tile_mem[0][TRANSPARENT].data[3] = 0x00000000;
tile_mem[0][TRANSPARENT].data[4] = 0x00000000;
tile_mem[0][TRANSPARENT].data[5] = 0x00000000;
tile_mem[0][TRANSPARENT].data[6] = 0x00000000;
tile_mem[0][TRANSPARENT].data[7] = 0x00000000;

  tile_mem[0][OUTSIDE].data[0] = 0x33333333;
tile_mem[0][OUTSIDE].data[1] = 0x33333333;
tile_mem[0][OUTSIDE].data[2] = 0x33333333;
tile_mem[0][OUTSIDE].data[3] = 0x33333333;
tile_mem[0][OUTSIDE].data[4] = 0x33333333;
tile_mem[0][OUTSIDE].data[5] = 0x33333333;
tile_mem[0][OUTSIDE].data[6] = 0x33333333;
tile_mem[0][OUTSIDE].data[7] = 0x33333333;
  tile_mem[0][BOTTOM_EDGE].data[0] = 0x11111111;
tile_mem[0][BOTTOM_EDGE].data[1] = 0x33333333;
tile_mem[0][BOTTOM_EDGE].data[2] = 0x33333333;
tile_mem[0][BOTTOM_EDGE].data[3] = 0x33333333;
tile_mem[0][BOTTOM_EDGE].data[4] = 0x33333333;
tile_mem[0][BOTTOM_EDGE].data[5] = 0x33333333;
tile_mem[0][BOTTOM_EDGE].data[6] = 0x33333333;
tile_mem[0][BOTTOM_EDGE].data[7] = 0x33333333;
  tile_mem[0][RIGHT_EDGE].data[0] = 0x33333331;
tile_mem[0][RIGHT_EDGE].data[1] = 0x33333331;
tile_mem[0][RIGHT_EDGE].data[2] = 0x33333331;
tile_mem[0][RIGHT_EDGE].data[3] = 0x33333331;
tile_mem[0][RIGHT_EDGE].data[4] = 0x33333331;
tile_mem[0][RIGHT_EDGE].data[5] = 0x33333331;
tile_mem[0][RIGHT_EDGE].data[6] = 0x33333331;
tile_mem[0][RIGHT_EDGE].data[7] = 0x33333331;
  tile_mem[0][BOTTOM_RIGHT_CORNER].data[0] = 0x33333331;
tile_mem[0][BOTTOM_RIGHT_CORNER].data[1] = 0x33333333;
tile_mem[0][BOTTOM_RIGHT_CORNER].data[2] = 0x33333333;
tile_mem[0][BOTTOM_RIGHT_CORNER].data[3] = 0x33333333;
tile_mem[0][BOTTOM_RIGHT_CORNER].data[4] = 0x33333333;
tile_mem[0][BOTTOM_RIGHT_CORNER].data[5] = 0x33333333;
tile_mem[0][BOTTOM_RIGHT_CORNER].data[6] = 0x33333333;
tile_mem[0][BOTTOM_RIGHT_CORNER].data[7] = 0x33333333;

  // Load map into SBB 30
  for (int i = 0; i < 32 * 32; i++) se_mem[30][i] = OUTSIDE;
  for (int c = 0; c < NUR_COLS; c++) se_mem[30][NUR_ROWS * 32 + c] = BOTTOM_EDGE;
  for (int r = 0; r < NUR_ROWS; r++) se_mem[30][r * 32 + NUR_COLS] = RIGHT_EDGE;
  se_mem[30][NUR_ROWS * 32 + NUR_COLS] = BOTTOM_RIGHT_CORNER;
  for (int r = 0; r < NUR_ROWS; r++) {
    for (int c = 0; c < NUR_COLS; c++) {
      se_mem[30][r * 32 + c] = puzzle[r][c];
    }
  }

  // Load map into SBB 31
  for (int i = 0; i < 32 * 32; i++) se_mem[31][i] = TRANSPARENT;
  se_mem[31][cursor_r * 32 + cursor_c] = CURSOR;

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
    se_mem[31][cursor_r * 32 + cursor_c] = TRANSPARENT; // remove the cursor
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
          puzzle[cursor_r][cursor_c] = se_mem[30][cursor_r * 32 + cursor_c] = DOT;
          clearing = false;
          break;
        case DOT:
          puzzle[cursor_r][cursor_c] = se_mem[30][cursor_r * 32 + cursor_c] = WHITE;
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
            puzzle[cursor_r][cursor_c] = se_mem[30][cursor_r * 32 + cursor_c] = WHITE;
          }
          else {
            puzzle[cursor_r][cursor_c] = se_mem[30][cursor_r * 32 + cursor_c] = DOT;
          }
          break;
      }
    }

    if (key_hit(1 << KI_B)) {
      switch (puzzle[cursor_r][cursor_c]) {
        case WHITE:
        case DOT:
          puzzle[cursor_r][cursor_c] = se_mem[30][cursor_r * 32 + cursor_c] = BLACK;
          clearing = false;
          break;
        case BLACK:
          puzzle[cursor_r][cursor_c] = se_mem[30][cursor_r * 32 + cursor_c] = WHITE;
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
            puzzle[cursor_r][cursor_c] = se_mem[30][cursor_r * 32 + cursor_c] = WHITE;
          }
          else {
            puzzle[cursor_r][cursor_c] = se_mem[30][cursor_r * 32 + cursor_c] = BLACK;
          }
          break;
      }
    }

    se_mem[31][cursor_r * 32 + cursor_c] = CURSOR; // readd the cursor
  }
}
