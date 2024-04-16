#pragma once

#include<pugixml.hpp>

#include<tgbot/Bot.h>

#define USER_INFO_DB_FILE "UserDB.xml"

class UserWorker
{
public:
	UserWorker() {}

	void AddInfo(const TgBot::Chat::Ptr& uP)
	{
		AddToArray(uP, "users");
	}
	void AddAmdin(const TgBot::Chat::Ptr& uP)
	{
		AddToArray(uP, "admins");
	}

	std::string GetAllUsers()
	{
		return GetFromArray("users");
	}
	std::string GetAllAdmins()
	{
		return GetFromArray("admins");
	}

	bool IsAdmin(const TgBot::Chat::Ptr& uP)
	{
		doc.load_file(USER_INFO_DB_FILE);
		pugi::xml_node root = doc.child("root");
		pugi::xml_node users = root.child("admins");
		if (HaveIDUser(users, uP->id))
		{
			doc.reset();
			return true;
		}
		doc.reset();
		return false;
	}

private:
	pugi::xml_document doc;

private:

	void AddToArray(const TgBot::Chat::Ptr& uP, const char* name)
	{
		doc.load_file(USER_INFO_DB_FILE);
		pugi::xml_node root = doc.child("root");
		pugi::xml_node users = root.child(name);
		if (HaveIDUser(users, uP->id))
		{
			doc.reset();
			return;
		}
		pugi::xml_node userInfo = users.append_child("user");
		InitUserXML(uP, userInfo);
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

	static bool HaveIDUser(const pugi::xml_node& users, const size_t id)
	{
		for (pugi::xml_node nd = users.child("user"); nd; nd = nd.next_sibling("user"))
			if (nd.attribute("id").as_llong() == id)
				return true;
		return false;
	}

	static void InitUserXML(const TgBot::Chat::Ptr& uch, pugi::xml_node& node)
	{
		node.set_name("user");
		node.append_attribute("id").set_value(std::to_string(uch->id).c_str());
		std::string info = uch->username + " " + uch->firstName + " " + uch->lastName;
		node.append_child(pugi::node_pcdata).set_value(info.c_str());
	}


};