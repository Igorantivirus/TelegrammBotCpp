#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include "MyApiKey.hpp"

#include"Bot.hpp"

// void COnsoleGadalka()
// {
//     Responser resp;

//     std::string s;

//     bool agr = false;

//     while (true)
//     {
//         std::getline(std::cin, s);
//         std::cout << resp.GetAnswer(s, agr) << std::endl;
//     }
// }

const std::string TgBot::InlineQueryResultArticle::TYPE = "article";
const std::string TgBot::InputTextMessageContent::TYPE = "text";

int main()
{
#if defined(_WIN32) || defined(_WIN64)
    system("chcp 65001 > nul");
#endif

    Bot bot(TG_API_KEY);
    bot.run();

    return 0;
}
