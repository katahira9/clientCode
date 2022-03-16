#pragma once

#include"SceneManager.h"
#include"Font.h"
#include"framework.h"
#include"http.h"
#include"KeyInput.h"
#define ONEDATA 1

class Gather :public SceneBase
{
private:
	Http http;
	std::unique_ptr<Font> font;
	std::string key[ONEDATA];
	std::string value[ONEDATA];
	std::thread th;
	std::string playerID="" ;
	std::string tableID ="";
	Key start{VK_RETURN};
	Key back{ VK_BACK };
	bool endflug = false;
public:
	Gather(ID3D11Device* d) :SceneBase(d)
	{
		
	};
	~Gather();
	
	void initialize(ID3D11Device*);
	void update(float);
	void render(ID3D11DeviceContext*, float);
	void uninitialize();
	

};