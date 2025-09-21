#pragma once

#include "AnekdotURLCreator.hpp"

#include "../StringUtility.hpp"
#include "HTTPClient.hpp"

class AnekdotGenerator
{
public:
    AnekdotGenerator(const std::string& file = {}) :
        creator_(file)
    {}

    inline bool isInlineAnekRequest(const std::string& str) const
    {
        return str.find("анекдот") != std::string::npos;
    }

    std::vector<std::string> parseAnekdotsFromUrl(const std::string &url, const bool deleteSwearing = false)
    {
        std::string buffer = http_.getRequest(url);
        if (buffer.empty())
            return {};
        if (!forbidExcess(buffer))
            return {};

        std::vector<std::string> res;
        if (!initAnekdots(buffer, res, deleteSwearing))
            return {};
        return res;
    }

    std::string parseAnekdotAtKeyword(const std::string keyWords, const bool deleteSwearing = false)
    {
        std::string url = creator_.generateAtKeyWord(keyWords, true);
        std::vector<std::string> anekdots = parseAnekdotsFromUrl(url, deleteSwearing);
        return rnd_.randOf<std::string>(anekdots);
    }

private:

    HttpClient http_;
    rnd::Random<unsigned int> rnd_;
    AnekdotURLCreator creator_;

private:
    bool forbidExcess(std::string &str) const
    {
        size_t ind1 = str.find(R"(<ul class="item-list">)");
        if (ind1 == std::string::npos)
            return false;
        size_t ind2 = str.find(R"(<div class="pagination-holder g-clearfix">)", ind1);
        if (ind2 == std::string::npos)
            return false;

        str = str.substr(ind1, ind2 - ind1);
        StringUtility::replaceAll(str, "<br>", '\n');
        return true;
    }

    bool initAnekdots(const std::string &buffer, std::vector<std::string> &res, const bool deleteSwearing = false) const
    {
        pugi::xml_document doc;
        if (!doc.load_string(buffer.c_str()))
            return false;
        pugi::xml_node root = doc.child("ul");
        if (!root)
            return false;

        for (const auto &i : root.children())
        {
            std::string anek = extractText(i.child("div").child("div").child("p"));
            // Если надо удалить цензуру и в слове она есть - не добавляем этот анекдот
            if (!(deleteSwearing && (anek.find('@') != std::string::npos || anek.find('*') != std::string::npos)))
                res.push_back(anek);
        }
        return true;
    }

    std::string extractText(const pugi::xml_node &node) const
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
};