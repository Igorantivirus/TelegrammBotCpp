#pragma once

#include"BotUtils.hpp"

#include"AnekdotParser.hpp"
#include<MathParse/MathParse.hpp>

class GadalkaData
{
public:
	GadalkaData()
	{
		doc.load_file("data.xml");
		root = doc.child("root");
	}

	std::string GenerateAnswer(std::string str)
	{
		ToLower(str);

		std::string res;

		if (unsigned int id = CheckFunckWords(str); id)
		{
			GenerateAtFuncWord(str, res, id);
			return res;
		}
		else if (CheckVariables(str, res))
			nullptr;
		else if (CheckConstants(str, res))
			nullptr;
		else
			GeneretaDefault(res);

		if (!agresiveMode)
			agresiveMode = HaveAgressive(str);
		if (HaveIzv(str))
		{
			agresiveMode = false;
			SetIzv(res);
		}
		if (agresiveMode)
			AddAgressive(res);


		return res;
	}

	const bool AgressiveMode() const
	{
		return agresiveMode;
	}
	bool& AgressiveMode()
	{
		return agresiveMode;
	}

	void Update()
	{
		doc.load_file("data.xml");
		root = doc.child("root");
	}

private:
	pugi::xml_document doc;
	pugi::xml_node root;

	bool agresiveMode = false;

	Randomiser<unsigned int> rnd;

	AnedotGenerator generator;

	bool CheckConstants(const std::string& str, std::string& res)
	{
		pugi::xml_node variants = root.child("constants");

		for (pugi::xml_node vari = variants.child("variantReplic"); vari; vari = vari.next_sibling("variantReplic"))
		{
			if (Have(vari.attribute("key").as_string(), str))
			{
				unsigned int r = rnd.generate(0, vari.attribute("count").as_uint() - 1);
				pugi::xml_node result;
				InitChildAt(result, vari, "text", r);
				res = result.text().as_string();
				if (result.attribute("special").as_string() == std::string("1"))
					replace(res, "%R", std::to_string(rnd.generate(20, 100)));
				return true;
			}
		}
		return false;
	}


	bool CheckVariables(const std::string& str, std::string& res)
	{
		pugi::xml_node vars = root.child("variants");


		for (pugi::xml_node var = vars.child("variant"); var; var = var.next_sibling("variant"))
		{
			if (HaveInText(str, var.child("keys")))
			{
				pugi::xml_node resN;
				InitChildAt(resN, var.child("results"), "text", rnd.generate(0, var.attribute("resultCount").as_uint() - 1));
				res = resN.text().as_string();
				return true;
			}
		}
		return false;
	}

	void GeneretaDefault(std::string& res)
	{
		pugi::xml_node defAns = root.child("defaultAnswers");
		unsigned int r = rnd.generate(0, defAns.attribute("count").as_uint() - 1);
		pugi::xml_node result;
		InitChildAt(result, defAns, "text", r);
		res = result.text().as_string();
		if (result.attribute("special").as_string() == std::string("1"))
			replace(res, "%R", std::to_string(rnd.generate(20, 100)));
	}

	bool HaveAgressive(const std::string& str)
	{
		pugi::xml_node agr = root.child("rugan");
		return HaveInText(str, agr.child("userRugan"));
	}
	void AddAgressive(std::string& res)
	{
		pugi::xml_node agr = root.child("rugan");
		pugi::xml_node gr = agr.child("gadalkaRugan");

		pugi::xml_node resN;
		InitChildAt(resN, gr, "text", rnd.generate(0, gr.attribute("count").as_uint() - 1));

		res += std::string(1uLL, ' ') + resN.text().as_string();
	}

	bool HaveIzv(const std::string& str)
	{
		pugi::xml_node agr = root.child("izv");
		return HaveInText(str, agr.child("userIzv"));
	}
	void SetIzv(std::string& res)
	{
		pugi::xml_node agr = root.child("izv");
		pugi::xml_node gr = agr.child("gadalkaIzv");

		pugi::xml_node resN;
		InitChildAt(resN, gr, "text", rnd.generate(0, gr.attribute("count").as_uint() - 1));

		res = resN.text().as_string();
	}

	unsigned int CheckFunckWords(const std::string& str)
	{
		pugi::xml_node words = root.child("funcWords");
		for (pugi::xml_node i = words.child("text"); i; i = i.next_sibling("text"))
		{
			if (str.find(i.text().as_string()) != std::string::npos)
			{
				return i.attribute("id").as_uint();
			}
		}
		return 0;
	}

	void GenerateAtFuncWord(const std::string& str, std::string& res, const unsigned int id)
	{
		if (id == 1)
			generator.GenerateAtKeyWord(str, res);
		else if (id == 2)
		{
			if (size_t ind = str.rfind(' '); ind != std::string::npos)
			{
				try
				{
					expr::Parser pars;
					res = complexToString(pars.parse(str.substr(++ind, str.size() - ind)).getProcessingResult().getValue());
				}
				catch (const expr::ParseException& e)
				{
					res = e.what();
				}
				catch (...)
				{
					res = to_utf8(L"����, ����! ����� �� �� ����������, � �����-�� ����, ����� �������� ����! �� ��� ��� ����� �� 15 �����!");
				}
			}
			else
				res = to_utf8(L"��������� ���������, � ��� ������ �� ����! ��� �� ������ ���������!");
		}
	}

private:

	static bool Have(const std::string& val, const std::string& str)
	{
		return str.find(val) != std::string::npos;
	}
	static bool HaveInText(const std::string& str, const pugi::xml_node& node)
	{
		for (pugi::xml_node i = node.child("text"); i; i = i.next_sibling("text"))
			if (str.find(i.text().as_string()) != std::string::npos)
				return true;
		return false;
	}
	static void InitChildAt(pugi::xml_node& node, const pugi::xml_node& rt, const char* name, const unsigned int ind)
	{
		unsigned int i = 0;
		for (pugi::xml_node inode = rt.child(name); inode; inode = inode.next_sibling(name), ++i)
		{
			if (i == ind)
			{
				node = inode;
				return;
			}
		}
	}
	static void ToLower(std::string& str)
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
	static void replace(std::string& str, const std::string& oldS, const std::string& newS, unsigned int count = 1)
	{
		size_t ind = 0;
		for (unsigned int i = 0; i < count; ++i)
			if (ind = str.find(oldS); ind != std::string::npos)
				str.replace(ind, oldS.size(), newS);
			else
				return;
	}

};