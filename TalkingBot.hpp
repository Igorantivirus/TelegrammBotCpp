#pragma once

#include"ResponseGenerator.hpp"
#include"UserInfoWorker.hpp"
#include"MyApiKey.hpp"

#include <tgbot/tgbot.h>

class TalkingBot
{
public:
	TalkingBot(const char* key) : bot(key), comm(TG_API_KEY, data, bot)
	{
		InitMessageResponse();
	}

	void Run()
	{
		try
		{
			std::cout << "Bot username: " << bot.getApi().getMe()->username.c_str() << '\n';
			TgBot::TgLongPoll longPoll(bot);
			while (true)
			{
				std::cout << "Long poll started" << '\n';
				longPoll.start();
			}
		}
		catch (const TgBot::TgException& e)
		{
			std::cout << "Bot error: " << e.what() << '\n';
		}
		catch (...)
		{
			std::cout << "Unknown Error" << '\n';
		}
	}

private:

	TgBot::Bot bot;

	GadalkaData data;

	UserWorker worker;
	CommandResponser comm;

private:

	void InitMessageResponse()
	{
		bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message)
		{
			std::string resut;
			// std::cout << "User wrote: " << message->text << " User ID: " << message->chat->id << '\n';
			try
			{
				if (message->text[0] != '/')
				{
					resut = data.GenerateAnswer(message->text);
					bot.getApi().sendMessage(message->chat->id, resut);

					// cout << "Bot wrote: " << result << "\n\n";
				}
				else
				{
					bot.getApi().sendMessage(message->chat->id, comm.GetResponse(message->text, message->chat));
				}
			}
			catch (...)
			{
				bot.getApi().sendMessage(message->chat->id, mySTRutils::to_utf8(L"Ошибка!"));
				std::cout << "Error: " << resut << '\n';
			}
		});
	}

};