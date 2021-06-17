#include "manage.h"
#include <ctime>
#include <cmath>
using namespace std;

Manage::Manage() {
  // 기본 프레임 설정
  delay = 100000;
  // 벽 확인 변수
  ckWall = false;
  // Gate 확인 변수
  ckGate = false;
  ckLevel = false;
  // 아이템 문자 - Growth: Green, Poison: Red
  growth_item = 'G';
  poison_item = 'P';
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_RED, COLOR_BLACK);
  init_pair(5, COLOR_YELLOW, COLOR_BLACK); // Gate 색상
  init_pair(6, COLOR_BLACK, COLOR_WHITE);
  makeGrowItem();
  makePoisonItem();
}

// 게임 실행을 위한 함수
void Manage::Run() {
  int gate_time = 0;
  time_t total_s_time = time(NULL);
  time_t start_time = time(NULL);
  while (true) {
    // 일정 시간이 지나면 Gate 생성
    time_t now_time = time(NULL);
    double diff_time = difftime(now_time, start_time);
    if (gate_time == 10) makeGate();
    // 일정 시간이 지나면 새로운 위치에 item 생성
    if (diff_time >= 5) {
      start_time = time(NULL);
      move(gy, gx);
      addch(' ');
      move(py, px);
      addch(' ');
      refresh();
      makeGrowItem();
      makePoisonItem();
    }
    // Snake 클래스의 객체 s를 통해 Snake의 이동 함수 호출 - 매번 Gate를 통과하는지 확인
    if (!ckGate) s.moveSnake();
    else {
      passGate(); // Gate 통과 시 수행하는 함수
      useGate++;
    }
    // 매 이동 시마다 item을 먹었는지 확인하는 함수 호출
    takeItem();
    // 매 이동 시마다 벽에 부딪혔는지 확인하는 함수 호출
    ckWall = checkWallNGate();
    if (gate_time > 10 && !ckGate) {
      move(gate_y1, gate_x1);
      attron(COLOR_PAIR(5));
      addch('X');
      move(gate_y2, gate_x2);
      addch('Y');
      attroff(COLOR_PAIR(5));
      refresh();
    }
    //벽에 부딪힌 경우 게임 종료 or 현재 입력 방향에 반대 방향키 입력 시 게임 종료
    // or snake의 길이가 3보다 작아지면 종료 or 머리가 몸에 닿으면 종료
    if (ckWall || s.getDir() == 'Q' || s.snake.size() < 3 || checkBody()) {
      m.overWindow();
      time_t total_e_time = time(NULL);
      double total_diff_time = difftime(total_e_time, total_s_time);
      attron(COLOR_PAIR(6));
      mvprintw(16, 33,"B: %d", currentLen);
      mvprintw(17, 33,"Growth Item Num: %d", growNum);
      mvprintw(18, 33,"Poison Item Num: %d", poisonNum);
      mvprintw(19, 33,"Use Gate Num: %d", useGate);
      mvprintw(20, 33,"Play Time: %d sec", int(total_diff_time));
      attroff(COLOR_PAIR(6));
      break;
    }
    if (!ckLevel) {
      switch(level) {
        case 2:
        move(gate_y1, gate_x1);
        addch('X');
        move(gate_y2, gate_x2);
        addch('Y');
        refresh();
        ckLevel = true;
        gate_time = 0;
        setDelay(70000);
        break;
        case 3:
        move(gate_y1, gate_x1);
        addch('X');
        move(gate_y2, gate_x2);
        addch('Y');
        refresh();
        ckLevel = true;
        gate_time = 0;
        m.makeWall();
        setDelay(50000);
        break;
        case 4:
        move(gate_y1, gate_x1);
        addch('X');
        move(gate_y2, gate_x2);
        addch('Y');
        refresh();
        ckLevel = true;
        gate_time = 0;
        m.makeWall(4);
        setDelay(30000);
        break;
      }
      if (level == 5) {
        m.MissionComplete();
        time_t total_e_time = time(NULL);
        double total_diff_time = difftime(total_e_time, total_s_time);
        attron(COLOR_PAIR(6));
        mvprintw(16, 33,"B: %d", currentLen);
        mvprintw(17, 33,"Growth Item Num: %d", growNum);
        mvprintw(18, 33,"Poison Item Num: %d", poisonNum);
        mvprintw(19, 33,"Use Gate Num: %d", useGate);
        mvprintw(20, 33,"Play Time: %d sec", int(total_diff_time));
        attroff(COLOR_PAIR(6));
        break;
      }
    }

    setScore();
    setMission();
    checkLevel();
    if (gate_time < 12) gate_time++;
    // 프레임 조절
    usleep(delay);
  }
}

// Snake가 벽에 부딪혔는지 확인하는 함수
bool Manage::checkWallNGate() {
  // Snake의 머리 좌표
  snake_loc sloc = s.getSnakePos();
  if ((sloc.y == gate_y1 && sloc.x == gate_x1) || (sloc.y == gate_y2 && sloc.x == gate_x2)) {
      ckGate = true;
      return false;
  }
  // Snake의 머리 좌표가 게임맵의 경계 벽에 해당하는지 확인
  if (sloc.y == 2 || sloc.y == m.getMapPos().y || sloc.x == 2 || sloc.x == m.getMapPos().x) {
    return true;
  }
  // Snake의 머리가 경계 안에 있는 벽에 해당하는지 확인
  for (int i = 0; i < m.wall.size(); i++) {
    if (sloc.x == m.wall[i].x && sloc.y == m.wall[i].y) return true;
  }
  ckGate = false;
  return false;
}

// Snake의 머리가 몸에 닿는지 확인하는 함수
bool Manage::checkBody() {
  for (int i = 1; i < s.snake.size(); i++) {
      if (s.snake[0].x == s.snake[i].x && s.snake[0].y == s.snake[i].y) return true;
  }
  return false;
}

// 게임맵에 Growth Item 생성하기
void Manage::makeGrowItem() {
  srand(time(NULL));
  while(true) {
    gx = rand() % m.getMapPos().x + 1; // width
    gy = rand() % m.getMapPos().y + 1; // height
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
    //  경게 안에 생성된 벽인 경우 - 다시 랜덤 추출
    for (int i = 0; i < m.wall.size(); i++) {
      if (gx == m.wall[i].x && gy == m.wall[i].y) {
        ck = true;
        break;
      }
    }
    if (ck) continue;
    // Gate인 경우 - 다시 랜덤 추출
    if ((gx == gate_x1 && gy == gate_y1) || (gx == gate_x2 && gy == gate_y2)) continue;
    // 임의의 점이 Poison Item의 좌표와 일치하는 경우 - 다시 랜덤 추출
    if (gx == px || gy == py) continue;
    // 위의 조건에 해당하지 않으면 알맞은 위치 - break
    break;
  }
  // 랜덤 추출한 좌표로 커서 이동
  move(gy, gx);
  // Green 색상으로 attribute 설정
  attron(COLOR_PAIR(3));
  // item에 해당하는 문자 'G'를 게임맵에 추가
  addch(growth_item);
  // Green 색상 attribute 해제
  attroff(COLOR_PAIR(3));
  // 위의 설정을 화면에 출력
  refresh();
}

// 게임맵에 Poison Item 생성하기
void Manage::makePoisonItem() {
  srand(time(NULL));
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
    //  경게 안에 생성된 벽인 경우 - 다시 랜덤 추출
    for (int i = 0; i < m.wall.size(); i++) {
      if (px == m.wall[i].x && py == m.wall[i].y) {
        ck = true;
        break;
      }
    }
    if (ck) continue;
    // Gate인 경우 - 다시 랜덤 추출
    if ((px == gate_x1 && py == gate_y1) || (px == gate_x2 && py == gate_y2)) continue;
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
    currentLen++;
    growNum++;
    makeGrowItem();
  } else if (sloc.x == px && sloc.y == py) { // 현재 Snake의 머리 좌표가 Poison item의 좌표와 일치하는 경우
    s.setStatus(2);
    currentLen--;
    poisonNum++;
    makePoisonItem();
  } else {
    s.setStatus(0);
  }
}

// Gate 생성 함수
void Manage::makeGate() {
  srand(time(NULL));
  bool ck1 = false; bool ck2 = false;
  while(true) {
    gate_x1 = rand() % m.getMapPos().x + 1; // width를 의미
    gate_y1 = rand() % m.getMapPos().y + 1; // height를 의미
    gate_x2 = rand() % m.getMapPos().x + 1;
    gate_y2 = rand() % m.getMapPos().y + 1;
    // (x1, y1)과 (x2, y2)가 같거나 같은 경계에 있는 경우 - 다시 랜덤 추출
    if (gate_x1 == gate_x2 || gate_y1 == gate_y2) continue;
    // 임의의 점이 게임맵의 경계 바깥쪽에 나타나는 경우 - 다시 랜덤 추출
    if (gate_x1 < 2 || gate_x1 > m.getMapPos().x || gate_y1 < 2 || gate_y1 > m.getMapPos().y)
      continue;
    if (gate_x2 < 2 || gate_x2 > m.getMapPos().x || gate_y2 < 2 || gate_y2 > m.getMapPos().y)
      continue;
    // Gate가 Immune Wall에 나타나는 경우 - 다시 랜덤 추출
    if ((gate_x1 == 2 || gate_x1 == m.getMapPos().x) && (gate_y1 == 2 || gate_y1 == m.getMapPos().y)) continue;
    if ((gate_x2 == 2 || gate_x2 == m.getMapPos().x) && (gate_y2 == 2 || gate_y2 == m.getMapPos().y)) continue;

    // level 3부터 경계 안쪽에 벽이 생긴다.
    if (level > 2) {
      // Gate가 게임맵 경계 안쪽 벽에 나타는 경우 - 진행
      for (int i = 0; i < m.wall.size(); i++) {
        if (gate_x1 == m.wall[i].x && gate_y1 == m.wall[i].y) {
          ck1 = true;
        }
        if (gate_x2 == m.wall[i].x && gate_y2 == m.wall[i].y) {
          ck2 = true;
        }
        // 게임맵 경계 안쪽 벽에 Gate 1과 2가 생기는 경우 - 진행
        if (ck1 && ck2) break;
      }
      // Gate가 게임맵 경계 안쪽에 나타나는 경우 - 다시 랜덤 추출
      if (!ck1 && !ck2) continue;
    } else {
      // Gate가 게임맵 경계 안쪽에 나타나는 경우 - 다시 랜덤 추출
      if ((gate_x1 > 2 && gate_x1 < m.getMapPos().x) && (gate_y1 > 2 && gate_y1 < m.getMapPos().y)) continue;
      if ((gate_x2 > 2 && gate_x2 < m.getMapPos().x) && (gate_y2 > 2 && gate_y2 < m.getMapPos().y)) continue;
    }
    break;
  }
  move(gate_y1, gate_x1);
  attron(COLOR_PAIR(5));
  addch('X');
  move(gate_y2, gate_x2);
  addch('Y');
  attroff(COLOR_PAIR(5));
  refresh();
}

// Gate로 들어가는 경우를 처리하는 함수
void Manage::passGate() {
  if ((s.getSnakePos().x == gate_x1 && s.getSnakePos().y == gate_y1) || (s.getSnakePos().x == gate_x2 && s.getSnakePos().y == gate_y2)) {
    //  Gate 통과 시 Snake의 꼬리 부분 지우기
    move(s.snake[s.snake.size()-1].y, s.snake[s.snake.size()-1].x);
    addch(' ');
    refresh();
    s.snake.pop_back();
    // 들어간 Gate가 1번 Gate인 경우 - 2번 Gate의 방향으로 나온다.
    if (s.getSnakePos().x == gate_x1) {
      checkOuputGate(gate_x2, gate_y2);
    } else { // 들어간 Gate가 2번 Gate인 경우 - 1번 Gate의 방향으로 나온다.
      checkOuputGate(gate_x1, gate_y1);
    }
    move(s.snake[0].y, s.snake[0].x);
    addch(s.snake_shape);
    // move(gate_y1, gate_x1);
    // attron(COLOR_PAIR(5));
    // addch('X');
    // move(gate_y2, gate_x2);
    // addch('Y');
    // attroff(COLOR_PAIR(5));
    refresh();
  }
}

// Gate에서 나오는 경우를 처리하는 함수
void Manage::checkOuputGate(int col, int row) {
  int dx[4] = {-1, 0, 1, 0};
  int dy[4] = {0, 1, 0, -1};
  if (col == 2) {
      s.snake.insert(s.snake.begin(), snake_loc(col+1, row));
      s.setDir('R');
  } else if (col == m.getMapPos().x) {
      s.snake.insert(s.snake.begin(), snake_loc(col-1, row));
      s.setDir('L');
  } else if(row == 2) {
      s.snake.insert(s.snake.begin(), snake_loc(col, row+1));
      s.setDir('D');
  } else if(row == m.getMapPos().y) {
      s.snake.insert(s.snake.begin(), snake_loc(col, row-1));
      s.setDir('U');
  } else {
    // 경계 안쪽 벽 Gate에서 나올 때 방향 처리
    // 현재 위치에서 상하좌우를 탐색하여 경계 안쪽 벽에 해당하는 위치를 파악
    int cnt = 0; bool ck;
    int nx, ny, idx;
    for (int i = 0; i < 4; i++) {
      ck = false;
      int tx = col + dx[i];
      int ty = row + dy[i];
      for (int j = 0; j < m.wall.size(); j++) {
        if (tx == m.wall[j].x && ty == m.wall[j].y) {
            cnt++;
            ck = true;
            break;
        }
      }
      if (!ck) {
        nx = tx;
        ny = ty;
        idx = i;
      }
    }
    // 상하좌우 탐색 결과 벽이 없다면 - 원래 진행 방향대로 진행
    if (!cnt) {
      if (s.getDir() == 'R') {
        s.snake.insert(s.snake.begin(), snake_loc(col+1, row));
      } else if (s.getDir() == 'L') {
        s.snake.insert(s.snake.begin(), snake_loc(col-1, row));
      } else if (s.getDir() == 'U') {
        s.snake.insert(s.snake.begin(), snake_loc(col, row-1));
      } else {
        s.snake.insert(s.snake.begin(), snake_loc(col, row+1));
      }
    } else { // 벽이 있는 경우 - 없는 방향으로 빠져 나온다.
      s.snake.insert(s.snake.begin(), snake_loc(nx, ny));
      if (idx == 0) {
        s.setDir('L');
      } else if (idx == 1) {
        s.setDir('D');
      } else if (idx == 2) {
        s.setDir('R');
      } else {
        s.setDir('U');
      }
    }
  }
}

void Manage::setScore() {
  mvprintw(5, 57,"B: %d", currentLen);
  mvprintw(6, 57,"+: %d", growNum);
  mvprintw(7, 57,"-: %d", poisonNum);
  mvprintw(8, 57,"G: %d", useGate);
}

void Manage::setMission() {
  switch(level) {
    case 1:
    mcurrentLen = 5; mgrowNum = 2; mpoisonNum = 1; museGate = 1;
    break;
    case 2:
    mcurrentLen = 7; mgrowNum = 3; mpoisonNum = 2; museGate = 3;
    break;
    case 3:
    mcurrentLen = 7; mgrowNum = 5; mpoisonNum = 3; museGate = 4;
    break;
    case 4:
    mcurrentLen = 8; mgrowNum = 7; mpoisonNum = 5; museGate = 5;
    break;
  }
}

void Manage::checkLevel() {
  mvprintw(17, 57,"level: %d", level);
  if (currentLen >= mcurrentLen) mvprintw(18, 57,"B: %d ( V )", mcurrentLen);
  else mvprintw(18, 57,"B: %d (   )", mcurrentLen);
  if (growNum >= mgrowNum) mvprintw(19, 57,"+: %d ( V )", mgrowNum);
  else mvprintw(19, 57,"+: %d (   )", mgrowNum);
  if (poisonNum >= mpoisonNum) mvprintw(20, 57,"-: %d ( V )", mpoisonNum);
  else mvprintw(20, 57,"-: %d (   )", mpoisonNum);
  if (useGate >= museGate) mvprintw(21, 57,"G: %d ( V )", museGate);
  else mvprintw(21, 57,"G: %d (   )", museGate);
  if (currentLen >= mcurrentLen && growNum >= mgrowNum &&
    poisonNum >= mpoisonNum && useGate >= museGate) {
        level++;
        ckLevel = false;
    }
}


// 게임 난이도 조절을 위한 함수 - delay값이 작을수록 속도가 빨라짐
void Manage::setDelay(int delay) {
  this->delay = delay;
}
