#pragma once
#include"Font.h"
#include"CardManager.h"
#include<thread>
#include"directxmath.h"
#include"SceneManager.h"
#include"http.h"
#include"KeyInput.h"
struct HoldKey
{
	float button_timer;
	int state;
	int count;
};
struct Arrow_Data
{
	FLOAT2 pos;
	float angle;
	FLOAT4 color;
};
class Game :public SceneBase
{
private:
	std::unique_ptr<Font> font;
	std::shared_ptr<Sprite>back;
	std::shared_ptr<Sprite>arrow;
	Http http;
	std::thread th[2];
	Arrow_Data arrow_datas[4];
	std::string oldcardData;
	CardManager cardsmanager;
	int state = 0;
	int result = false;
	int my_id = 0;
	bool hit;
	Key enter{ VK_RETURN };
	Key space{ VK_SPACE};
	std::string tableID;
	std::string playerID;
	std::string thread_key[2] = { "tableID","playerID" };
	char thread_response[70] = { "" };
	int raise_tips = 0;
	std::string thread_value[2];
	HoldKey arrow_R;
	int original_tips = 0;

	HoldKey arrow_L;
	void HoldKey_updata(HoldKey &key,int value);
	enum gamestate
	{

		DEAL,
		HIT,
		WAIT_END,
		RESULT,
	};
	enum gameresult
	{

		Lose,
		Win,
		DRAW
	};
public:
	Game(ID3D11Device* d) :SceneBase(d) {}
	~Game()
	{
		
		
			release();
			http.Stop();
			for (int i = 0; i < 2; i++)
			{
				if (th[i].joinable())
					th[i].join();
			}
		
	}
	void initialize(ID3D11Device*);
	void update(float);
	void render(ID3D11DeviceContext*, float);
	void release();
	void uninitialize();
	
	

	
};

