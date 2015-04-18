#include <tonc.h>

int main() {
  REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

#define NUR_ROWS 10
#define NUR_COLS 10

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

  while (1) {
    vid_vsync();
    vid_vsync();
    vid_vsync();
    vid_vsync();
    vid_vsync();
    vid_vsync();
    key_poll();
    if (key_hit(1 << KI_LEFT) && cursor_c > 0) cursor_c--;
    if (key_hit(1 << KI_RIGHT) && cursor_c < NUR_COLS - 1) cursor_c++;
    if (key_hit(1 << KI_UP) && cursor_r > 0) cursor_r--;
    if (key_hit(1 << KI_DOWN) && cursor_r < NUR_ROWS - 1) cursor_r++;
    m3_rect(0, 0, 8 * NUR_COLS + 1, 8 * NUR_ROWS + 1, CLR_MAROON);
    for (int r = 0; r < NUR_ROWS; r++) {
      for (int c = 0; c < NUR_COLS; c++) {
        m3_rect(c * 8 + 1, r * 8 + 1, c * 8 + 8, r * 8 + 8,
          puzzle[r][c] == -1
            ? (cursor_r == r && cursor_c == c ? RGB15(8, 0, 0) : CLR_BLACK)
            : (cursor_r == r && cursor_c == c ? RGB15(31, 23, 23) : CLR_WHITE)
          );
      }
    }
  }
}
