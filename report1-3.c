#include <stdio.h>

int pak[20];  // 0~8: 各ゾーンに対応, -1: 消滅フラグ
int cnt = 0;  // 時刻
const int aa = 14, ii = 44, uu = 42, ee = 8;

int chkzone(int zone);
int cntzone(int zone);
int chkend();


int main() {
  int i;
  int nowA, nowa, nowI, nowb, nowU, nowc, nowE;
  int Icnt = 0, Ucnt = 0, Ecnt = 0;

  /* ---- 初期化 ---- */
  for (i = 0; i < 20; i++) {
    pak[i] = 0;
  }
  pak[0] = 1;
  /* -- 初期化終了 -- */

  while (1) {  // 全パケットが到達または消滅するまでカウント
    cnt++;
    if (chkend() == 1) break;

    // ゾーンえ
    nowE = chkzone(7);
    if (nowE != -1) {
      Ecnt++;
      if (Ecnt == ee) {  // ゾーンうに入ってから8ms経過
	pak[nowE]++;
	if (nowE == 19) break;
	Ecnt = 0;  // カウントリセット
      }
    }
    
    // ゾーンC
    nowc = chkzone(6);
    if (nowc != -1 && cntzone(7) == 0) {
      pak[nowc]++;
    }
    
    // ゾーンう
    nowU = chkzone(5);
    if (nowU != -1) {
      Ucnt++;
      if (Ucnt == uu) {  // ゾーンうに入ってから42ms経過
	if (cntzone(6) < 5) {  // 次のルーターに空きがあるなら送る（次は勝手にry）
	  pak[nowU]++;
	}
	else {  // 次のルーター(zone)が満杯ならパケットを破棄
	  pak[nowU] = -1;
	}
	Ucnt = 0;  // カウントリセット
      }
    }
    
    // ゾーンB
    nowb = chkzone(4);  // ゾーンBの待機列先頭のパケット番号を取得
    if (nowb != -1 && cntzone(5) == 0) {
      pak[nowb]++;
    }
    
    // ゾーンい
    nowI = chkzone(3);
    if (nowI != -1) {
      Icnt++;
      if (Icnt == ii) {  // ゾーンいに入ってから44ms経過
	if (cntzone(4) < 5) {  // 次のルーターに空きがあるなら送る（次は勝手にry）
	  pak[nowI]++;
	}
	else {  // 次のルーター(zone)が満杯ならパケットを破棄
	  pak[nowI] = -1;
	}
	Icnt = 0;  // カウントリセット
      }
    }
    
    // ゾーンA
    nowa = chkzone(2);  // ゾーンAの待機列先頭のパケット番号を取得
    if (nowa != -1 && cntzone(3) == 0) {  // 伝送路がフリーなら送る (次は勝手に来るので呼ぶ必要なし)
      pak[nowa]++;
    }

    // ゾーンあ
    nowA = chkzone(1);
    if (nowA != -1 && cnt % (aa + 1) == 0) {    // タイマが14の倍数なら
      if (nowA < 19 && cntzone(2) < 5) {  // 送って次を呼ぶ
	pak[nowA]++;
	pak[nowA + 1] = 1;
      }
      else if (4 < cntzone(2)) {  // 次のルーター(zone)が満杯ならパケットを破棄
	pak[nowA] = -1;
	pak[nowA + 1] = 1;
      }
      else if (nowA == 19) {
	pak[nowA]++;
      }
    }

    /* デバッグ */
    printf("pak[] = {");
    for (i = 0; i < 19; i++) printf("%2d, ", pak[i]);
    printf("%2d}  time = %d\n", pak[19], cnt);
      
  }

  printf("Bobに到達したパケット数: %d", cntzone(8));
  return 0;
}


int chkzone(int zone) {
  int i;

  for (i = 0; i < 20; i++) {
    if (pak[i] == zone) return i;
  }
  return -1;
}


int cntzone(int zone) {
  int i, num = 0;
  
  for (i = 0; i < 20; i++) {
    if (pak[i] == zone) num++;
  }
  return num;
}


int chkend() {
  int i, num = 0;

  for (i = 0; i < 20; i++) {
    if (pak[i] == 8 || pak[i] == -1) {
      num++;
    }
  }

  if (num == 20) return 1;
  return 0;
}
