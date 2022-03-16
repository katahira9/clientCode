#pragma once


#define CARD_MAX 30

#include"Card.h"
#include"sprite.h"

class CardManager
{
public:
	void Render(ID3D11DeviceContext*);
	void Update(float timer);
	void HitSetDatas(char**info,int index);
	void Initialize(ID3D11Device*d);
	bool AllMoved();
	void AllVisible();
	int setcount = 0;
	int gamestate = 0;
private:
	int dealer_draw_count = 0;
	
	std::shared_ptr<Sprite> card;
	Card cardsData[CARD_MAX];
};


