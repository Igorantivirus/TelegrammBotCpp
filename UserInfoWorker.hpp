#pragma once

#include<pugixml.hpp>

#include<tgbot/Bot.h>

#include"ResponseGenerator.hpp"

#define USER_INFO_DB_FILE "UserDB.xml"

class UserWorker
{
public:
	UserWorker()
	{
		if (myFILEutils::isExist(USER_INFO_DB_FILE))
		{
			pugi::xml_node root = doc.append_child("root");
			root.append_child("admins");
			root.append_child("users");
			doc.save_file(USER_INFO_DB_FILE);
			doc.reset();
		}
	}

	void AddUser(const TgBot::Chat::Ptr& uP)
	{
		AddToArray(uP, "users");
	}
	void AddAmdin(const TgBot::Chat::Ptr& uP)
	{
		AddToArray(uP, "admins");
	}
	
	void AddAdmin(const TgBot::Chat::Ptr& uP)
	{
		AddToArray(uP, "admins");
	}
	bool DeleteAdmin(const std::string& ids)
	{
		long long id;
		try
		{
			id = std::stoll(ids);
		}
		catch (...)
		{
			return false;
		}

		DeleteFromArray("admins", "id", id);
		return true;
	}

	std::string GetAllUsers()
	{
		return "Users:\n" + GetFromArray("users");
	}
	std::string GetAllAdmins()
	{
		return "Admins:\n" + GetFromArray("admins");
	}

	bool IsAdmin(const TgBot::Chat::Ptr& uP)
	{
		doc.load_file(USER_INFO_DB_FILE);
		pugi::xml_node root = doc.child("root");
		pugi::xml_node users = root.child("admins");
		bool res = myXMLutils::haveAttribInArray(users, uP->id, "user", "id");
		doc.reset();
		return res;
	}

private:
	pugi::xml_document doc;

private:

	void AddToArray(const TgBot::Chat::Ptr& uP, const char* name)
	{
		doc.load_file(USER_INFO_DB_FILE);
		pugi::xml_node root = doc.child("root");
		pugi::xml_node users = root.child(name);
		if (myXMLutils::haveAttribInArray(users, uP->id, "users", "id"))
		{
			doc.reset();
			return;
		}
		pugi::xml_node userInfo = users.append_child("user");
		InitUserXML(uP, userInfo);
		doc.save_file(USER_INFO_DB_FILE);
		doc.reset();
	}

	void DeleteFromArray(const char* name, const char* attrib, const long long attribValue)
	{
		doc.load_file(USER_INFO_DB_FILE);
		pugi::xml_node root = doc.child("root");
		pugi::xml_node admins = root.child(name);
		for(pugi::xml_node inode = admins.child("user");inode;inode=inode.next_sibling("user"))
		{
			if (inode.attribute(attrib).as_llong() == attribValue)
			{
				admins.remove_child(inode);
				break;
			}
		}
		doc.save_file(USER_INFO_DB_FILE);
		doc.reset();
	}


	std::string GetFromArray(const char* name)
	{
		doc.load_file(USER_INFO_DB_FILE);
		pugi::xml_node root = doc.child("root");
		pugi::xml_node users = root.child(name);
		std::string res;
		for (pugi::xml_node nd = users.child("user"); nd; nd = nd.next_sibling("user"))
			res += std::string("User ") + nd.text().as_string() + " ID " + nd.attribute("id").as_string() + '\n';
		doc.reset();
		return res.empty() ? "List is empty" : res;
	}

	void InitUserXML(const TgBot::Chat::Ptr& uch, pugi::xml_node& node)
	{
		node.set_name("user");
		node.append_attribute("id").set_value(std::to_string(uch->id).c_str());
		std::string info = uch->username + " " + uch->firstName + " " + uch->lastName;
		node.append_child(pugi::node_pcdata).set_value(info.c_str());
	}

};

class CommandResponser
{
public:
	CommandResponser(const char* pas, GadalkaData& d, TgBot::Bot& b) : pasword(pas), data(d), bot(b) {}

	std::string GetResponse(std::string str, const TgBot::Chat::Ptr& uP)
	{
		str.erase(str.begin());
		std::vector<std::string> tkns;
		mySTRutils::tokenize(str, tkns);

		std::string res;
		if (IsPublicCommand(tkns, res, uP))
			return res;
		if (!wrk.IsAdmin(uP))
			return mySTRutils::to_utf8(L"Нет такой команды! Не обманывайтесь!");

		InitPrivateCommand(tkns, res, uP);

		return res;
	}



private:
	UserWorker wrk;

	GadalkaData& data;
	TgBot::Bot& bot;

	std::string pasword;

private://public command

	bool IsPublicCommand(const std::vector<std::string>& tkns, std::string& res, const TgBot::Chat::Ptr& uP)
	{
		if (tkns[0] == "start")
		{
			res = mySTRutils::to_utf8(L"Здравствуй, отрак! Я - Великая гадалка! Готова ответить на все твои вопросы! Поздоровайтесь мо мной!");
			wrk.AddUser(uP);
			return true;
		}
		else if (tkns[0] == "help")
		{
			res = mySTRutils::to_utf8(L"Не говори со мной на бесовском языке! Лучше спроси, что я могу, и я дам ответ!");
			return true;
		}
		else if (tkns[0] == "register")
		{
			RegistrCommand(tkns, res, uP);
			return true;
		}
		return false;
	}

	void RegistrCommand(const std::vector<std::string>& tkns, std::string& res, const TgBot::Chat::Ptr& uP)
	{
		if (tkns.size() < 2)
			return (res = mySTRutils::to_utf8(L"Не знаете, что делаете - так и не суйтесь, куда не надо!")), void();
		if (tkns[1] == pasword)
		{
			wrk.AddAmdin(uP);
			res = mySTRutils::to_utf8(L"Здравствуй, создатель! С возвращением!");
		}
		else
			return (res = mySTRutils::to_utf8(L"Неверно! Фигню глаголите! Штраф - 15 минут порчи!")), void();
	}

private://private command

	void InitPrivateCommand(const std::vector<std::string>& tkns, std::string& res, const TgBot::Chat::Ptr& uP)
	{
		const std::string& word = tkns[0];
		if (word == "boroda")
		{
			data.Update();
			res = mySTRutils::to_utf8(L"Я обновила словарный запас! Готова к дальнейшей работе!");
			return;
		}
		else if (word == "users")
			return UsersCommand(tkns, res, uP);
		else if (word == "admins")
			return AdminsCommand(tkns, res, uP);
		else
		{
			res = mySTRutils::to_utf8(L"Неизвестная команда!");
			return;
		}



	}

	void UsersCommand(const std::vector<std::string>& tkns, std::string& res, const TgBot::Chat::Ptr& uP)
	{
		if (tkns.size() < 2)
			return (res = mySTRutils::to_utf8(L"Слишком мало аргументов для этой функции!")), void();
		if(tkns[1] == "print")
			return (res = wrk.GetAllUsers()), void();
		else
			return (res = mySTRutils::to_utf8(L"Неверный аргумент для команды users!")), void();
	}
	void AdminsCommand(const std::vector<std::string>& tkns, std::string& res, const TgBot::Chat::Ptr& uP)
	{
		if (tkns.size() < 2)
			return (res = mySTRutils::to_utf8(L"Слишком мало аргументов для этой функции!")), void();
		if (tkns[1] == "print")
			return (res = wrk.GetAllAdmins()), void();
		else if (tkns[1] == "remove")
		{
			if(tkns.size() < 3)
				return (res = mySTRutils::to_utf8(L"Слишком мало аргументов для этой функции!")), void();
			if (wrk.DeleteAdmin(tkns[2]))
				res = mySTRutils::to_utf8(L"Вы успешно убрали админа с должности!");
			else
				res = mySTRutils::to_utf8(L"Ошибка при удалении!");
		}
		else if (tkns[1] == "add")
		{
			if (tkns.size() < 3)
				return (res = mySTRutils::to_utf8(L"Слишком мало аргументов для этой функции!")), void();

			long long id;
			try
			{
				id = std::stoll(tkns[2]);
			}
			catch (...)
			{
				res = mySTRutils::to_utf8(L"Ошибка при добавлении!");
			}
			try
			{
				TgBot::Chat::Ptr user = bot.getApi().getChat(id);
			}
			catch (...)
			{
				res = mySTRutils::to_utf8(L"Ошибка при добавлении!");
			}
			wrk.AddAdmin(bot.getApi().getChat(id));
			res = mySTRutils::to_utf8(L"Вы успешно поставили админа на должность!");
		}
		else
			return (res = mySTRutils::to_utf8(L"Неверный аргумент для команды admins!")), void();
	}

};