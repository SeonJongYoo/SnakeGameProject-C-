#include <iostream>
#include <vector>
#include <ncurses.h>
#include "map.h"

#ifndef __SNAKE__
#define __SNAKE__

// Snake의 좌표를 설정하기 위한 구조체
struct snake_loc {
  int x, y;
  snake_loc(int col, int row);
};

class Snake {
  int maxheight;
  int maxwidth;
  int status;
  char dir;
  // Map 클래스 객체 생성
  Map m;
public:
  // Map 상에서 Snake를 표시하기 위해 Vector 사용
  std::vector<snake_loc> snake;
  char snake_shape;

  Snake();
  ~Snake();
  void moveSnake();
  snake_loc getSnakePos();
  char getDir();
  void setDir(char dir);
  void setStatus(int status);
};
#endif
