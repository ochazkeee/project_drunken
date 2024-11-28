#include <stdio.h>
#include <stdlib.h>
#include <time.h>

///전역 변수 설정
int key_main=1;
///구조체 정의 집 위치, 펍 위치
/// struct
struct Location {
  int x;
  int y;
};

//함수 선언
int make_background();
int make_location(int Grid[10][10]);
int One_paze(int Grid[10][10]);
int One_paze(int Grid[10][10]);

int main(){
  make_background();
  while(1){}

  return 0;
}
///플레이할 게임의 백그라운드, 배경 만들기
int make_background(){
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
int make_location(int Grid[10][10]) {
  srand(time(NULL));
  ///임의로 x좌표와 y좌표를 담을 변수
  int xh,yh;
  int xp,yp;
///현재 key_main은 글로벌 변수로 1로 지정되어 있음
  while(key_main!=0) {
    xh=rand()%10;
    yh=rand()%10;
    ///그리드값이 길위여야 하고 x는 0에서 4까지 그리고 y는 5에서 9까지이면
    ///1사분면에 속한다.
    if((Grid[yh][xh]==0) && (xh<5)&&(yh>=5)) {
      ///Pub은 4사분면에 속하게
      ///그리드값은 0, x는 5에서 9까지, y는 0에서 4까지
      xp=rand()%5+5;
      yp=rand()%5;
      if (Grid[yp][xp]==0) {
        key_main=0;
      }
    }
    if((Grid[yh][xh]==0) && (yh<5)&&(xh>=5)) {
      ///Pub은 1사분면에 속하게
      ///그리드값은 0, x는 0에서 4까지, y는 5에서 9까지
      yp=rand()%5+5;
      xp=rand()%5;
      if (Grid[yp][xp]==0) {
        key_main=0;
      }
    }

  }

  ///구조체 생성
  struct Location P = {xp,yp};
  struct Location H = {xh,yh};





}