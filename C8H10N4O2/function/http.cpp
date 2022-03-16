#include"SceneManager.h"
#include"http.h"
pplx::task<void> Http::post(char* anser, std::string* key, std::string* value, int count, int funcnum)
{

	{
		return pplx::create_task([=]
			{
				// クライアントの設定
				http_client client(url[funcnum]);
				// 送信データの作成
				json::value postdata;
				for (int i = 0; i < count; i++)
				{
					std::wstring key_wc;
					key_wc.resize(key[i].size());
					mbstowcs((wchar_t*)key_wc.c_str(), key[i].c_str(), key[i].size());
					std::wstring value_wc;
					value_wc.resize(value[i].size());
					mbstowcs((wchar_t*)value_wc.c_str(), value[i].c_str(), value[i].size());
					postdata[key_wc] = json::value::string(value_wc);
				}
				// リクエスト送信
				return client.request(methods::POST, L"", postdata.serialize(), L"application/json");

			})
			.then([=](http_response  response)
				{
					// ステータスコード判定
					if (response.status_code() == status_codes::OK)
					{
						char result[HTTP_MEMORY] = "";
						auto body = response.extract_string();
						wcstombs(result, body.get().c_str(), sizeof(result));

						memcpy(anser, result, sizeof(result));

					}
					else
					{
						memcpy(anser, "", sizeof(char));
					}

				});
	}

}
std::string Http::findbody_mysql(char*info)
{
	char* ptr = ":\"";
	char *c=strstr(info, ptr);
	char* c2 = strstr(c, "\"");
	std::string res=dividejson(c2);
	
	
	return res;
}
void Http::findbody_go()
{
	stop_flg = false;
	while (1)
	{
		if (stop_flg)break;
		if (SceneManager::getInstance()->actionflug[search])
		{
			
		

			char word[] = "\\\"";
			int count = 0;
			char result[2048] = "";
			std::string key[1] = { "" };
			std::string value[1] = { "" };
			key[0] = "";
			value[0] = "";
			post(result, key, value, 1, search).wait();
			SceneManager::getInstance()->Table_initialize();
			std::string res_id = "";
			std::string res_status = "";
			char *c;
			c = strstr(result, "body:");
			if (c == NULL)break;
			while (1)
			{


				c = strstr(c, "tableID:");
				if (c != NULL)
				{
					c = strstr(c, "N:");
					c = strstr(c, word);
					if (c != nullptr)
					{
						SceneManager::getInstance()->tables[count].id = dividejson(c);
						count++;
					}
				}
				else break;
			}
			count = 0;
			c = strstr(result, "body:");
			while (1)
			{
				c = strstr(c, "status:");
				if (c != NULL)
				{
					c = strstr(c, "N:");
					c = strstr(c, word);
					if (c != nullptr)
					{
						std::string str = dividejson(c);
						if (str == "1")
						{
							SceneManager::getInstance()->tables[count].status = str;
							count++;

						}
						else
						{
							SceneManager::getInstance()->tables[count].id = "";
							SceneManager::getInstance()->tables[count].status = "";
						}
						

					}
				}
				else break;
				
				
				
			}
			
			SceneManager::getInstance()->actionflug[search] = false;
		}
	}
}
void Http::wakethread_action()
{
	stop_flg = false;
	while (1)
	{
		if (SceneManager::getInstance()->actionflug[tip_updata])
		{
			char result[HTTP_MEMORY] = "";
			std::string key[2] = { "" };
			std::string value[2] = { "" };
			key[0] = "userID";
			value[0] = SceneManager::getInstance()->userID;
			key[1] = "tipCount";
			value[1] = SceneManager::getInstance()->result_tip;
			post(result, key, value, 2, tip_updata).wait();

			char* check = strstr(result, "errorMessage");
			if (check == NULL)
			{
				SceneManager::getInstance()->actionflug[tip_updata] = false;
				
			}
		}
		if (SceneManager::getInstance()->actionflug[signup])
		{
			char result[HTTP_MEMORY] = "";
			std::string key[1] = { "" };
			std::string value[1] = { "" };
			key[0] = "";
			value[0] = "";
			post(result, key, value, 1, signup).wait();
		
			char* check = strstr(result, "errorMessage");
			if (check == NULL)
			{
				
				SceneManager::getInstance()->userID =result;
				SceneManager::getInstance()->select_userID = result;
				SceneManager::getInstance()->actionflug[signup] = false;
				SceneManager::getInstance()->actionflug[tip_select] = true;
			}
		}
		if (SceneManager::getInstance()->actionflug[tip_select])
		{
			char result[HTTP_MEMORY] = "";
			std::string key[1] = { "" };
			std::string value[1] = { "" };
			key[0] = "userID";
			value[0] = SceneManager::getInstance()->select_userID;
			post(result, key, value, 1, tip_select).wait();
			FILE* f;
			f = fopen("user.txt", "w");
			if (strstr(result, "errorMessage") == NULL)
			{
			std::string anser=findbody_mysql(result);
			SceneManager::getInstance()->tipCount = anser;
				SceneManager::getInstance()->actionflug[tip_select] = false;
				SceneManager::getInstance()->userID = SceneManager::getInstance()->select_userID;
				fputs(SceneManager::getInstance()->userID.c_str(), f);

			}
			fclose(f);

		}
		if (SceneManager::getInstance()->actionflug[hit])
		{
			char result[HTTP_MEMORY] = "";
			std::string key[2] = { "" };
			std::string value[2] = { "" };
			key[0] = "tableID";
			value[0] = SceneManager::getInstance()->tableID;
			key[1] = "playerID";
			value[1] = SceneManager::getInstance()->playerID;
			post(result, key, value, 2, hit).wait();

			SceneManager::getInstance()->actionflug[hit] = false;
		}
		if (SceneManager::getInstance()->actionflug[stand])
		{
			char result[HTTP_MEMORY] = "";
			std::string key[1] = { "" };
			std::string value[1] = { "" };
			key[0] = "tableID";
			value[0] = SceneManager::getInstance()->tableID;
			post(result, key, value, 1, stand).wait();
			SceneManager::getInstance()->actionflug[stand] = false;
		}
		if (stop_flg)break;
	}

}
void Http::wakethread_wait()
{
	stop_flg = false;

	if (SceneManager::getInstance()->actionflug[gather])
	{
		char result[HTTP_MEMORY] = "";
		std::string key[ONEDATA] = { "" };
		std::string value[ONEDATA] = { "" };
		key[0] = "";
		value[0] = SceneManager::getInstance()->tableID;
		post(result, key, value, ONEDATA, gather).wait();
		std::string str = findbody(result);
		SceneManager::getInstance()->tableID = str;
		SceneManager::getInstance()->actionflug[gather] = false;
	}

	while (1)
	{
		
		char anser[HTTP_MEMORY] = "";
		std::string key[ONEDATA] = { "" };
		std::string value[ONEDATA] = { "" };
		key[0] = "tableID";
		value[0] = SceneManager::getInstance()->tableID;
		char* divideresult[10] = { nullptr };
		post(anser, key, value, ONEDATA, wait).wait();
		std::string str = findbody(anser);
		bool flug = false;
		char info[HTTP_MEMORY] = "";
		memcpy(info, str.c_str(), str.size());
		SceneManager::getInstance()->divideinfo(info, divideresult, "_");

		for (int i = 0; i < 3; i++)
		{
			if (divideresult[i] == nullptr)
				flug = true;
		}
		if (!flug)
		{
			SceneManager::getInstance()->tableStatus = divideresult[0];
			SceneManager::getInstance()->tableMember = divideresult[1];
			SceneManager::getInstance()->tableMaster = divideresult[2];
		}

		if (SceneManager::getInstance()->actionflug[gamestart])
		{
			char result[HTTP_MEMORY] = "";
			std::string key[ONEDATA * 2] = { "" };
			std::string value[ONEDATA * 2] = { "" };
			key[0] = "tableID";
			value[0] = SceneManager::getInstance()->tableID;
			key[1] = "playerID";
			value[1] = SceneManager::getInstance()->playerID;
			post(result, key, value, ONEDATA * 2, deadline).wait();
			post(result, key, value, ONEDATA * 2, gamestart).wait();
			SceneManager::getInstance()->actionflug[gamestart] = false;
		}

		if (stop_flg)break;
	}
}
void Http::wakethread_game()
{
	stop_flg = false;
	while (1)
	{
		if (stop_flg)break;

		char anser[HTTP_MEMORY] = "";
		char* divideresult[12] = { nullptr };

		std::string key[ONEDATA] = { "" };
		std::string value[ONEDATA] = { "" };
		key[0] = "tableID";
		value[0] = SceneManager::getInstance()->tableID;
		post(anser, key, value, ONEDATA, gamecheck).wait();
		char info[HTTP_MEMORY] = "";
		std::string str = findbody(anser);
		memcpy(info, str.c_str(), str.size());
		if (anser != NULL)
		{
			SceneManager::getInstance()->divideinfo(info, divideresult, "$");
			bool flug = false;
			for (int i = 0; i < 2; i++)
			{
				if (divideresult[i] == nullptr)
					flug = true;
			}
			if (!flug)
			{
				SceneManager::getInstance()->tableStatus = divideresult[0];
				SceneManager::getInstance()->gameMember = divideresult[1];
				SceneManager::getInstance()->tableMaster = divideresult[2];
				for (int i = 0; i < 4; i++)
				{
					if (divideresult[i + 3] != nullptr)
					{
						SceneManager::getInstance()->put_data(divideresult[i + 3], i);
					}
				}
			}
		}



	}


}
std::string Http::findbody(char* info)
{
	std::string res = "";

	char* c = strstr(info, "\\\"");
	if (c != nullptr)
	{
		res = dividejson(c);
	}
	else
	{
		res = dividejson(info);
	}
	return res;
}
std::string Http::dividejson(char* info)
{
	std::string res = "";
	int count = 0;
	int j_count = 0;
	while (1)
	{
		if (info[count] != '\"' && info[count] != '\\' && info[count] != '\0'&& info[count] != '}')
		{
			res += info[count];
		}
		else
		{
			j_count++;
		}
		if (j_count > 2)break;
		count++;
	}
	return res;
}