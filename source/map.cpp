#include "map.h"
using namespace std;

// 현재 게임맵의 좌표를 설정하기 위한 구조체
map_loc::map_loc(int col ,int row) {
  x = col;
  y = row;
}

Map::Map() {
  initscr(); // ncurses 모드 시작
  start_color();
  resize_term(35, 80); // 터미널의 크기 조정

  // 사용할 색상 정의
  // 2번쨰 인자가 폰트색, 3번째 인자가 배경색
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_WHITE);

  border('|', '|', '-', '-', '+', '+', '+', '+');
  attron(COLOR_PAIR(2));
  mvprintw(1, 27, "Welcome To the Snake Game");
  attroff(COLOR_PAIR(2));
  refresh(); // 디폴트 윈도우 내용을 스크린에 출력

  gameMap(); // 게임맵 생성
  scoreWindow(); // 점수판 생성
  missionWindow();
}

Map::~Map() {
  getch();
  // 새로 생성한 윈도우에 할당된 메모리 해제
  delwin(gmap);
  delwin(swin);
  delwin(owin);
  endwin();
}

// 게임맵을 위한 윈도우를 생성하고 화면에 출력하는 함수
// 인자를 통해 게임맵의 크기를 설정할 수 있음 - default: 30, 50
void Map::gameMap(int col, int row) {

  // 3번 째 인자와 4번 째 인자는 window 시작 위치를 나타냄
  gmap = newwin(col, row, 2, 2);
  // 새로 생성한 윈도우에서 Height의 최댓값과 Width의 최댓값을
  // Height변수와 Width변수에 저장한다.
  getmaxyx(gmap, Width, Height);
  wattron(gmap, COLOR_PAIR(1));
  // 게임맵의 경계 설정
  wborder(gmap, '|', '|', '_', '_', '+', '+', '+', '+');
  wattroff(gmap, COLOR_PAIR(1));
  // 위의 설정 내용을 화면에 출력
  wrefresh(gmap);
}

// 점수판을 위한 윈도우 생성하고 화면에 출력하는 함수
void Map::scoreWindow() {
  swin = newwin(10, 17, 2, 55);
  wattron(swin, COLOR_PAIR(1));
  mvwprintw(swin, 1, 2, "Score Board");
  wborder(swin, '|', '|', '_', '_', '+', '+', '+', '+');
  wattroff(swin, COLOR_PAIR(1));
  wrefresh(swin);
}

void Map::missionWindow() {
  mwin = newwin(10, 17, 15, 55);
  wattron(mwin, COLOR_PAIR(1));
  mvwprintw(mwin, 1, 2, "Mission Board");
  wborder(mwin, '|', '|', '_', '_', '+', '+', '+', '+');
  wattroff(mwin, COLOR_PAIR(1));
  wrefresh(mwin);
}

// Game Over 윈도우를 생성하고 화면에 출력하는 함수
void Map::overWindow() {
   // newwin을 통해 새로운 윈도우를 생성하기 위한 메모리를 할당 받음
   owin = newwin(3, 15, 13, 30);
   // 윈도우의 배경색 설정
   wbkgd(owin, COLOR_PAIR(2));
   // 윈도우의 attribute 색상 설정
   wattron(owin, COLOR_PAIR(2));
   // mvwprintw의 좌표는 새로 생성한 윈도우를 기준으로 설정해야 한다.
   mvwprintw(owin, 1, 3, "Game Over");
   // 위의 설정 내용을 새로 생성한 윈도우 화면에 출력
   wrefresh(owin);
}

void Map::MissionComplete() {
  owin = newwin(3, 15, 13, 30);
  wbkgd(owin, COLOR_PAIR(2));
  wattron(owin, COLOR_PAIR(2));
  mvwprintw(owin, 1, 3, "Mission Complete");
  wrefresh(owin);
}

// 게임맵의 끝 좌표 return
map_loc Map::getMapPos() {
  return map_loc(Height+1, Width+1);
}
