#pragma once

#include <utility>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>

enum class LogLevel : unsigned char
{
    None = 0,
    Fatal,
    Debug,
    Error,
    Info,
    Warning
};

class Logger
{
public:
    Logger() = default;
    Logger(const std::string &fileName, const bool writeToConsole = false) : writeToConsole_{writeToConsole}
    {
        openFile(fileName);
    }
    ~Logger()
    {
        closeFile();
    }

    bool isEntryToConsole() const
    {
        return writeToConsole_;
    }
    void setEntryToConsole(const bool writeToConsole)
    {
        writeToConsole_ = writeToConsole;
    }

    void closeFile()
    {
        log("---  Log session ended  ---", LogLevel::Info);
        logFile_.close();
    }
    bool openFile(const std::string &fileName)
    {
        if (isOpen())
            closeFile();
        logFile_.open(fileName, std::ios::app);
            
        log("--- Log session started ---", LogLevel::Info);
        
        if(!logFile_.is_open())
           log("Couldn't open the logs file: " + fileName, LogLevel::Warning);
        
        return logFile_.is_open();
    }
    bool isOpen() const
    {
        return logFile_.is_open();
    }

    void log(const std::string& message, LogLevel level)
    {
        std::string time = getCurentTime();

        const auto [strName, color] = toString(level);

        std::string resultMessage = time + ' ' + strName + ' ' + message + '\n';

        if (logFile_.is_open())
        {
            logFile_ << resultMessage;
            logFile_.flush();
        }
        if (writeToConsole_)
        {
            setColor(color);
            std::cerr << resultMessage;
            setColor(Color::Default);
        }
    }

private:
    std::ofstream logFile_;
    bool writeToConsole_ = false;

private: // static

    enum class Color : unsigned char
    {
        Black = 30, // DEBUG
        Red,        // ERROR
        Green,      // INFO
        Yellow,     // WARNING
        Blue,
        Purple,     // Fatal
        Cyan,
        White,
        Default = 39 // DEBUG
    };

    static constexpr const std::pair<const char*, Color> converter_[6] =
    {
        {"[ UNKNOWN ]", Color::Default},
        {"[  FATAL  ]", Color::Purple },
        {"[  DEBUG  ]", Color::Default},
        {"[  ERROR  ]", Color::Red    },
        {"[   INF   ]", Color::Green  },
        {"[ WARNING ]", Color::Yellow }
    };

    static void setColor(const Color c)
    {
        std::cerr << '\x1b' << '[' << static_cast<int>(c) << 'm';
    }

    static std::pair<std::string, Color> toString(const LogLevel level)
    {
        return converter_[static_cast<unsigned char>(level)];
    }

    static std::string getCurentTime()
    {
        using namespace std::chrono;

        auto now = system_clock::now();
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        // Преобразуем в time_t для strftime/put_time
        std::time_t t = system_clock::to_time_t(now);
        std::tm tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << "("
            << std::put_time(&tm, "%d-%m-%Y %X")  // формат даты и времени
            << ":" << std::setw(3) << std::setfill('0') << ms.count()  // миллисекунды
            << ")";
        return oss.str();
    }

    // static std::string getCurentTime()
    // {
    //     auto now = std::chrono::system_clock::now();
    //     auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    //     return std::format("({:%d-%m-%Y %X}:{:03d})", now, ms.count());
    // }
};
