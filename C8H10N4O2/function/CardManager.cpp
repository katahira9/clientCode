#include"CardManager.h"
#include"framework.h"
#include"Player.h"
#include"SceneManager.h"
#define CARD_SIZE_X 220
#define CARD_SIZE_Y 328

void CardManager::Update(float timer)
{
	for (int i = 0; i < CARD_MAX; i++)
	{
		if (cardsData[i].GetState() == -1)
		{
			bool sameflug = false;

			for (int j = 0; j < CARD_MAX; j++)
			{
				if (cardsData[i].GetState() == cardsData[j].GetState())
				{
					sameflug = true;
				}
				else
				{
					if (cardsData[j].GetState() < 1)
					{
						sameflug = false;
						break;
					}
				}
			}
			if (sameflug)
			{
				cardsData[i].AddState();
			}
		}
		
			cardsData[i].Update(timer);
		
	}

}

void CardManager::Render(ID3D11DeviceContext*context)
{
	for (int i = CARD_MAX-1; i >-1; i--)
	{
		cardsData[i].Render(card,context);
		
	}
}
void CardManager::Initialize(ID3D11Device*d)
{
	wchar_t* w_card = L"./data/card.png";
	card = std::make_shared<Sprite>(d, w_card);
	for (int i = 0; i < CARD_MAX; i++)
	{
		cardsData[i].Initialize();
	}
}
void CardManager::HitSetDatas(char** info,int index)
{
	for (int i = P_MANAMGER.players[index].cardcount; i < CARD_MAX; i++)
	{
		setcount++;
		if (info[i] == NULL)
		{
			P_MANAMGER.players[index].cardcount = i;
			
			break;
		}
		if (strtol(info[i], NULL, 10) != -1)
		{
			for (int j = 0; j < CARD_MAX; j++)
			{
				if (!cardsData[j].GetExist())
				{
					cardsData[j].HitSet(info[i], index);
					int samecount = 0;
					for (int k = 0; k < CARD_MAX; k++)
					{

						if (cardsData[j].GetPlayerID() == cardsData[k].GetPlayerID() && j != k)
						{
							samecount++;
						}
					}
					cardsData[j].SetCardIndex(samecount);
					P_MANAMGER.players[index].SetScore(info[i], cardsData[j].GetCardIndex());
					break;
				}
			}
		}

	}
}
bool CardManager::AllMoved()
{
	if (!cardsData[0].GetExist())
	{
		return false;
	}
	for (int i = 0; i < CARD_MAX; i++)
	{
		if (cardsData[i].GetState() != 2 && cardsData[i].GetExist())
		{
			return false;
		}
		if (!cardsData[i].GetExist())break;
	}
	
	return true;
}
void CardManager::AllVisible()
{
	for (int i = 0; i < CARD_MAX; i++)
	{
		if (cardsData[i].GetExist())
		{
			cardsData[i].SetVisible(true);
		}
		else break;
	}
}