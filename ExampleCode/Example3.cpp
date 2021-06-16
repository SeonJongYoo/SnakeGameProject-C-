//키보드 입력 받기
#include <ncurses.h>
int main() {
  char key;
  char userName[8];

  initscr();

  keypad(stdscr, TRUE); // 키보드 입력 가능, stdscr: 디폴트 윈도우 포인터
  curs_set(0); // 화면에 보이는 커서 설정, 인자: 0 - 커서 사라짐, 1 - 일반 커서, 2 - 큰 커서
  //noecho(); // 문자 입력 시 입력한 값을 화면에 보이지 않게 한다. <-> 반대는 echo()

  printw("User name: ");
  scanw("%s", userName); // 데이터를 입력 받음, scanf와 유사

  printw("%s\n", userName);
  printw("키 입력시 화면이 지워집니다.");
  refresh();

  getch(); // 한 문자를 입력 받음
  clear(); // 윈도우 모든 곳을 빈칸으로 처리
  printw("Hi");
  refresh();

  getch();
  endwin();
  return 0;
}
