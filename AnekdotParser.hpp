#pragma once

#include<cstring>
#include<string>
#include<vector>
#include<filesystem>
#include<fstream>

#include<pugixml.hpp>
#include<curl/curl.h>

#include "Randomiser.hpp"

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
void replaceAll(std::string& str, const char oldS, const char newS)
{
    for (auto& i : str)
        if (i == oldS)
            i = newS;
}

size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* buffer)
{
    buffer->append((char*)contents, size * nmemb);
    return size * nmemb;
}

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

class AnekdotURLCreator
{
public:
    AnekdotURLCreator()
    {
        doc.load_file("PagesData.xml");
        root = doc.child("pageData");
        count = root.attribute("count").as_uint();
    }

    const std::string GenerateRandomURL()
    {
        pugi::xml_node resNode;
        std::string res = root.attribute("mainURL").as_string();

        InitChildAt(resNode, root, "page", rnd.generate(0, count - 1));

        res += std::string("/") + resNode.text().as_string() + "/";
        const unsigned int rndNum = rnd.generate(0, resNode.attribute("sections").as_uint() - 1);
        if (rndNum == 0)
            return res;
        res += std::to_string(rndNum + 1) + "/";
        return res;
    }

    const std::string GenerateAtKeyWord(const std::string key, const bool elseRandom = false)
    {
        std::string res = root.attribute("mainURL").as_string();
        for (pugi::xml_node i = root.child("page"); i; i = i.next_sibling("page"))
        {
            if (key.find(i.attribute("key").as_string()) != std::string::npos)
            {
                res += std::string("/") + i.text().as_string() + "/";
                const unsigned int rndNum = rnd.generate(0, i.attribute("sections").as_uint() - 1);
                if (rndNum == 0)
                    return res;
                res += std::to_string(rndNum + 1) + "/";
                return res;
            }
        }
        if (elseRandom)
            return GenerateRandomURL();
        return res;
    }

private:
    pugi::xml_document doc;
    pugi::xml_node root;

    unsigned int count = 0;

    Randomiser<unsigned int> rnd;


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

};

namespace fs = std::filesystem;
class WebParserHTML
{
public:

    WebParserHTML()
    {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (!fs::is_directory("downloaded_pages"))
            fs::create_directory("downloaded_pages");
    }
    ~WebParserHTML()
    {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }


    bool Parse(const char* fName)
    {
        buffer.clear();
        std::string myfName = fName;
        replaceAll(myfName, '/', '_');
        replaceAll(myfName, ':', '_');
        myfName = "downloaded_pages\\" + myfName + ".html";

        if (std::ifstream in(myfName); in.good())
        {
            std::string pr;
            while (std::getline(in, pr))
            {
                buffer += pr;
                buffer.push_back('\n');
            }
            buffer.pop_back();
            in.close();
            return true;
        }
        curl_easy_setopt(curl, CURLOPT_URL, fName);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            return false;

        std::ofstream out(myfName);
        out << buffer;
        out.close();

        return true;
    }

    const std::string& getBuffer() const
    {
        return buffer;
    }


private:
    CURL* curl;
    CURLcode res{};
    std::string buffer;

};

class ParseAnekdots
{
public:

    bool Parse(const std::string& buffer, const bool deleteNotSensure = true)
    {
        std::string xmlBuf;
        if (!InitXmlString(buffer, xmlBuf))
            return false;

        if (!InitArray(xmlBuf, deleteNotSensure))
            return false;

        return false;
    }

    const std::vector<std::string>& GetAnekdots() const
    {
        return anekdots;
    }

private:
    std::vector<std::string> anekdots;



    bool InitArray(const std::string& str, const bool deleteNoCensure)
    {
        pugi::xml_document doc;
        if (pugi::xml_parse_result res = doc.load_string(str.c_str()); !res)
            return false;
        pugi::xml_node root = doc.child("ul");

        anekdots.clear();

        std::string anek;
        for (const auto& i : root.children())
        {
            anek = extractText(i.child("div").child("div").child("p"));
            if (!(deleteNoCensure && (anek.find('@') != std::string::npos || anek.find('*') != std::string::npos)))
                anekdots.push_back(anek);
        }

        return true;
    }

    bool InitXmlString(const std::string& buffer, std::string& xmlBuf)
    {
        size_t ind1 = buffer.find("<ul class=\"item-list\">");
        if (ind1 == std::string::npos)
            return false;
        size_t ind2 = buffer.find("<div class=\"pagination-holder g-clearfix\">", ind1);
        if (ind2 == std::string::npos)
            return false;

        xmlBuf = buffer.substr(ind1, ind2 - ind1);
        replaceAll(xmlBuf, "<br>", "\n");
        return true;
    }

};

class AnedotGenerator
{
public:
    void GenerateAtKeyWord(const std::string &key, std::string &res)
    {
        std::string url = creator.GenerateAtKeyWord(key, true);

        wparser.Parse(url.c_str());
        aparser.Parse(wparser.getBuffer());

        Randomiser<unsigned int> rnd;

        res = aparser.GetAnekdots()[rnd.generate(0, aparser.GetAnekdots().size() - 1)];
    }

private:
    AnekdotURLCreator creator;
    WebParserHTML wparser;
    ParseAnekdots aparser;
};
