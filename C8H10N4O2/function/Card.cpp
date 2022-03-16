#include"Card.h"
#include"CardManager.h"
#include"SceneManager.h"
#include"Player.h"
#define CARD_SIZE_X 220.0f
#define CARD_SIZE_Y 328.0f
const DirectX::XMFLOAT2 vartical_up_max = { 500.0f,100.0f };
const DirectX::XMFLOAT2 vartical_down_max = { 500.0f,500.0f };
const DirectX::XMFLOAT2 beside_right_max = { 1000.0f,100.0f };
const DirectX::XMFLOAT2 beside_left_max = { 100.0f,100.0f };

const float card_gap = 80.0f;
void Card::Initialize()
{
	cardtype = 0;
	pos =initialize_pos ;
	w_size = {initialize_size.x/3.5f,initialize_size.y/3.5f};
	t_pos={ 0,0 };
	exist = false;
	visible = false;
	angle=0;
	playerID = -1;
	timer = 0;
	state = -1;
	cardindex = -1;
	cardspeed = 800;

}
void Card::Render(std::shared_ptr<Sprite> card_texture, ID3D11DeviceContext*context)
{
	DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
		if (!visible)
		{
			color = { 0.0f,0.0f,0.0f,1.0f };

		}

		if (exist)
		{
			card_texture->render(context, NULL,pos, w_size, t_pos,FLOAT2(CARD_SIZE_X,CARD_SIZE_Y),angle,color);
		}
}
int  Card::GetState()
{
	return state;
}
void Card::AddState()
{
	state++;
}
void Card::HitSet(char* num, int index)
{
	playerID = index;

	cardtype = strtol(num, NULL, 10);
	exist = true;
	t_pos = { initialize_size.x * (cardtype % 13) + (cardtype % 13 * 15),initialize_size.y * (cardtype / 13) };

}
void Card::Update(float timer)
{
	float s = cardspeed * timer;
	if (playerID == 0)
	{
		switch (state)
		{

		case 0:
			pos.y -= cardspeed * timer;
			if (pos.y < vartical_up_max.y)
			{
				pos.y = vartical_up_max.y;
				if (pos.x < vartical_up_max.x + card_gap * cardindex)cardspeed *= -1;
				state++;
			}
			break;
		case 1:
			pos.x -= cardspeed * timer;
			if (cardspeed > 0)
			{
				if (pos.x < vartical_up_max.x + card_gap * cardindex)
				{
					pos.x = vartical_up_max.x + card_gap * cardindex;
					state++;
					if (cardindex == 0)
						visible = true;
				}
			}
			else
			{
				if (pos.x > vartical_up_max.x + card_gap * cardindex)
				{
					pos.x = vartical_up_max.x + card_gap * cardindex;
					state++;
					if (cardindex == 0)
						visible = true;
				}
			}
			break;

		}
	}
	else
	{
		if (std::stoi(SceneManager::getInstance()->playerID) == playerID)
		{
			switch (state)
			{
				visible = true;
			case 0:

				pos.y += cardspeed * timer;
				if (pos.y > vartical_down_max.y)
				{
					if (pos.x < vartical_down_max.x + card_gap * cardindex)cardspeed *= -1;
					
					pos.y = vartical_down_max.y;
					state++;
				}
				break;
			case 1:
				pos.x -= cardspeed * timer;
				if (cardspeed > 0)
				{
					if (pos.x < vartical_down_max.x + card_gap * cardindex)
					{
						pos.x = vartical_down_max.x + card_gap * cardindex;
						visible = true;
						state++;
					}
				}
				else
				{
					if (pos.x > vartical_down_max.x + card_gap * cardindex)
					{
						pos.x = vartical_down_max.x + card_gap * cardindex;
						visible = true;
						state++;
					}
				}
				break;

			}
		}
		else
		{
			

			

			if (playerID ==P_MANAMGER.others[0])
			{
				switch (state)
				{

				case 0:
					pos.x -= cardspeed * timer;
					angle = 90;

					if (pos.x < beside_left_max.x)
					{
						if (pos.y < beside_left_max.y + 80 * cardindex)cardspeed *= -1;
						
						pos.x = beside_left_max.x;
						state++;

					}

					break;
				case 1:
					pos.y -= cardspeed * timer;
					if (cardspeed > 0)
					{
						if (pos.y < beside_left_max.y + 80 * cardindex)
						{
							pos.y = beside_left_max.y + 80.0f * cardindex;
							state++;
						}
					}
					else
					{
						if (pos.y > beside_left_max.y + 80 * cardindex)
						{
							pos.y = beside_left_max.y + 80.0f * cardindex;
							state++;
						}
					}
					break;

				}
			}
			if (playerID == P_MANAMGER.others[1])
			{
				switch (state)
				{

				case 0:
					pos.x += cardspeed * timer;
					angle = 90;

					if (pos.x > beside_right_max.x)
					{
						if (pos.y < beside_right_max.y + card_gap * cardindex)cardspeed *= -1;
						
						pos.x = beside_right_max.x;
						state++;
					}

					break;
				case 1:
					pos.y -= cardspeed * timer;
					if (cardspeed > 0)
					{
						if (pos.y < beside_right_max.y + card_gap * cardindex)
						{
							pos.y = beside_right_max.y + card_gap * cardindex;
							state++;
						}
					}
					else
					{
						if (pos.y > beside_right_max.y + card_gap * cardindex)
						{
							pos.y = beside_right_max.y + card_gap * cardindex;
							state++;
						}
					}
					break;

				}
			}
		}
	}
	
	
	
}