#pragma once

#include <vector>
#include <string>

#include <tgbot/tgbot.h>

#include "MathSolver.hpp"
#include "Anekdots/AnekdotGenerator.hpp"

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

private:
    MathSolver solver_;
    AnekdotGenerator generator_;

private:
};