#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <MIDIData.h>
#include <FreeImage.h>
#include <vector>
#include <numeric>
#include "Case1.h"

using namespace std;

typedef struct RgbColor
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RgbColor; // RGBそれぞれ最大値は255

typedef struct HsvColor
{
	unsigned char h;
	unsigned char s;
	unsigned char v;
} HsvColor; //HSVそれぞれ最大値は255

void RgbToHsv(RgbColor& rgb, HsvColor& hsv)
{
	unsigned char rgbMin, rgbMax;

	rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
	rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

	hsv.v = rgbMax;
	if (hsv.v == 0)
	{
		hsv.h = 0;
		hsv.s = 0;
		return;
	}

	hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
	if (hsv.s == 0)
	{
		hsv.h = 0;
		return;
	}

	if (rgbMax == rgb.r)
		hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
	else if (rgbMax == rgb.g)
		hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
	else
		hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

	return;
}

int TestMIDIData()
{
	MIDIData* pMIDIData;
	MIDITrack* pMIDITrack;

	/* MIDIデータの生成(フォーマット0,トラック数1,TPQNベース,120) */
	pMIDIData = MIDIData_Create(MIDIDATA_FORMAT0, 1, MIDIDATA_TPQNBASE, 120);
	if (pMIDIData == NULL) {
		printf("MIDIデータの生成に失敗しました。\n");
		return 0;
	}

	/* 最初のトラックへのポインタを取得 */
	pMIDITrack = MIDIData_GetFirstTrack(pMIDIData);

	/* イベントを挿入 */
	MIDITrack_InsertTrackName(pMIDITrack, 0, L"ちょうちょ"); /* タイトル */
	MIDITrack_InsertTempo(pMIDITrack, 0, 60000000 / 120); /* 120BPM */
	MIDITrack_InsertProgramChange(pMIDITrack, 0, 0, 0); /* Piano1 */

	/* ノートイベントを挿入            Time  ch  key  vel  dur */
	MIDITrack_InsertNote(pMIDITrack, 0, 0, 67, 100, 120); /* そ */
	MIDITrack_InsertNote(pMIDITrack, 120, 0, 64, 100, 120); /* み */
	MIDITrack_InsertNote(pMIDITrack, 240, 0, 64, 100, 240); /* みー */
	MIDITrack_InsertNote(pMIDITrack, 480, 0, 65, 100, 120); /* ふぁ */
	MIDITrack_InsertNote(pMIDITrack, 600, 0, 62, 100, 120); /* れ */
	MIDITrack_InsertNote(pMIDITrack, 720, 0, 62, 100, 240); /* れー */
	MIDITrack_InsertNote(pMIDITrack, 960, 0, 60, 100, 120); /* ど */
	MIDITrack_InsertNote(pMIDITrack, 1080, 0, 62, 100, 120); /* れ */
	MIDITrack_InsertNote(pMIDITrack, 1200, 0, 64, 100, 120); /* み */
	MIDITrack_InsertNote(pMIDITrack, 1320, 0, 65, 100, 120); /* ふぁ */
	MIDITrack_InsertNote(pMIDITrack, 1440, 0, 67, 100, 120); /* そ */
	MIDITrack_InsertNote(pMIDITrack, 1560, 0, 67, 100, 120); /* そ */
	MIDITrack_InsertNote(pMIDITrack, 1680, 0, 67, 100, 240); /* そー */

	/* エンドオブトラックイベントを挿入 */
	MIDITrack_InsertEndofTrack(pMIDITrack, 1920);

	/* MIDIデータを保存 */
	MIDIData_SaveAsSMF(pMIDIData, L"chocho.mid");

	/* MIDIデータをメモリ上から削除 */
	MIDIData_Delete(pMIDIData);
	pMIDIData = NULL;

	return 1;
}

void TestFreeImage()
{
	try {
		// GIF画像を読み込み
		auto filename = "C:\\Users\\zen\\Desktop\\SS2assets\\モナリザ.jpg";
		auto image = std::shared_ptr<FIBITMAP>(
			FreeImage_Load(FIF_JPEG, filename, JPEG_DEFAULT), FreeImage_Unload);
		if (!image) {
			throw std::runtime_error("Load failed");
		}

		// グレースケールに変換
		auto greyimg = std::shared_ptr<FIBITMAP>(
			FreeImage_ConvertToGreyscale(image.get()), FreeImage_Unload);
		if (!greyimg) {
			throw std::runtime_error("Convert failed");
		}

		// PNGとして保存
		if (!FreeImage_Save(FIF_PNG, greyimg.get(), "convert.png", PNG_DEFAULT)) {
			throw std::runtime_error("Save failed");
		}
	}
	catch (std::exception & e) {
		std::cout << "exception!\n" << e.what() << std::endl;
	}
}

int TestMIDIDataFormat()
{
	MIDIData* pMIDIData = 0;
	MIDITrack* pMIDITrack0 = 0;
	MIDITrack* pMIDITrack1 = 0;
	MIDITrack* pMIDITrack2 = 0;

	/* MIDIデータの生成(フォーマット1,トラック数3,TPQNベース,120) */
	pMIDIData = MIDIData_Create(MIDIDATA_FORMAT1, 3, MIDIDATA_TPQNBASE, 120);

	/*--------------------------------------------------------------------*/
	/* トラック0 */
	pMIDITrack0 = MIDIData_GetFirstTrack(pMIDIData);
	MIDITrack_InsertTrackName(pMIDITrack0, 0, L"コンダクタートラック"); /* タイトル */
	MIDITrack_InsertTempo(pMIDITrack0, 0, 60000000 / 120); /* 120BPM */

	/* エンドオブトラックイベントを挿入 */
	MIDITrack_InsertEndofTrack(pMIDITrack0, 500);

	/*--------------------------------------------------------------------*/
	/*トラック1 */
	pMIDITrack1 = MIDITrack_GetNextTrack(pMIDITrack0);

	/* メロディのイベントを挿入 */
	MIDITrack_InsertTrackName(pMIDITrack1, 0, L"メロディ"); /* タイトル */
	MIDITrack_InsertTempo(pMIDITrack1, 0, 60000000 / 120); /* 120BPM */
	MIDITrack_InsertProgramChange(pMIDITrack1, 0, 0, 2); /* Piano */

	/* メロディのノートイベントを挿入*/
	MIDITrack_InsertNote(pMIDITrack1, 0, 0, 60, 100, 120); /* ど */
	MIDITrack_InsertNote(pMIDITrack1, 120, 0, 64, 100, 120); /* み */
	MIDITrack_InsertNote(pMIDITrack1, 240, 0, 67, 100, 120); /* そ */
	MIDITrack_InsertNote(pMIDITrack1, 360, 0, 60, 100, 120); /* ど */

	/* エンドオブトラックイベントを挿入 */
	MIDITrack_InsertEndofTrack(pMIDITrack1, 500);

	/*--------------------------------------------------------------------*/
	/*トラック2 */
	pMIDITrack2 = MIDITrack_GetNextTrack(pMIDITrack1);

	/* 伴奏のイベントを挿入 */
	MIDITrack_InsertTrackName(pMIDITrack2, 0, L"伴奏"); /* タイトル */
	MIDITrack_InsertTempo(pMIDITrack2, 0, 60000000 / 120); /* 120BPM */
	MIDITrack_InsertProgramChange(pMIDITrack2, 0, 1, 26); /* Guitar */

	/* 伴奏（和音）のノートイベントを挿入*/
	MIDITrack_InsertNote(pMIDITrack2, 0, 0, 60, 50, 480); /* ど */
	MIDITrack_InsertNote(pMIDITrack2, 0, 0, 64, 50, 480); /* み */
	MIDITrack_InsertNote(pMIDITrack2, 0, 0, 67, 50, 480); /* そ */
	MIDITrack_InsertNote(pMIDITrack2, 0, 0, 60, 50, 480); /* ど */

	/* エンドオブトラックイベントを挿入 */
	MIDITrack_InsertEndofTrack(pMIDITrack2, 500);

	/* MIDIデータを保存 */
	MIDIData_SaveAsSMF(pMIDIData, L"メロディ＆伴奏.mid");

	/* MIDIデータをメモリ上から削除 */
	MIDIData_Delete(pMIDIData);
	pMIDIData = NULL;
	return 1;
}

int Setup();//画像の読み込みと解析

void Case1_main();//画像を音楽に変える関数

	/*!---------------システム変数--------------*/
	char myPath[100];
	RGBQUAD myRGBQUAD;
	RgbColor myRGBcol;
	HsvColor myHSVcol;
	int number, myR, myG, myB, myH, myS, myV,
		sNumber, sMyR, sMyG, sMyB, sMyH, sMyS, sMyV;
	vector<int> vecR, vecG, vecB, vecH, vecS, vecV,
		sVecR, sVecG, sVecB, sVecH, sVecS, sVecV;
	std::string filename;
	/*!----------------------環境変数---------------------*/
	int smallImageWidth = 10;	//縮小画像の横幅
	int smallImageHeight = 10;  //縮小画像の高さ
	bool saveSmallImage = false; //縮小画像を保存するかどうか　trueかfalse
	/* 1 : 絵画の音化(オルゴール式)*/

	int case1_tempo = 240;				//曲全体のテンポ（BPM）デフォ120　使用していない
	float case1_tempo倍率 = 1.0;		//テンポに掛ける倍率　1で等倍

	int case1_lowestPitch_H = 60;		//一番低い音　0~127まで
	int case1_highestPitch_H = 72;		//一番高い音　0~127まで
	int case1_borderValue_H = 15;       //境界と認識する値の幅　0~255
	int case1_during_H = 120;			// 次の音までの長さ　120で100％　60で50％の長さ　120を超えると次の音と重なる

	int case1_lowestPitch_S = 48;		//一番低い音　0~127まで
	int case1_highestPitch_S = 72;		//一番高い音　0~127まで
	int case1_borderValue_S = 15;       //境界と認識する値の幅　0~255
	int case1_during_S = 120;			// 次の音までの長さ　120で100％　60で50％の長さ　120を超えると次の音と重なる
	/*!----------------------補足情報---------------------*/
	/*
	WIN!
	MIDIデータのピッチは普通のドが 60 (C4)で0～127まで
	*/

int main()
{
	/*!------------------------------------------管理---------------------------------------------*/
	for (string str = "1"; str == "1";) {
		if (Setup() == 0) {
			std::cout << "キーを入力して終了します\n";
			std::cin;
			return 1;
		}
		printf("使用するパターンを下の番号から選んで入力してください\n");
		printf("1 : 絵画の音化(オルゴール式)\n");

		int selectNumber;
		std::cin >> selectNumber;

		switch (selectNumber)
		{
		case 1:
			Case1_main();
			break;
		default:
			printf("正しい選択肢が入力されませんでした。\n");
			break;
		}

		printf("他の画像を使用してもう一度音楽を作成したい場合、 1 と入力してください。");
		std::cin >> str;
	}
	/*!--------------------------------------デバッグ用---------------------------------------------*/

	//TestMIDIData();                                                  //MIDIData.hの正常動作確認
	//TestFreeImage();                                                 //FreeImage.hの正常動作確認
	//TestMIDIDataFormat();									    //MIDIFormat1の正常動作確認
}

int Setup() {
	/*---------------------------画像の読み込みと各ピクセルの情報処理------------------------------*/

	printf("画像の絶対パスを入力するか、画像をドラック＆ドロップしてください\n");
	std::cin >> myPath;                                                 //絶対パスの入力
	std::string editPath = myPath;

	if (editPath[0] == '"') {
		editPath = editPath.substr(1);
		editPath = editPath.erase(editPath.size() - 1);
	}

	const char* myPathf = editPath.c_str();

	/*パス名からファイル名を抽出*/
	filename = editPath;
	const size_t last_slash_idx = filename.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		filename.erase(0, last_slash_idx + 1);
	}
	// Remove extension if present.
	const size_t period_idx = filename.rfind('.');
	if (std::string::npos != period_idx)
	{
		filename.erase(period_idx);
	}

	FIBITMAP* myImage = FreeImage_Load(FIF_JPEG, myPathf, JPEG_DEFAULT); //Bitmapイメージの作成
	if (!myImage) {
		printf("画像の読み込みに失敗しました\n");             //Bitmap イメージの存在判定
		return 0;
	}
	//std::cout << FreeImage_GetColorsUsed(myImage);                    //パレット数確認　結果 : 0
	int myWidth = FreeImage_GetWidth(myImage);                          // 幅取得
	int myHeight = FreeImage_GetHeight(myImage);                        //高さ取得
	printf("幅%d,高さ%d\n", myWidth, myHeight);

	/*
	myImageの縮小版の画像smallMyImageを定義　
	2，3個目の引数に幅を指定（width,height）
	*/
	if (myWidth < smallImageWidth || myHeight < smallImageHeight) {
		std::cout << "指定された画像の横幅もしくは高さが元画像を上回っています。\n元画像\n 幅 :" << myWidth << "\n 高さ" << myHeight;
		return 0;
	}
	FIBITMAP* sMyImage = FreeImage_Rescale(myImage, smallImageWidth, smallImageHeight);

	/*smallMyimageを保存　完成版には必要ない可能性*/
	std::string jpgFilenname = filename + ".jpg";
	if (saveSmallImage == true) { FreeImage_Save(FIF_JPEG, sMyImage, jpgFilenname.c_str(), JPEG_DEFAULT); }

	int sMyWidth = FreeImage_GetWidth(sMyImage);                          // 幅取得
	int sMyHeight = FreeImage_GetHeight(sMyImage);                        //高さ取得

	/*小さい画像のvector情報の入手*/
	for (int y = 0; y < sMyHeight; y++) {
		for (int x = 0; x < sMyWidth; x++) {
			FreeImage_GetPixelColor(sMyImage, x, y, &myRGBQUAD);
			sNumber = sMyWidth * y + x + 1;
			sMyR = (int)myRGBQUAD.rgbRed;
			sMyG = (int)myRGBQUAD.rgbGreen;
			sMyB = (int)myRGBQUAD.rgbBlue;
			/*HSV値に変換*/
			myRGBcol = { (unsigned char)sMyR,(unsigned char)sMyG,(unsigned char)sMyB };
			RgbToHsv(myRGBcol, myHSVcol);
			sMyH = myHSVcol.h;
			sMyS = myHSVcol.s;
			sMyV = myHSVcol.v;
			/*vectorに代入*/
			sVecR.push_back(sMyR);
			sVecG.push_back(sMyG);
			sVecB.push_back(sMyB);
			sVecH.push_back(sMyH);
			sVecS.push_back(sMyS);
			sVecV.push_back(sMyV);
			//printf("%d, R%d, G%d, B%d, H%d, S%d, V%d\n",sNumber,sMyR,sMyG,sMyB,sMyH,sMyS,sMyV);
			//printf("%d\n",vecR[number-1]);                           //vectorはvenR[0]から始まる
		}
	}

	return 1;
}

void Case1_main() {
	vector<int> pitch_H = Case1::Pitch(sVecH, case1_highestPitch_H, case1_lowestPitch_H); //Hから作られるの音の高さを決定
	vector<int> place_H = Case1::Place(sVecH, case1_borderValue_H, case1_during_H);		//Hから作られるの音の鳴る場所を決定
	vector<int> distance_H = Case1::Distance(sVecH, case1_borderValue_H, case1_during_H);  //Hから作られる音の次の音までの長さ

	vector<int> pitch_S = Case1::Pitch(sVecS, case1_highestPitch_S, case1_lowestPitch_S);//Sから作られるの音の高さを決定
	vector<int> place_S = Case1::Place(sVecS, case1_borderValue_S, case1_during_S);		//Sから作られる音の鳴る場所を決定
	vector<int> distance_S = Case1::Distance(sVecS, case1_borderValue_S, case1_during_S);  //Sから作られる音の次の音までの長さ
	/*place_Hについての例外規定*/
	if (place_H.size() == 0) { std::cout << "条件設定の欠陥により、音楽を生成できませんでした。\n"; return; }

	int sumALL = 0;			//H価とS値の合計　テンポの決定に使用
	int case1_lasttempo;	//最終的なテンポ

	for (int i = 0; i < 100; i++)
	{
		sumALL += sVecH[i];
		sumALL += sVecS[i];
	}

	sumALL = sumALL / 200;

	case1_lasttempo = (sumALL * 180 / 255 + 60) * case1_tempo倍率;
	printf("テンポは%dです\n", case1_lasttempo);

	/*!*********************************************************************************************/

	/*実際にMIDIにする*/

	MIDIData* pMIDIData;
	MIDITrack* pMIDITrack0 = 0;
	MIDITrack* pMIDITrack1 = 0;
	MIDITrack* pMIDITrack2 = 0;
	MIDITrack* pMIDITrack3 = 0;

	/* !MIDIデータの生成 (フォーマット1,トラック数4,TPQNベース,120) */
	pMIDIData = MIDIData_Create(MIDIDATA_FORMAT1, 4, MIDIDATA_TPQNBASE, 120);

	if (pMIDIData == NULL) {
		printf("MIDIデータの生成に失敗しました。\n");
	}
	/*!---------------------------------------------------------------------------------------------------*/
	/*!トラック0(コンダクタートラック)*/
	/* 最初のトラックへのポインタを取得 */
	pMIDITrack0 = MIDIData_GetFirstTrack(pMIDIData);

	/*イベントを挿入*/
	MIDITrack_InsertTrackName(pMIDITrack0, 0/*これは絶対時刻*/, L"コンダクタートラック");
	MIDITrack_InsertTempo(pMIDITrack0, 0, 60000000 / case1_lasttempo);

	/*!---------------------------------------------------------------------------------------------------*/
	/*!トラック1 ( Hの値 ) */
	/*二つ目のトラックへのポインタを取得*/
	pMIDITrack1 = MIDITrack_GetNextTrack(pMIDITrack0);

	/*イベントを挿入*/
	MIDITrack_InsertTrackName(pMIDITrack1, 0, L"H値トラック");
	MIDITrack_InsertProgramChange(pMIDITrack1, 0, 0, 24); /*ナイロンギター*/
	for (unsigned int i = 0; i < place_H.size(); i++) {
		/* ノートイベントを挿入            Time  ch  key  vel  dur */
		MIDITrack_InsertNote(pMIDITrack1, place_H[i], 0, pitch_H[i], 127, distance_H[i + 1]);
	}

	/* エンドオブトラックイベントを挿入 */
	MIDITrack_InsertEndofTrack(pMIDITrack1, place_H[place_H.size() - 1] + 120);

	/*!---------------------------------------------------------------------------------------------------*/
	/*!トラック2 ( Sの値 ) */
	/*3つ目のトラックへのポインタを取得*/
	pMIDITrack2 = MIDITrack_GetNextTrack(pMIDITrack1);
	MIDITrack_InsertProgramChange(pMIDITrack2, 0, 1, 24); /*ナイロンギター*/

	/*イベントを挿入*/
	MIDITrack_InsertTrackName(pMIDITrack2, 0, L"S値トラック");
	for (unsigned int i = 0; i < place_S.size(); i++) {
		/* ノートイベントを挿入            Time  ch  key  vel  dur */
		MIDITrack_InsertNote(pMIDITrack2, place_S[i], 1, pitch_S[i], 127, distance_S[i + 1]);
	}

	/* エンドオブトラックイベントを挿入 */
	MIDITrack_InsertEndofTrack(pMIDITrack2, place_S[place_S.size() - 1] + 120);

	/*!---------------------------------------------------------------------------------------------------*/
	/*!トラック3 (ドラムベーストラック)*/
	/*ドラムベーストラックへのポインタを取得*/
	pMIDITrack3 = MIDITrack_GetNextTrack(pMIDITrack2);

	/*イベントを挿入*/
	MIDITrack_InsertTrackName(pMIDITrack3, 0, L"ドラムトラック");
	for (int i = 0; i * 120 < place_H[place_H.size() - 1] || i * 120 < place_S[place_S.size() - 1]; i++) {
		/* ノートイベントを挿入            Time  ch  key  vel  dur */
		MIDITrack_InsertNote(pMIDITrack3, i * 120, 9, 35, 127,240);/*35番　アコースティックバスドラム*/
	}

	/* エンドオブトラックイベントを挿入 */
	MIDITrack_InsertEndofTrack(pMIDITrack3, MIDITrack_GetEndTime(pMIDITrack3) + 120);

	/*!---------------------------------------------------------------------------------------------------*/
	/* MIDIデータを保存 */
	setlocale(LC_CTYPE, "jpn");
	std::string midFilename = filename + ".mid";
	const char* midfileChar = midFilename.c_str();
	size_t capacity = strlen(midfileChar) + 1;
	wchar_t* str2 = (wchar_t*)malloc(sizeof(wchar_t) * capacity);

	// char -> wchar_tの変換
	int result = mbstowcs(str2, midfileChar, capacity);
	if (MIDIData_SaveAsSMF(pMIDIData, str2) == 1)printf("MIDIデータの生成に成功しました\n");

	/* MIDIデータをメモリ上から削除 */
	MIDIData_Delete(pMIDIData);
	pMIDIData = NULL;
}
