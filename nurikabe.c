#include <string.h>
#include <tonc.h>
#include "tiles16.h"
#include "tiles8.h"

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
  // First write to the 8x8 maps
  if      (charblock == 24) se_mem[22][r * 32 + c] = tile;
  else if (charblock == 28) se_mem[23][r * 32 + c] = tile;
  // Now the 16x16 maps
  if (c >= 16) {
    // move over 1 charblock
    c -= 16;
    charblock += 1;
  }
  if (r >= 16) {
    // move down 1 charblock (= over 2)
    r -= 16;
    charblock += 2;
  }
  se_mem[charblock][r * 2 * 32 +      c * 2    ] = tile * 4    ;
  se_mem[charblock][r * 2 * 32 +      c * 2 + 1] = tile * 4 + 1;
  se_mem[charblock][r * 2 * 32 + 32 + c * 2    ] = tile * 4 + 2;
  se_mem[charblock][r * 2 * 32 + 32 + c * 2 + 1] = tile * 4 + 3;
}

// Returns true if all squares are filled in with something.
bool checkFull(int NUR_ROWS, int NUR_COLS, int puzzle[NUR_ROWS][NUR_COLS]) {
  for (int r = 0; r < NUR_ROWS - 1; r++) {
    for (int c = 0; c < NUR_COLS - 1; c++) {
      if (puzzle[r][c] == WHITE) return false;
    }
  }
  return true;
}

// Returns true if there are no 2x2 pools of black squares.
bool checkPools(int NUR_ROWS, int NUR_COLS, int puzzle[NUR_ROWS][NUR_COLS]) {
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
bool checkBlack(int NUR_ROWS, int NUR_COLS, int puzzle[NUR_ROWS][NUR_COLS]) {
  return false;
}

// Returns true if each number n connects to n-1 dots, and no other numbers.
bool checkNumbers(int NUR_ROWS, int NUR_COLS, int puzzle[NUR_ROWS][NUR_COLS]) {
  return false;
}

int playPuzzle(int NUR_ROWS, int NUR_COLS, int puzzle[NUR_ROWS][NUR_COLS], int puzzle_index) {

  int cursor_r = 0;
  int cursor_c = 0;

  // Load palette
  memcpy16(pal_bg_mem, SharedPal, SharedPalLen / 2);

  // Load tiles into CBB 0 (16x16) and 1 (8x8)
  // Each charblock is 0x4000, an 8x8 tile is 0x20 bytes,
  // so there are 512 8x8 tiles or 128 16x16 tiles in each charblock.
  memcpy16(tile_mem[0], tiles16Tiles, tiles16TilesLen / 2);
  memcpy16(tile_mem[1], tiles8Tiles, tiles8TilesLen / 2);

  // Load the 16x16 puzzle map into screenblocks 28-31
  for (int r = 0; r < 32; r++) {
    for (int c = 0; c < 32; c++) {
      set_tile(28, r, c, OUTSIDE);
    }
  }
  for (int c = 0; c < NUR_COLS; c++) set_tile(28, NUR_ROWS, c, BOTTOM_EDGE);
  for (int r = 0; r < NUR_ROWS; r++) set_tile(28, r, NUR_COLS, RIGHT_EDGE);
  set_tile(28, NUR_ROWS, NUR_COLS, BOTTOM_RIGHT_CORNER);
  for (int r = 0; r < NUR_ROWS; r++) {
    for (int c = 0; c < NUR_COLS; c++) {
      set_tile(28, r, c, puzzle[r][c]);
    }
  }

  // Load the 16x16 cursor map into screenblocks 24-27
  for (int r = 0; r < 32; r++) {
    for (int c = 0; c < 32; c++) {
      set_tile(24, r, c, TRANSPARENT);
    }
  }
  set_tile(24, cursor_r, cursor_c, CURSOR);

  // 8x8 tiles:
  // set up BG2 for a 4bpp 32x32t map, using charblock 1 and screenblock 22 (cursor)
  REG_BG2CNT = BG_CBB(1) | BG_SBB(22) | BG_4BPP | BG_REG_32x32;
  // set up BG3 for a 4bpp 32x32t map, using charblock 1 and screenblock 23 (puzzle squares)
  REG_BG3CNT = BG_CBB(1) | BG_SBB(23) | BG_4BPP | BG_REG_32x32;
  // 16x16 tiles:
  // set up BG0 for a 4bpp 64x64t map, using charblock 0 and screenblocks 24-27 (cursor)
  REG_BG0CNT = BG_CBB(0) | BG_SBB(24) | BG_4BPP | BG_REG_64x64;
  // set up BG1 for a 4bpp 64x64t map, using charblock 0 and screenblocks 28-31 (puzzle squares)
  REG_BG1CNT = BG_CBB(0) | BG_SBB(28) | BG_4BPP | BG_REG_64x64;
  REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;
  bool small_tiles = false;

  int max_horiz_offset_16 = NUR_COLS * 16 - 240;
  if (max_horiz_offset_16 < 0) max_horiz_offset_16 = 0;
  int max_vert_offset_16  = NUR_ROWS * 16 - 160;
  if (max_vert_offset_16 < 0) max_vert_offset_16 = 0;

  int max_horiz_offset_8 = NUR_COLS * 8 - 240;
  if (max_horiz_offset_8 < 0) max_horiz_offset_8 = 0;
  int max_vert_offset_8  = NUR_ROWS * 8 - 160;
  if (max_vert_offset_8 < 0) max_vert_offset_8 = 0;

  REG_BG0HOFS = REG_BG1HOFS = REG_BG2HOFS = REG_BG3HOFS = 0;
  REG_BG0VOFS = REG_BG1VOFS = REG_BG2VOFS = REG_BG3VOFS = 0;

  irq_init(NULL);
  irq_add(II_VBLANK, NULL);

  int key_repeat = 0;
  bool clearing = false;
  while (1) {
    VBlankIntrWait();
    key_poll();
    set_tile(24, cursor_r, cursor_c, TRANSPARENT); // remove the cursor
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
    if (virtual_left  && cursor_c > 0           ) {
      cursor_c--;
      REG_BG0HOFS = REG_BG1HOFS = (cursor_c * max_horiz_offset_16) / (NUR_COLS - 1);
      REG_BG2HOFS = REG_BG3HOFS = (cursor_c * max_horiz_offset_8 ) / (NUR_COLS - 1);
      moved_cursor = true;
    }
    if (virtual_right && cursor_c < NUR_COLS - 1) {
      cursor_c++;
      REG_BG0HOFS = REG_BG1HOFS = (cursor_c * max_horiz_offset_16) / (NUR_COLS - 1);
      REG_BG2HOFS = REG_BG3HOFS = (cursor_c * max_horiz_offset_8 ) / (NUR_COLS - 1);
      moved_cursor = true;
    }
    if (virtual_up    && cursor_r > 0           ) {
      cursor_r--;
      REG_BG0VOFS = REG_BG1VOFS = (cursor_r * max_vert_offset_16) / (NUR_ROWS - 1);
      REG_BG2VOFS = REG_BG3VOFS = (cursor_r * max_vert_offset_8 ) / (NUR_ROWS - 1);
      moved_cursor = true;
    }
    if (virtual_down  && cursor_r < NUR_ROWS - 1) {
      cursor_r++;
      REG_BG0VOFS = REG_BG1VOFS = (cursor_r * max_vert_offset_16) / (NUR_ROWS - 1);
      REG_BG2VOFS = REG_BG3VOFS = (cursor_r * max_vert_offset_8 ) / (NUR_ROWS - 1);
      moved_cursor = true;
    }

    if (key_hit(1 << KI_A)) {
      switch (puzzle[cursor_r][cursor_c]) {
        case WHITE:
        case BLACK:
          puzzle[cursor_r][cursor_c] = DOT;
          sram_mem[(puzzle_index + 1) * 1024 + cursor_r * NUR_COLS + cursor_c] = DOT;
          set_tile(28, cursor_r, cursor_c, DOT);
          clearing = false;
          break;
        case DOT:
          puzzle[cursor_r][cursor_c] = WHITE;
          sram_mem[(puzzle_index + 1) * 1024 + cursor_r * NUR_COLS + cursor_c] = WHITE;
          set_tile(28, cursor_r, cursor_c, WHITE);
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
            sram_mem[(puzzle_index + 1) * 1024 + cursor_r * NUR_COLS + cursor_c] = WHITE;
            set_tile(28, cursor_r, cursor_c, WHITE);
          }
          else {
            puzzle[cursor_r][cursor_c] = DOT;
            sram_mem[(puzzle_index + 1) * 1024 + cursor_r * NUR_COLS + cursor_c] = DOT;
            set_tile(28, cursor_r, cursor_c, DOT);
          }
          break;
      }
    }

    if (key_hit(1 << KI_B)) {
      switch (puzzle[cursor_r][cursor_c]) {
        case WHITE:
        case DOT:
          puzzle[cursor_r][cursor_c] = BLACK;
          sram_mem[(puzzle_index + 1) * 1024 + cursor_r * NUR_COLS + cursor_c] = BLACK;
          set_tile(28, cursor_r, cursor_c, BLACK);
          clearing = false;
          break;
        case BLACK:
          puzzle[cursor_r][cursor_c] = WHITE;
          sram_mem[(puzzle_index + 1) * 1024 + cursor_r * NUR_COLS + cursor_c] = WHITE;
          set_tile(28, cursor_r, cursor_c, WHITE);
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
            sram_mem[(puzzle_index + 1) * 1024 + cursor_r * NUR_COLS + cursor_c] = WHITE;
            set_tile(28, cursor_r, cursor_c, WHITE);
          }
          else {
            puzzle[cursor_r][cursor_c] = BLACK;
            sram_mem[(puzzle_index + 1) * 1024 + cursor_r * NUR_COLS + cursor_c] = BLACK;
            set_tile(28, cursor_r, cursor_c, BLACK);
          }
          break;
      }
    }

    if (key_hit(1 << KI_SELECT)) {
      small_tiles = !small_tiles;
      if (small_tiles) {
        REG_DISPCNT = DCNT_MODE0 | DCNT_BG2 | DCNT_BG3;
      }
      else {
        REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;
      }
    }

    if (key_hit(1 << KI_L)) {
      return -1; // move 1 puzzle to the left
    }

    if (key_hit(1 << KI_R)) {
      return 1; // move 1 puzzle to the right
    }

    set_tile(24, cursor_r, cursor_c, CURSOR); // readd the cursor
  }
}



// SRAM format:
// byte 0: number of puzzles
// byte 1: index of current puzzle
// byte 2: rows of puzzle 0
// byte 3: cols of puzzle 0
// byte 4: rows of puzzle 1
// byte 5: cols of puzzle 1
// ...
// at byte 1024:
//   puzzle 0, 1 byte per square
// at byte 2048:
//   puzzle 1, 1 byte per square
// ...
void load_to_sram() {
  // TODO: load a bunch of puzzles
  sram_mem[0] = 1;
  sram_mem[1] = 0;
  sram_mem[2] = 15;
  sram_mem[3] = 15;
  sram_mem[1024 + 0] = NUMBER(1);
sram_mem[1024 + 1] = WHITE;
sram_mem[1024 + 2] = WHITE;
sram_mem[1024 + 3] = WHITE;
sram_mem[1024 + 4] = WHITE;
sram_mem[1024 + 5] = WHITE;
sram_mem[1024 + 6] = NUMBER(4);
sram_mem[1024 + 7] = WHITE;
sram_mem[1024 + 8] = WHITE;
sram_mem[1024 + 9] = WHITE;
sram_mem[1024 + 10] = WHITE;
sram_mem[1024 + 11] = WHITE;
sram_mem[1024 + 12] = NUMBER(4);
sram_mem[1024 + 13] = WHITE;
sram_mem[1024 + 14] = WHITE;
sram_mem[1024 + 15] = WHITE;
sram_mem[1024 + 16] = WHITE;
sram_mem[1024 + 17] = WHITE;
sram_mem[1024 + 18] = WHITE;
sram_mem[1024 + 19] = WHITE;
sram_mem[1024 + 20] = WHITE;
sram_mem[1024 + 21] = WHITE;
sram_mem[1024 + 22] = WHITE;
sram_mem[1024 + 23] = WHITE;
sram_mem[1024 + 24] = WHITE;
sram_mem[1024 + 25] = WHITE;
sram_mem[1024 + 26] = WHITE;
sram_mem[1024 + 27] = WHITE;
sram_mem[1024 + 28] = WHITE;
sram_mem[1024 + 29] = WHITE;
sram_mem[1024 + 30] = WHITE;
sram_mem[1024 + 31] = WHITE;
sram_mem[1024 + 32] = WHITE;
sram_mem[1024 + 33] = NUMBER(3);
sram_mem[1024 + 34] = WHITE;
sram_mem[1024 + 35] = WHITE;
sram_mem[1024 + 36] = WHITE;
sram_mem[1024 + 37] = WHITE;
sram_mem[1024 + 38] = WHITE;
sram_mem[1024 + 39] = WHITE;
sram_mem[1024 + 40] = WHITE;
sram_mem[1024 + 41] = WHITE;
sram_mem[1024 + 42] = WHITE;
sram_mem[1024 + 43] = WHITE;
sram_mem[1024 + 44] = WHITE;
sram_mem[1024 + 45] = WHITE;
sram_mem[1024 + 46] = WHITE;
sram_mem[1024 + 47] = WHITE;
sram_mem[1024 + 48] = WHITE;
sram_mem[1024 + 49] = WHITE;
sram_mem[1024 + 50] = WHITE;
sram_mem[1024 + 51] = WHITE;
sram_mem[1024 + 52] = NUMBER(3);
sram_mem[1024 + 53] = WHITE;
sram_mem[1024 + 54] = WHITE;
sram_mem[1024 + 55] = WHITE;
sram_mem[1024 + 56] = WHITE;
sram_mem[1024 + 57] = WHITE;
sram_mem[1024 + 58] = WHITE;
sram_mem[1024 + 59] = NUMBER(4);
sram_mem[1024 + 60] = WHITE;
sram_mem[1024 + 61] = WHITE;
sram_mem[1024 + 62] = NUMBER(4);
sram_mem[1024 + 63] = WHITE;
sram_mem[1024 + 64] = WHITE;
sram_mem[1024 + 65] = WHITE;
sram_mem[1024 + 66] = WHITE;
sram_mem[1024 + 67] = WHITE;
sram_mem[1024 + 68] = WHITE;
sram_mem[1024 + 69] = WHITE;
sram_mem[1024 + 70] = WHITE;
sram_mem[1024 + 71] = WHITE;
sram_mem[1024 + 72] = NUMBER(4);
sram_mem[1024 + 73] = WHITE;
sram_mem[1024 + 74] = WHITE;
sram_mem[1024 + 75] = WHITE;
sram_mem[1024 + 76] = WHITE;
sram_mem[1024 + 77] = WHITE;
sram_mem[1024 + 78] = WHITE;
sram_mem[1024 + 79] = WHITE;
sram_mem[1024 + 80] = NUMBER(3);
sram_mem[1024 + 81] = WHITE;
sram_mem[1024 + 82] = WHITE;
sram_mem[1024 + 83] = WHITE;
sram_mem[1024 + 84] = WHITE;
sram_mem[1024 + 85] = NUMBER(5);
sram_mem[1024 + 86] = WHITE;
sram_mem[1024 + 87] = WHITE;
sram_mem[1024 + 88] = WHITE;
sram_mem[1024 + 89] = WHITE;
sram_mem[1024 + 90] = WHITE;
sram_mem[1024 + 91] = WHITE;
sram_mem[1024 + 92] = WHITE;
sram_mem[1024 + 93] = WHITE;
sram_mem[1024 + 94] = WHITE;
sram_mem[1024 + 95] = WHITE;
sram_mem[1024 + 96] = WHITE;
sram_mem[1024 + 97] = WHITE;
sram_mem[1024 + 98] = WHITE;
sram_mem[1024 + 99] = WHITE;
sram_mem[1024 + 100] = WHITE;
sram_mem[1024 + 101] = WHITE;
sram_mem[1024 + 102] = WHITE;
sram_mem[1024 + 103] = WHITE;
sram_mem[1024 + 104] = NUMBER(3);
sram_mem[1024 + 105] = WHITE;
sram_mem[1024 + 106] = NUMBER(2);
sram_mem[1024 + 107] = WHITE;
sram_mem[1024 + 108] = NUMBER(3);
sram_mem[1024 + 109] = WHITE;
sram_mem[1024 + 110] = WHITE;
sram_mem[1024 + 111] = WHITE;
sram_mem[1024 + 112] = WHITE;
sram_mem[1024 + 113] = WHITE;
sram_mem[1024 + 114] = WHITE;
sram_mem[1024 + 115] = WHITE;
sram_mem[1024 + 116] = WHITE;
sram_mem[1024 + 117] = WHITE;
sram_mem[1024 + 118] = WHITE;
sram_mem[1024 + 119] = WHITE;
sram_mem[1024 + 120] = WHITE;
sram_mem[1024 + 121] = WHITE;
sram_mem[1024 + 122] = WHITE;
sram_mem[1024 + 123] = WHITE;
sram_mem[1024 + 124] = WHITE;
sram_mem[1024 + 125] = WHITE;
sram_mem[1024 + 126] = WHITE;
sram_mem[1024 + 127] = NUMBER(4);
sram_mem[1024 + 128] = WHITE;
sram_mem[1024 + 129] = NUMBER(3);
sram_mem[1024 + 130] = WHITE;
sram_mem[1024 + 131] = WHITE;
sram_mem[1024 + 132] = WHITE;
sram_mem[1024 + 133] = NUMBER(6);
sram_mem[1024 + 134] = WHITE;
sram_mem[1024 + 135] = NUMBER(2);
sram_mem[1024 + 136] = WHITE;
sram_mem[1024 + 137] = WHITE;
sram_mem[1024 + 138] = WHITE;
sram_mem[1024 + 139] = WHITE;
sram_mem[1024 + 140] = WHITE;
sram_mem[1024 + 141] = NUMBER(4);
sram_mem[1024 + 142] = WHITE;
sram_mem[1024 + 143] = WHITE;
sram_mem[1024 + 144] = WHITE;
sram_mem[1024 + 145] = NUMBER(1);
sram_mem[1024 + 146] = WHITE;
sram_mem[1024 + 147] = WHITE;
sram_mem[1024 + 148] = WHITE;
sram_mem[1024 + 149] = WHITE;
sram_mem[1024 + 150] = WHITE;
sram_mem[1024 + 151] = WHITE;
sram_mem[1024 + 152] = WHITE;
sram_mem[1024 + 153] = NUMBER(5);
sram_mem[1024 + 154] = WHITE;
sram_mem[1024 + 155] = WHITE;
sram_mem[1024 + 156] = WHITE;
sram_mem[1024 + 157] = WHITE;
sram_mem[1024 + 158] = NUMBER(3);
sram_mem[1024 + 159] = WHITE;
sram_mem[1024 + 160] = WHITE;
sram_mem[1024 + 161] = WHITE;
sram_mem[1024 + 162] = WHITE;
sram_mem[1024 + 163] = WHITE;
sram_mem[1024 + 164] = WHITE;
sram_mem[1024 + 165] = WHITE;
sram_mem[1024 + 166] = WHITE;
sram_mem[1024 + 167] = WHITE;
sram_mem[1024 + 168] = WHITE;
sram_mem[1024 + 169] = WHITE;
sram_mem[1024 + 170] = WHITE;
sram_mem[1024 + 171] = WHITE;
sram_mem[1024 + 172] = WHITE;
sram_mem[1024 + 173] = WHITE;
sram_mem[1024 + 174] = WHITE;
sram_mem[1024 + 175] = WHITE;
sram_mem[1024 + 176] = NUMBER(3);
sram_mem[1024 + 177] = WHITE;
sram_mem[1024 + 178] = NUMBER(2);
sram_mem[1024 + 179] = WHITE;
sram_mem[1024 + 180] = NUMBER(2);
sram_mem[1024 + 181] = WHITE;
sram_mem[1024 + 182] = WHITE;
sram_mem[1024 + 183] = NUMBER(2);
sram_mem[1024 + 184] = WHITE;
sram_mem[1024 + 185] = WHITE;
sram_mem[1024 + 186] = NUMBER(4);
sram_mem[1024 + 187] = WHITE;
sram_mem[1024 + 188] = NUMBER(1);
sram_mem[1024 + 189] = WHITE;
sram_mem[1024 + 190] = WHITE;
sram_mem[1024 + 191] = WHITE;
sram_mem[1024 + 192] = NUMBER(1);
sram_mem[1024 + 193] = WHITE;
sram_mem[1024 + 194] = WHITE;
sram_mem[1024 + 195] = WHITE;
sram_mem[1024 + 196] = WHITE;
sram_mem[1024 + 197] = WHITE;
sram_mem[1024 + 198] = WHITE;
sram_mem[1024 + 199] = WHITE;
sram_mem[1024 + 200] = WHITE;
sram_mem[1024 + 201] = WHITE;
sram_mem[1024 + 202] = WHITE;
sram_mem[1024 + 203] = WHITE;
sram_mem[1024 + 204] = WHITE;
sram_mem[1024 + 205] = WHITE;
sram_mem[1024 + 206] = WHITE;
sram_mem[1024 + 207] = WHITE;
sram_mem[1024 + 208] = WHITE;
sram_mem[1024 + 209] = WHITE;
sram_mem[1024 + 210] = WHITE;
sram_mem[1024 + 211] = WHITE;
sram_mem[1024 + 212] = NUMBER(2);
sram_mem[1024 + 213] = WHITE;
sram_mem[1024 + 214] = WHITE;
sram_mem[1024 + 215] = NUMBER(2);
sram_mem[1024 + 216] = WHITE;
sram_mem[1024 + 217] = WHITE;
sram_mem[1024 + 218] = NUMBER(2);
sram_mem[1024 + 219] = WHITE;
sram_mem[1024 + 220] = WHITE;
sram_mem[1024 + 221] = WHITE;
sram_mem[1024 + 222] = NUMBER(3);
sram_mem[1024 + 223] = WHITE;
sram_mem[1024 + 224] = NUMBER(3);
}

int main() {
  if (sram_mem[0] == 0) {
    // this is the first load; initialize the puzzles in save memory
    load_to_sram();
  }

  u8 num_puzzles  = sram_mem[0];
  u8 puzzle_index = sram_mem[1];
  while (1) {
    short num_rows = sram_mem[2 + puzzle_index * 2];
    short num_cols = sram_mem[2 + puzzle_index * 2];
    int puzzle[num_rows][num_cols];
    for (int r = 0; r < num_rows; r++) {
      for (int c = 0; c < num_cols; c++) {
        puzzle[r][c] = sram_mem[(puzzle_index + 1) * 1024 + r * num_cols + c];
      }
    }
    int puzzle_bump = playPuzzle(num_rows, num_cols, puzzle, puzzle_index);
    int new_puzzle_index = puzzle_index + puzzle_bump;
    if (0 <= new_puzzle_index && new_puzzle_index < num_puzzles) {
      puzzle_index = new_puzzle_index;
      sram_mem[1] = puzzle_index;
    }
  }
}
