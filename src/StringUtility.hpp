#pragma once

#include <vector>
#include <string>

namespace StringUtility
{
    
    std::vector<std::string_view> split(const std::string& s, const char* c)
    {
        const std::size_t cSize = std::strlen(c);
        std::vector<std::string_view> res;
        std::size_t last = 0;
        std::size_t ind = s.find(c);
        while(ind != std::string::npos)
        {
            if(ind != last)
                res.push_back(std::string_view(s.c_str() + last, ind - last));
            last = ind + cSize;
            ind = s.find(c, last);
        }
        if(s.size() != last)
            res.push_back(std::string_view(s.c_str() + last, s.size() - last));
        return res;
    }
    std::vector<std::string_view> split(const std::string& s, const char c)
    {
        std::vector<std::string_view> res;
        std::size_t last = 0;
        std::size_t ind = s.find(c);
        while(ind != std::string::npos)
        {
            if(ind != last)
                res.push_back(std::string_view(s.c_str() + last, ind - last));
            last = ind + 1;
            ind = s.find(c, last);
        }
        if(s.size() != last)
            res.push_back(std::string_view(s.c_str() + last, s.size() - last));
        return res;
    }

    void toLower(std::string& str)
	{
		int pr = 0;
		for (size_t i = 0; i < str.size(); ++i)
		{
			if (str[i] == -48)
			{
				i++;

				if (str[i] == -127)
				{
					str[i - 1] = -47;
					str[i] = -111;
					continue;
				}

				if (str[i] >= -112 && str[i] <= -97)
					str[i] += 32;
				else if (str[i] >= -96 && str[i] <= -81)
				{
					str[i - 1] = -47;
					str[i] -= 32;
				}
			}
			else
				str[i] = tolower(str[i]);
		}
	}

    void replaceAll(std::string &str, const char *oldS, const char *newS)
    {
        const std::size_t s1 = std::strlen(oldS);
        const std::size_t s2 = std::strlen(newS);
        std::size_t ind;
        std::size_t last = 0;
        while ((ind = str.find(oldS, last)) != std::string::npos)
        {
            str.replace(ind, s1, newS);
            last = ind + s2;
        }
    }
    void replaceAll(std::string &str, const char *oldS, const char newS, const std::size_t count = 1)
    {
        const std::size_t s1 = std::strlen(oldS);
        std::size_t ind;
        std::size_t last = 0;
        while ((ind = str.find(oldS, last)) != std::string::npos)
        {
            str.replace(ind, s1, count, newS);
            last = ind + 1;
        }
    }
    void replaceAll(std::string &str, const char oldS, const char newS)
    {
        for (auto &i : str)
            if (i == oldS)
                i = newS;
    }

} // namespace StringUtility
