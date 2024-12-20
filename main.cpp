﻿#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include"MyApiKey.hpp"

#include"BotResponser.hpp"

void COnsoleGadalka()
{
    Responser resp;

    std::string s;

    bool agr = false;

    while (true)
    {
        std::getline(std::cin, s);
        std::cout << resp.GetAnswer(s, agr) << std::endl;
    }
}


class GadalkaBot
{
public:
    GadalkaBot(const char* key) : bot(key), processor(key)
    {
        InitResponses();
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

    InputProcessor processor;

    void InitResponses()
    {
        bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message)
            {
                std::string res;
                try
                {
                    res = processor.Processing(message->text, message);
                    bot.getApi().sendMessage(message->chat->id, res);
                }
                catch (...)
                {
                    std::cout << "Error: " << "Trying send message \"" << res << "\"\n";
                    bot.getApi().sendMessage(message->chat->id, to_utf8(L"Ошибка!"));
                }
            });
        bot.getEvents().onInlineQuery([this](const TgBot::InlineQuery::Ptr& query)
            {
                std::string res = processor.ProcessingLineMessage(query);
                if (res.empty())
                    return;

                std::vector<TgBot::InlineQueryResult::Ptr> results;
                TgBot::InlineQueryResultArticle::Ptr article = std::make_shared<TgBot::InlineQueryResultArticle>();

                article->title = "Math result.";
                article->id = "1";

                TgBot::InputTextMessageContent::Ptr messageContent = std::make_shared<TgBot::InputTextMessageContent>();
                messageContent->messageText = res;
                article->inputMessageContent = messageContent;

                results.push_back(article);
                bot.getApi().answerInlineQuery(query->id, results);


            });
    }

};

const std::string TgBot::InlineQueryResultArticle::TYPE = "article";
const std::string TgBot::InputTextMessageContent::TYPE = "text";

int main()
{
#if defined(_WIN32) || defined(_WIN64)
    system("chcp 65001 > nul");
#endif

    GadalkaBot bot(TG_API_KEY);
    bot.Run();


    return 0;
}
