#pragma once

#include <locale>
#include <codecvt>
#include <complex>
#include <vector>
#include <sstream>
#include <fstream>

#include<pugixml.hpp>

#define forXML(node, chr) for(pugi::xml_node inode = node.child(chr);inode;inode = inode.next_sibling(chr))

namespace myFILEutils
{
	bool isExist(const char* fName)
	{
		std::ifstream in(fName);
		bool res = false;
		if (in.is_open())
			res = true;
		in.close();
		return res;
	}
}

namespace myXMLutils
{
	std::string extractText(const pugi::xml_node& node)
	{
		std::string text;
		for (pugi::xml_node child : node.children())
		{
			if (child.type() == pugi::node_pcdata)
				text += child.text().get();
			else
				text += extractText(child);
		}
		return text;
	}

	bool haveInArray(const std::string& str, const pugi::xml_node& array, const char* childName)
	{
		for (pugi::xml_node i = array.child(childName); i; i = i.next_sibling(childName))
			if (str.find(i.text().as_string()) != std::string::npos)
				return true;
		return false;
	}
	bool haveAttribInArray(const pugi::xml_node& users, const size_t item, const char* childName, const char* attribName)
	{
		for (pugi::xml_node nd = users.child(childName); nd; nd = nd.next_sibling(childName))
			if (nd.attribute(attribName).as_llong() == item)
				return true;
		return false;
	}

	void initChildAt(pugi::xml_node& resNode, const pugi::xml_node& root, const char* name, const unsigned int ind)
	{
		unsigned int i = 0;
		for (pugi::xml_node inode = root.child(name); inode; inode = inode.next_sibling(name), ++i)
		{
			if (i == ind)
			{
				resNode = inode;
				return;
			}
		}
	}
}

namespace mySTRutils
{
	void tokenize(const std::string& str, std::vector<std::string>& tokens)
	{
		std::istringstream iss(str);
		std::string token;
		while (iss >> token)
			tokens.push_back(token);
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
	void replaceAll(std::string& str, const std::string& oldS, const std::string& newS)
	{
		size_t ind;
		size_t last = 0;
		while ((ind = str.find(oldS, last)) != std::string::npos)
		{
			str.replace(ind, oldS.size(), newS);
			last = ind + newS.size();
		}
	}
	void replaceAll(std::string& str, const char oldS, const char newS)
	{
		for (auto& i : str)
			if (i == oldS)
				i = newS;
	}

	bool have(const std::string& val, const std::string& str)
	{
		return str.find(val) != std::string::npos;
	}
	static void toLower(std::string& str)
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

}