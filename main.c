#include <string.h>
#include <tonc.h>

int main() {
  REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

#define NUR_ROWS 10
#define NUR_COLS 13

  int puzzle[NUR_ROWS][NUR_COLS];
  //  0: blank
  // -1: black
  // -2: dot
  // positive: numbered
  for (int r = 0; r < NUR_ROWS; r++) {
    for (int c = 0; c < NUR_COLS; c++) {
      puzzle[r][c] = 0;
    }
  }
  puzzle[4][3] = -1;

  int cursor_r = 0;
  int cursor_c = 0;

  // Load palette
  pal_bg_mem[0] = CLR_BLACK;
  pal_bg_mem[1] = CLR_WHITE;
  pal_bg_mem[2] = CLR_BLUE;
  pal_bg_mem[3] = RGB15(8, 8, 8);
  pal_bg_mem[4] = CLR_RED;

  // Load tiles into CBB 0
  
  // outside the puzzle
  tile_mem[0][0].data[0] = 0x22222222;
tile_mem[0][0].data[1] = 0x22222222;
tile_mem[0][0].data[2] = 0x22222222;
tile_mem[0][0].data[3] = 0x22222222;
tile_mem[0][0].data[4] = 0x22222222;
tile_mem[0][0].data[5] = 0x22222222;
tile_mem[0][0].data[6] = 0x22222222;
tile_mem[0][0].data[7] = 0x22222222;
  // white space
  tile_mem[0][1].data[0] = 0x00000000;
tile_mem[0][1].data[1] = 0x11111110;
tile_mem[0][1].data[2] = 0x11111110;
tile_mem[0][1].data[3] = 0x11111110;
tile_mem[0][1].data[4] = 0x11111110;
tile_mem[0][1].data[5] = 0x11111110;
tile_mem[0][1].data[6] = 0x11111110;
tile_mem[0][1].data[7] = 0x11111110;
  // black space
  tile_mem[0][2].data[0] = 0x00000000;
tile_mem[0][2].data[1] = 0x33333330;
tile_mem[0][2].data[2] = 0x33333330;
tile_mem[0][2].data[3] = 0x33333330;
tile_mem[0][2].data[4] = 0x33333330;
tile_mem[0][2].data[5] = 0x33333330;
tile_mem[0][2].data[6] = 0x33333330;
tile_mem[0][2].data[7] = 0x33333330;
  // bottom edge of puzzle
  tile_mem[0][3].data[0] = 0x00000000;
tile_mem[0][3].data[1] = 0x22222222;
tile_mem[0][3].data[2] = 0x22222222;
tile_mem[0][3].data[3] = 0x22222222;
tile_mem[0][3].data[4] = 0x22222222;
tile_mem[0][3].data[5] = 0x22222222;
tile_mem[0][3].data[6] = 0x22222222;
tile_mem[0][3].data[7] = 0x22222222;
  // right edge of puzzle
  tile_mem[0][4].data[0] = 0x22222220;
tile_mem[0][4].data[1] = 0x22222220;
tile_mem[0][4].data[2] = 0x22222220;
tile_mem[0][4].data[3] = 0x22222220;
tile_mem[0][4].data[4] = 0x22222220;
tile_mem[0][4].data[5] = 0x22222220;
tile_mem[0][4].data[6] = 0x22222220;
tile_mem[0][4].data[7] = 0x22222220;
  // bottom-right corner of puzzle
  tile_mem[0][5].data[0] = 0x22222220;
tile_mem[0][5].data[1] = 0x22222222;
tile_mem[0][5].data[2] = 0x22222222;
tile_mem[0][5].data[3] = 0x22222222;
tile_mem[0][5].data[4] = 0x22222222;
tile_mem[0][5].data[5] = 0x22222222;
tile_mem[0][5].data[6] = 0x22222222;
tile_mem[0][5].data[7] = 0x22222222;
  // white space with cursor
  tile_mem[0][6].data[0] = 0x00000000;
tile_mem[0][6].data[1] = 0x44111440;
tile_mem[0][6].data[2] = 0x41111140;
tile_mem[0][6].data[3] = 0x11111110;
tile_mem[0][6].data[4] = 0x11111110;
tile_mem[0][6].data[5] = 0x11111110;
tile_mem[0][6].data[6] = 0x41111140;
tile_mem[0][6].data[7] = 0x44111440;
  // black space with cursor
  tile_mem[0][7].data[0] = 0x00000000;
tile_mem[0][7].data[1] = 0x44333440;
tile_mem[0][7].data[2] = 0x43333340;
tile_mem[0][7].data[3] = 0x33333330;
tile_mem[0][7].data[4] = 0x33333330;
tile_mem[0][7].data[5] = 0x33333330;
tile_mem[0][7].data[6] = 0x43333340;
tile_mem[0][7].data[7] = 0x44333440;

  // Load map into SBB 30
  for (int i = 0; i < 32 * 32; i++) se_mem[30][i] = 0;
  for (int c = 0; c < NUR_COLS; c++) se_mem[30][NUR_ROWS * 32 + c] = 3;
  for (int r = 0; r < NUR_ROWS; r++) se_mem[30][r * 32 + NUR_COLS] = 4;
  se_mem[30][NUR_ROWS * 32 + NUR_COLS] = 5;

  // set up BG0 for a 4bpp 64x32t map, using
  //   using charblock 0 and screenblock 31
  REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;
  REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

  irq_init(NULL);
  irq_add(II_VBLANK, NULL);

  while (1) {
    VBlankIntrWait();
    key_poll();
    if (key_hit(1 << KI_LEFT) && cursor_c > 0) cursor_c--;
    if (key_hit(1 << KI_RIGHT) && cursor_c < NUR_COLS - 1) cursor_c++;
    if (key_hit(1 << KI_UP) && cursor_r > 0) cursor_r--;
    if (key_hit(1 << KI_DOWN) && cursor_r < NUR_ROWS - 1) cursor_r++;
    for (int r = 0; r < NUR_ROWS; r++) {
      for (int c = 0; c < NUR_COLS; c++) {
        se_mem[30][r * 32 + c] =
          cursor_r == r && cursor_c == c
            ? (puzzle[r][c] ? 7 : 6)
            : (puzzle[r][c] ? 2 : 1);
      }
    }
  }
}
