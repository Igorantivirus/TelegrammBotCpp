#pragma once

#include <iostream>

#include <tgbot/tgbot.h>

#include "Responser.hpp"

class Bot
{
public:
    Bot(const char *key) : bot_(key)
    {
        bot_.getEvents().onNonCommandMessage(   std::bind(&Bot::onNonCommandMessage,    this, std::placeholders::_1));
        bot_.getEvents().onInlineQuery(         std::bind(&Bot::onInlineQuery,          this, std::placeholders::_1));

        bot_.getEvents().onCommand("start",     std::bind(&Bot::start,                  this, std::placeholders::_1));
    }

    void run()
    {
        TgBot::TgLongPoll longPoll(bot_);
        while (true)
        {
            try
            {
                std::cout << "Long poll started" << '\n';
                longPoll.start();
            }
            catch (const TgBot::TgException& e)
            {
                std::cout << "Bot error: " << e.what() << '\n';
            }
            catch(const std::exception& e)
            {
                std::cout << "Error: " << e.what() << '\n';
            }
            catch (...)
            {
                std::cout << "Unknown Error" << '\n';
            }
        }
    }

private:

    TgBot::Bot bot_;

    Responser responser_;

private:

    void onNonCommandMessage(TgBot::Message::Ptr message)
    {

    }

    void onInlineQuery(const TgBot::InlineQuery::Ptr& query)
    {
        auto res = responser_.getResponsesOnInlineMessages(query);
        if (res.first.empty())
            return;

        std::vector<TgBot::InlineQueryResult::Ptr> results;
        TgBot::InlineQueryResultArticle::Ptr article = std::make_shared<TgBot::InlineQueryResultArticle>();

        article->title = res.second ? "Result." : "Error";
        article->id = "1";

        TgBot::InputTextMessageContent::Ptr messageContent = std::make_shared<TgBot::InputTextMessageContent>();
        messageContent->messageText = res.first;
        article->inputMessageContent = messageContent;

        results.push_back(article);
        bot_.getApi().answerInlineQuery(query->id, results);
    }

    void start(TgBot::Message::Ptr message) const
    {

    }

    

    void initResponses()
    {
        // bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message)
        //     {
        //         std::string res;
        //         try
        //         {
        //             res = processor.Processing(message->text, message);
        //             bot.getApi().sendMessage(message->chat->id, res);
        //         }
        //         catch (...)
        //         {
        //             std::cout << "Error: " << "Trying send message \"" << res << "\"\n";
        //             bot.getApi().sendMessage(message->chat->id, to_utf8(L"Ошибка!"));
        //         }
        //     });
        // bot.getEvents().onInlineQuery([this](const TgBot::InlineQuery::Ptr& query)
        //     {
        //         std::string res = processor.ProcessingLineMessage(query);
        //         if (res.empty())
        //             return;

        //         std::vector<TgBot::InlineQueryResult::Ptr> results;
        //         TgBot::InlineQueryResultArticle::Ptr article = std::make_shared<TgBot::InlineQueryResultArticle>();

        //         article->title = "Math result.";
        //         article->id = "1";

        //         TgBot::InputTextMessageContent::Ptr messageContent = std::make_shared<TgBot::InputTextMessageContent>();
        //         messageContent->messageText = res;
        //         article->inputMessageContent = messageContent;

        //         results.push_back(article);
        //         bot.getApi().answerInlineQuery(query->id, results);

        //     });
    }
};
