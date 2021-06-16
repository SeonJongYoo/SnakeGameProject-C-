#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "snake.h"

#ifndef __MANAGE__
#define __MANAGE__

class Manage {
  int delay;
  bool ckWall, ckGate;
  char growth_item, poison_item;
  // gx, gy: Growth Item의 좌표
  // px, py: Poison Item의 좌표
  int gx, gy, px, py;
  // 한 쌍의 gate 좌표
  int gate_x1, gate_y1, gate_x2, gate_y2;
  // Map 클래스 객체와 Snake 클래스의 객체 생성
  Map m;
  Snake s;

  // manage.cpp 파일에 설명
  bool checkWall();
  void makeGrowItem();
  void makePoisonItem();
  void takeItem();
  void makeGate();
  void passGate();
public:
  Manage();
  void Run();
  void setDelay(int delay);
};
#endif
