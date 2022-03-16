#include"SceneManager.h"
#include"Gather.h"
void Gather::initialize(ID3D11Device*d)
{
	endflug = false;
http.Initialize();
	if (th.joinable())th.join();
SceneManager::getInstance()->tableStatus ="1";
SceneManager::getInstance()->playerID = "1";
SceneManager::getInstance()->tableMaster = "1";
playerID = SceneManager::getInstance()->playerID;
SceneManager::getInstance()->actionflug[http.gather] =true;
th=std::thread(&Http::wakethread_wait, &http);
font = std::make_unique<Font>(d, "data/MS Gothic.fnt", 1024);
}
void Gather::uninitialize()
{
	http.Stop();

	if (th.joinable())th.join();
}
Gather::~Gather()
{
	http.Stop();

	if (th.joinable())th.join();
	
}
void Gather::update(float timer)
{
	char result[ HTTP_MEMORY] = "";
	tableID = SceneManager::getInstance()->tableID;
	if(start.down())
	{
		if (!endflug)
		{
			endflug = true;
			SceneManager::getInstance()->actionflug[http.gamestart] = true;
		}
	}
	
	if (std::stoi(SceneManager::getInstance()->tableStatus) == 2)
	{
		http.Stop();
		SceneManager::getInstance()->changeScene("Game");
		
		
	}
	if (back.down()) 
	{
		http.Stop();
		endflug = true;

		if (th.joinable())th.join();
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


}
void Gather::render(ID3D11DeviceContext*context, float)
{
	
	font->Begin(context);
	font->Draw(480, 200, L"テーブルID", 2);
	font->Draw(380, 500, L"Enterで締め切る", 2);
	font->Draw(380, 600, L"BackSpaceで戻る", 2);
	if (endflug||SceneManager::getInstance()->tableID=="")
	{
		font->Draw(0, 40, L"Connecting", 1);
	}
	std::wstring wc=L"";
	mbstowcs((wchar_t*)wc.c_str(), SceneManager::getInstance()->tableID.c_str(), SceneManager::getInstance()->tableID.size()
		+ 1);
	font->Draw(600, 280, (wchar_t*)wc.c_str(), 4);
	font->Draw(0, 0, L"人数:", 1);
	wc = L"";
	mbstowcs((wchar_t*)wc.c_str(), SceneManager::getInstance()->tableMember.c_str(), SceneManager::getInstance()->tableMember.length()
		+ 1);
	font->Draw(90, 0, (wchar_t*)wc.c_str(), 1);

	font->End(context);

}