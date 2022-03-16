#include"Title.h"
void Title::initialize(ID3D11Device*d)
{
	window_flug = false;
	font = std::make_unique<Font>(d, "data/MS Gothic.fnt", 1024);
	SceneManager::getInstance()->Initialize();
	FILE* f;
	th=std::thread(&Http::wakethread_action, &http);
	if ((f=fopen("user.txt", "r")) == NULL)
	{
		SceneManager::getInstance()->actionflug[http.signup] = true;
	}
	else
	{
		char c = fgetc(f);
		SceneManager::getInstance()->select_userID = c;
		SceneManager::getInstance()->actionflug[http.tip_select] = true;
		fclose(f);

	}

}
void Title::uninitialize()
{
	http.Stop();
	if (th.joinable())
	{
		th.join();
	}
}
void Title::update(float timer)
{
	if (space.down())
	{
		window_flug = true;
		

	}
	


	if (GetKeyState(VK_LEFT) < 0)
	{
		flug = false;
	}
	else if(GetKeyState(VK_RIGHT) < 0)
	{
		flug = true;
	}
	if (window_flug)
	{
		ImGui::SetNextWindowSize(ImVec2(320, 100), ImGuiSetCond_Once);
		ImGui::Begin("Sign in");
		ImGui::InputInt("ID", &select_id, 1);
		if (enter.down())
		{
			
			SceneManager::getInstance()->select_userID= std::to_string(select_id);
			SceneManager::getInstance()->actionflug[Http::Funcnum::tip_select] = true;
			window_flug = false;

		}
		ImGui::Text(u8"Enterで実行&閉じる");
		ImGui::End();
	}
	else
	{
		
			myID = SceneManager::getInstance()->userID;
		
		if (flug)
		{
			rightfontsize = 1.3f;
			leftfontsize = 1;
			if (enter.down()&&!SceneManager::getInstance()->actionflug[Http::Funcnum::tip_select])
			{
				SceneManager::getInstance()->changeScene("Select");
			}

		}
		else
		{
			if (enter.down()&&!SceneManager::getInstance()->actionflug[Http::Funcnum::tip_select])
			{
				SceneManager::getInstance()->changeScene("Gather");
			}
			rightfontsize = 1;
			leftfontsize = 1.3f;


		}
	}
	
}

void Title::render(ID3D11DeviceContext*context, float)
{
	font->Begin(context);
	font->Draw(470, 300, L"Blackjack", 2);
	font->Draw(500, 360, L"Select Arrowkey", 1);
	font->Draw(550, 390, L"Push Enter", 1);
	font->Draw(450, 420, L"SPACEでアカウント変更", 1);
	font->Draw(200, 500, L"募集する", leftfontsize);
	font->Draw(900, 500, L"参加する", rightfontsize);
	
	std::wstring wc_user;
	std::string user_str = "userID:" + myID;
	mbstowcs((wchar_t*)wc_user.c_str(),user_str.c_str(), user_str.size());
	font->Draw(1000, 0,wc_user.c_str(), 1.3);
	std::wstring wc_tip;
	std::string tip_str = "tips:" + SceneManager::getInstance()->tipCount;
	mbstowcs((wchar_t*)wc_tip.c_str(), tip_str.c_str(), tip_str.size());
	font->Draw(1000, 50, wc_tip.c_str(), 1.3);

	font->End(context);


}
