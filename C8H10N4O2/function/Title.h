#pragma once

#include"SceneManager.h"
#include"framework.h"
#include"Font.h"
#include"KeyInput.h"
#include"http.h"
class Title :public SceneBase
{
private:
	Http http;
	bool flug = false;
	float rightfontsize = 1;
	float leftfontsize = 1;
	std::unique_ptr<Font> font;
	Key enter = { VK_RETURN };
	Key space = { VK_SPACE };
	int select_id;
	std::thread th;
	std::string myID;
	bool window_flug = false;
public:
	Title(ID3D11Device* d) :SceneBase(d) {}
	void initialize(ID3D11Device*);
	void update(float);
	void render(ID3D11DeviceContext*, float);
	void uninitialize();

	~Title() {
		http.Stop();
		
			if (th.joinable())
				th.join();
		
	}

};
