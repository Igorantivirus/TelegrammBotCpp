#pragma once

#include <memory>

#include "Logger.hpp"

class Services
{
public:
    Services(const Services& other) = delete;
    Services(Services&& other) = delete;

    static Services& instance()
    {
        static Services locator;
        return locator;
    }

    std::shared_ptr<Logger> log;

private:

    Services() = default;

};