#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77

int main (void){
  int ch;
  int r = 2; //starting row
  int c = 0; //starting col
  initscr();
  move(2, 0); 
  hline(0, COLS);
  noecho();
  nodelay(stdscr,FALSE);
  keypad(stdscr,TRUE);

  clear();
  refresh();
  int write = scr_restore("test.jml");
  if( write == ERR){
    printf("Error writing\n");
  }
  refresh();

  while ((ch = getch()) != '~'){
    if (ch == '\n'){
      move(++r, 0);
      c = 0;
    }else if (ch == 127){ //Backspace
      if (c > 0){
	move(r, c-=1);
	delch();
      }else{
	c = COLS;
	move(--r, c);
	delch();
      }
    }else if (ch == KEY_LEFT){
      if (c > 0){
	move(r, c-=1);
      }
    }else if (ch == KEY_RIGHT){
      move(r, c+=1);
    }else if (ch == KEY_UP){
      move(--r, c);
    }else if (ch == KEY_DOWN){
      move(++r, c);
    }else{
      c++;
      addch(ch);
    }
  }

  int read = scr_dump("test.jml");
  if( read == ERR){
        //addstr(“Error writing window to disk”);
        printf("Error\n");
  }
  else{
      //addstr(“File written; press Enter to quit”);
        printf("File written\n");

  }
  
  // clear();
  // refresh();
  // napms(100);
  // write = scr_restore("test.jml");
  // if( write == ERR){
  //   printf("Error writing\n");
  // }

  // refresh();
  // r = 0;
  // c = 0;

  
  // while ((ch = getch()) != '~'){
  //   if (ch == '\n'){
  //     move(++r, 0);
  //     c = 0;
  //   }else if (ch == 127){ //Backspace
  //     if (c > 0){
  // move(r, c-=1);
  // delch();
  //     }else{
  // c = COLS;
  // move(--r, c);
  // delch();
  //     }
  //   }else if (ch == KEY_LEFT){
  //     if (c > 0){
  // move(r, c-=1);
  //     }
  //   }else if (ch == KEY_RIGHT){
  //     move(r, c+=1);
  //   }else if (ch == KEY_UP){
  //     move(--r, c);
  //   }else if (ch == KEY_DOWN){
  //     move(++r, c);
  //   }else{
  //     c++;
  //     addch(ch);
  //   }
  // }
  
  endwin();
  return 0;
  
}
