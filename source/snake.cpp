#include "snake.h"
using namespace std;

// Snake의 좌표 설정을 위한 구조체
snake_loc::snake_loc(int col ,int row) {
  x = col;
  y = row;
}

Snake::Snake() {
  // Snake의 모양
  snake_shape = 'O';
  // Snake의 시작 방향
  dir = 'L';
  status = 0;

  // 게임맵의 너비와 높이
  maxwidth = m.getMapPos().x;
  maxheight = m.getMapPos().y;

  // delay없이 입력을 받도록 설정 - Snake의 모양 출력 가능
  nodelay(stdscr, true);
  noecho();
  // 키보드 입력 받기
  keypad(stdscr, true);
  curs_set(0);

  // Snake의 시작 위치 표시 - 맵의 중간 위치로 설정
  // 현재 게임 맵의 height와 width 정보를 이용
  for (int i = 0; i < 3; i++) {
    snake.push_back(snake_loc(maxwidth/2 + i, maxheight/2));
  }

  // 위에서 설정한 Snake의 위치 좌표에 snake shape인 'O'를 화면에 추가
  for (int i = 0; i < snake.size(); i++) {
    move(snake[i].y, snake[i].x);
    addch(snake_shape);
  }
  // 위에서 설정한 내용을 화면에 출력
  refresh();
}

Snake::~Snake() {
  nodelay(stdscr, false);
  getch();
  endwin();
}

// 게임맵에서 방향키에 따라 Snake의 이동을 위한 함수
void Snake::moveSnake() {
  // 키보드 입력 받기
  int input = getch();

  // 입력을 방향키에 따라 구분
  switch(input) {
    case KEY_LEFT:
        // 왼쪽 방향키 입력 시 현재 진행 방향이 오른쪽이 아니면 dir = 'L'
        if(dir != 'R') dir = 'L';
        else dir = 'Q'; // 현재 진행 방향이 오른쪽이면 game over 종료
        break;
    case KEY_UP:
        if(dir != 'D') dir = 'U';
        else dir = 'Q';
        break;
    case KEY_DOWN:
        if(dir != 'U') dir = 'D';
        else dir = 'Q';
        break;
    case KEY_RIGHT:
        if(dir != 'L') dir = 'R';
        else dir = 'Q';
        break;
    case 'q':
        dir = 'Q';
        break;
  }
  // status == 0인 경우 - item을 먹지 않은 경우
  // status == 2인 경우 - Poison item을 먹은 경우
  if (!status || status == 2) {
    // Snake의 꼬리 부분
    // 이상이 없거나 Poison item을 먹은 경우 이동할 때마다 꼬리 부분의 출력을 지운다.
    move(snake[snake.size()-1].y, snake[snake.size()-1].x);
    printw(" ");
    refresh();
    snake.pop_back();
  }
  // item을 먹지 않은 경우와 Growth item을 먹은 경우
  // status == 1인 경우 - Growth item을 먹은 경우
  if (!status || status == 1) {
    // 입력된 방향키에 따라 알맞은 좌표에 Snake의 head가 생성됨
    if (dir == 'L') {
        snake.insert(snake.begin(), snake_loc(snake[0].x-1, snake[0].y));
    } else if (dir == 'R') {
        snake.insert(snake.begin(), snake_loc(snake[0].x+1, snake[0].y));
    } else if( dir == 'U') {
        snake.insert(snake.begin(), snake_loc(snake[0].x, snake[0].y-1));
    } else if( dir == 'D') {
        snake.insert(snake.begin(), snake_loc(snake[0].x, snake[0].y+1));
    }
  }
  // item을 먹지 않은 경우와 Growth item을 먹은 경우
  // 입력된 방향키에 맞게 Snake의 모양 'O'를 화면에 출력
  if (!status || status == 1) {
    move(snake[0].y, snake[0].x);
    addch(snake_shape);
    refresh();
  }
}

// Snake의 머리 좌표를 return하는 함수
snake_loc Snake::getSnakePos() {
  return snake_loc(snake[0].x, snake[0].y);
}

// Snake의 진행 방향을 return하는 함수
char Snake::getDir() {
  return this->dir;
}

void Snake::setDir(char dir) {
  this->dir = dir;
}

// Snake의 상태를 설정하는 함수
// status == 0이면 정상
// status == 1이면 Growth item을 먹은 상태
// status == 2이면 Poison item을 먹은 상태
void Snake::setStatus(int status) {
  this->status = status;
}
