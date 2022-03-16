#include"Player.h"
#include"SceneManager.h"
void Player::On_Active(int playerID)
{
	active = true;
	score = 0;
	id = playerID;
	cardcount = 0;

}
void Player::SetScore(char* data,int num)
{
	if (active)
	{
		int cardscore = 0;
		cardscore = strtol(data, NULL, 10);
		
		if (cardscore != -1)
		{
			score = 0;
			cardscore += 1;
			cardscore %= 13;
			if (cardscore > 9||cardscore==0)
			{
				cardscore = 10;
			}

			cards[num] = cardscore;
			for (int i = 0; i < 10; i++)
			{
				score += cards[i];
			}
		}



		if (score + 10 <= 21)
		{
			score = 0;
			bool changescore = false;
			for (int i = 0; i < 10; i++)
			{
				
				if (cards[i] == 1 && !changescore)
				{
					score += 10;
					changescore = true;
				}
				score += cards[i];
			}

		}
		if (score > 21)
		{
			score = 0;
			bool changescore = false;
			for (int i = 0; i < 10; i++)
			{

				if (cards[i] == 11 && !changescore)
				{
					cards[i] = 1;
					changescore = true;
				}
				score += cards[i];
			}
		}

	}

}
int Player::GetID()
{
	return id;
}
int Player::GetScore()
{
	return score;
}
void Player::Initialize()
{
	 active = false;
	 score = 0;
	 for (int i = 0; i < 10; i++)
	 {
		 cards[i] = 0;
	 }
	 id = -1;
}
void PlayerManager::Initialize()
{
	for (int i = 0; i < PL_Count; i++)
	{
		players[i].Initialize();
	}
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (IDs[i] != std::stoi(SceneManager::getInstance()->playerID) && others[0] != IDs[i])
			{
				others[j] = IDs[i];
				break;
			}
		}
	}
}