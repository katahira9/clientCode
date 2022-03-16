#pragma once
#include"Sprite.h"
#include <memory>
#include<DirectXMath.h>
class Card
{
public:
	void Update(float timer);
	void Initialize();
	//void SetData(char*type,int index);
	void HitSet(char* type, int index);
	void AddState();
	void Render(std::shared_ptr<Sprite> card_texture, ID3D11DeviceContext*);
	int  GetState();
	void SetCardIndex(int index)
	{
		cardindex = index;
	}
	void SetVisible(bool flug)
	{
		visible = flug;
	}
	int GetPlayerID()
	{
		return playerID;
	}
	bool GetExist()
	{
		return exist;
	}
	int GetCardIndex()
	{
		return cardindex;
	}
private:
	const FLOAT2 initialize_pos = { 600,300 };
	const FLOAT2 initialize_size = { 220,328 };
	int cardtype;
	FLOAT2 pos;
	FLOAT2 w_size;
	FLOAT2 t_pos;
	float cardspeed ;
	float angle;
	int state = -1;
	float timer = 0;
	int playerID;
	bool exist;
	bool visible;
	int cardindex;
};

