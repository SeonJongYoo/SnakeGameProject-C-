#include "manage.h"
#include <ctime>
using namespace std;

Manage::Manage() {
  // 기본 프레임 설정
  delay = 100000;
  // 벽 확인 변수
  ckWall = false;
  // Gate 확인 변수
  ckGate = false;
  // 아이템 문자 - Growth: Green, Poison: Red
  growth_item = 'G';
  poison_item = 'P';
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_RED, COLOR_BLACK);
  init_pair(5, COLOR_YELLOW, COLOR_BLACK); // Gate 색상
  // 게임맵 생성과 동시에 item과 Gate 생성
  makeGrowItem();
  makePoisonItem();
}

// 게임 실행을 위한 함수
void Manage::Run() {
  int gate_time = 0; int item_time = 0;
  while (true) {
    // 일정 시간이 지나면 Gate 생성
    if (gate_time == 10) makeGate();
    // 일정 시간이 지나면 새로운 위치에 item 생성
    if (item_time == 20) {
      item_time = 0;
      move(gy, gx);
      addch(' ');
      move(py, px);
      addch(' ');
      refresh();
      makeGrowItem();
      makePoisonItem();
    }
    // Snake 클래스의 객체 s를 통해 Snake의 이동 함수 호출
    if (!ckGate) s.moveSnake();
    else {
      passGate();
    }
    // 매 이동 시마다 item을 먹었는지 확인하는 함수 호출
    takeItem();
    // 매 이동 시마다 벽에 부딪혔는지 확인하는 함수 호출
    ckWall = checkWall();
    //벽에 부딪힌 경우 게임 종료 or 현재 입력 방향에 반대 방향키 입력 시 게임 종료 or snake의 길이가 3보다 작아지면 종료
    if (ckWall || s.getDir() == 'Q' || s.snake.size() < 3) {
      m.overWindow();
      break;
    }
    if (gate_time < 20) gate_time++;
    item_time++;
    // 프레임 조절
    usleep(delay);
  }
}

// Snake가 벽에 부딪혔는지 확인
bool Manage::checkWall() {
  // Snake의 머리 좌표
  snake_loc sloc = s.getSnakePos();
  // Snake의 머리 좌표가 Gate에 해당하는 경우
  if ((sloc.y == gate_y1 && sloc.x == gate_x1) || (sloc.y == gate_y2 && sloc.x == gate_x2)) {
    ckGate = true;
    return false;
  }
  // Snake의 머리 좌표가 게임맵의 경계에 해당하는지 확인
  if (sloc.y == 2 || sloc.y == m.getMapPos().y || sloc.x == 2 || sloc.x == m.getMapPos().x) {
    return true;
  }
  ckGate = false;
  return false;
}

// 게임맵에 Growth Item 생성하기
void Manage::makeGrowItem() {
  srand(time(NULL));
  snake_loc sloc = s.getSnakePos();
  while(true) {
    gx = rand() % m.getMapPos().x + 1;
    gy = rand() % m.getMapPos().y + 1;
    // 임의의 점이 게임맵을 벗어나는 경우 - 다시 랜덤 추출
    if (gx <= 2 || gx >= m.getMapPos().x || gy <= 2 || gy >= m.getMapPos().y)
      continue;
    // 임의의 점이 Snake의 몸체인 경우 - 다시 랜덤 추출
    bool ck = false;
    for (int i = 0; i < s.snake.size(); i++) {
        if (gx == s.snake[i].x && gy == s.snake[i].y) {
          ck = true;
          break;
        }
    }
    if (ck) continue;
    // 임의의 점이 Poison Item의 좌표와 일치하는 경우 - 다시 랜덤 추출
    if (gx == px || gy == py) continue;
    // 위의 조건에 해당하지 않으면 알맞은 위치 - break
    break;
  }
  // 랜덤 추출한 좌표로 커서 이동
  move(gy, gx);
  // Green 색상으로 attribute 설정
  attron(COLOR_PAIR(3));
  // item에 해당하는 문자 '@'를 게임맵에 추가
  addch(growth_item);
  // Green 색상 attribute 해제
  attroff(COLOR_PAIR(3));
  // 위의 설정을 화면에 출력
  refresh();
}

// 게임맵에 Poison Item 생성하기
void Manage::makePoisonItem() {
  srand(time(NULL));
  snake_loc sloc = s.getSnakePos();
  while(true) {
    px = rand() % m.getMapPos().x + 1;
    py = rand() % m.getMapPos().y + 1;
    // 임의의 점이 게임맵을 벗어나는 경우 - 다시 랜덤 추출
    if (px <= 2 || px >= m.getMapPos().x || py <= 2 || py >= m.getMapPos().y)
      continue;
    // 임의의 점이 Snake의 몸체인 경우 - 다시 랜덤 추출
    bool ck = false;
    for (int i = 0; i < s.snake.size(); i++) {
        if (px == s.snake[i].x && py == s.snake[i].y) {
          ck = true;
          break;
        }
    }
    if (ck) continue;
    // 임의의 점이 Growth Item의 좌표와 일치하는 경우 - 다시 랜덤 추출
    if (px == gx || py == gy) continue;
    break;
  }
  // Growth Item과 동일
  move(py, px);
  attron(COLOR_PAIR(4));
  addch(poison_item);
  attroff(COLOR_PAIR(4));
  refresh();
}

// Growth Item이나 Poison Item을 먹었을 경우
void Manage::takeItem() {
  snake_loc sloc = s.getSnakePos();
  if (sloc.x == gx && sloc.y == gy) { // 현재 Snake의 머리 좌표가 Growth item의 좌표와 일치하는 경우
    s.setStatus(1);
    makeGrowItem();
  } else if (sloc.x == px && sloc.y == py) { // 현재 Snake의 머리 좌표가 Poison item의 좌표와 일치하는 경우
    s.setStatus(2);
    makePoisonItem();
  } else {
    s.setStatus(0);
  }
}

// Gate 생성 함수
void Manage::makeGate() {
  srand(time(NULL));
  while(true) {
    gate_x1 = rand() % m.getMapPos().x + 1;
    gate_y1 = rand() % m.getMapPos().y + 1;
    gate_x2 = rand() % m.getMapPos().x + 1;
    gate_y2 = rand() % m.getMapPos().y + 1;
    // (x1, y1)과 (x2, y2)가 같거나 같은 경계에 있는 경우
    if (gate_x1 == gate_x2 || gate_y1 == gate_y2) continue;
    // 임의의 점이 게임맵의 경계 바깥쪽에 나타나는 경우 - 다시 랜덤 추출
    if (gate_x1 < 2 || gate_x1 > m.getMapPos().x || gate_y1 < 2 || gate_y1 > m.getMapPos().y)
      continue;
    if (gate_x2 < 2 || gate_x2 > m.getMapPos().x || gate_y2 < 2 || gate_y2 > m.getMapPos().y)
      continue;
    // Gate가 Immune Wall에 나타나는 경우 - 다시 랜덤 추출
    if ((gate_x1 == 2 || gate_x1 == m.getMapPos().x) && (gate_y1 == 2 || gate_y1 == m.getMapPos().y)) continue;
    if ((gate_x2 == 2 || gate_x2 == m.getMapPos().x) && (gate_y2 == 2 || gate_y2 == m.getMapPos().y)) continue;
    // Gate가 게임맵 경계 안쪽에 나타나는 경우 - 다시 랜덤 추출
    if ((gate_x1 > 2 && gate_x1 < m.getMapPos().x) && (gate_y1 > 2 && gate_y1 < m.getMapPos().y)) continue;
    if ((gate_x2 > 2 && gate_x2 < m.getMapPos().x) && (gate_y2 > 2 && gate_y2 < m.getMapPos().y)) continue;

    break;
  }
  move(gate_y1, gate_x1);
  attron(COLOR_PAIR(5));
  addch(' ');
  move(gate_y2, gate_x2);
  addch(' ');
  attroff(COLOR_PAIR(5));
  refresh();
}

// Gate로 들어가는 경우
void Manage::passGate() {
  if ((s.getSnakePos().x == gate_x1 && s.getSnakePos().y == gate_y1) || (s.getSnakePos().x == gate_x2 && s.getSnakePos().y == gate_y2)) {
    // Snake가 통과하면서 사라지는 Gate shape 다시 생성
    // move(s.getSnakePos().y, s.getSnakePos().x);
    // attron(COLOR_PAIR(5));
    // addch('X');
    // attroff(COLOR_PAIR(5));
    // refresh();
    
    //  Gate 통과 시 Snake의 꼬리 부분 지우기
    move(s.snake[s.snake.size()-1].y, s.snake[s.snake.size()-1].x);
    printw(" ");
    refresh();
    s.snake.pop_back();
    // 들어간 Gate가 1번 Gate인 경우 - 2번 Gate의 방향으로 나온다.
    if (s.getSnakePos().x == gate_x1) {
      if (gate_x2 == 2) {
          s.snake.insert(s.snake.begin(), snake_loc(gate_x2+1, gate_y2));
          s.setDir('R');
      } else if (gate_x2 == m.getMapPos().x) {
          s.snake.insert(s.snake.begin(), snake_loc(gate_x2-1, gate_y2));
          s.setDir('L');
      } else if(gate_y2 == 2) {
          s.snake.insert(s.snake.begin(), snake_loc(gate_x2, gate_y2+1));
          s.setDir('D');
      } else if(gate_y2 == m.getMapPos().y) {
          s.snake.insert(s.snake.begin(), snake_loc(gate_x2, gate_y2-1));
          s.setDir('U');
      }
    } else { // 들어간 Gate가 2번 Gate인 경우 - 1번 Gate의 방향으로 나온다.
      if (gate_x1 == 2) {
          s.snake.insert(s.snake.begin(), snake_loc(gate_x1+1, gate_y1));
          s.setDir('R');
      } else if (gate_x1 == m.getMapPos().x) {
          s.snake.insert(s.snake.begin(), snake_loc(gate_x1-1, gate_y1));
          s.setDir('L');
      } else if(gate_y1 == 2) {
          s.snake.insert(s.snake.begin(), snake_loc(gate_x1, gate_y1+1));
          s.setDir('D');
      } else if(gate_y1 == m.getMapPos().y) {
          s.snake.insert(s.snake.begin(), snake_loc(gate_x1, gate_y1-1));
          s.setDir('U');
      }
    }
    move(s.snake[0].y, s.snake[0].x);
    addch(s.snake_shape);
    refresh();
  }
}

// 게임 난이도 조절을 위한 함수 - delay값이 작을수록 속도가 빨라짐
void Manage::setDelay(int delay) {
  this->delay = delay;
}
