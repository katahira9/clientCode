#pragma once
#define PL_Count 4
class Player
{
public:
	void On_Active(int playerID);
	void Off_Active()
	{
		active = false;
	}

	void SetScore(char* data,int num);
	int GetScore();
	int GetID();
	void Initialize();
	int cardcount = 0;

private:
	bool active=false;
	int score=0;
	int cards[10];
	int id = -1;

};
class PlayerManager
{
public:
	Player players[PL_Count];
	int IDs[3] = { 1, 2, 3 };
	int others[2] = { 0,0 };
	static  PlayerManager& getInstance()
	{
		static PlayerManager instance;
		return	instance;
	}
	void Initialize();
	
};

#define	P_MANAMGER	PlayerManager::getInstance()



