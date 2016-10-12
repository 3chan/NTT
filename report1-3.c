#include <stdio.h>

#define ROUTER 5

int pak[20];  // 0~8: 各ゾーンに対応, 9: 到達, 10:delayA待機(仮), -1: 消滅フラグ
int cnt = 0;  // 時刻
const int aa = 12, ii = 24, uu = 12, ee = 6;
const int delayA = 2, delayI = 20, delayU = 30, delayE = 2;

int chkzone(int zone);
int cntzone(int zone);
int chkend();


int main() {
  int i;
  int nowA, nowa, nowI, nowb, nowU, nowc, nowE, nowBob, nowAdelay;
  int Icnt = 0, Ucnt = 0, Ecnt = 0;
  int Acnt = 0, Bcnt = 0, Ccnt = 0, Bobcnt = 0;
  int delayAcnt = 0;

  /* ---- 初期化 ---- */
  for (i = 0; i < 20; i++) {
    pak[i] = 0;
  }
  pak[0] = 1;
  /* -- 初期化終了 -- */

  while (1) {  // 全パケットが到達または消滅するまでカウント
    cnt++;

    // Bob
    nowBob = chkzone(8);
    if (nowBob != -1) {
      Bobcnt++;
      if (Bobcnt == delayE) {
	pak[nowBob]++;
	Bobcnt = 0;
      }
    }
    
    // ゾーンえ
    nowE = chkzone(7);
    if (nowE != -1) {
      Ecnt++;
      if (Ecnt == ee) {  // ゾーンうに入ってから8ms経過
	pak[nowE]++;
	// if (nowE == 19) break;
	Ecnt = 0;  // カウントリセット
      }
    }
    
    // ゾーンC
    nowc = chkzone(6);
    if (nowc != -1) {
      Ccnt++;
      if (delayU <= Ccnt && cntzone(7) == 0) {
	pak[nowc]++;
	Ccnt = 0;
      }
    }
    
    // ゾーンう
    nowU = chkzone(5);
    if (nowU != -1) {
      Ucnt++;
      if (Ucnt == uu) {
	if (cntzone(6) < ROUTER) {  // 次のルーターに空きがあるなら送る
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
    if (nowb != -1) {
      Bcnt++;
      if (delayI <= Bcnt && cntzone(5) == 0) {
	pak[nowb]++;
	Bcnt = 0;  // カウントリセット
      }
    }
    
    // ゾーンい
    nowI = chkzone(3);
    if (nowI != -1) {
      Icnt++;
      if (Icnt == ii) {
	if (cntzone(4) < ROUTER) {  // 次のルーターに空きがあるなら送る
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
    if (nowa != -1) {
      Acnt++;
      if (cntzone(3) == 0) {  // 伝送路がフリーなら送る (次は勝手に来るので呼ぶ必要なし)
	pak[nowa]++;
	Acnt = 0;  // カウントリセット
      }
    }

    // 伝送遅延A待機
    nowAdelay = chkzone(10);
    if (nowAdelay != -1) {
      delayAcnt++;
      if (delayAcnt == delayA) {
	if (cntzone(3) == 0) pak[nowAdelay] = 3;  // 伝送路が空いていたら直接送る
	else {
	  if (cntzone(2) < ROUTER) {  // 伝送路が空いてないが、ルーターに空きがあればルーターへ送る
	    pak[nowAdelay] = 2;
	  }
	  else {
	    pak[nowAdelay] = -1;  //  上記2パターンに当てはまらなければ破棄
	  }
	}
	delayAcnt = 0;
      }
    }
    
    // ゾーンあ
    nowA = chkzone(1);
    if (nowA != -1 && (cnt - 1) % (aa) == 0 && cnt != 1) {    // タイマが12の倍数なら
      if (nowA < 20) {  // 送って次を呼ぶ
	pak[nowA] = 10;
      }
      pak[nowA + 1] = 1;
    }

    /* デバッグ */
    printf("pak[] = {");
    for (i = 0; i < 19; i++) printf("%2d, ", pak[i]);
    printf("%2d}  time = %d\n", pak[19], cnt);

    /* 終了確認 */
    if (chkend() == 1) break;
  }

  printf("Bobに到達したパケット数: %d", cntzone(9));
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
    if (pak[i] == 9 || pak[i] == -1) {
      num++;
      // printf("%d, num = %d\n", i, num);      
    }
    // printf("num = %d\n", num);
  }

  if (num == 20) {
    // printf("num = %d  <= 20\n", num);
    return 1;
  }
  return 0;
}
