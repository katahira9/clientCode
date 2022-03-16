#pragma once
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>


using namespace utility;                    // 文字列変換などの一般的なユーティリティ
using namespace web;                        // URIのような共通の機能
using namespace web::http;                  // 共通のHTTP機能
using namespace web::http::client;          // HTTP クライアントの機能
using namespace concurrency::streams;       // 非同期ストリーム
#define ONEDATA 1

class Http
{
public:
	
	enum Funcnum
	{
		gather=0,
		wait,
		select,
		logout,
		deadline,
		gamestart,
		gamecheck,
		hit,
		stand,
		search,
		signup,
		tip_select,
		tip_updata,
	};

	//ゲーム開始待機時に使うスレッド関数
	void wakethread_wait();
	//ゲーム中にゲームの状況を確認するスレッド関数
	void wakethread_game();

	void wakethread_action();
	void Initialize()
	{
		
		stop_flg = false;
	}
	std::string dividejson(char* info);
	std::string findbody(char* info);
	std::string findbody_mysql(char* info);
	void findbody_go();
	void Stop() { stop_flg = true; }
	// ポスト送信を行う関数 anser : 結果を返す count : key, valueの配列の要素数
	pplx::task<void> post(char* anser, std::string* key, std::string* value, int count,int funcnum);
private:
	//スレッドの停止フラグ
	bool stop_flg = false;
	std::wstring url[FUNC_COUNT] =
	{
		L"https://72pfojq9qj.execute-api.ap-northeast-1.amazonaws.com/APItest/gather",
		L"https://72pfojq9qj.execute-api.ap-northeast-1.amazonaws.com/APItest/dynamodbctrl",
		L"https://orcf5tsrmc.execute-api.ap-northeast-1.amazonaws.com/game_join/dynamodbctrl",
		L"https://a4s9v4nfel.execute-api.ap-northeast-1.amazonaws.com/logout/dynamodbctrl",
		L"https://lby1jguxmj.execute-api.ap-northeast-1.amazonaws.com/deadline/dynamodbctrl",
		L"https://lc5brc05ib.execute-api.ap-northeast-1.amazonaws.com/gamestart/dynamodbctrl",
		L"https://a1b1zqk8tf.execute-api.ap-northeast-1.amazonaws.com/gamecheck/dynamodbctrl",
		L"https://vd6uvixu6a.execute-api.ap-northeast-1.amazonaws.com/hit/dynamodbctrl",
		L"https://nshqyq3wx2.execute-api.ap-northeast-1.amazonaws.com/stand/dynamodbctrl",
		L"https://za650nxkf8.execute-api.ap-northeast-1.amazonaws.com/default/go-test",
		L"https://xd0umttwbe.execute-api.ap-northeast-1.amazonaws.com/default/test_ruby",
		L"https://35kc8kpvdb.execute-api.ap-northeast-1.amazonaws.com/select_data/mysql_post",
		L"https://tflm91lsa8.execute-api.ap-northeast-1.amazonaws.com/tip_updata/mysql_post",
	};
		
};


