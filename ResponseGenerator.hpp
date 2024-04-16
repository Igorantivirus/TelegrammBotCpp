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
		mySTRutils::toLower(str);

		std::string res;

		if (unsigned int id = CheckFunckWords(str); id)
		{
			GenerateAtFuncWord(str, res, id);
			return res;
		}
		else if (!(CheckVariables(str, res) || CheckConstants(str, res)))
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
			if (mySTRutils::have(vari.attribute("key").as_string(), str))
			{
				unsigned int r = rnd.generate(0, vari.attribute("count").as_uint() - 1);
				pugi::xml_node result;
				myXMLutils::initChildAt(result, vari, "text", r);
				res = result.text().as_string();
				if (result.attribute("special").as_string() == std::string("1"))
					mySTRutils::replaceAll(res, "%R", std::to_string(rnd.generate(20, 100)));
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
			if (myXMLutils::haveInArray(str, var.child("keys"), "text"))
			{
				pugi::xml_node resN;
				myXMLutils::initChildAt(resN, var.child("results"), "text", rnd.generate(0, var.attribute("resultCount").as_uint() - 1));
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
		myXMLutils::initChildAt(result, defAns, "text", r);
		res = result.text().as_string();
		if (result.attribute("special").as_string() == std::string("1"))
			mySTRutils::replaceAll(res, "%R", std::to_string(rnd.generate(20, 100)));
	}

	bool HaveAgressive(const std::string& str)
	{
		pugi::xml_node agr = root.child("rugan");
		return myXMLutils::haveInArray(str, agr.child("userRugan"), "text");
	}
	void AddAgressive(std::string& res)
	{
		pugi::xml_node agr = root.child("rugan");
		pugi::xml_node gr = agr.child("gadalkaRugan");

		pugi::xml_node resN;
		myXMLutils::initChildAt(resN, gr, "text", rnd.generate(0, gr.attribute("count").as_uint() - 1));

		res += std::string(1uLL, ' ') + resN.text().as_string();
	}

	bool HaveIzv(const std::string& str)
	{
		pugi::xml_node agr = root.child("izv");
		return myXMLutils::haveInArray(str, agr.child("userIzv"), "text");
	}
	void SetIzv(std::string& res)
	{
		pugi::xml_node agr = root.child("izv");
		pugi::xml_node gr = agr.child("gadalkaIzv");

		pugi::xml_node resN;
		myXMLutils::initChildAt(resN, gr, "text", rnd.generate(0, gr.attribute("count").as_uint() - 1));

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
					res = mySTRutils::complexToString(pars.parse(str.substr(++ind, str.size() - ind)).getProcessingResult().getValue());
				}
				catch (const expr::ParseException& e)
				{
					res = e.what();
				}
				catch (...)
				{
					res = mySTRutils::to_utf8(L"Вижу, вижу! Ввели вы не математику, а какую-то чушь, чтобы запутать меня! За это вам порча на 15 минут!");
				}
			}
			else
				res = mySTRutils::to_utf8(L"Попросили посчитать, а сам пример не дали! Вот вы хорошо придумали!");
		}
	}
};