#include	"framework.h"
#include"game.h"
#include<thread>
#include	<time.h>
#include"SceneManager.h"
#include <random>
#include"Player.h"
/*******************************************************************************
	グローバル変数
*******************************************************************************/
#define TIME_LIMIT 60
#define MAX_MEMBER 4
void Game::initialize(ID3D11Device* d)
{
	result = -1;
	my_id = std::stoi(SceneManager::getInstance()->playerID);
	font = std::make_unique<Font>(d, "data/MS Gothic.fnt", 1024);
	wchar_t* w_back = L"data/casino-background-vectors.jpg";
	back = std::make_shared<Sprite>(d, w_back);
	wchar_t* w_arrow = L"data/arrow.png";
	arrow = std::make_shared<Sprite>(d, w_arrow);
	cardsmanager.Initialize(d);
	arrow_datas[0].angle = 90;
	arrow_datas[0].pos = { 1120,630 };
	arrow_datas[0].color = { 1,0,0,1 };
	arrow_datas[1].angle = -90;
	arrow_datas[1].pos = { 1010,630 };
	arrow_datas[1].color = { 0,0,1,1 };
	P_MANAMGER.Initialize();
	for (int i = 0; i < MAX_MEMBER; i++)
	{
		SceneManager::getInstance()->Player_data[i] = "";
	}
	th[0] = std::thread(&Http::wakethread_game, &http);
	th[1] = std::thread(&Http::wakethread_action, &http);

	for (int i = 0; i < MAX_MEMBER; i++)
	{
		P_MANAMGER.players[i].On_Active(i);
	}
	tableID = SceneManager::getInstance()->tableID;
	playerID = SceneManager::getInstance()->playerID;
	thread_value[0] = tableID;
	thread_value[1] = playerID;
	state = 0;
	hit = false;
	raise_tips =0;
	original_tips = atoi(SceneManager::getInstance()->tipCount.c_str());

}
void Game::uninitialize()
{
	char result[HTTP_MEMORY] = "";
	std::string key[ONEDATA * 2] = { "" };
	std::string value[ONEDATA * 2] = { "" };
	key[0] = "tableID";
	value[0] = SceneManager::getInstance()->tableID;
	key[1] = "playerID";
	value[1] = SceneManager::getInstance()->playerID;
	http.post(result, key, value, ONEDATA * 2, http.logout).wait();
	http.Stop();
	for (int i = 0; i < 2; i++)
	{
		if (th[i].joinable())
		{
			th[i].join();
		}
	}
}
void  Game::HoldKey_updata(HoldKey &key,int value)
{
	if (original_tips > 0)
	{
		switch (key.state)
		{
		case 1:
			key.count = 0;
			if (original_tips > raise_tips && value > 0 || value < 0)
			{
				raise_tips += value;
				key.state++;

			}
			if (raise_tips < 0)
			{
				raise_tips = 0;
				key.state = 0;
			}
			break;
		case 2:
			if (key.button_timer > 1)
				key.state++;
			break;
		case 3:
			key.count++;
			if (key.count > 3)
			{
				if (original_tips > raise_tips && value > 0 || value < 0)
					raise_tips += value;
				if (raise_tips < 0)raise_tips = 0;
				key.count = 0;
			}
			break;


		}
	}
	else 
	{
		switch (key.state)
		{
		case 1:
			key.count = 0;
			
				raise_tips += value;
				key.state++;

			
			if (raise_tips < 0)
			{
				raise_tips = 0;
				key.state = 0;
			}
			break;
		case 2:
			if (key.button_timer > 1)
				key.state++;
			break;
		case 3:
			key.count++;
			if (key.count > 3)
			{
				
					raise_tips += value;
				if (raise_tips < 0)raise_tips = 0;
				key.count = 0;
			}
			break;


		}
	}
}
void	Game::release()
{
	char result[HTTP_MEMORY] = "";
	std::string key[ONEDATA * 2] = { "" };
	std::string value[ONEDATA * 2] = { "" };
	if (state > HIT)
	{
		key[0] = "tableID";
		value[0] = tableID;
		key[1] = "playerID";
		value[1] = playerID;

		http.post(result, key, value, ONEDATA * 2, http.stand).wait();
	}
	key[0] = "tableID";
	value[0] = SceneManager::getInstance()->tableID;
	key[1] = "playerID";
	value[1] = SceneManager::getInstance()->playerID;
	http.post(result, key, value, ONEDATA * 2, http.logout).wait();

}
void Game::update(float timer)
{
	for (int i = 0; i < MAX_MEMBER; i++)
	{
		char info[100] = { "" };
		if (SceneManager::getInstance()->Player_data[i] != "No")
		{
			char* resultdata[50] = { nullptr };
			strcpy(info, SceneManager::getInstance()->Player_data[i].c_str());
			SceneManager::getInstance()->divideinfo(info, resultdata, "_");
			cardsmanager.HitSetDatas(resultdata, i);
		}
	}

	cardsmanager.Update(timer);


	switch (state)
	{
	case DEAL:
	{
		if (cardsmanager.setcount > 1 + 2 * std::stoi(SceneManager::getInstance()->tableMember) && cardsmanager.AllMoved() == true)
		{
			state++;
		}
	}

	break;

	case HIT:

		if (GetKeyState(VK_LEFT) < 0)
		{

		arrow_L.button_timer += timer;
		arrow_R.button_timer = 0;
			if (arrow_L.state == 0)arrow_L.state = 1;
			HoldKey_updata(arrow_L, -1);

		}
		else if (GetKeyState(VK_RIGHT) < 0)
		{
			arrow_R.button_timer += timer;
			arrow_L.button_timer = 0;
			if (arrow_R.state == 0)arrow_R.state = 1;
			HoldKey_updata(arrow_R, 1);

		}
		else
		{
			arrow_R.button_timer = 0;
			arrow_L.button_timer = 0;
			arrow_R.state = 0;
			arrow_L.state = 0;

		}
		

		if (enter.down() && !SceneManager::getInstance()->actionflug[http.hit])
		{
			SceneManager::getInstance()->actionflug[http.hit] = true;
		}
		else
		{
			hit = false;

			if (space.down() || P_MANAMGER.players[my_id].GetScore() > 21&& P_MANAMGER.players[my_id].cardcount != 2)
			{
				SceneManager::getInstance()->actionflug[http.stand] = true;
				state = WAIT_END;
			}

		}

		break;
	case WAIT_END:

		if (SceneManager::getInstance()->tableStatus == "3" && cardsmanager.AllMoved())
		{
			int p_subscore = 21 - P_MANAMGER.players[my_id].GetScore();
			int n_subscore = 21 - P_MANAMGER.players[0].GetScore();
			if (P_MANAMGER.players[my_id].GetScore() > 21 || P_MANAMGER.players[0].GetScore() == 21 && P_MANAMGER.players[0].cardcount == 2)
			{


				result = Lose;
			}
			else
			{
				result = Win;
				if (p_subscore > n_subscore && P_MANAMGER.players[0].GetScore() < 22)
				{
					result = Lose;

				}

			}
			if (P_MANAMGER.players[my_id].GetScore() <= 21 && P_MANAMGER.players[0].GetScore() <= 21 && P_MANAMGER.players[my_id].GetScore() == P_MANAMGER.players[0].GetScore() && result == Win)
			{
				result = DRAW;
			}
			if(P_MANAMGER.players[my_id].GetScore()==21&&P_MANAMGER.players[my_id].cardcount==2&& P_MANAMGER.players[0].GetScore() == 21 && P_MANAMGER.players[0].cardcount != 2)
				result=Win;
			if (cardsmanager.AllMoved())
			{
				int resulttip = original_tips;
				
					if (result == Win)
					{
						resulttip += raise_tips;
					}
					else if (result == Lose)
					{
						resulttip -= raise_tips;
					}
				
				
				SceneManager::getInstance()->result_tip = std::to_string(resulttip);
				SceneManager::getInstance()->actionflug[http.tip_updata] = true;
				state++;

			}
		}

		break;
	case RESULT:
		cardsmanager.AllVisible();

		if (enter.down())
		{
			SceneManager::getInstance()->changeScene("Title");
		}
		break;
	}
}
void Game::render(ID3D11DeviceContext* context, float)
{
	back->render(context, NULL, FLOAT2(0, 0), FLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), FLOAT2(0, 0), FLOAT2(445, 450), 0);

	font->Begin(context);
	std::wstring raisetip_wc;
	std::string info = std::to_string(raise_tips);
	mbstowcs((wchar_t*)raisetip_wc.c_str(), info.c_str(), info.length()
		+ 1);
	font->Draw(1100, 640, raisetip_wc.c_str(), 1);
	if (state == HIT)
	{
		font->Draw(1000, 500, L"Enterでヒット", 1);
		font->Draw(1000, 550, L"Spaceでスタンド", 1);
		if (SceneManager::getInstance()->actionflug[http.hit])
		{
			font->Draw(1000, 450, L"しばらくお待ちください", 0.8);

		}
	}
	if (state > HIT && state < RESULT)
	{
		if (P_MANAMGER.players[my_id].GetScore() > 21)
		{
			font->Draw(1000, 500, L"バースト", 1);
		}
		else
		{
			if (P_MANAMGER.players[my_id].GetScore() == 21 && P_MANAMGER.players[my_id].cardcount == 2)
			{
				font->Draw(1000, 500, L"ブラックジャック", 1);
			}
			else
			{
				if (P_MANAMGER.players[my_id].GetScore() <= 21)
				{
					font->Draw(1000, 500, L"スタンド", 1);
				}
			}
		}
		font->Draw(1000, 550, L"他の人を待とう", 1);

	}
	if (original_tips > 0)
	{
		std::wstring originaltip_wc;
		std::string mytip_info = "MAX:" + SceneManager::getInstance()->tipCount;
		mbstowcs((wchar_t*)originaltip_wc.c_str(), mytip_info.c_str(), mytip_info.length()
			+ 1);
		font->Draw(1080, 600, originaltip_wc.c_str(), 0.7);
	}
	font->Draw(1160,640, L"+", 1);
	font->Draw(1050,640, L"-", 1);

	arrow->render(context, NULL, arrow_datas[0].pos, FLOAT2(100, 57), FLOAT2(0, 1), FLOAT2(100, 57), arrow_datas[0].angle, arrow_datas[0].color);
	arrow->render(context, NULL, arrow_datas[1].pos, FLOAT2(100, 57), FLOAT2(0, 1), FLOAT2(100, 57), arrow_datas[1].angle, arrow_datas[1].color);

	for (int i = 0; i < MAX_MEMBER; i++)
	{
		std::wstring wc;
		std::string info = std::to_string(P_MANAMGER.players[i].GetScore());
		mbstowcs((wchar_t*)wc.c_str(), info.c_str(), info.length()
			+ 1);
		if (state >= HIT)
		{
			if (P_MANAMGER.players[i].GetID() == my_id)
			{
				font->Draw(600, 450, (wchar_t*)wc.c_str(), 1);

			}
		}
		
		if (state == RESULT)
		{
			if (P_MANAMGER.players[i].GetID() == 0)
			{
				font->Draw(600, 200, (wchar_t*)wc.c_str(), 1);
			}
			if (P_MANAMGER.players[i].GetID() == P_MANAMGER.others[0] && P_MANAMGER.players[i].GetScore() != 0)
			{
				font->Draw(200, 300, (wchar_t*)wc.c_str(), 1);
			}
			if (P_MANAMGER.players[i].GetID() == P_MANAMGER.others[1] && P_MANAMGER.players[i].GetScore() != 0)
			{
				font->Draw(900, 300, (wchar_t*)wc.c_str(), 1);
			}
			font->Draw(900, 500, L"Enterでタイトルへ戻る", 1);
			if (result == Lose)
			{
				font->Draw(600, 300, L"Player_Lose", 1);

			}
			if (result == Win)
			{
				font->Draw(600, 300, L"Player_Win", 1);

			}
			if (result == DRAW)
			{
				font->Draw(600, 300, L"Draw", 1);

			}
		}
	}

	font->End(context);

	cardsmanager.Render(context);

}
