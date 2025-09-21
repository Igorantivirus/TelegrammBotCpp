#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include <pugixml.hpp>

#include "Random.hpp"

class AnekdotURLCreator
{
public:
    AnekdotURLCreator() = default;
    AnekdotURLCreator(const std::string file)
    {
        initLinks(file);
    }

    void initLinks(const std::string& fileName)
    {
        pugi::xml_document root;
        if(!root.load_file(fileName.c_str()))
            return;

        pugi::xml_node pageData = root.child("pageData");
        mainURL_ = pageData.attribute("mainURL").as_string();
        count_ = pageData.attribute("count").as_int();

        for(const auto& i : pageData.children())
        {
            LinkInfo info;
            info.sections = i.attribute("sections").as_int();
            info.key = i.attribute("key").as_string();
            info.field = i.attribute("field").as_string();
            links_.push_back(std::move(info));
        }
    }

    const std::string generateRandomURL()
    {
        unsigned randInd = rnd.generate(0, count_ - 1);
        const LinkInfo& info = links_[randInd];
        unsigned randSecsion = rnd.generate(1, info.sections);
        return mainURL_ + '/' + info.field + '/' + std::to_string(randSecsion) + '/';
    }

    const std::string generateAtKeyWord(const std::string word, const bool elseRandom = false)
    {
        const LinkInfo* linkO = getFieldAtKeyword(word);
        if(!linkO)
            return elseRandom ? generateRandomURL() : "";
        const LinkInfo& info = *linkO;
        unsigned randSecsion = rnd.generate(1, info.sections);
        return mainURL_ + '/' + info.field + '/' + std::to_string(randSecsion) + '/';
    }

private:

    struct LinkInfo
    {
        std::uint8_t sections;
        std::string key;
        std::string field;
    };

private:

    std::vector<LinkInfo> links_;
    std::string mainURL_;
    std::uint8_t count_ = 0;

    rnd::Random<unsigned int> rnd;

private:

    const LinkInfo* getFieldAtKeyword(const std::string& word) const
    {
        for(const auto& link : links_)
            if(word.find(link.key) != std::string::npos)
                return &link;
        return nullptr;   
    }
};