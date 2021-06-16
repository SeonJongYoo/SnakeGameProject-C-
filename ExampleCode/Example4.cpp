// 새로운 윈도우 생성
#include <ncurses.h>

int main() {
  WINDOW *win1; // 특정 윈도우를 나타내는 포인터 타입
  WINDOW *win2;

  initscr();
  resize_term(25, 25);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_RED);
  init_pair(2, COLOR_BLUE, COLOR_GREEN);

  border('*', '*', '*', '*', '*', '*', '*', '*');
  mvprintw(1, 1, "A default window");
  refresh();
  getch();

  // 특정 윈도우 함수 - 함수 앞에 w가 붙는다.
  win1 = newwin(10, 10, 2, 2); // 새 윈도우 생성 - 20, 20 . 생성 시 메모리 할당
  // 3: 윈도우 시작 y좌표, 5: 윈도우 시작 x좌표
  // 새 윈도우 포인터 return
  wbkgd(win1, COLOR_PAIR(1));
  wattron(win1, COLOR_PAIR(1));
  mvwprintw(win1, 1, 1, "A new window");
  //wborder(win1, '@', '@', '@', '@', '@', '@', '@', '@');
  wrefresh(win1);

  win2 = newwin(5, 5, 5, 5); // 새 윈도우 생성 - 20, 20 . 생성 시 메모리 할당
  wbkgd(win2, COLOR_PAIR(2));
  wattron(win2, COLOR_PAIR(2));
  mvwprintw(win2, 1, 1, "A new window");
  wborder(win2, '@', '@', '@', '@', '@', '@', '@', '@');
  wrefresh(win2);

  getch();
  delwin(win1); // 새로 생성한 윈도우에 할당된 메모리 해제
  delwin(win2);
  endwin();


  return 0;
}
