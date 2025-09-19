#pragma once

#include <locale>
#include <codecvt>
#include <complex>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

#include<pugixml.hpp>

#define forxml(a, array, name) for(pugi::xml_node a = array.child(name);a;a=a.next_sibling(name))

namespace MyXMLutils
{

	void InitNodeAt(const pugi::xml_node& array, const char* name, const std::size_t ind, pugi::xml_node& res)
	{
		std::size_t i = 0;
		for(pugi::xml_node inode = array.child(name);inode;inode = inode.next_sibling(name), i++)
			if (i == ind)
			{
				res = inode;
				return;
			}
		res.text() = "empty.";
	}

	std::string to_utf8(const std::wstring& str) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(str);
	}


}

namespace MySTRutils
{
	void ToLower(std::string& str)
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

	std::string to_utf8(const std::wstring& str) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(str);
	}

	void replaceAll(std::string& str, const char* oldS, const char* newS)
	{
		const size_t s1 = std::strlen(oldS);
		const size_t s2 = std::strlen(newS);
		size_t ind;
		size_t last = 0;
		while ((ind = str.find(oldS, last)) != std::string::npos)
		{
			str.replace(ind, s1, newS);
			last = ind + s2;
		}
	}

	void tokenize(const std::string& str, std::vector<std::string>& tokens)
	{
		std::istringstream iss(str);
		std::string token;
		while (iss >> token)
			tokens.push_back(token);
		tokens[0].erase(tokens[0].begin());
	}

	long long SafelyStoll(const std::string& str)
	{
		long long res = 0;
		try
		{
			res = std::stoll(str);
		}
		catch (...)
		{
			res = 0;
		}
		return res;
	}

}

void existOrCreateFile(const char* fName)
{
	std::ofstream file(fName, std::ios::app);
	file.close();
}

std::string to_utf8(const std::wstring& str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(str);
}

std::string toFixedString(const long double x)
{
	std::string s = std::to_string(x);
	if (s.find('.') == std::string::npos)
		return s;
	while (s.back() == '0')
		s.pop_back();
	if (s.back() == '.')
		s.pop_back();
	return s;
}
std::string complexToString(const std::complex<long double>& z)
{
	if (!z.imag())
		return toFixedString(z.real());
	else if (!z.real())
		return toFixedString(z.imag()) + 'i';
	else
		return toFixedString(z.real()) + (z.imag() > 0 ? '+' : '-') + toFixedString(std::abs(z.imag())) + 'i';
}