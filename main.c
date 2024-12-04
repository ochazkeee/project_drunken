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
void Two_paze(int Grid[10][10],struct Location P,struct Location H,struct Location M,int turn);
void Three_paze(int Grid[10][10],struct Location P,struct Location H,struct Location M,int turn);
int is_shoot(int Grid[10][10],struct Location I, struct Location J, struct Location M);
int Shoot(int shoot_i,int shoot_j);
void Last_paze(int Grid[10][10],struct Location P,struct Location H,struct Location M,int turn);

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
  int turn=0;
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
    turn++;
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
    Two_paze(Grid,P,H,M,turn);
  }else if (key_main == 3) {
    Three_paze(Grid,P,H,M,turn);
  }
}

void Two_paze (int Grid[10][10],struct Location P,struct Location H,struct Location M,int turn) {
  int cnt=0;
  int x,y;
  int walk_m,walk_i,walk_j;
  int die;
  int paze=0;
  int key_make=1;
  struct Location J={-2,-2};
  struct Location I={-3,-3};
///paze는 가야하는 곳
  if (H.x<5) {
    paze=1;
  }else if (H.x>=5) {
    paze=4;
  }


  while (key_main==2) {


    turn++;
    usleep(300000);
    ///임상은 cnt가 10일 떄, 자경은 cnt가 20일 떄
    cnt++;
    ///임상 생성
    if (cnt==10) {
      //임상의 x좌표 y좌표 랜덤으로 생성
      while (key_make!=0) {
        //2사분면 x는 5에서 9까지 y는 0에서 4까지
        x=rand()%5+5;
        y=rand()%5;
        if (Grid[y][x]==0) {
          key_make=0;
        }
      }
      I.x=x;
      I.y=y;
    }
    ///자격 생성
    if (cnt==20) {
      //자경의 x좌표 y좌표 랜덤으로 생성
      while (key_make!=0) {
        x=rand()%5+5;
        y=rand()%5;
        if (Grid[y][x]==0) {
          key_make=0;
        }
      }
      J.x=x;
      J.y=y;
    }

    clearScreen();
    ///출력 부분
    ///
    setlocale(LC_ALL,"");
    //i는 y j는 x
    for (int i=0;i<10;i++) {
      for (int j=0;j<10;j++) {

        if (Grid[i][j] == 1) { // 1. 벽일 때
          printf("%*s", 2, "█︎");
        } else if (i == M.y && j == M.x && i == I.y && j == I.x && i == J.y && j == J.x) {
          // 2. 자경(J), 임상(I), 주인공(M) 겹칠 때 -> 주인공 출력
          printf("%2s", "M");
        } else if (i == M.y && j == M.x && i == J.y && j == J.x) {
          // 3. 자경(J)과 주인공(M) 겹칠 때 -> 주인공 출력
          printf("%2s", "M");
        } else if (i == I.y && j == I.x && i == J.y && j == J.x) {
          // 4. 자경(J), 임상(I) 겹칠 때 -> 임상 출력
          printf("%2s", "I");
        } else if (i == M.y && j == M.x && i == I.y && j == I.x) {
          // 5. 임상(I)과 주인공(M) 겹칠 때 -> 주인공 출력
          printf("%2s", "M");
        } else if (i == J.y && j == J.x) {
          // 6. 자경(J)만 있을 때 -> 자경 출력
          printf("%2s", "J");
        } else if (i == I.y && j == I.x) {
          // 7. 임상(I)만 있을 때 -> 임상 출력
          printf("%2s", "I");
        } else if (i == M.y && j == M.x) {
          // 8. 주인공(M)만 있을 때 -> 주인공 출력
          printf("%2s", "M");
        } else if (i == P.y && j == P.x) {
          // 9. 펍(P)이 있을 때 -> 펍 출력
          printf("%2s", "P");
        } else if (i == H.y && j == H.x) {
          // 10. 집(H)이 있을 때 -> 집 출력
          printf("%2s", "H");
        } else {
          // 빈 공간 출력
          printf("%2s", " ");
        }
      }
      printf("|\n");
    }
    printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");

    ///총쏘기
    ///0이면 전부 생존.
    ///1이면 주인공 죽음. 게임 끝
    ///2이면 I죽음 임상 I.x=-1 I.y=-1
    ///3이면 J죽음 자경 J.x=-1 J.y=-1
    die=is_shoot(Grid,I,J,M);
    if (die==1) {
      key_main=0;
    }else if (die==2) {
      I.x=-1;
      I.y=-1;
    }else if (die==3) {
      J.x=-1;
      J.y=-1;
    }
    ///랜덤 워크 범위 지정
    //M의 랜덤워크 전부다
    if (key_main==2) {
      walk_m=rand()%4;

      switch (walk_m) {
        case 0: // x좌표 -1
          if (M.x > 0 && Grid[M.y][M.x - 1] == 0) {  // 왼쪽으로 이동
            M.x--;
            //4사분면으로 가야하므로
            if (paze == 4 && M.x == 4) {
              M.x++;
            }
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
            if (paze == 1 && M.y == 5) {
              M.y--;
            }
          }
        break;
      }
    }

    //I 랜덤워크 2사분면내에서 움직여야함. I J 둘 다
    //임상, 자경은 x, y를 -1로 바꿔서 사망 표시
    if (cnt>=10 && I.x!=-1 && key_main==2) {

      walk_i=rand()%4;

      switch (walk_i) {
        case 0: // x좌표 -1
          if (I.x > 5 && Grid[I.y][I.x - 1] == 0) {  // 왼쪽으로 이동
            I.x--;
          }
        break;
        case 1: // x좌표 +1
          if (I.x < 9 && Grid[I.y][I.x + 1] == 0) {  // 오른쪽으로 이동
            I.x++;
          }
        break;
        case 2: // y좌표 -1
          if (I.y > 0 && Grid[I.y - 1][I.x] == 0) {  // 위로 이동
            I.y--;
          }
        break;
        case 3: // y좌표 +1
          if (I.y < 4 && Grid[I.y + 1][I.x] == 0) {  // 아래로 이동
            I.y++;
          }
        break;
      }
      //페이즈 범위를 넘었는지
      //4사분면이 막힘

    }
    if (cnt>=20 && J.x!=-1 && key_main==2) {
      //J 랜덤워크

      walk_j=rand()%4;

      switch (walk_j) {
        case 0: // x좌표 -1
          if (J.x > 5 && Grid[J.y][J.x - 1] == 0) {  // 왼쪽으로 이동
            J.x--;

          }
        break;
        case 1: // x좌표 +1
          if (J.x < 9 && Grid[J.y][J.x + 1] == 0) {  // 오른쪽으로 이동
            J.x++;
          }
        break;
        case 2: // y좌표 -1
          if (J.y > 0 && Grid[J.y - 1][J.x] == 0) {  // 위로 이동
            J.y--;
          }
        break;
        case 3: // y좌표 +1
          if (J.y < 4 && Grid[J.y + 1][J.x] == 0) {  // 아래로 이동
            J.y++;
          }
        break;
      }
    }
    ///paze==1일때 M.x==4면 key_main==1
    ///paze==4 M.y==5이면 key_main==4
    if (paze==1 && M.x==4) {
      key_main=1;
    }
    if (paze==4 && M.y==5) {
      key_main=4;
    }

  }
  Last_paze(Grid,P,H,M,turn);

}

void Three_paze (int Grid[10][10],struct Location P,struct Location H,struct Location M,int turn) {
  int cnt=0;
  int x,y;
  int walk;
  int die;
  int s_percent=0;
  int paze=0;
  int key_make=1;
  struct Location S={-2,-2};

  ///paze는 가야하는 곳
  if (H.x<5) {
    paze=1;
  }else if (H.x>=5) {
    paze=4;
  }
  while (key_main==3) {
    turn++;
    usleep(300000);
    cnt++;
    ///스나이퍼 생성
    if (cnt==10) {
      //임상의 x좌표 y좌표 랜덤으로 생성
      while (key_make!=0) {
        //2사분면 x는 5에서 9까지 y는 0에서 4까지
        x=rand()%5;
        y=rand()%5+5;
        if (Grid[y][x]==1) {
          key_make=0;
        }
      }
      S.x=x;
      S.y=y;
    }
    clearScreen();
    ///출력 부분
    ///
    setlocale(LC_ALL,"");
    //i는 y j는 x
    for (int i=0;i<10;i++) {
      for (int j=0;j<10;j++) {

        if (Grid[i][j] == 1 && S.y==i && S.x==j) { // 스나이퍼 위치
          printf("%2d",s_percent);
        } else if (Grid[i][j] == 1) { // 1. 벽일 때
          printf("%*s", 2, "█︎");
        } else if (i == M.y && j == M.x) {
          // 8. 주인공(M)만 있을 때 -> 주인공 출력
          printf("%2s", "M");
        } else if (i == P.y && j == P.x) {
          // 9. 펍(P)이 있을 때 -> 펍 출력
          printf("%2s", "P");
        } else if (i == H.y && j == H.x) {
          // 10. 집(H)이 있을 때 -> 집 출력
          printf("%2s", "H");
        } else {
          // 빈 공간 출력
          printf("%2s", " ");
        }
      }
      printf("|\n");
    }
    printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");


  }
}

///is_shoot함수
///I부터 우선으로 x좌표나 y좌표가 같고 벽이 사이에 없다면 Shoot 함수 호출
int is_shoot(int Grid[10][10],struct Location I,struct Location J,struct Location M) {
  //flag는 둘 사이에 벽이 있는지 확인 0이면 벽이 있는 거고 1이면 벽이 없는 것.
  //shoot_i,j가 0이면 안쏘고 1이면 50%확률로 M을 쏘고 2이면 30%확률로 서로를 쏨.
  int flag;
  int shoot_i=0;
  int shoot_j=0;
  //I와 M의 x 좌표
  if (I.x == M.x) {
    //y좌표 사이에 반복문으로 벽이 있는지 확인
    if (I.y == M.y) {
      // Shoot(I,M);
      shoot_i=1;
    }else if (I.y < M.y) {
      flag=1;
      for (int y=I.y; y<M.y; y++) {
        if (Grid[y][I.x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        // Shoot(I,M);
        shoot_i=1;
      }
    }else if (I.y > M.y) {
      flag=1;
      for (int y=M.y; y<I.y; y++) {
        if (Grid[y][I.x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        // Shoot(I,M);
        shoot_i=1;
      }
    }//이 아래 괄호끝나는 곳부터 I.y==M.y 비교
  }if (I.y == M.y) {
    //y좌표 사이에 반복문으로 벽이 있는지 확인
    if (I.x == M.x) {
      // Shoot(I,M);
      shoot_i=1;
    }else if (I.x < M.x) {
      flag=1;
      for (int x=I.x; x<M.x; x++) {
        if (Grid[I.y][x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        // Shoot(I,M);
        shoot_i=1;
      }
    }else if (I.x > M.x) {
      flag=1;
      for (int x=M.x; x<I.x; x++) {
        if (Grid[I.y][x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        // Shoot(I,M);
        shoot_i=1;
      }
    }
  }else if (I.x==J.x) {
    if (I.y == J.y) {
      // Shoot(I,J);
      shoot_i=2;
    }else if (I.y < J.y) {
      flag=1;
      for (int y=I.y; y<J.y; y++) {
        if (Grid[y][I.x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        // Shoot(I,J);
        shoot_i=2;
      }
    }else if (I.y > J.y) {
      flag=1;
      for (int y=J.y; y<I.y; y++) {
        if (Grid[y][I.x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        // Shoot(I,J);
        shoot_i=2;
      }
    }//이 아래는 y비교
  }else if (I.y==J.y) {
    if (I.x == J.x) {
      // Shoot(I,J);
      shoot_i=2;
    }else if (I.x < J.x) {
      flag=1;
      for (int x=I.x; x<J.y; x++) {
        if (Grid[I.y][x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        // Shoot(I,J);
        shoot_i=2;
      }
    }else if (I.x > J.x) {
      flag=1;
      for (int x=J.x; x<I.y; x++) {
        if (Grid[I.y][x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        // Shoot(I,J);
        shoot_i=2;
      }
    }
  }
  ///J가 총 쏨
  if (J.x == M.x) {
    //y좌표 사이에 반복문으로 벽이 있는지 확인
    if (J.y == M.y) {
      // Shoot(J,M);
      shoot_j=1;
    }else if (J.y < M.y) {
      flag=1;
      for (int y=J.y; y<M.y; y++) {
        if (Grid[y][J.x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        //Shoot(J,M);
        shoot_j=1;
      }
    }else if (J.y > M.y) {
      flag=1;
      for (int y=M.y; y<J.y; y++) {
        if (Grid[y][J.x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        //Shoot(J,M);
        shoot_j=1;
      }
    }//y
  }else if (J.y == M.y) {
    //y좌표 사이에 반복문으로 벽이 있는지 확인
    if (J.x == M.x) {
      // Shoot(J,M);
      shoot_j=1;
    }else if (J.x < M.x) {
      flag=1;
      for (int x=J.x; x<M.x; x++) {
        if (Grid[J.y][x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        //Shoot(J,M);
        shoot_j=1;
      }
    }else if (J.x > M.x) {
      flag=1;
      for (int x=M.x; x<J.x; x++) {
        if (Grid[J.y][x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        //Shoot(J,M);
        shoot_j=1;
      }
    }
  }else if (I.x==J.x) {
    if (I.y == J.y) {
      // Shoot(J,I);
      shoot_j=2;
    }else if (I.y < J.y) {
      flag=1;
      for (int y=I.y; y<J.y; y++) {
        if (Grid[y][J.x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        // Shoot(J,I);
        shoot_j=2;
      }
    }else if (I.y > J.y) {
      flag=1;
      for (int y=J.y; y<I.y; y++) {
        if (Grid[y][J.x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        // Shoot(J,I);
        shoot_j=2;
      }
    }
  }else if (I.y==J.y) {
    if (I.x == J.x) {
      // Shoot(J,I);
      shoot_j=2;
    }else if (I.x < J.x) {
      flag=1;
      for (int x=I.x; x<J.x; x++) {
        if (Grid[J.y][x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        // Shoot(J,I);
        shoot_j=2;
      }
    }else if (I.x > J.x) {
      flag=1;
      for (int x=J.x; x<I.x; x++) {
        if (Grid[J.y][x]==1) {
          flag=0;
        }
      }
      if (flag==1) {
        // Shoot(J,I);
        shoot_j=2;
      }
    }
  }
  //shoot 함수 호출
  return Shoot(shoot_i,shoot_j);
}

int Shoot(int shoot_i,int shoot_j) {
  srand(time(NULL));
  //확률이 50이므로 0이면 살고 1이면 죽음
  int victim_m;
  //확률이 30이므로 0에서 6이면 살고 789면 죽음
  int victim_each;

  if (shoot_i==1) {
    victim_m=rand()%2;
    if (victim_m==1) {
      printf("임상이 술 취한 남자한테 총을 쏴서 맞췄습니다.\n");
      return 1;
    }else {
      printf("임상이 술 취한 남자한테 총을 쐈지만 맞지 않았습니다.\n");

    }
  }else if (shoot_i==2) {
    victim_each=rand()%10;
    if (7<=victim_each && victim_each<=9) {
      printf("임상이 자경에게 총을 쏴서 맞췄습니다.\n");
      return 3;
    }else {
      printf("임상이 자경에게 총을 쐈지만 맞지 않았습니다.\n");
    }
  }else if (shoot_j==1) {
      victim_m=rand()%2;
      if (victim_m==1) {
        printf("자경이 술 취한 남자에게 총을 쏴서 맞췄습니다.\n");
        return 1;
      }else {
        printf("자경이 술 취한 남자에게 총을 쐈지만 맞지 않았습니다.\n");
      }
  }else if (shoot_j==2) {
    victim_each=rand()%10;
    if (7<=victim_each && victim_each<=9) {
      printf("자경이 임상에게 총을 쏴서 맞췄습니다.\n");
      return 2;
    }else {
      printf("자경이 임상에게 총을 쐈지만 맞지 않았습니다.\n");
    }
  }
  return 0;
}

void Last_paze(int Grid[10][10],struct Location P,struct Location H,struct Location M,int turn) {
  printf("4");
}