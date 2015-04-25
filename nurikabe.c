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

bool small_tiles = false;

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
  if (small_tiles) {
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG2 | DCNT_BG3;
  }
  else {
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;
  }

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
  
  sram_mem[0] = 10;
  sram_mem[1] = 0;
  
    sram_mem[2] = 7;
    sram_mem[3] = 7;
    
      
        sram_mem[1024] =
        WHITE;
      
        sram_mem[1025] =
        WHITE;
      
        sram_mem[1026] =
        WHITE;
      
        sram_mem[1027] =
        WHITE;
      
        sram_mem[1028] =
        WHITE;
      
        sram_mem[1029] =
        WHITE;
      
        sram_mem[1030] =
        NUMBER(1);
      
    
      
        sram_mem[1031] =
        WHITE;
      
        sram_mem[1032] =
        WHITE;
      
        sram_mem[1033] =
        WHITE;
      
        sram_mem[1034] =
        WHITE;
      
        sram_mem[1035] =
        NUMBER(1);
      
        sram_mem[1036] =
        WHITE;
      
        sram_mem[1037] =
        WHITE;
      
    
      
        sram_mem[1038] =
        WHITE;
      
        sram_mem[1039] =
        WHITE;
      
        sram_mem[1040] =
        WHITE;
      
        sram_mem[1041] =
        WHITE;
      
        sram_mem[1042] =
        WHITE;
      
        sram_mem[1043] =
        WHITE;
      
        sram_mem[1044] =
        NUMBER(2);
      
    
      
        sram_mem[1045] =
        NUMBER(1);
      
        sram_mem[1046] =
        WHITE;
      
        sram_mem[1047] =
        WHITE;
      
        sram_mem[1048] =
        WHITE;
      
        sram_mem[1049] =
        WHITE;
      
        sram_mem[1050] =
        WHITE;
      
        sram_mem[1051] =
        WHITE;
      
    
      
        sram_mem[1052] =
        WHITE;
      
        sram_mem[1053] =
        WHITE;
      
        sram_mem[1054] =
        WHITE;
      
        sram_mem[1055] =
        WHITE;
      
        sram_mem[1056] =
        WHITE;
      
        sram_mem[1057] =
        WHITE;
      
        sram_mem[1058] =
        WHITE;
      
    
      
        sram_mem[1059] =
        WHITE;
      
        sram_mem[1060] =
        WHITE;
      
        sram_mem[1061] =
        WHITE;
      
        sram_mem[1062] =
        WHITE;
      
        sram_mem[1063] =
        WHITE;
      
        sram_mem[1064] =
        WHITE;
      
        sram_mem[1065] =
        WHITE;
      
    
      
        sram_mem[1066] =
        WHITE;
      
        sram_mem[1067] =
        WHITE;
      
        sram_mem[1068] =
        NUMBER(3);
      
        sram_mem[1069] =
        WHITE;
      
        sram_mem[1070] =
        NUMBER(1);
      
        sram_mem[1071] =
        WHITE;
      
        sram_mem[1072] =
        NUMBER(11);
      
    
  
    sram_mem[4] = 7;
    sram_mem[5] = 7;
    
      
        sram_mem[2048] =
        NUMBER(2);
      
        sram_mem[2049] =
        WHITE;
      
        sram_mem[2050] =
        WHITE;
      
        sram_mem[2051] =
        NUMBER(7);
      
        sram_mem[2052] =
        WHITE;
      
        sram_mem[2053] =
        WHITE;
      
        sram_mem[2054] =
        NUMBER(2);
      
    
      
        sram_mem[2055] =
        WHITE;
      
        sram_mem[2056] =
        WHITE;
      
        sram_mem[2057] =
        WHITE;
      
        sram_mem[2058] =
        WHITE;
      
        sram_mem[2059] =
        WHITE;
      
        sram_mem[2060] =
        WHITE;
      
        sram_mem[2061] =
        WHITE;
      
    
      
        sram_mem[2062] =
        NUMBER(2);
      
        sram_mem[2063] =
        WHITE;
      
        sram_mem[2064] =
        WHITE;
      
        sram_mem[2065] =
        WHITE;
      
        sram_mem[2066] =
        WHITE;
      
        sram_mem[2067] =
        WHITE;
      
        sram_mem[2068] =
        WHITE;
      
    
      
        sram_mem[2069] =
        WHITE;
      
        sram_mem[2070] =
        WHITE;
      
        sram_mem[2071] =
        NUMBER(3);
      
        sram_mem[2072] =
        WHITE;
      
        sram_mem[2073] =
        WHITE;
      
        sram_mem[2074] =
        WHITE;
      
        sram_mem[2075] =
        WHITE;
      
    
      
        sram_mem[2076] =
        NUMBER(2);
      
        sram_mem[2077] =
        WHITE;
      
        sram_mem[2078] =
        WHITE;
      
        sram_mem[2079] =
        WHITE;
      
        sram_mem[2080] =
        WHITE;
      
        sram_mem[2081] =
        WHITE;
      
        sram_mem[2082] =
        WHITE;
      
    
      
        sram_mem[2083] =
        WHITE;
      
        sram_mem[2084] =
        WHITE;
      
        sram_mem[2085] =
        WHITE;
      
        sram_mem[2086] =
        WHITE;
      
        sram_mem[2087] =
        NUMBER(1);
      
        sram_mem[2088] =
        WHITE;
      
        sram_mem[2089] =
        WHITE;
      
    
      
        sram_mem[2090] =
        WHITE;
      
        sram_mem[2091] =
        WHITE;
      
        sram_mem[2092] =
        WHITE;
      
        sram_mem[2093] =
        WHITE;
      
        sram_mem[2094] =
        WHITE;
      
        sram_mem[2095] =
        WHITE;
      
        sram_mem[2096] =
        NUMBER(1);
      
    
  
    sram_mem[6] = 7;
    sram_mem[7] = 7;
    
      
        sram_mem[3072] =
        NUMBER(2);
      
        sram_mem[3073] =
        WHITE;
      
        sram_mem[3074] =
        WHITE;
      
        sram_mem[3075] =
        NUMBER(2);
      
        sram_mem[3076] =
        WHITE;
      
        sram_mem[3077] =
        NUMBER(2);
      
        sram_mem[3078] =
        WHITE;
      
    
      
        sram_mem[3079] =
        WHITE;
      
        sram_mem[3080] =
        WHITE;
      
        sram_mem[3081] =
        WHITE;
      
        sram_mem[3082] =
        WHITE;
      
        sram_mem[3083] =
        WHITE;
      
        sram_mem[3084] =
        WHITE;
      
        sram_mem[3085] =
        WHITE;
      
    
      
        sram_mem[3086] =
        NUMBER(3);
      
        sram_mem[3087] =
        WHITE;
      
        sram_mem[3088] =
        WHITE;
      
        sram_mem[3089] =
        WHITE;
      
        sram_mem[3090] =
        NUMBER(3);
      
        sram_mem[3091] =
        WHITE;
      
        sram_mem[3092] =
        NUMBER(3);
      
    
      
        sram_mem[3093] =
        WHITE;
      
        sram_mem[3094] =
        WHITE;
      
        sram_mem[3095] =
        WHITE;
      
        sram_mem[3096] =
        WHITE;
      
        sram_mem[3097] =
        WHITE;
      
        sram_mem[3098] =
        WHITE;
      
        sram_mem[3099] =
        WHITE;
      
    
      
        sram_mem[3100] =
        WHITE;
      
        sram_mem[3101] =
        WHITE;
      
        sram_mem[3102] =
        WHITE;
      
        sram_mem[3103] =
        WHITE;
      
        sram_mem[3104] =
        WHITE;
      
        sram_mem[3105] =
        WHITE;
      
        sram_mem[3106] =
        WHITE;
      
    
      
        sram_mem[3107] =
        WHITE;
      
        sram_mem[3108] =
        NUMBER(4);
      
        sram_mem[3109] =
        WHITE;
      
        sram_mem[3110] =
        WHITE;
      
        sram_mem[3111] =
        WHITE;
      
        sram_mem[3112] =
        WHITE;
      
        sram_mem[3113] =
        WHITE;
      
    
      
        sram_mem[3114] =
        WHITE;
      
        sram_mem[3115] =
        WHITE;
      
        sram_mem[3116] =
        WHITE;
      
        sram_mem[3117] =
        WHITE;
      
        sram_mem[3118] =
        NUMBER(1);
      
        sram_mem[3119] =
        WHITE;
      
        sram_mem[3120] =
        NUMBER(1);
      
    
  
    sram_mem[8] = 7;
    sram_mem[9] = 7;
    
      
        sram_mem[4096] =
        NUMBER(3);
      
        sram_mem[4097] =
        WHITE;
      
        sram_mem[4098] =
        NUMBER(3);
      
        sram_mem[4099] =
        WHITE;
      
        sram_mem[4100] =
        WHITE;
      
        sram_mem[4101] =
        WHITE;
      
        sram_mem[4102] =
        WHITE;
      
    
      
        sram_mem[4103] =
        WHITE;
      
        sram_mem[4104] =
        WHITE;
      
        sram_mem[4105] =
        WHITE;
      
        sram_mem[4106] =
        WHITE;
      
        sram_mem[4107] =
        WHITE;
      
        sram_mem[4108] =
        WHITE;
      
        sram_mem[4109] =
        WHITE;
      
    
      
        sram_mem[4110] =
        WHITE;
      
        sram_mem[4111] =
        WHITE;
      
        sram_mem[4112] =
        NUMBER(4);
      
        sram_mem[4113] =
        WHITE;
      
        sram_mem[4114] =
        WHITE;
      
        sram_mem[4115] =
        NUMBER(4);
      
        sram_mem[4116] =
        WHITE;
      
    
      
        sram_mem[4117] =
        WHITE;
      
        sram_mem[4118] =
        NUMBER(2);
      
        sram_mem[4119] =
        WHITE;
      
        sram_mem[4120] =
        WHITE;
      
        sram_mem[4121] =
        WHITE;
      
        sram_mem[4122] =
        WHITE;
      
        sram_mem[4123] =
        WHITE;
      
    
      
        sram_mem[4124] =
        WHITE;
      
        sram_mem[4125] =
        WHITE;
      
        sram_mem[4126] =
        WHITE;
      
        sram_mem[4127] =
        WHITE;
      
        sram_mem[4128] =
        WHITE;
      
        sram_mem[4129] =
        NUMBER(2);
      
        sram_mem[4130] =
        WHITE;
      
    
      
        sram_mem[4131] =
        WHITE;
      
        sram_mem[4132] =
        WHITE;
      
        sram_mem[4133] =
        WHITE;
      
        sram_mem[4134] =
        WHITE;
      
        sram_mem[4135] =
        WHITE;
      
        sram_mem[4136] =
        WHITE;
      
        sram_mem[4137] =
        WHITE;
      
    
      
        sram_mem[4138] =
        NUMBER(4);
      
        sram_mem[4139] =
        WHITE;
      
        sram_mem[4140] =
        WHITE;
      
        sram_mem[4141] =
        WHITE;
      
        sram_mem[4142] =
        WHITE;
      
        sram_mem[4143] =
        NUMBER(2);
      
        sram_mem[4144] =
        WHITE;
      
    
  
    sram_mem[10] = 7;
    sram_mem[11] = 7;
    
      
        sram_mem[5120] =
        NUMBER(2);
      
        sram_mem[5121] =
        WHITE;
      
        sram_mem[5122] =
        NUMBER(3);
      
        sram_mem[5123] =
        WHITE;
      
        sram_mem[5124] =
        WHITE;
      
        sram_mem[5125] =
        WHITE;
      
        sram_mem[5126] =
        NUMBER(2);
      
    
      
        sram_mem[5127] =
        WHITE;
      
        sram_mem[5128] =
        WHITE;
      
        sram_mem[5129] =
        WHITE;
      
        sram_mem[5130] =
        WHITE;
      
        sram_mem[5131] =
        WHITE;
      
        sram_mem[5132] =
        WHITE;
      
        sram_mem[5133] =
        WHITE;
      
    
      
        sram_mem[5134] =
        WHITE;
      
        sram_mem[5135] =
        NUMBER(4);
      
        sram_mem[5136] =
        WHITE;
      
        sram_mem[5137] =
        WHITE;
      
        sram_mem[5138] =
        NUMBER(3);
      
        sram_mem[5139] =
        WHITE;
      
        sram_mem[5140] =
        WHITE;
      
    
      
        sram_mem[5141] =
        WHITE;
      
        sram_mem[5142] =
        WHITE;
      
        sram_mem[5143] =
        WHITE;
      
        sram_mem[5144] =
        WHITE;
      
        sram_mem[5145] =
        WHITE;
      
        sram_mem[5146] =
        WHITE;
      
        sram_mem[5147] =
        WHITE;
      
    
      
        sram_mem[5148] =
        NUMBER(3);
      
        sram_mem[5149] =
        WHITE;
      
        sram_mem[5150] =
        WHITE;
      
        sram_mem[5151] =
        WHITE;
      
        sram_mem[5152] =
        NUMBER(3);
      
        sram_mem[5153] =
        WHITE;
      
        sram_mem[5154] =
        WHITE;
      
    
      
        sram_mem[5155] =
        WHITE;
      
        sram_mem[5156] =
        WHITE;
      
        sram_mem[5157] =
        WHITE;
      
        sram_mem[5158] =
        WHITE;
      
        sram_mem[5159] =
        WHITE;
      
        sram_mem[5160] =
        WHITE;
      
        sram_mem[5161] =
        WHITE;
      
    
      
        sram_mem[5162] =
        WHITE;
      
        sram_mem[5163] =
        WHITE;
      
        sram_mem[5164] =
        NUMBER(1);
      
        sram_mem[5165] =
        WHITE;
      
        sram_mem[5166] =
        WHITE;
      
        sram_mem[5167] =
        WHITE;
      
        sram_mem[5168] =
        WHITE;
      
    
  
    sram_mem[12] = 7;
    sram_mem[13] = 7;
    
      
        sram_mem[6144] =
        NUMBER(2);
      
        sram_mem[6145] =
        WHITE;
      
        sram_mem[6146] =
        WHITE;
      
        sram_mem[6147] =
        NUMBER(2);
      
        sram_mem[6148] =
        WHITE;
      
        sram_mem[6149] =
        NUMBER(2);
      
        sram_mem[6150] =
        WHITE;
      
    
      
        sram_mem[6151] =
        WHITE;
      
        sram_mem[6152] =
        WHITE;
      
        sram_mem[6153] =
        WHITE;
      
        sram_mem[6154] =
        WHITE;
      
        sram_mem[6155] =
        WHITE;
      
        sram_mem[6156] =
        WHITE;
      
        sram_mem[6157] =
        WHITE;
      
    
      
        sram_mem[6158] =
        NUMBER(5);
      
        sram_mem[6159] =
        WHITE;
      
        sram_mem[6160] =
        WHITE;
      
        sram_mem[6161] =
        WHITE;
      
        sram_mem[6162] =
        NUMBER(2);
      
        sram_mem[6163] =
        WHITE;
      
        sram_mem[6164] =
        WHITE;
      
    
      
        sram_mem[6165] =
        WHITE;
      
        sram_mem[6166] =
        WHITE;
      
        sram_mem[6167] =
        WHITE;
      
        sram_mem[6168] =
        WHITE;
      
        sram_mem[6169] =
        WHITE;
      
        sram_mem[6170] =
        WHITE;
      
        sram_mem[6171] =
        WHITE;
      
    
      
        sram_mem[6172] =
        WHITE;
      
        sram_mem[6173] =
        WHITE;
      
        sram_mem[6174] =
        WHITE;
      
        sram_mem[6175] =
        NUMBER(3);
      
        sram_mem[6176] =
        WHITE;
      
        sram_mem[6177] =
        WHITE;
      
        sram_mem[6178] =
        WHITE;
      
    
      
        sram_mem[6179] =
        NUMBER(2);
      
        sram_mem[6180] =
        WHITE;
      
        sram_mem[6181] =
        NUMBER(1);
      
        sram_mem[6182] =
        WHITE;
      
        sram_mem[6183] =
        WHITE;
      
        sram_mem[6184] =
        WHITE;
      
        sram_mem[6185] =
        WHITE;
      
    
      
        sram_mem[6186] =
        WHITE;
      
        sram_mem[6187] =
        WHITE;
      
        sram_mem[6188] =
        WHITE;
      
        sram_mem[6189] =
        WHITE;
      
        sram_mem[6190] =
        NUMBER(2);
      
        sram_mem[6191] =
        WHITE;
      
        sram_mem[6192] =
        WHITE;
      
    
  
    sram_mem[14] = 10;
    sram_mem[15] = 10;
    
      
        sram_mem[7168] =
        NUMBER(3);
      
        sram_mem[7169] =
        WHITE;
      
        sram_mem[7170] =
        NUMBER(3);
      
        sram_mem[7171] =
        WHITE;
      
        sram_mem[7172] =
        WHITE;
      
        sram_mem[7173] =
        NUMBER(3);
      
        sram_mem[7174] =
        WHITE;
      
        sram_mem[7175] =
        NUMBER(2);
      
        sram_mem[7176] =
        WHITE;
      
        sram_mem[7177] =
        NUMBER(4);
      
    
      
        sram_mem[7178] =
        WHITE;
      
        sram_mem[7179] =
        WHITE;
      
        sram_mem[7180] =
        WHITE;
      
        sram_mem[7181] =
        WHITE;
      
        sram_mem[7182] =
        WHITE;
      
        sram_mem[7183] =
        WHITE;
      
        sram_mem[7184] =
        WHITE;
      
        sram_mem[7185] =
        WHITE;
      
        sram_mem[7186] =
        WHITE;
      
        sram_mem[7187] =
        WHITE;
      
    
      
        sram_mem[7188] =
        WHITE;
      
        sram_mem[7189] =
        WHITE;
      
        sram_mem[7190] =
        NUMBER(3);
      
        sram_mem[7191] =
        WHITE;
      
        sram_mem[7192] =
        WHITE;
      
        sram_mem[7193] =
        WHITE;
      
        sram_mem[7194] =
        WHITE;
      
        sram_mem[7195] =
        WHITE;
      
        sram_mem[7196] =
        WHITE;
      
        sram_mem[7197] =
        WHITE;
      
    
      
        sram_mem[7198] =
        WHITE;
      
        sram_mem[7199] =
        WHITE;
      
        sram_mem[7200] =
        WHITE;
      
        sram_mem[7201] =
        WHITE;
      
        sram_mem[7202] =
        NUMBER(2);
      
        sram_mem[7203] =
        WHITE;
      
        sram_mem[7204] =
        NUMBER(7);
      
        sram_mem[7205] =
        WHITE;
      
        sram_mem[7206] =
        WHITE;
      
        sram_mem[7207] =
        WHITE;
      
    
      
        sram_mem[7208] =
        NUMBER(1);
      
        sram_mem[7209] =
        WHITE;
      
        sram_mem[7210] =
        WHITE;
      
        sram_mem[7211] =
        WHITE;
      
        sram_mem[7212] =
        WHITE;
      
        sram_mem[7213] =
        WHITE;
      
        sram_mem[7214] =
        WHITE;
      
        sram_mem[7215] =
        WHITE;
      
        sram_mem[7216] =
        WHITE;
      
        sram_mem[7217] =
        WHITE;
      
    
      
        sram_mem[7218] =
        WHITE;
      
        sram_mem[7219] =
        WHITE;
      
        sram_mem[7220] =
        WHITE;
      
        sram_mem[7221] =
        WHITE;
      
        sram_mem[7222] =
        WHITE;
      
        sram_mem[7223] =
        NUMBER(2);
      
        sram_mem[7224] =
        WHITE;
      
        sram_mem[7225] =
        WHITE;
      
        sram_mem[7226] =
        WHITE;
      
        sram_mem[7227] =
        WHITE;
      
    
      
        sram_mem[7228] =
        NUMBER(4);
      
        sram_mem[7229] =
        WHITE;
      
        sram_mem[7230] =
        NUMBER(4);
      
        sram_mem[7231] =
        WHITE;
      
        sram_mem[7232] =
        WHITE;
      
        sram_mem[7233] =
        WHITE;
      
        sram_mem[7234] =
        WHITE;
      
        sram_mem[7235] =
        WHITE;
      
        sram_mem[7236] =
        WHITE;
      
        sram_mem[7237] =
        WHITE;
      
    
      
        sram_mem[7238] =
        WHITE;
      
        sram_mem[7239] =
        WHITE;
      
        sram_mem[7240] =
        WHITE;
      
        sram_mem[7241] =
        WHITE;
      
        sram_mem[7242] =
        WHITE;
      
        sram_mem[7243] =
        WHITE;
      
        sram_mem[7244] =
        NUMBER(2);
      
        sram_mem[7245] =
        WHITE;
      
        sram_mem[7246] =
        WHITE;
      
        sram_mem[7247] =
        WHITE;
      
    
      
        sram_mem[7248] =
        WHITE;
      
        sram_mem[7249] =
        WHITE;
      
        sram_mem[7250] =
        WHITE;
      
        sram_mem[7251] =
        WHITE;
      
        sram_mem[7252] =
        WHITE;
      
        sram_mem[7253] =
        WHITE;
      
        sram_mem[7254] =
        WHITE;
      
        sram_mem[7255] =
        WHITE;
      
        sram_mem[7256] =
        WHITE;
      
        sram_mem[7257] =
        WHITE;
      
    
      
        sram_mem[7258] =
        WHITE;
      
        sram_mem[7259] =
        WHITE;
      
        sram_mem[7260] =
        WHITE;
      
        sram_mem[7261] =
        NUMBER(2);
      
        sram_mem[7262] =
        WHITE;
      
        sram_mem[7263] =
        WHITE;
      
        sram_mem[7264] =
        WHITE;
      
        sram_mem[7265] =
        WHITE;
      
        sram_mem[7266] =
        NUMBER(1);
      
        sram_mem[7267] =
        WHITE;
      
    
  
    sram_mem[16] = 10;
    sram_mem[17] = 10;
    
      
        sram_mem[8192] =
        WHITE;
      
        sram_mem[8193] =
        WHITE;
      
        sram_mem[8194] =
        WHITE;
      
        sram_mem[8195] =
        NUMBER(4);
      
        sram_mem[8196] =
        WHITE;
      
        sram_mem[8197] =
        WHITE;
      
        sram_mem[8198] =
        NUMBER(2);
      
        sram_mem[8199] =
        WHITE;
      
        sram_mem[8200] =
        NUMBER(4);
      
        sram_mem[8201] =
        WHITE;
      
    
      
        sram_mem[8202] =
        NUMBER(3);
      
        sram_mem[8203] =
        WHITE;
      
        sram_mem[8204] =
        WHITE;
      
        sram_mem[8205] =
        WHITE;
      
        sram_mem[8206] =
        WHITE;
      
        sram_mem[8207] =
        WHITE;
      
        sram_mem[8208] =
        WHITE;
      
        sram_mem[8209] =
        WHITE;
      
        sram_mem[8210] =
        WHITE;
      
        sram_mem[8211] =
        WHITE;
      
    
      
        sram_mem[8212] =
        WHITE;
      
        sram_mem[8213] =
        WHITE;
      
        sram_mem[8214] =
        WHITE;
      
        sram_mem[8215] =
        WHITE;
      
        sram_mem[8216] =
        WHITE;
      
        sram_mem[8217] =
        WHITE;
      
        sram_mem[8218] =
        WHITE;
      
        sram_mem[8219] =
        WHITE;
      
        sram_mem[8220] =
        WHITE;
      
        sram_mem[8221] =
        WHITE;
      
    
      
        sram_mem[8222] =
        NUMBER(5);
      
        sram_mem[8223] =
        WHITE;
      
        sram_mem[8224] =
        NUMBER(1);
      
        sram_mem[8225] =
        WHITE;
      
        sram_mem[8226] =
        WHITE;
      
        sram_mem[8227] =
        WHITE;
      
        sram_mem[8228] =
        NUMBER(3);
      
        sram_mem[8229] =
        WHITE;
      
        sram_mem[8230] =
        WHITE;
      
        sram_mem[8231] =
        WHITE;
      
    
      
        sram_mem[8232] =
        WHITE;
      
        sram_mem[8233] =
        WHITE;
      
        sram_mem[8234] =
        WHITE;
      
        sram_mem[8235] =
        NUMBER(1);
      
        sram_mem[8236] =
        WHITE;
      
        sram_mem[8237] =
        NUMBER(4);
      
        sram_mem[8238] =
        WHITE;
      
        sram_mem[8239] =
        WHITE;
      
        sram_mem[8240] =
        NUMBER(3);
      
        sram_mem[8241] =
        WHITE;
      
    
      
        sram_mem[8242] =
        WHITE;
      
        sram_mem[8243] =
        WHITE;
      
        sram_mem[8244] =
        WHITE;
      
        sram_mem[8245] =
        WHITE;
      
        sram_mem[8246] =
        WHITE;
      
        sram_mem[8247] =
        WHITE;
      
        sram_mem[8248] =
        WHITE;
      
        sram_mem[8249] =
        WHITE;
      
        sram_mem[8250] =
        WHITE;
      
        sram_mem[8251] =
        WHITE;
      
    
      
        sram_mem[8252] =
        WHITE;
      
        sram_mem[8253] =
        WHITE;
      
        sram_mem[8254] =
        NUMBER(4);
      
        sram_mem[8255] =
        WHITE;
      
        sram_mem[8256] =
        WHITE;
      
        sram_mem[8257] =
        WHITE;
      
        sram_mem[8258] =
        WHITE;
      
        sram_mem[8259] =
        WHITE;
      
        sram_mem[8260] =
        WHITE;
      
        sram_mem[8261] =
        WHITE;
      
    
      
        sram_mem[8262] =
        NUMBER(3);
      
        sram_mem[8263] =
        WHITE;
      
        sram_mem[8264] =
        WHITE;
      
        sram_mem[8265] =
        WHITE;
      
        sram_mem[8266] =
        WHITE;
      
        sram_mem[8267] =
        WHITE;
      
        sram_mem[8268] =
        WHITE;
      
        sram_mem[8269] =
        WHITE;
      
        sram_mem[8270] =
        WHITE;
      
        sram_mem[8271] =
        WHITE;
      
    
      
        sram_mem[8272] =
        WHITE;
      
        sram_mem[8273] =
        WHITE;
      
        sram_mem[8274] =
        WHITE;
      
        sram_mem[8275] =
        WHITE;
      
        sram_mem[8276] =
        WHITE;
      
        sram_mem[8277] =
        NUMBER(3);
      
        sram_mem[8278] =
        WHITE;
      
        sram_mem[8279] =
        WHITE;
      
        sram_mem[8280] =
        WHITE;
      
        sram_mem[8281] =
        NUMBER(1);
      
    
      
        sram_mem[8282] =
        WHITE;
      
        sram_mem[8283] =
        WHITE;
      
        sram_mem[8284] =
        WHITE;
      
        sram_mem[8285] =
        NUMBER(1);
      
        sram_mem[8286] =
        WHITE;
      
        sram_mem[8287] =
        WHITE;
      
        sram_mem[8288] =
        WHITE;
      
        sram_mem[8289] =
        WHITE;
      
        sram_mem[8290] =
        WHITE;
      
        sram_mem[8291] =
        WHITE;
      
    
  
    sram_mem[18] = 10;
    sram_mem[19] = 10;
    
      
        sram_mem[9216] =
        NUMBER(3);
      
        sram_mem[9217] =
        WHITE;
      
        sram_mem[9218] =
        NUMBER(1);
      
        sram_mem[9219] =
        WHITE;
      
        sram_mem[9220] =
        NUMBER(3);
      
        sram_mem[9221] =
        WHITE;
      
        sram_mem[9222] =
        WHITE;
      
        sram_mem[9223] =
        WHITE;
      
        sram_mem[9224] =
        NUMBER(5);
      
        sram_mem[9225] =
        WHITE;
      
    
      
        sram_mem[9226] =
        WHITE;
      
        sram_mem[9227] =
        WHITE;
      
        sram_mem[9228] =
        WHITE;
      
        sram_mem[9229] =
        WHITE;
      
        sram_mem[9230] =
        WHITE;
      
        sram_mem[9231] =
        WHITE;
      
        sram_mem[9232] =
        WHITE;
      
        sram_mem[9233] =
        WHITE;
      
        sram_mem[9234] =
        WHITE;
      
        sram_mem[9235] =
        WHITE;
      
    
      
        sram_mem[9236] =
        WHITE;
      
        sram_mem[9237] =
        WHITE;
      
        sram_mem[9238] =
        NUMBER(1);
      
        sram_mem[9239] =
        WHITE;
      
        sram_mem[9240] =
        NUMBER(6);
      
        sram_mem[9241] =
        WHITE;
      
        sram_mem[9242] =
        WHITE;
      
        sram_mem[9243] =
        WHITE;
      
        sram_mem[9244] =
        WHITE;
      
        sram_mem[9245] =
        WHITE;
      
    
      
        sram_mem[9246] =
        WHITE;
      
        sram_mem[9247] =
        NUMBER(2);
      
        sram_mem[9248] =
        WHITE;
      
        sram_mem[9249] =
        NUMBER(11);
      
        sram_mem[9250] =
        WHITE;
      
        sram_mem[9251] =
        WHITE;
      
        sram_mem[9252] =
        WHITE;
      
        sram_mem[9253] =
        WHITE;
      
        sram_mem[9254] =
        WHITE;
      
        sram_mem[9255] =
        WHITE;
      
    
      
        sram_mem[9256] =
        WHITE;
      
        sram_mem[9257] =
        WHITE;
      
        sram_mem[9258] =
        WHITE;
      
        sram_mem[9259] =
        WHITE;
      
        sram_mem[9260] =
        WHITE;
      
        sram_mem[9261] =
        WHITE;
      
        sram_mem[9262] =
        WHITE;
      
        sram_mem[9263] =
        WHITE;
      
        sram_mem[9264] =
        WHITE;
      
        sram_mem[9265] =
        NUMBER(1);
      
    
      
        sram_mem[9266] =
        WHITE;
      
        sram_mem[9267] =
        WHITE;
      
        sram_mem[9268] =
        WHITE;
      
        sram_mem[9269] =
        WHITE;
      
        sram_mem[9270] =
        WHITE;
      
        sram_mem[9271] =
        WHITE;
      
        sram_mem[9272] =
        WHITE;
      
        sram_mem[9273] =
        WHITE;
      
        sram_mem[9274] =
        WHITE;
      
        sram_mem[9275] =
        WHITE;
      
    
      
        sram_mem[9276] =
        NUMBER(3);
      
        sram_mem[9277] =
        WHITE;
      
        sram_mem[9278] =
        NUMBER(1);
      
        sram_mem[9279] =
        WHITE;
      
        sram_mem[9280] =
        WHITE;
      
        sram_mem[9281] =
        WHITE;
      
        sram_mem[9282] =
        WHITE;
      
        sram_mem[9283] =
        NUMBER(3);
      
        sram_mem[9284] =
        WHITE;
      
        sram_mem[9285] =
        WHITE;
      
    
      
        sram_mem[9286] =
        WHITE;
      
        sram_mem[9287] =
        WHITE;
      
        sram_mem[9288] =
        WHITE;
      
        sram_mem[9289] =
        NUMBER(2);
      
        sram_mem[9290] =
        WHITE;
      
        sram_mem[9291] =
        WHITE;
      
        sram_mem[9292] =
        WHITE;
      
        sram_mem[9293] =
        WHITE;
      
        sram_mem[9294] =
        WHITE;
      
        sram_mem[9295] =
        WHITE;
      
    
      
        sram_mem[9296] =
        WHITE;
      
        sram_mem[9297] =
        WHITE;
      
        sram_mem[9298] =
        WHITE;
      
        sram_mem[9299] =
        WHITE;
      
        sram_mem[9300] =
        WHITE;
      
        sram_mem[9301] =
        WHITE;
      
        sram_mem[9302] =
        WHITE;
      
        sram_mem[9303] =
        WHITE;
      
        sram_mem[9304] =
        WHITE;
      
        sram_mem[9305] =
        WHITE;
      
    
      
        sram_mem[9306] =
        NUMBER(2);
      
        sram_mem[9307] =
        WHITE;
      
        sram_mem[9308] =
        WHITE;
      
        sram_mem[9309] =
        WHITE;
      
        sram_mem[9310] =
        WHITE;
      
        sram_mem[9311] =
        WHITE;
      
        sram_mem[9312] =
        WHITE;
      
        sram_mem[9313] =
        WHITE;
      
        sram_mem[9314] =
        WHITE;
      
        sram_mem[9315] =
        WHITE;
      
    
  
    sram_mem[20] = 12;
    sram_mem[21] = 12;
    
      
        sram_mem[10240] =
        NUMBER(7);
      
        sram_mem[10241] =
        WHITE;
      
        sram_mem[10242] =
        NUMBER(2);
      
        sram_mem[10243] =
        WHITE;
      
        sram_mem[10244] =
        WHITE;
      
        sram_mem[10245] =
        NUMBER(2);
      
        sram_mem[10246] =
        WHITE;
      
        sram_mem[10247] =
        NUMBER(3);
      
        sram_mem[10248] =
        WHITE;
      
        sram_mem[10249] =
        WHITE;
      
        sram_mem[10250] =
        NUMBER(1);
      
        sram_mem[10251] =
        WHITE;
      
    
      
        sram_mem[10252] =
        WHITE;
      
        sram_mem[10253] =
        WHITE;
      
        sram_mem[10254] =
        WHITE;
      
        sram_mem[10255] =
        WHITE;
      
        sram_mem[10256] =
        WHITE;
      
        sram_mem[10257] =
        WHITE;
      
        sram_mem[10258] =
        WHITE;
      
        sram_mem[10259] =
        WHITE;
      
        sram_mem[10260] =
        WHITE;
      
        sram_mem[10261] =
        WHITE;
      
        sram_mem[10262] =
        WHITE;
      
        sram_mem[10263] =
        WHITE;
      
    
      
        sram_mem[10264] =
        WHITE;
      
        sram_mem[10265] =
        WHITE;
      
        sram_mem[10266] =
        WHITE;
      
        sram_mem[10267] =
        WHITE;
      
        sram_mem[10268] =
        WHITE;
      
        sram_mem[10269] =
        WHITE;
      
        sram_mem[10270] =
        NUMBER(1);
      
        sram_mem[10271] =
        WHITE;
      
        sram_mem[10272] =
        WHITE;
      
        sram_mem[10273] =
        NUMBER(2);
      
        sram_mem[10274] =
        WHITE;
      
        sram_mem[10275] =
        WHITE;
      
    
      
        sram_mem[10276] =
        WHITE;
      
        sram_mem[10277] =
        WHITE;
      
        sram_mem[10278] =
        WHITE;
      
        sram_mem[10279] =
        WHITE;
      
        sram_mem[10280] =
        WHITE;
      
        sram_mem[10281] =
        NUMBER(3);
      
        sram_mem[10282] =
        WHITE;
      
        sram_mem[10283] =
        WHITE;
      
        sram_mem[10284] =
        NUMBER(2);
      
        sram_mem[10285] =
        WHITE;
      
        sram_mem[10286] =
        WHITE;
      
        sram_mem[10287] =
        WHITE;
      
    
      
        sram_mem[10288] =
        NUMBER(2);
      
        sram_mem[10289] =
        WHITE;
      
        sram_mem[10290] =
        WHITE;
      
        sram_mem[10291] =
        WHITE;
      
        sram_mem[10292] =
        WHITE;
      
        sram_mem[10293] =
        WHITE;
      
        sram_mem[10294] =
        WHITE;
      
        sram_mem[10295] =
        WHITE;
      
        sram_mem[10296] =
        WHITE;
      
        sram_mem[10297] =
        WHITE;
      
        sram_mem[10298] =
        NUMBER(3);
      
        sram_mem[10299] =
        WHITE;
      
    
      
        sram_mem[10300] =
        WHITE;
      
        sram_mem[10301] =
        WHITE;
      
        sram_mem[10302] =
        NUMBER(3);
      
        sram_mem[10303] =
        WHITE;
      
        sram_mem[10304] =
        WHITE;
      
        sram_mem[10305] =
        WHITE;
      
        sram_mem[10306] =
        WHITE;
      
        sram_mem[10307] =
        WHITE;
      
        sram_mem[10308] =
        WHITE;
      
        sram_mem[10309] =
        WHITE;
      
        sram_mem[10310] =
        WHITE;
      
        sram_mem[10311] =
        WHITE;
      
    
      
        sram_mem[10312] =
        NUMBER(3);
      
        sram_mem[10313] =
        WHITE;
      
        sram_mem[10314] =
        WHITE;
      
        sram_mem[10315] =
        WHITE;
      
        sram_mem[10316] =
        WHITE;
      
        sram_mem[10317] =
        NUMBER(3);
      
        sram_mem[10318] =
        WHITE;
      
        sram_mem[10319] =
        WHITE;
      
        sram_mem[10320] =
        NUMBER(1);
      
        sram_mem[10321] =
        WHITE;
      
        sram_mem[10322] =
        NUMBER(3);
      
        sram_mem[10323] =
        WHITE;
      
    
      
        sram_mem[10324] =
        WHITE;
      
        sram_mem[10325] =
        WHITE;
      
        sram_mem[10326] =
        NUMBER(3);
      
        sram_mem[10327] =
        WHITE;
      
        sram_mem[10328] =
        WHITE;
      
        sram_mem[10329] =
        WHITE;
      
        sram_mem[10330] =
        WHITE;
      
        sram_mem[10331] =
        WHITE;
      
        sram_mem[10332] =
        WHITE;
      
        sram_mem[10333] =
        WHITE;
      
        sram_mem[10334] =
        WHITE;
      
        sram_mem[10335] =
        WHITE;
      
    
      
        sram_mem[10336] =
        WHITE;
      
        sram_mem[10337] =
        WHITE;
      
        sram_mem[10338] =
        WHITE;
      
        sram_mem[10339] =
        WHITE;
      
        sram_mem[10340] =
        WHITE;
      
        sram_mem[10341] =
        NUMBER(2);
      
        sram_mem[10342] =
        WHITE;
      
        sram_mem[10343] =
        NUMBER(6);
      
        sram_mem[10344] =
        WHITE;
      
        sram_mem[10345] =
        WHITE;
      
        sram_mem[10346] =
        WHITE;
      
        sram_mem[10347] =
        WHITE;
      
    
      
        sram_mem[10348] =
        WHITE;
      
        sram_mem[10349] =
        WHITE;
      
        sram_mem[10350] =
        WHITE;
      
        sram_mem[10351] =
        WHITE;
      
        sram_mem[10352] =
        WHITE;
      
        sram_mem[10353] =
        WHITE;
      
        sram_mem[10354] =
        WHITE;
      
        sram_mem[10355] =
        WHITE;
      
        sram_mem[10356] =
        WHITE;
      
        sram_mem[10357] =
        WHITE;
      
        sram_mem[10358] =
        WHITE;
      
        sram_mem[10359] =
        WHITE;
      
    
      
        sram_mem[10360] =
        NUMBER(5);
      
        sram_mem[10361] =
        WHITE;
      
        sram_mem[10362] =
        WHITE;
      
        sram_mem[10363] =
        NUMBER(2);
      
        sram_mem[10364] =
        WHITE;
      
        sram_mem[10365] =
        WHITE;
      
        sram_mem[10366] =
        NUMBER(1);
      
        sram_mem[10367] =
        WHITE;
      
        sram_mem[10368] =
        WHITE;
      
        sram_mem[10369] =
        WHITE;
      
        sram_mem[10370] =
        WHITE;
      
        sram_mem[10371] =
        WHITE;
      
    
      
        sram_mem[10372] =
        WHITE;
      
        sram_mem[10373] =
        WHITE;
      
        sram_mem[10374] =
        WHITE;
      
        sram_mem[10375] =
        WHITE;
      
        sram_mem[10376] =
        WHITE;
      
        sram_mem[10377] =
        WHITE;
      
        sram_mem[10378] =
        WHITE;
      
        sram_mem[10379] =
        WHITE;
      
        sram_mem[10380] =
        WHITE;
      
        sram_mem[10381] =
        WHITE;
      
        sram_mem[10382] =
        WHITE;
      
        sram_mem[10383] =
        WHITE;
      
    
  
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
