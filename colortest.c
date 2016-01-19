#include <ncurses.h>
#include <stdlib.h>

void bomb(char *msg);

int main(void){
  initscr();
  if (!has_colors())
    bomb("Terminal cannot do colors\n");
  
  printw("Colors have been properly initialized");
  printw("Congratulations!\n");
  printw("NCurses reports that you are using %d colors,\n", COLORS);
  printw("and %d color pairs.", COLOR_PAIRS);
  refresh();
  getch();

  endwin();
  return 0;
}
void bomb(char *msg){
  endwin();
  puts(msg);
  exit(1);
}
