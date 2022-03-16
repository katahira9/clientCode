#pragma once

#include"SceneManager.h"
#include"Font.h"
#include"framework.h"
#include"http.h"
#include"KeyInput.h"
#define login 3

class Select :public SceneBase
{
private:
	Http http;
	char selectID[30]="";
	std::unique_ptr<Font> font;
	bool flug = false;
	std::string playerID;
	std::string tableID;
	std::thread th[2];
	bool select_on = false;
	bool window_on = false;
	bool end_flug = false;
	bool miss;
	Key enter{ VK_RETURN };
	Key back{ VK_BACK };
	Key space{ VK_SPACE };
	Key reload{ VK_F5 };
	char text[30] = "";

	enum Infonum
	{
		TABLEID,
		TABLESTATUS,
		PLAYERID
		
	};
public:
	Select(ID3D11Device* d) :SceneBase(d) {}
	~Select();
	
	void initialize(ID3D11Device*);
	void update(float);
	void render(ID3D11DeviceContext*, float);
	void uninitialize();

};
