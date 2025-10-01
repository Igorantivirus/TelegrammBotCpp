#pragma once

#include <iostream>

#include <tgbot/tgbot.h>

#include "Responser.hpp"
#include "ServiceLocator/GeneralLocator.hpp"

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
        Services::instance().log->log("Bot running", LogLevel::Info);
        while (true)
        {
            try
            {
                longPoll.start();
            }
            catch (const TgBot::TgException& e)
            {
                Services::instance().log->log(std::string("Bot error: ") + e.what(), LogLevel::Error);
            }
            catch(const std::exception& e)
            {
                Services::instance().log->log(std::string("Bot error: ") + e.what(), LogLevel::Error);
            }
            catch (...)
            {
                Services::instance().log->log("Unknown Error", LogLevel::Error);
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
};
