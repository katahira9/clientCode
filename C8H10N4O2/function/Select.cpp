#include"SceneManager.h"
#include"Select.h"
void Select::initialize(ID3D11Device*d)
{
	strcpy(text, u8"ENTERで実行&閉じる");
	select_on = false;
	font = std::make_unique<Font>(d,"data/MS Gothic.fnt", 1024);
	window_on = false;
	miss = false;
	end_flug = false;
	SceneManager::getInstance()->actionflug[http.search] = true;
	th[0] = std::thread(&Http::findbody_go, &http);
}
void Select::uninitialize()
{
	
	http.Stop();
	for (int i = 0; i < 2; i++)
	{
		if (th[i].joinable())
			th[i].join();
	}
}
Select::~Select()
{
	
	
	http.Stop();
	for (int i = 0; i < 2; i++)
	{
		if (th[i].joinable())
			th[i].join();
	}
}
void Select::update(float timer)
{
	if (!select_on&&reload.down()&& SceneManager::getInstance()->actionflug[http.search]==false)
	{
		SceneManager::getInstance()->actionflug[http.search] = true;
	}
	if (space.down()&& !select_on)
	{
		int tablecount = 0;
		for (int i = 0; i < TABLE_NUM; i++)
		{
			if (SceneManager::getInstance()->tables[i].id != "" && SceneManager::getInstance()->tables[i].status == "1")
			{
				tablecount++;
			}
		}
	
		if (tablecount > 0)
		{
			tablecount--;
			std::random_device rnd;     // 非決定的な乱数生成器を生成
			std::mt19937 mt(rnd());
			std::uniform_int_distribution<> randmachi(0, tablecount);
			int randID = randmachi(mt);
			//selectID = randID;
			char result[HTTP_MEMORY] = "";
			std::string key[1] = { "" };
			std::string value[1] = { "" };
			key[0] = "tableID";
			std::string rand_str = SceneManager::getInstance()->tables[randID].id.c_str();
			value[0] = rand_str;
			http.post(result, key, value, ONEDATA, http.select).wait();
			char info[HTTP_MEMORY] = "";
			std::string str = http.findbody(result);

			memcpy(info, str.c_str(), str.size());
			char* check = strstr(result, "errorMessage");

			if (check == NULL)
			{
				select_on = true;
				char* divide[3];
				SceneManager::getInstance()->divideinfo(info, divide, "_");
				SceneManager::getInstance()->tableID = divide[0];
				SceneManager::getInstance()->playerID = divide[1];
				tableID = SceneManager::getInstance()->tableID;
				playerID = SceneManager::getInstance()->playerID;
				th[1] = std::thread(&Http::wakethread_wait, &http);

			}
		}

	}
	if (window_on&&!select_on)
	{
		
		ImGui::SetNextWindowSize(ImVec2(320, 100), ImGuiSetCond_Once);
		ImGui::Begin("Select");
		ImGui::InputText("ID", selectID, 30);
		if (enter.down())
		{
			char result[ HTTP_MEMORY] = "";
			std::string key[1] = { "" };
			std::string value[1] = { "" };
			key[0] = "tableID";
			value[0] = selectID;
			http.post(result, key, value, ONEDATA,http.select).wait();
			char info[HTTP_MEMORY] = "";
			std::string str = http.findbody(result);

			memcpy(info, str.c_str(), str.size());
			char* check = strstr(result, "errorMessage");

			if (check==NULL)
			{
				select_on = true;
				char* divide[3];
				SceneManager::getInstance()->divideinfo(info, divide, "_");
				SceneManager::getInstance()->tableID = divide[0];
				SceneManager::getInstance()->playerID = divide[1];
				tableID = SceneManager::getInstance()->tableID;
				playerID = SceneManager::getInstance()->playerID;
				th[1] = std::thread(&Http::wakethread_wait, &http);
				
			}
			else miss = true;
		}
			ImGui::Text(text);
		


		ImGui::End();
	}
	else if (back.down())
	{
		char result[ HTTP_MEMORY] = "";
		std::string key[ONEDATA * 2] = { "" };
		std::string value[ONEDATA * 2] = { "" };
		key[0] = "tableID";
		value[0] = SceneManager::getInstance()->tableID;
		key[1] = "playerID";
		value[1] = SceneManager::getInstance()->playerID;
		http.post(result, key, value, ONEDATA * 2,http.logout).wait();
		SceneManager::getInstance()->changeScene("Title");
	}
	if (enter.down() && !select_on)
	{
		if (!window_on)
		{
			window_on = true;
			strcpy(selectID, u8"クリックして入力");

		}
		else window_on = false;


	}
	if (enter.down() && select_on)
	{
		if (SceneManager::getInstance()->tableMaster == playerID&&!end_flug)
		{
			end_flug = true;
			SceneManager::getInstance()->actionflug[http.gamestart] = true;

		}
	}
	
	if (SceneManager::getInstance()->tableStatus == "2")
	{
		SceneManager::getInstance()->changeScene("Game");
	}
}
void  Select::render(ID3D11DeviceContext*context, float)
{

			font->Begin(context);
			if (end_flug)
			{
				font->Draw(0, 40, L"Connecting", 1);
			}
			if(SceneManager::getInstance()->tables[0].id!="")
			font->Draw(1000, 0, L"今入れる部屋", 1.3);

	for (int i = 0; i < TABLE_NUM; i++)
	{
		std::wstring wc_table;
		std::string str = "tableNo:" + SceneManager::getInstance()->tables[i].id;
		mbstowcs((wchar_t*)wc_table.c_str(),str.c_str() , str.size());
		if (SceneManager::getInstance()->tables[i].status == "1")
		{
			font->Draw(1000, 50+50*i, wc_table.c_str(), 1.3);
		}
	}

		if (!window_on||select_on)
			font->Draw(400, 400, L"BackSpaceで戻る", 2);
		if (!select_on)
		{
			font->Draw(400, 300, L"EnterでID入力", 2);
			font->Draw(300, 500, L"Spaceでランダムマッチ", 2);


		}
		else
		{
			if (SceneManager::getInstance()->tableMaster == playerID)
			{
				font->Draw(470, 300, L"Enterで締め切るよ", 2);
			}
			else
			{
				font->Draw(470, 300, L"開始まで待機", 2);
			}

		}
		if (select_on)
		{
			font->Draw(0, 0, L"テーブルID:", 1);
			std::wstring wc;
			mbstowcs((wchar_t*)wc.c_str(),tableID.c_str(),sizeof(tableID.c_str()));
			font->Draw(30*6.0f, 0, wc.c_str(), 1);
		}

		font->End(context);
	

}
