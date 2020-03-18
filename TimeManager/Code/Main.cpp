#include <DxLib.h>
#include"Time/Manager.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);//Log.txtを生成しないように設定
	ChangeWindowMode(TRUE);//非全画面にセット
	SetGraphMode(640, 480, 32);//画面サイズ指定
	SetMainWindowText("sample");//ウインドウに表示されるタイトルを指定
	SetAlwaysRunFlag(TRUE);//非アクティブ時も処理を続行
#ifndef NDEBUG
	SetWaitVSyncFlag(TRUE); //垂直同期する デバッグモードでは絶対にFALSEにしない
#else
	SetWaitVSyncFlag(FALSE); //垂直同期しない
#endif
	if (DxLib_Init() == -1) { return -1; }//初期化に失敗時にエラーを吐かせて終了
	SetDrawScreen(DX_SCREEN_BACK);//描画先を裏画面に
	SetWindowSizeChangeEnableFlag(TRUE); //ウィンドウを自由に拡大縮小できるように設定
	SetDrawMode(DX_DRAWMODE_BILINEAR); //拡大縮小で負荷を重くしてきれいに描画できるようにする

	Time::Level level;

	Time time(&level);

	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		Time::update();
		level.update();

		DrawFormatString(0, 0, 0xffffff, "%u", time.getNowCount());

		if( CheckHitKey(KEY_INPUT_SPACE) ) {
			level.stop();
		}
		else {
			level.start();
		}


		if( CheckHitKey(KEY_INPUT_ESCAPE) ) break;
		ScreenFlip();
		
#ifdef NDEBUG
		WaitTimer(1); //垂直同期を切ったのでWaitを追加
#endif
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}