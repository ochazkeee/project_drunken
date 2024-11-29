#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>

///전역 변수 설정
int key_main=1;
///구조체 정의 집 위치, 펍 위치
/// struct
struct Location {
  int x;
  int y;
};

//함수 선언
void clearScreen();
void make_background();
void make_location(int Grid[10][10]);
void One_paze(int Grid[10][10],struct Location P,struct Location H);
void Two_paze(int Grid[10][10],struct Location P,struct Location H,struct Location M);
void Three_paze(int Grid[10][10],struct Location P,struct Location H,struct Location M);

int main(){
  make_background();
  while(1){}

  return 0;
}

void clearScreen() {
  system("clear");
}

///플레이할 게임의 백그라운드, 배경 만들기
void make_background(){
  int Grid[10][10];
  ///10by10을 0으로 초기화
  for(int i=0;i<10;i++){
    for(int j=0;j<10;j++){
      Grid[i][j]=0;
    }
  }
  ///벽이 새워질 부분은 1로 직접 처리
  Grid[0][0]=1;Grid[0][1]=1;Grid[0][4]=1;
  Grid[1][6]=1;Grid[1][7]=1;Grid[1][8]=1;
  Grid[2][1]=1;Grid[2][3]=1;Grid[2][4]=1;
  Grid[3][6]=1;Grid[3][9]=1;Grid[4][1]=1;
  Grid[4][2]=1;Grid[4][4]=1;Grid[6][1]=1;
  Grid[6][2]=1;Grid[6][4]=1;Grid[6][6]=1;
  Grid[6][8]=1;Grid[7][8]=1;Grid[8][1]=1;
  Grid[8][2]=1;Grid[8][3]=1;Grid[8][4]=1;
  Grid[8][6]=1;Grid[8][8]=1;

  ///다음 함수 호출
  make_location(Grid);
}
///펍(Pub)과 집(Home)의 위치를 랜덤으로 1사분면 혹은 4사분면에 위치하게.
///하지만 하나가 1사분면이면 다른 하나는 4사분면이어야 함.
void make_location(int Grid[10][10]) {
  srand(time(NULL));
  ///임의로 x좌표와 y좌표를 담을 변수
  int xh,yh;
  int xp,yp;
///현재 key_main은 글로벌 변수로 1로 지정되어 있음
  while(key_main!=0) {
    xh=rand()%10;
    yh=rand()%10;
    ///그리드값이 길위여야 하고 x,y는 0에서 4까지
    ///1사분면에 속한다.
    if((Grid[yh][xh]==0) && (xh<5)&&(yh<5)) {
      ///Pub은 4사분면에 속하게
      ///그리드값은 0, x는 5에서 9까지, y는 0에서 4까지
      xp=rand()%5+5;
      yp=rand()%5+5;
      if (Grid[yp][xp]==0) {
        key_main=0;
      }
    }else if((Grid[yh][xh]==0) && (yh>=5)&&(xh>=5)) {
      ///Pub은 1사분면에 속하게
      ///그리드값은 0, x, 0에서 4까지
      yp=rand()%5;
      xp=rand()%5;
      if (Grid[yp][xp]==0) {
        key_main=0;
      }
    }
  }

  ///구조체 생성
  struct Location P = {xp,yp};
  struct Location H = {xh,yh};
///1페이즈로 그리도와 펍위치, 집위치 전달
  One_paze(Grid,P,H);
}

void One_paze(int Grid[10][10],struct Location P,struct Location H) {
  ///walk는 랜덤워크 수 0,1,2,3 중 하나를 담을 변수이고, time은 턴을 나타내는 시간 변수
  int walk;
  int time=0;
  int paze=0;
  key_main=1;
  ///M은 술 취한 사람
  struct Location M = {P.x,P.y};
  ///paze변수에 펍의 x좌표로 1사분면인지, 4사분면인지 구분
  if (P.x < 5) {
    paze=1;
  }else {
    paze=4;
  }
  ///while문으로 조건은 key_main은 시작은 1이고 2가 되면 2페이즈로 3이 되면 3페이즈로
  while(key_main!=2 && key_main!=3) {
    ///1턴부터 시작하는 것이 낫다고 판단.
    time++;
    usleep(300000);

    clearScreen();
    // 출력부분
    setlocale(LC_ALL,"");
    for (int i=0;i<10;i++) {
      for (int j=0;j<10;j++) {
        ///1. 벽일 때
        ///2. 펍과 주인공이 겹칠 때 -> 주인공 출력
        ///3. 펍만 -> 펍 출력
        ///4. 주인공만 -> 주인공 출력
        ///5. 집 -> 집
        if (Grid[i][j]==1) {
          printf("%*s",2,"█︎");
        }else if (i==P.y&&j==P.x && i==M.y && j==M.x) {
          printf("%2s", "M");
        }else if (i==M.y && j==M.x) {
          printf("%2s", "M");
        }else if (i==P.y && j==P.x) {
          printf("%2s", "P");
        }else if (i==H.y && j==H.x) {
          printf("%2s", "H");
        }else {
          printf("%2s", " ");
        }
      }
      printf("|\n");
    }

    printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
    printf("%d턴\n",time);
    ///랜덤 워크 부분 switch로 구현
    walk=rand()%4;

    switch (walk) {
      case 0: // x좌표 -1
        if (M.x > 0 && Grid[M.y][M.x - 1] == 0) {  // 왼쪽으로 이동
          M.x--;
        }
      break;
      case 1: // x좌표 +1
        if (M.x < 9 && Grid[M.y][M.x + 1] == 0) {  // 오른쪽으로 이동
          M.x++;
        }
      break;
      case 2: // y좌표 -1
        if (M.y > 0 && Grid[M.y - 1][M.x] == 0) {  // 위로 이동
          M.y--;
        }
      break;
      case 3: // y좌표 +1
        if (M.y < 9 && Grid[M.y + 1][M.x] == 0) {  // 아래로 이동
          M.y++;
        }
      break;
    }
    ///다음 페이즈로 넘어가기 위한 조건을 만족했는지 확인
    if (paze == 1) {//1사분면에서 x가 먼저 5이면 2사분면 y가 5이면 3사분면
      if (M.x==5) {
        key_main=2;
      } else if (M.y==5) {
        key_main=3;
      }
    }
    if (paze == 4) {//페이즈 4일 때 x가 5미만이면 3사분면, y가 5미만이면 2사분면
      if(M.y==4) {
        key_main=2;
      }else if (M.x==4) {
        key_main=3;
      }
    }
  }

  //key_main이 2이면 2페이즈로, 3이면 3페이즈로

  if (key_main==2) {
    Two_paze(Grid,P,H,M);
  }else if (key_main == 3) {
    Three_paze(Grid,P,H,M);
  }
}

void Two_paze (int Grid[10][10],struct Location P,struct Location H,struct Location M) {
  int now=2;
  clearScreen();
  printf("123");
}

void Three_paze (int Grid[10][10],struct Location P,struct Location H,struct Location M) {
  int now=3;
  clearScreen();
  printf("몰라");
}