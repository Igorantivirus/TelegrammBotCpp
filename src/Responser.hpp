#pragma once

#include <string>

#include <tgbot/tgbot.h>

#include "MathSolver.hpp"
#include "Anekdots/AnekdotGenerator.hpp"
#include "StringUtility.hpp"

class Responser
{
public:
    Responser() :
        generator_("assets/PagesData.xml")
    {}

    std::pair<std::string, bool> getResponsesOnInlineMessages(const TgBot::InlineQuery::Ptr &query)
    {
        if (solver_.isInlineMathQuestion(query->query))
            return solver_.getMathInlineResponseResult(query->query);
        std::string request = query->query;
        StringUtility::toLower(request);

        if (generator_.isInlineAnekRequest(request))
            return {generator_.parseAnekdotAtKeyword(request), true};

        return {"", false};
    }

    std::string getResponse(TgBot::Message::Ptr message)
    {
        if(message->text.starts_with("calc "))
            return mathResponse(message->text);
        return baseResponse(message);
    }
    std::string getResponse(std::string str)
    {
        if(str.starts_with("calc "))
            return mathResponse(str);
        return "Error";
    }

private:
    MathSolver solver_;
    AnekdotGenerator generator_;

private:

    std::string mathResponse(std::string& str)
    {
        str.erase(0, 5);//Удаляем "calc "
        StringUtility::replaceAll(str, " ", "");
            return solver_.getMathResponseResult(str);
    }

    std::string baseResponse(TgBot::Message::Ptr message)
    {
        return "Мои ответы в режиме чата пока что в тестовом режиме. Пока что я могу только считать математику, но скоро я многому научусь!";
    }

};