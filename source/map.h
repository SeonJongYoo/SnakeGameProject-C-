#include <iostream>
#include <ncurses.h>
#include <cstdlib>

// #ifndef를 통해 구현한 클래스를 외부 파일에서 재정의하지 않도록 설정
#ifndef __MAP__
#define __MAP__

// 생성한 윈도우의 최대 좌표를 위한 구조체
struct map_loc {
  int x, y;
  map_loc(int col, int row);
};


class Map {
  // 게임맵 크기 변수
  int Height, Width;
  // 게임맵, 점수판, 게임오버 윈도우를 위한 WINDOW 포인터 타입의 변수 선언
  WINDOW* gmap;
  WINDOW* swin;
  WINDOW* owin;
  // col - heigth, row - width
  // 게임맵의 최대 크기는 height = 30, width = 50으로 default 값으로 설정
  // 개임맵의 최소 크기는 height = 21, width = 21
  void gameMap(int col = 30, int row = 50);
  void scoreWindow();
  void missionWindow();
  
public:
  Map();
  ~Map();
  void overWindow();
  map_loc getMapPos();
};
#endif
