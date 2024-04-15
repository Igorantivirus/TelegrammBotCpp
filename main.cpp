#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include"TalkingBot.hpp"

void ConsoleGadalka()
{
	GadalkaData data;

	std::string s;

	while (true)
	{
		std::getline(std::cin, s);
        std::cout << data.GenerateAnswer(s) << std::endl;
    }
}

int main()
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    //ConsoleGadalka();
    
    TalkingBot gadalka(TG_API_KEY_TEST);
    gadalka.Run();

    return 0;
}
