#pragma once

#include<string>
#include<unordered_map>
#include<sstream>
#include<vector>

#include<pugixml.hpp>
#include<tgbot/tgbot.h>

#include"BotUtils.hpp"
#include"Randomiser.hpp"

#include"AnekdotParser.hpp"
#include<MathParse/MathParse.hpp>

#define USERS_DATA_FILE "UsserDB.xml"
#define RESPONS_DATA_FILE "ResponseData.xml"

class ChatsInfo
{
public:

	bool ExistChat(const std::int64_t& id) const
	{
		return users.find(id) != users.end();
	}
	bool IsAgressive(const std::int64_t& id) const
	{
		if (auto it = users.find(id); it != users.end())
			return it->second;
		return false;
	}

	void AddChat(const std::int64_t& id)
	{
		if (!ExistChat(id))
			users.insert({ id, false });
	}
	void ReplaceAgressive(const std::int64_t& id)
	{
		if (auto it = users.find(id); it != users.end() && !users.empty())
			users[id] = !users[id];
	}

private:

	std::unordered_map<std::int64_t, bool> users;

};

class UserDBWorker
{
public:
	UserDBWorker()
	{
		existOrCreateFile(USERS_DATA_FILE);
	}

	bool IsUser(const std::int64_t& id)
	{
		doc.load_file(USERS_DATA_FILE);
		root = doc.child("root");
		bool res = ExistUserInArray("users", id);
		doc.reset();
		return res;
	}
	bool IsAdmins(const std::int64_t& id)
	{
		doc.load_file(USERS_DATA_FILE);
		root = doc.child("root");
		bool res = ExistUserInArray("admins", id);
		doc.reset();
		return res;
	}

	void AddUser(const TgBot::Message::Ptr& message)
	{
		std::string value = message->from->username + "|" + message->from->firstName + "|" + message->from->lastName;
		doc.load_file(USERS_DATA_FILE);
		root = doc.child("root");
		AddUserToArray("users", message->from->id, value);
		doc.save_file(USERS_DATA_FILE);
	}
	bool AddAdmin(const std::int64_t& id)
	{
		doc.load_file(USERS_DATA_FILE);
		root = doc.child("root");
		
		std::string value;
		pugi::xml_node users = root.child("users");
		bool isUser = false;
		for(pugi::xml_node i = users.child("user");i;i = i.next_sibling("user"))
			if (i.attribute("id").as_llong() == id)
			{
				value = i.text().as_string();
				isUser = true;
				break;
			}
		if (!isUser)
			return false;
		AddUserToArray("admins", id, value);
		doc.save_file(USERS_DATA_FILE);
		return true;
	}
	
	bool DeleteAdmin(const std::int64_t& id)
	{
		doc.load_file(USERS_DATA_FILE);
		root = doc.child("root");
		if (!ExistUserInArray("admins", id))
		{
			doc.reset();
			return false;
		}
		DeleteUserFromArray("admins", id);
		doc.save_file(USERS_DATA_FILE);
		return true;
	}

	std::string GetAllUsers()
	{
		doc.load_file(USERS_DATA_FILE);
		root = doc.child("root");
		std::string res;
		for (pugi::xml_node node = root.child("users").child("user"); node; node = node.next_sibling("user"))
			res += std::string("User: ") + node.attribute("id").as_string() + "|" + node.text().as_string() + '\n';
		doc.reset();
		return res;
	}
	std::string GetAllAdmins()
	{
		doc.load_file(USERS_DATA_FILE);
		root = doc.child("root");
		std::string res;
		for (pugi::xml_node node = root.child("admins").child("user"); node; node = node.next_sibling("user"))
			res += std::string("User: ") + node.attribute("id").as_string() + "|" + node.text().as_string() + '\n';
		doc.reset();
		return res;
	}

private:
	pugi::xml_document doc;
	pugi::xml_node root;

	bool ExistUserInArray(const char* name, const std::int64_t& id)
	{
		pugi::xml_node array = root.child(name);
		for (pugi::xml_node i = array.child("user"); i; i = i.next_sibling("user"))
			if (i.attribute("id").as_llong() == id)
				return true;
		return false;
	}

	void AddUserToArray(const char* name, const std::int64_t& id, const std::string& value)
	{
		if (ExistUserInArray(name, id))
			return;

		pugi::xml_node array = root.child(name);
		pugi::xml_node newNode = array.append_child("user");
		newNode.append_attribute("id").set_value(id);
		newNode.text() = value.c_str();
	}
	void DeleteUserFromArray(const char* name, const std::int64_t& id)
	{
		pugi::xml_node array = root.child(name);
		for (pugi::xml_node i = array.child("user"); i;)
		{
			if (i.attribute("id").as_llong() == id)
			{
				pugi::xml_node nextNode = i.next_sibling("user");
				array.remove_child(i);
				i = nextNode;
			}
			else
				i = i.next_sibling("user");
		}
	}

};

class Responser
{
public:
	Responser()
	{
		doc.load_file(RESPONS_DATA_FILE);
		root = doc.child("root");

		specialWords = root.child("specialWords");
		defaultWords = root.child("defaultWords");
		funcWords = root.child("funcWords");
		emotWords = root.child("emotWords");
	}
	~Responser()
	{
		doc.reset();
	}

	void Update()
	{
		doc.load_file(RESPONS_DATA_FILE);
		root = doc.child("root");

		specialWords = root.child("specialWords");
		defaultWords = root.child("defaultWords");
		funcWords = root.child("funcWords");
	}

	std::string GetAnswer(std::string str, bool& agr)
	{
		MySTRutils::ToLower(str);
		std::string res;
	
		if (InitAtFuncWords(str, res))
			return res;

		if(!InitAtSpecial(str, res))
			InitAtDefault(res);

		ProcessEmot(str, res, agr);

		return res;
	}


private:
	pugi::xml_document doc;
	pugi::xml_node root;

	pugi::xml_node specialWords;
	pugi::xml_node defaultWords;

	pugi::xml_node funcWords;
	pugi::xml_node emotWords;

	Randomiser<int> rnd;

	AnedotGenerator aneks;
	expr::Parser mathPars;

private:

	bool InitAtSpecial(const std::string& str, std::string& res)
	{
		forxml(variant, specialWords, "variantReplic")
		{
			forxml(key, variant.child("keys"), "text")
			{
				if (str.find(key.text().as_string()) != std::string::npos)
				{
					pugi::xml_node values = variant.child("values");
					pugi::xml_node resNode;
					MyXMLutils::InitNodeAt(values, "text", rnd.generate(0, values.attribute("count").as_int() - 1), resNode);
					InitTextAtNode(resNode, res);
					return true;
				}
			}
		}
		return false;
	}

	bool InitAtFuncWords(const std::string& str, std::string& res)
	{
		forxml(inode, funcWords, "text")
		{
			if (str.find(inode.text().as_string()) != std::string::npos)
			{
				unsigned int id = inode.attribute("id").as_uint();
				if (InitFuncWordAtID(str, res, id))
					return true;
				return false;
			}
		}
		return false;
	}

	void InitAtDefault(std::string& res)
	{
		std::size_t ind = rnd.generate(0, defaultWords.attribute("count").as_ullong() - 1);
		pugi::xml_node node;
		MyXMLutils::InitNodeAt(defaultWords, "text", ind, node);
		InitTextAtNode(node, res);
	}

	bool InitFuncWordAtID(const std::string& str, std::string& res, const unsigned id)
	{
		if (id == 1)
		{
			aneks.GenerateAtKeyWord(str, res);
			return true;
		}
		else if (id == 2)
		{
			if (size_t ind = str.rfind(' '); ind != std::string::npos)
			{
				try
				{
					res = complexToString(mathPars.parse(str.substr(++ind, str.size() - ind)).getProcessingResult().getValue());
				}
				catch (...)
				{
					res = to_utf8(L"����, ����! ����� �� �� ����������, � �����-�� ����, ����� �������� ����! �� ��� ��� ����� �� 15 �����!");
				}
				return true;
			}
			return false;
		}
		else if(id == 3)
		{
			std::vector<std::string> tkns;
			MySTRutils::tokenize(str, tkns);
			int begin = rnd.MinValue();
			int end = rnd.MaxValue();
			std::string otW = MySTRutils::to_utf8(L"��");
			std::string doW = MySTRutils::to_utf8(L"��");
			for (size_t i = 0; i < tkns.size()-1; ++i)
			{
				if (tkns[i] == otW)
					begin = std::atoi(tkns[i+1].c_str());
				else if(tkns[i] == doW)
					end = std::atoi(tkns[i + 1].c_str());
			}
			if (begin > end)
				std::swap(begin, end);
			try
			{
				res = std::to_string(rnd.generate(begin, end));
			}
			catch (...)
			{
				res = MySTRutils::to_utf8(L"��, �� ����������!");
			}
			return true;
		}
		return false;
	}

	void InitTextAtNode(const pugi::xml_node& node, std::string& res)
	{
		res = node.text().as_string();
		if (node.attribute("special").as_bool() == true)
		{
			MySTRutils::replaceAll(res, "%R", std::to_string(rnd.generate(50, 150)).c_str());
		}
	}

private://emotions

	void ProcessEmot(const std::string& str, std::string& res, bool& agr)
	{
		if (agr)
			AddAngryWord(res);
		else
		{
			if (IsAngry(str))
			{
				agr = true;
				AddAngryWord(res);
			}
		}
		if (IsFuny(str))
		{
			SetFunyWord(res);
			agr = false;
		}
	}

	void AddAngryWord(std::string& res)
	{
		std::string pr;
		GetWordFromEmotArray(pr, "angryWords");
		res += std::string(1, ' ') + pr;
	}

	void SetFunyWord(std::string& res)
	{
		std::string pr;
		GetWordFromEmotArray(res, "funyFords");
	}

	void GetWordFromEmotArray(std::string& str, const char* array)
	{
		pugi::xml_node words = emotWords.child(array).child("responses");
		std::size_t ind = rnd.generate(0, words.attribute("count").as_ullong() - 1);
		pugi::xml_node resNode;
		MyXMLutils::InitNodeAt(words, "text", ind, resNode);

		str = resNode.text().as_string();
	}


	bool IsAngry(const std::string& str)
	{
		return InEmotArray(str, "angryWords");
	}

	bool IsFuny(const std::string& str)
	{
		return InEmotArray(str, "funyFords");
	}

	bool InEmotArray(const std::string& str, const char* array)
	{
		pugi::xml_node words = emotWords.child(array).child("trigers");
		forxml(i, words, "text")
			if (str.find(i.text().as_string()) != std::string::npos)
				return true;
		return false;
	}

};

class InputProcessor
{
public:
	InputProcessor(const std::string& p) : pass(p)
	{}

	std::string Processing(const std::string& str, const TgBot::Message::Ptr& message)
	{
		if (str.empty())
			return "Empty Input!";

		if (str[0] == '/')
			return CommandProcessor(str, message);

		bool agr = chatsInfo.IsAgressive(message->chat->id);
		bool agrVal = agr;
		std::string res =  responser.GetAnswer(str, agrVal);
		if (agr != agrVal)
			chatsInfo.ReplaceAgressive(message->chat->id);
		return res;
	}

private:

	ChatsInfo chatsInfo;

	UserDBWorker usersInfo;

	Responser responser;

	const std::string pass;

private://Command

	std::string CommandProcessor(const std::string& str, const TgBot::Message::Ptr& message)
	{
		std::vector<std::string> tkns;
		MySTRutils::tokenize(str, tkns);
		std::string res;

		FillPublicCommand(tkns, message, res) || FillPrivateCommand(tkns, message, res);
		if(res.empty())
			res = to_utf8(L"���� ����� �������, �����! �� ���� ������ �����!!!");
		return res;
	}


	bool FillPublicCommand(const std::vector<std::string>& tkns, const TgBot::Message::Ptr& message, std::string& res)
	{
		if (tkns[0] == "start")
		{
			res = to_utf8(L"����� ����������, �����!");
			usersInfo.AddUser(message);
			chatsInfo.AddChat(message->chat->id);
			return true;
		}
		else if (tkns[0] == "help")
		{
			res = to_utf8(L"�� ������ �� ���� �� ��������� �����! ����� ������, ��� � ����, � � ��� �����!");
			return true;
		}
		else if (tkns[0] == "register")
		{
			res = ProcComandRegister(tkns, message);
			return true;
		}
		return false;
	}

	bool FillPrivateCommand(const std::vector<std::string>& tkns, const TgBot::Message::Ptr& message, std::string& res)
	{
		if (!usersInfo.IsAdmins(message->from->id))
			return false;

		if (tkns[0] == "users")
		{
			res = ProcCommandUsers(tkns);
			return true;
		}
		if (tkns[0] == "admins")
		{
			res = ProcCommandAdmins(tkns);
			return true;
		}
		if (tkns[0] == "boroda")
		{
			chatsInfo.ReplaceAgressive(message->chat->id);
			if (chatsInfo.IsAgressive(message->chat->id))
				res = MySTRutils::to_utf8(L"������ ���, ���������� ����!");
			else
				res = MySTRutils::to_utf8(L"������ ���, ���������� ������!");
			return true;
		}
		if (tkns[0] == "update")
		{
			responser.Update();
			res = MySTRutils::to_utf8(L"� �������� ���� ��������� �����! ������ � ���������� ������!");
			return true;
		}
		return false;
	}

	std::string ProcCommandUsers(const std::vector<std::string>& tkns)
	{
		if (tkns.size() < 2)
			return to_utf8(L"���� ����������!");
		if (tkns[1] == "print")
			return usersInfo.GetAllUsers();
		return to_utf8(L"������� ����������, �������� - ���!");
	}
	std::string ProcCommandAdmins(const std::vector<std::string>& tkns)
	{
		if (tkns.size() < 2)
			return to_utf8(L"���� ����������!");
		if (tkns[1] == "print")
			return usersInfo.GetAllAdmins();
		if (tkns[1] == "add")
		{
			if(tkns.size() < 3)
				return to_utf8(L"������� ����������, �������� - ���!");
			if(usersInfo.AddAdmin(MySTRutils::SafelyStoll(tkns[2])))
				return to_utf8(L"����� �������� �������� �� ���������!");
			else
				return to_utf8(L"������! ��� ������ id!");
		}
		if (tkns[1] == "delete")
		{
			if (tkns.size() < 3)
				return to_utf8(L"������� ����������, �������� - ���!");
			if (usersInfo.DeleteAdmin(MySTRutils::SafelyStoll(tkns[2])))
				return to_utf8(L"����� �������� ����� � ��������!");
			else
				return to_utf8(L"������! ��� ������ id!");
		}
		return to_utf8(L"������� ����������, �������� - ���!");
	}

	std::string ProcComandRegister(const std::vector<std::string>& tkns, const TgBot::Message::Ptr& message)
	{
		if (tkns.size() < 2)
			return to_utf8(L"���� ����������!");
		if (tkns[1] == pass)
		{
			usersInfo.AddAdmin(message->from->id);
			return MySTRutils::to_utf8(L"� ������������, ���������!");
		}
		else
		{
			return MySTRutils::to_utf8(L"�� ������, ��� ������� - ��� �� ������! ��� ����� �� 15 �����!");
		}
	}
};