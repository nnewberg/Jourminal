#include <ncurses.h>
#include <stdlib.h>
#include <string>

void OpenFile (std::string filepath) {
    
    int key; //character we will store into from the keyboard
    int row = 1;
    int col = 0;
  
    initscr(); //initialize the screen

    noecho();
    nodelay(stdscr,FALSE);
    keypad(stdscr,TRUE);


    clear();
    refresh();
    int open = scr_restore(filepath.c_str());
    refresh();


    while ((key = getch()) != '~'){


    if (key == KEY_DOWN){

    move(++row, col);

    } else if (key == KEY_UP){
    
    if (row > 0){ 
    move(--row, col);
    }

    } else if (key == KEY_LEFT){

    if (col > 0){
    move(row, --col);
    }

    } else if (key == KEY_RIGHT){

    if(col < COLS){
    move(row, ++col);
    }

    } else if (key == 127){ //backspace

    if(col > 0){
    col--;
    } else{
    if (row > 0){
    col = COLS;
    row--;
    }
    }
    move(row, col);
    delch();

    } else if (key == '\n'){ //enter
    row++;
    col = 0;
    move(row, col);
    } else if (key == '$'){ //shift alt S (save)

    int save = scr_dump(filepath.c_str());
      endwin();
      return;

    }else{

    // chtype* rowText = new chtype[10];
    // move(row, col);
    // inchnstr(rowText, 10);
    // move(row - 1, col);
    // for (int i = 0; i < 10; i++){
    //   addch(rowText[i]);
    //   move(row - 1, i);
    // }
    chtype rowText[COLS];
    for (int i = col; i < COLS; i++){
      move(row, i);
      rowText[i - col] = inch() & A_CHARTEXT;
      char temp = inch() & A_CHARTEXT;
      move(row+i, col);
      addch(temp);
    }

    // move(++row, 0);
    // for (int i = 0; i < COLS; i++){
    //   move(row, i);
    //   addch(rowText[i]);
    // }
    //printw(rowText);

    // for (int i = 0; i < COLS; i++){
    //   addch(rowText[i]);
    // }

    //printw(rowText);
    // addch(key);
    // move(row, ++col);
    // addstr(rowText);


    }

    move(0, COLS - 8);
    clrtoeol(); // clear line
    printw("%d , %d", row, col);
    move(row, col);
    refresh();

    }

    endwin();
}

int main (){
  OpenFile("test.jml");
  return 0;
}