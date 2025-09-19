#pragma once

#include <vector>
#include <string>

#include <tgbot/tgbot.h>

#include "MathSolver.hpp"

class Responser
{
public:


    std::pair<std::string, bool> getResponsesOnInlineMessages(const TgBot::InlineQuery::Ptr& query)
    {
        if(solver_.isInlineMathQuestion(query->query))
            return solver_.getMathInlineResponseResult(query->query);
        return {"", false};
        
    }

private:

    MathSolver solver_;

private:



};