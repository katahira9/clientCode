#pragma once

#ifdef _M_IX86
static_assert(false, "Not compatible with x86 version");
#endif // _M_IX86

//塗りつぶし色
static CONST FLOAT		FILL_COLOR[4]{ 0.0f, 0.8f, 0.2f, 1.0f };
//ウィンドウの幅
static CONST LONG		SCREEN_WIDTH{ 1280 };
//ウィンドウの高さ
static CONST LONG		SCREEN_HEIGHT{ 720 };
//フルスクリーンフラグ
static CONST BOOL		FULLSCREEN{ FALSE };
//見出しに表示する文字列
static CONST LPWSTR		APPLICATION_NAME{ L"blackjack" };
//スワップチェインのバックバッファーへ切り替えるタイミング
static CONST UINT		SYNC_INTERVAL{ TRUE };
//シェーダーのCSOファイルが置かれているディレクトリーのパス
static CONST CHAR*		CSO_FILE_DIRECTORY{ "data\\shader\\" };
//ImGuiを使用することをコンパイラーに伝達
#define USE_IMGUI		TRUE
//ImGuiのフォントサイズ
#define IMGUI_FONT_SIZE	16
//スクリーンショットを有効にするか
#define USE_SCREEN_SHOT TRUE