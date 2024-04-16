#pragma once

#include"ResponseGenerator.hpp"
#include"MyApiKey.hpp"

#include <tgbot/tgbot.h>

class TalkingBot
{
public:
	TalkingBot(const char* key) : bot(key)
	{
		InitCommans();
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

	GadalkaData data;

	TgBot::Bot bot;

private:

	void InitCommans()
	{
		bot.getEvents().onCommand("start", [this](TgBot::Message::Ptr message)
			{
				bot.getApi().sendMessage(message->chat->id, to_utf8(L"Здравствуй, отрак! Я - Великая гадалка! Готова ответить на все твои вопросы! Поздоровайтесь мо мной!"));
			}
		);
		/*bot.getEvents().onCommand("info", [this](TgBot::Message::Ptr message)
			{
				std::string s =

					message->chat->firstName + " " +
					message->chat->lastName + " " +
					message->chat->username;

				bot.getApi().sendMessage(message->chat->id, s);
			}
		);*/
		bot.getEvents().onCommand("update", [this](TgBot::Message::Ptr message)
			{
				data.Update();
				bot.getApi().sendMessage(message->chat->id, to_utf8(L"Я обновила словарный запас! Готова к дальнейшей работе!"));
			}
		);
		bot.getEvents().onCommand("help", [this](TgBot::Message::Ptr message)
			{
				bot.getApi().sendMessage(message->chat->id, to_utf8(L"Не говори со мной на бесовском языке! Лучше спроси, что я могу, и я дам ответ!"));
			}
		);
		bot.getEvents().onCommand("boroda", [this](TgBot::Message::Ptr message)
			{
				data.AgressiveMode() = !data.AgressiveMode();
				if (data.AgressiveMode())
					bot.getApi().sendMessage(message->chat->id, to_utf8(L"Поняла вас, становлюсь злой!"));
				else
					bot.getApi().sendMessage(message->chat->id, to_utf8(L"Поняла вас, становлюсь доброй!"));
			}
		);
	}

	void InitMessageResponse()
	{
		bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message)
		{
			// std::cout << "User wrote: " << message->text << " User ID: " << message->chat->id << '\n';
			if (message->text[0] != '/')
			{
				std::string result = data.GenerateAnswer(message->text);
				// cout << "Bot wrote: " << result << "\n\n";
				bot.getApi().sendMessage(message->chat->id, result);
			}
		});
	}

};
