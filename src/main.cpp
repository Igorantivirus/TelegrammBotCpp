#include "Bot.hpp"
#include "MyApiKey.hpp"

#if defined(_WIN32) || defined(_WIN64)
const std::string TgBot::InlineQueryResultArticle::TYPE = "article";
const std::string TgBot::InputTextMessageContent::TYPE = "text";
#endif

int main()
{
#if defined(_WIN32) || defined(_WIN64)
    system("chcp 65001 > nul");
#endif

    Bot bot(TG_API_KEY);
    bot.run();

    return 0;
}
