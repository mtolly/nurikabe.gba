#include <string.h>
#include <tonc.h>

int main() {

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
  pal_bg_mem[0] = CLR_MAG;
pal_bg_mem[1] = CLR_BLACK;
pal_bg_mem[2] = CLR_WHITE;
pal_bg_mem[3] = CLR_BLUE;
pal_bg_mem[4] = RGB15(8,8,8);
pal_bg_mem[5] = CLR_RED;

  // Load tiles into CBB 0
  
  tile_mem[0][0].data[0] = 0x11111111;
tile_mem[0][0].data[1] = 0x22222221;
tile_mem[0][0].data[2] = 0x22222221;
tile_mem[0][0].data[3] = 0x22222221;
tile_mem[0][0].data[4] = 0x22222221;
tile_mem[0][0].data[5] = 0x22222221;
tile_mem[0][0].data[6] = 0x22222221;
tile_mem[0][0].data[7] = 0x22222221;tile_mem[0][32].data[0] = 0x11111111;
tile_mem[0][32].data[1] = 0x55222551;
tile_mem[0][32].data[2] = 0x52222251;
tile_mem[0][32].data[3] = 0x22222221;
tile_mem[0][32].data[4] = 0x22222221;
tile_mem[0][32].data[5] = 0x22222221;
tile_mem[0][32].data[6] = 0x52222251;
tile_mem[0][32].data[7] = 0x55222551;
  
    tile_mem[0][1].data[0] = 0x11111111;
tile_mem[0][1].data[1] = 0x22222221;
tile_mem[0][1].data[2] = 0x22212221;
tile_mem[0][1].data[3] = 0x22212221;
tile_mem[0][1].data[4] = 0x22212221;
tile_mem[0][1].data[5] = 0x22212221;
tile_mem[0][1].data[6] = 0x22212221;
tile_mem[0][1].data[7] = 0x22222221;tile_mem[0][33].data[0] = 0x11111111;
tile_mem[0][33].data[1] = 0x55222551;
tile_mem[0][33].data[2] = 0x52212251;
tile_mem[0][33].data[3] = 0x22212221;
tile_mem[0][33].data[4] = 0x22212221;
tile_mem[0][33].data[5] = 0x22212221;
tile_mem[0][33].data[6] = 0x52212251;
tile_mem[0][33].data[7] = 0x55222551;
  
    tile_mem[0][2].data[0] = 0x11111111;
tile_mem[0][2].data[1] = 0x22222221;
tile_mem[0][2].data[2] = 0x21111121;
tile_mem[0][2].data[3] = 0x21222221;
tile_mem[0][2].data[4] = 0x21111121;
tile_mem[0][2].data[5] = 0x22222121;
tile_mem[0][2].data[6] = 0x21111121;
tile_mem[0][2].data[7] = 0x22222221;tile_mem[0][34].data[0] = 0x11111111;
tile_mem[0][34].data[1] = 0x55222551;
tile_mem[0][34].data[2] = 0x51111151;
tile_mem[0][34].data[3] = 0x21222221;
tile_mem[0][34].data[4] = 0x21111121;
tile_mem[0][34].data[5] = 0x22222121;
tile_mem[0][34].data[6] = 0x51111151;
tile_mem[0][34].data[7] = 0x55222551;
  
    tile_mem[0][3].data[0] = 0x11111111;
tile_mem[0][3].data[1] = 0x22222221;
tile_mem[0][3].data[2] = 0x21111121;
tile_mem[0][3].data[3] = 0x21222221;
tile_mem[0][3].data[4] = 0x21111121;
tile_mem[0][3].data[5] = 0x21222221;
tile_mem[0][3].data[6] = 0x21111121;
tile_mem[0][3].data[7] = 0x22222221;tile_mem[0][35].data[0] = 0x11111111;
tile_mem[0][35].data[1] = 0x55222551;
tile_mem[0][35].data[2] = 0x51111151;
tile_mem[0][35].data[3] = 0x21222221;
tile_mem[0][35].data[4] = 0x21111121;
tile_mem[0][35].data[5] = 0x21222221;
tile_mem[0][35].data[6] = 0x51111151;
tile_mem[0][35].data[7] = 0x55222551;
  
    tile_mem[0][4].data[0] = 0x11111111;
tile_mem[0][4].data[1] = 0x22222221;
tile_mem[0][4].data[2] = 0x21222121;
tile_mem[0][4].data[3] = 0x21222121;
tile_mem[0][4].data[4] = 0x21111121;
tile_mem[0][4].data[5] = 0x21222221;
tile_mem[0][4].data[6] = 0x21222221;
tile_mem[0][4].data[7] = 0x22222221;tile_mem[0][36].data[0] = 0x11111111;
tile_mem[0][36].data[1] = 0x55222551;
tile_mem[0][36].data[2] = 0x51222151;
tile_mem[0][36].data[3] = 0x21222121;
tile_mem[0][36].data[4] = 0x21111121;
tile_mem[0][36].data[5] = 0x21222221;
tile_mem[0][36].data[6] = 0x51222251;
tile_mem[0][36].data[7] = 0x55222551;
  
    tile_mem[0][5].data[0] = 0x11111111;
tile_mem[0][5].data[1] = 0x22222221;
tile_mem[0][5].data[2] = 0x21111121;
tile_mem[0][5].data[3] = 0x22222121;
tile_mem[0][5].data[4] = 0x21111121;
tile_mem[0][5].data[5] = 0x21222221;
tile_mem[0][5].data[6] = 0x21111121;
tile_mem[0][5].data[7] = 0x22222221;tile_mem[0][37].data[0] = 0x11111111;
tile_mem[0][37].data[1] = 0x55222551;
tile_mem[0][37].data[2] = 0x51111151;
tile_mem[0][37].data[3] = 0x22222121;
tile_mem[0][37].data[4] = 0x21111121;
tile_mem[0][37].data[5] = 0x21222221;
tile_mem[0][37].data[6] = 0x51111151;
tile_mem[0][37].data[7] = 0x55222551;
  
    tile_mem[0][6].data[0] = 0x11111111;
tile_mem[0][6].data[1] = 0x22222221;
tile_mem[0][6].data[2] = 0x21111121;
tile_mem[0][6].data[3] = 0x22222121;
tile_mem[0][6].data[4] = 0x21111121;
tile_mem[0][6].data[5] = 0x21222121;
tile_mem[0][6].data[6] = 0x21111121;
tile_mem[0][6].data[7] = 0x22222221;tile_mem[0][38].data[0] = 0x11111111;
tile_mem[0][38].data[1] = 0x55222551;
tile_mem[0][38].data[2] = 0x51111151;
tile_mem[0][38].data[3] = 0x22222121;
tile_mem[0][38].data[4] = 0x21111121;
tile_mem[0][38].data[5] = 0x21222121;
tile_mem[0][38].data[6] = 0x51111151;
tile_mem[0][38].data[7] = 0x55222551;
  
    tile_mem[0][7].data[0] = 0x11111111;
tile_mem[0][7].data[1] = 0x22222221;
tile_mem[0][7].data[2] = 0x21111121;
tile_mem[0][7].data[3] = 0x21222221;
tile_mem[0][7].data[4] = 0x21222221;
tile_mem[0][7].data[5] = 0x21222221;
tile_mem[0][7].data[6] = 0x21222221;
tile_mem[0][7].data[7] = 0x22222221;tile_mem[0][39].data[0] = 0x11111111;
tile_mem[0][39].data[1] = 0x55222551;
tile_mem[0][39].data[2] = 0x51111151;
tile_mem[0][39].data[3] = 0x21222221;
tile_mem[0][39].data[4] = 0x21222221;
tile_mem[0][39].data[5] = 0x21222221;
tile_mem[0][39].data[6] = 0x51222251;
tile_mem[0][39].data[7] = 0x55222551;
  
    tile_mem[0][8].data[0] = 0x11111111;
tile_mem[0][8].data[1] = 0x22222221;
tile_mem[0][8].data[2] = 0x21111121;
tile_mem[0][8].data[3] = 0x21222121;
tile_mem[0][8].data[4] = 0x21111121;
tile_mem[0][8].data[5] = 0x21222121;
tile_mem[0][8].data[6] = 0x21111121;
tile_mem[0][8].data[7] = 0x22222221;tile_mem[0][40].data[0] = 0x11111111;
tile_mem[0][40].data[1] = 0x55222551;
tile_mem[0][40].data[2] = 0x51111151;
tile_mem[0][40].data[3] = 0x21222121;
tile_mem[0][40].data[4] = 0x21111121;
tile_mem[0][40].data[5] = 0x21222121;
tile_mem[0][40].data[6] = 0x51111151;
tile_mem[0][40].data[7] = 0x55222551;
  
    tile_mem[0][9].data[0] = 0x11111111;
tile_mem[0][9].data[1] = 0x22222221;
tile_mem[0][9].data[2] = 0x21111121;
tile_mem[0][9].data[3] = 0x21222121;
tile_mem[0][9].data[4] = 0x21111121;
tile_mem[0][9].data[5] = 0x21222221;
tile_mem[0][9].data[6] = 0x21111121;
tile_mem[0][9].data[7] = 0x22222221;tile_mem[0][41].data[0] = 0x11111111;
tile_mem[0][41].data[1] = 0x55222551;
tile_mem[0][41].data[2] = 0x51111151;
tile_mem[0][41].data[3] = 0x21222121;
tile_mem[0][41].data[4] = 0x21111121;
tile_mem[0][41].data[5] = 0x21222221;
tile_mem[0][41].data[6] = 0x51111151;
tile_mem[0][41].data[7] = 0x55222551;
  
    tile_mem[0][10].data[0] = 0x11111111;
tile_mem[0][10].data[1] = 0x22222221;
tile_mem[0][10].data[2] = 0x21112121;
tile_mem[0][10].data[3] = 0x21212121;
tile_mem[0][10].data[4] = 0x21212121;
tile_mem[0][10].data[5] = 0x21212121;
tile_mem[0][10].data[6] = 0x21112121;
tile_mem[0][10].data[7] = 0x22222221;tile_mem[0][42].data[0] = 0x11111111;
tile_mem[0][42].data[1] = 0x55222551;
tile_mem[0][42].data[2] = 0x51112151;
tile_mem[0][42].data[3] = 0x21212121;
tile_mem[0][42].data[4] = 0x21212121;
tile_mem[0][42].data[5] = 0x21212121;
tile_mem[0][42].data[6] = 0x51112151;
tile_mem[0][42].data[7] = 0x55222551;
  
    tile_mem[0][11].data[0] = 0x11111111;
tile_mem[0][11].data[1] = 0x22222221;
tile_mem[0][11].data[2] = 0x22121221;
tile_mem[0][11].data[3] = 0x22121221;
tile_mem[0][11].data[4] = 0x22121221;
tile_mem[0][11].data[5] = 0x22121221;
tile_mem[0][11].data[6] = 0x22121221;
tile_mem[0][11].data[7] = 0x22222221;tile_mem[0][43].data[0] = 0x11111111;
tile_mem[0][43].data[1] = 0x55222551;
tile_mem[0][43].data[2] = 0x52121251;
tile_mem[0][43].data[3] = 0x22121221;
tile_mem[0][43].data[4] = 0x22121221;
tile_mem[0][43].data[5] = 0x22121221;
tile_mem[0][43].data[6] = 0x52121251;
tile_mem[0][43].data[7] = 0x55222551;
  
    tile_mem[0][12].data[0] = 0x11111111;
tile_mem[0][12].data[1] = 0x22222221;
tile_mem[0][12].data[2] = 0x21112121;
tile_mem[0][12].data[3] = 0x21222121;
tile_mem[0][12].data[4] = 0x21112121;
tile_mem[0][12].data[5] = 0x22212121;
tile_mem[0][12].data[6] = 0x21112121;
tile_mem[0][12].data[7] = 0x22222221;tile_mem[0][44].data[0] = 0x11111111;
tile_mem[0][44].data[1] = 0x55222551;
tile_mem[0][44].data[2] = 0x51112151;
tile_mem[0][44].data[3] = 0x21222121;
tile_mem[0][44].data[4] = 0x21112121;
tile_mem[0][44].data[5] = 0x22212121;
tile_mem[0][44].data[6] = 0x51112151;
tile_mem[0][44].data[7] = 0x55222551;
  
    tile_mem[0][13].data[0] = 0x11111111;
tile_mem[0][13].data[1] = 0x22222221;
tile_mem[0][13].data[2] = 0x21112121;
tile_mem[0][13].data[3] = 0x21222121;
tile_mem[0][13].data[4] = 0x21112121;
tile_mem[0][13].data[5] = 0x21222121;
tile_mem[0][13].data[6] = 0x21112121;
tile_mem[0][13].data[7] = 0x22222221;tile_mem[0][45].data[0] = 0x11111111;
tile_mem[0][45].data[1] = 0x55222551;
tile_mem[0][45].data[2] = 0x51112151;
tile_mem[0][45].data[3] = 0x21222121;
tile_mem[0][45].data[4] = 0x21112121;
tile_mem[0][45].data[5] = 0x21222121;
tile_mem[0][45].data[6] = 0x51112151;
tile_mem[0][45].data[7] = 0x55222551;
  
    tile_mem[0][14].data[0] = 0x11111111;
tile_mem[0][14].data[1] = 0x22222221;
tile_mem[0][14].data[2] = 0x21212121;
tile_mem[0][14].data[3] = 0x21212121;
tile_mem[0][14].data[4] = 0x21112121;
tile_mem[0][14].data[5] = 0x21222121;
tile_mem[0][14].data[6] = 0x21222121;
tile_mem[0][14].data[7] = 0x22222221;tile_mem[0][46].data[0] = 0x11111111;
tile_mem[0][46].data[1] = 0x55222551;
tile_mem[0][46].data[2] = 0x51212151;
tile_mem[0][46].data[3] = 0x21212121;
tile_mem[0][46].data[4] = 0x21112121;
tile_mem[0][46].data[5] = 0x21222121;
tile_mem[0][46].data[6] = 0x51222151;
tile_mem[0][46].data[7] = 0x55222551;
  
    tile_mem[0][15].data[0] = 0x11111111;
tile_mem[0][15].data[1] = 0x22222221;
tile_mem[0][15].data[2] = 0x21112121;
tile_mem[0][15].data[3] = 0x22212121;
tile_mem[0][15].data[4] = 0x21112121;
tile_mem[0][15].data[5] = 0x21222121;
tile_mem[0][15].data[6] = 0x21112121;
tile_mem[0][15].data[7] = 0x22222221;tile_mem[0][47].data[0] = 0x11111111;
tile_mem[0][47].data[1] = 0x55222551;
tile_mem[0][47].data[2] = 0x51112151;
tile_mem[0][47].data[3] = 0x22212121;
tile_mem[0][47].data[4] = 0x21112121;
tile_mem[0][47].data[5] = 0x21222121;
tile_mem[0][47].data[6] = 0x51112151;
tile_mem[0][47].data[7] = 0x55222551;
  
    tile_mem[0][16].data[0] = 0x11111111;
tile_mem[0][16].data[1] = 0x22222221;
tile_mem[0][16].data[2] = 0x21112121;
tile_mem[0][16].data[3] = 0x22212121;
tile_mem[0][16].data[4] = 0x21112121;
tile_mem[0][16].data[5] = 0x21212121;
tile_mem[0][16].data[6] = 0x21112121;
tile_mem[0][16].data[7] = 0x22222221;tile_mem[0][48].data[0] = 0x11111111;
tile_mem[0][48].data[1] = 0x55222551;
tile_mem[0][48].data[2] = 0x51112151;
tile_mem[0][48].data[3] = 0x22212121;
tile_mem[0][48].data[4] = 0x21112121;
tile_mem[0][48].data[5] = 0x21212121;
tile_mem[0][48].data[6] = 0x51112151;
tile_mem[0][48].data[7] = 0x55222551;
  
    tile_mem[0][17].data[0] = 0x11111111;
tile_mem[0][17].data[1] = 0x22222221;
tile_mem[0][17].data[2] = 0x21112121;
tile_mem[0][17].data[3] = 0x21222121;
tile_mem[0][17].data[4] = 0x21222121;
tile_mem[0][17].data[5] = 0x21222121;
tile_mem[0][17].data[6] = 0x21222121;
tile_mem[0][17].data[7] = 0x22222221;tile_mem[0][49].data[0] = 0x11111111;
tile_mem[0][49].data[1] = 0x55222551;
tile_mem[0][49].data[2] = 0x51112151;
tile_mem[0][49].data[3] = 0x21222121;
tile_mem[0][49].data[4] = 0x21222121;
tile_mem[0][49].data[5] = 0x21222121;
tile_mem[0][49].data[6] = 0x51222151;
tile_mem[0][49].data[7] = 0x55222551;
  
    tile_mem[0][18].data[0] = 0x11111111;
tile_mem[0][18].data[1] = 0x22222221;
tile_mem[0][18].data[2] = 0x21112121;
tile_mem[0][18].data[3] = 0x21212121;
tile_mem[0][18].data[4] = 0x21112121;
tile_mem[0][18].data[5] = 0x21212121;
tile_mem[0][18].data[6] = 0x21112121;
tile_mem[0][18].data[7] = 0x22222221;tile_mem[0][50].data[0] = 0x11111111;
tile_mem[0][50].data[1] = 0x55222551;
tile_mem[0][50].data[2] = 0x51112151;
tile_mem[0][50].data[3] = 0x21212121;
tile_mem[0][50].data[4] = 0x21112121;
tile_mem[0][50].data[5] = 0x21212121;
tile_mem[0][50].data[6] = 0x51112151;
tile_mem[0][50].data[7] = 0x55222551;
  
    tile_mem[0][19].data[0] = 0x11111111;
tile_mem[0][19].data[1] = 0x22222221;
tile_mem[0][19].data[2] = 0x21112121;
tile_mem[0][19].data[3] = 0x21212121;
tile_mem[0][19].data[4] = 0x21112121;
tile_mem[0][19].data[5] = 0x21222121;
tile_mem[0][19].data[6] = 0x21112121;
tile_mem[0][19].data[7] = 0x22222221;tile_mem[0][51].data[0] = 0x11111111;
tile_mem[0][51].data[1] = 0x55222551;
tile_mem[0][51].data[2] = 0x51112151;
tile_mem[0][51].data[3] = 0x21212121;
tile_mem[0][51].data[4] = 0x21112121;
tile_mem[0][51].data[5] = 0x21222121;
tile_mem[0][51].data[6] = 0x51112151;
tile_mem[0][51].data[7] = 0x55222551;
  
  tile_mem[0][26].data[0] = 0x11111111;
tile_mem[0][26].data[1] = 0x44444441;
tile_mem[0][26].data[2] = 0x44444441;
tile_mem[0][26].data[3] = 0x44444441;
tile_mem[0][26].data[4] = 0x44444441;
tile_mem[0][26].data[5] = 0x44444441;
tile_mem[0][26].data[6] = 0x44444441;
tile_mem[0][26].data[7] = 0x44444441;tile_mem[0][58].data[0] = 0x11111111;
tile_mem[0][58].data[1] = 0x55444551;
tile_mem[0][58].data[2] = 0x54444451;
tile_mem[0][58].data[3] = 0x44444441;
tile_mem[0][58].data[4] = 0x44444441;
tile_mem[0][58].data[5] = 0x44444441;
tile_mem[0][58].data[6] = 0x54444451;
tile_mem[0][58].data[7] = 0x55444551;
  tile_mem[0][27].data[0] = 0x11111111;
tile_mem[0][27].data[1] = 0x22222221;
tile_mem[0][27].data[2] = 0x22222221;
tile_mem[0][27].data[3] = 0x22242221;
tile_mem[0][27].data[4] = 0x22414221;
tile_mem[0][27].data[5] = 0x22242221;
tile_mem[0][27].data[6] = 0x22222221;
tile_mem[0][27].data[7] = 0x22222221;tile_mem[0][59].data[0] = 0x11111111;
tile_mem[0][59].data[1] = 0x55222551;
tile_mem[0][59].data[2] = 0x52222251;
tile_mem[0][59].data[3] = 0x22242221;
tile_mem[0][59].data[4] = 0x22414221;
tile_mem[0][59].data[5] = 0x22242221;
tile_mem[0][59].data[6] = 0x52222251;
tile_mem[0][59].data[7] = 0x55222551;
  tile_mem[0][28].data[0] = 0x33333333;
tile_mem[0][28].data[1] = 0x33333333;
tile_mem[0][28].data[2] = 0x33333333;
tile_mem[0][28].data[3] = 0x33333333;
tile_mem[0][28].data[4] = 0x33333333;
tile_mem[0][28].data[5] = 0x33333333;
tile_mem[0][28].data[6] = 0x33333333;
tile_mem[0][28].data[7] = 0x33333333;tile_mem[0][60].data[0] = 0x33333333;
tile_mem[0][60].data[1] = 0x55333553;
tile_mem[0][60].data[2] = 0x53333353;
tile_mem[0][60].data[3] = 0x33333333;
tile_mem[0][60].data[4] = 0x33333333;
tile_mem[0][60].data[5] = 0x33333333;
tile_mem[0][60].data[6] = 0x53333353;
tile_mem[0][60].data[7] = 0x55333553;
  tile_mem[0][29].data[0] = 0x11111111;
tile_mem[0][29].data[1] = 0x33333333;
tile_mem[0][29].data[2] = 0x33333333;
tile_mem[0][29].data[3] = 0x33333333;
tile_mem[0][29].data[4] = 0x33333333;
tile_mem[0][29].data[5] = 0x33333333;
tile_mem[0][29].data[6] = 0x33333333;
tile_mem[0][29].data[7] = 0x33333333;tile_mem[0][61].data[0] = 0x11111111;
tile_mem[0][61].data[1] = 0x55333553;
tile_mem[0][61].data[2] = 0x53333353;
tile_mem[0][61].data[3] = 0x33333333;
tile_mem[0][61].data[4] = 0x33333333;
tile_mem[0][61].data[5] = 0x33333333;
tile_mem[0][61].data[6] = 0x53333353;
tile_mem[0][61].data[7] = 0x55333553;
  tile_mem[0][30].data[0] = 0x33333331;
tile_mem[0][30].data[1] = 0x33333331;
tile_mem[0][30].data[2] = 0x33333331;
tile_mem[0][30].data[3] = 0x33333331;
tile_mem[0][30].data[4] = 0x33333331;
tile_mem[0][30].data[5] = 0x33333331;
tile_mem[0][30].data[6] = 0x33333331;
tile_mem[0][30].data[7] = 0x33333331;tile_mem[0][62].data[0] = 0x33333331;
tile_mem[0][62].data[1] = 0x55333551;
tile_mem[0][62].data[2] = 0x53333351;
tile_mem[0][62].data[3] = 0x33333331;
tile_mem[0][62].data[4] = 0x33333331;
tile_mem[0][62].data[5] = 0x33333331;
tile_mem[0][62].data[6] = 0x53333351;
tile_mem[0][62].data[7] = 0x55333551;
  tile_mem[0][31].data[0] = 0x33333331;
tile_mem[0][31].data[1] = 0x33333333;
tile_mem[0][31].data[2] = 0x33333333;
tile_mem[0][31].data[3] = 0x33333333;
tile_mem[0][31].data[4] = 0x33333333;
tile_mem[0][31].data[5] = 0x33333333;
tile_mem[0][31].data[6] = 0x33333333;
tile_mem[0][31].data[7] = 0x33333333;tile_mem[0][63].data[0] = 0x33333331;
tile_mem[0][63].data[1] = 0x55333553;
tile_mem[0][63].data[2] = 0x53333353;
tile_mem[0][63].data[3] = 0x33333333;
tile_mem[0][63].data[4] = 0x33333333;
tile_mem[0][63].data[5] = 0x33333333;
tile_mem[0][63].data[6] = 0x53333353;
tile_mem[0][63].data[7] = 0x55333553;

  // Load map into SBB 30
  for (int i = 0; i < 32 * 32; i++) se_mem[30][i] = 28;
  for (int c = 0; c < NUR_COLS; c++) se_mem[30][NUR_ROWS * 32 + c] = 29;
  for (int r = 0; r < NUR_ROWS; r++) se_mem[30][r * 32 + NUR_COLS] = 30;
  se_mem[30][NUR_ROWS * 32 + NUR_COLS] = 31;

  for (int r = 0; r < NUR_ROWS; r++) {
    for (int c = 0; c < NUR_COLS; c++) {
      switch (puzzle[r][c]) {
        case 0:
          se_mem[30][r * 32 + c] = 0;
          break;
        case -1:
          se_mem[30][r * 32 + c] = 26;
          break;
        case -2:
          se_mem[30][r * 32 + c] = 27;
          break;
        default:
          se_mem[30][r * 32 + c] = puzzle[r][c];
          break;
      }
    }
  }
  se_mem[30][cursor_r * 32 + cursor_c] += 32;

  // set up BG0 for a 4bpp 64x32t map, using charblock 0 and screenblock 30
  REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;
  REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

  irq_init(NULL);
  irq_add(II_VBLANK, NULL);

  while (1) {
    VBlankIntrWait();
    key_poll();
    se_mem[30][cursor_r * 32 + cursor_c] -= 32;
    if (key_hit(1 << KI_LEFT) && cursor_c > 0) cursor_c--;
    if (key_hit(1 << KI_RIGHT) && cursor_c < NUR_COLS - 1) cursor_c++;
    if (key_hit(1 << KI_UP) && cursor_r > 0) cursor_r--;
    if (key_hit(1 << KI_DOWN) && cursor_r < NUR_ROWS - 1) cursor_r++;
    se_mem[30][cursor_r * 32 + cursor_c] += 32;
  }
}
