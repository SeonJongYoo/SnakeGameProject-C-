// 윈도우 화면 구성하기 - 터미널 크기조정, 사용자 지정 위치에 문자열 출력
#include <ncurses.h>
#include <clocale>
#include <iostream>
using namespace std;

int main() {
  setlocale(LC_ALL, "");

  int row, col;
  cin >> row >> col;

  initscr();
  resize_term(row, col); // 터미널 크기 조정. 윈도우까지 동시에 조정. resize_window() - 윈도우 크기만 조정
  mvprintw(3, 4, "\u2B1B \u2B1C"); // (3, 4) 위치에 지정된 output 출력
  // - 실제 출력은 refresh()로! 출력하라는 명령!

  refresh();
  getch();
  endwin();

  return 0;
}
