#include <ncurses.h>

int main() {
  initscr(); // ncurses 모드 시작
  start_color();
  init_pair(1, COLOR_RED, COLOR_WHITE);

  attron(COLOR_PAIR(1));
  printw("Hello world"); // 디폴트 윈도우에만 출력
  attroff(COLOR_PAIR(1));

  refresh(); // 디폴트 윈도우 내용을 스크린에 출력
  getch(); // 한 문자를 입력 받음 - 실행 파일이 바로 종료되는 것을 방지(문자 하나를 입력 받기 전까지 대기)
  endwin(); // ncurses TUI 모드 사용 종료

  return 0;
}
