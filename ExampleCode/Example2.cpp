// Attribute 설정. 색 attribute, 윈도우 attribute
#include <ncurses.h>

int main() {
  initscr();
  start_color();
  init_pair(1, COLOR_BLUE, COLOR_YELLOW); // 1번 팔레트 폰트 배경 파란색, 폰트 노란색
  init_pair(2, COLOR_RED, COLOR_GREEN); // 2번 팔레트 폰트 배경 빨간색, 폰트 초록색

  bkgd(COLOR_PAIR(1)); // 윈도우 전체 적용
  attron(COLOR_PAIR(1)); // 적용할 팔레트 설정
  mvprintw(1, 1, "C++ Programming");
  mvprintw(2, 1, "Computer Science");
  attroff(COLOR_PAIR(1));

  attron(COLOR_PAIR(2));
  border('1', '2', '3', '4', '5', '6', '7', '8');
  attroff(COLOR_PAIR(2));

  refresh();
  getch();
  endwin();

  return 0;
}
