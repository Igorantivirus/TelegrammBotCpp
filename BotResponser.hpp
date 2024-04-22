#pragma once

#include<string>
#include<unordered_map>
#include<sstream>
#include<vector>

#include<pugixml.hpp>
#include<tgbot/tgbot.h>

#include"BotUtils.hpp"

#define USERS_DATA_FILE "UsserDB.xml"

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
			it->second;
	}

	void AddChat(const std::int64_t& id)
	{
		if (!ExistChat(id))
			users.insert({ id, false });
	}
	void SetAgressive(const std::int64_t& id, const bool value)
	{
		if (auto it = users.find(id); it != users.end())
			users[id] = value;
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
	void AddAdmin(const TgBot::Message::Ptr& message)
	{
		std::string value = message->from->username + "|" + message->from->firstName + "|" + message->from->lastName;
		doc.load_file(USERS_DATA_FILE);
		root = doc.child("root");
		AddUserToArray("admins", message->from->id, value);
		doc.save_file(USERS_DATA_FILE);
	}
	
	void DeleteAdmin(const std::int64_t& id)
	{
		doc.load_file(USERS_DATA_FILE);
		root = doc.child("root");
		DeleteUserFromArray("admins", id);
		doc.save_file(USERS_DATA_FILE);
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
		newNode.set_value(value.c_str());
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





private:








};




class InputProcessor
{
public:

	std::string Processing(const std::string& str, const TgBot::Chat::Ptr& chat)
	{
		if (str.empty())
			return "Empty Input!";

		if (str[0] == '/')
			0;// return


		return "";
	}

private:

	ChatsInfo chatsInfo;

	UserDBWorker usersInfo;

	Responser responser;

private://Command

	std::string CommandProcessor(const std::string& str, const TgBot::Chat::Ptr& chat)
	{
		std::vector<std::string> tkns;
		tokenize(str, tkns);

		std::string res;

		return "";
	}


	bool FillPublicCommand(const std::vector<std::string>& tkns, const TgBot::Chat::Ptr& chat, std::string& res)
	{
		if (tkns[0] == "start")
		{
			res = to_utf8(L"Добро пожаловать, отрак!");


			return true;
		}


		TgBot::Message::Ptr message;
		








		return false;
	}

private:

	static void tokenize(const std::string& str, std::vector<std::string>& tokens)
	{
		std::istringstream iss(str);
		std::string token;
		while (iss >> token)
			tokens.push_back(token);
		tokens[0].erase(tokens[0].begin());
	}
};