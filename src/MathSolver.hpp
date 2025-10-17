#pragma once

#include <span>
#include <string>
#include <utility>
#include <vector>


#include <MathWorker/MathWorker.hpp>

#include <MathWorker/StringConvert/ComplexStringConverter.hpp>
#include "StringUtility.hpp"

class MathSolver
{
  public:
    inline bool isInlineMathQuestion(const std::string &str) const
    {
        return str.empty() ? false : (str.back() == '=');
    }

    std::pair<std::string, bool> getMathInlineResponseResult(const std::string &str)
    {
        std::vector<std::string_view> params = StringUtility::split(str, '\n');
        if (params.empty() || params.back().size() < 2)
            return std::make_pair<std::string, bool>("Empty input.", false);
        params.back().remove_suffix(1);
        try
        {
            return std::make_pair<std::string, bool>(str + clearProcessMath(params.back(), std::span<std::string_view>(params.begin(), params.size() - 1)), true);
        }
        catch (const mathWorker::ParseException &e)
        {
            return std::make_pair<std::string, bool>(std::string("Math error: ") + e.what(), false);
        }
        catch (const std::exception &e)
        {
            return std::make_pair<std::string, bool>(std::string("Bot error: ") + e.what(), false);
        }
        catch (...)
        {
            return std::make_pair<std::string, bool>("Unknown error.", false);
        }
    }
    std::string getMathResponseResult(const std::string &str)
    {
        std::vector<std::string_view> params = StringUtility::split(str, '\n');
        mathWorker::ComplexStringConverter converter;
        fillConverterParams(converter, params);

        try
        {
            return str + '=' + clearProcessMath(params.back(), std::span<std::string_view>(params.begin(), params.size() - 1), converter);
        }
        catch (const mathWorker::ParseException &e)
        {
            return std::string("Math error: ") + e.what();
        }
        catch (const std::exception &e)
        {
            return std::string("Bot error: ") + e.what();
        }
        catch (...)
        {
            return "Unknown error.";
        }
    }

  private:
    std::string clearProcessMath(const std::string_view expression, const std::span<std::string_view> additional) noexcept(false)
    {
        mathWorker::Signature signature = mathWorker::generator::mathSignature();
        mathWorker::BaseTokenizer tokenizer{signature};
        mathWorker::MathParser parser(tokenizer);
        mathWorker::DirectSignatureConnector connector(signature);

        for (const auto &i : additional)
            connector.addTerm(std::string(i));

        std::string res = parser.parse(expression)->replace(signature.getVariableContext())->calculate(signature)->toString();
        return res;
    }
    std::string clearProcessMath(const std::string_view expression, const std::span<std::string_view> additional, mathWorker::ComplexStringConverter& converter) noexcept(false)
    {
        mathWorker::Signature signature = mathWorker::generator::mathSignature();
        mathWorker::BaseTokenizer tokenizer{signature};
        mathWorker::MathParser parser(tokenizer);
        mathWorker::DirectSignatureConnector connector(signature);

        for (const auto &i : additional)
            connector.addTerm(std::string(i));

        std::string res = parser.parse(expression)->replace(signature.getVariableContext())->calculate(signature)->toString(converter);
        return res;
    }

    void fillConverterParams(mathWorker::ComplexStringConverter &converter, std::vector<std::string_view> &params)
    {
        bool needToDell = false;
        for (std::size_t i = 0; i < params.size(); ++i)
        {
            std::string_view &param = params[i];
            if (param.starts_with("_s="))
                converter.precession = safetyFromString(param.substr(3), converter.precession), needToDell = true;
            else if (param.starts_with("_t="))
                converter.complexType = safetyFromString(param.substr(3), converter.complexType), needToDell = true;
            else if (param.starts_with("_a="))
                converter.complexType = safetyFromString(param.substr(3), converter.complexType), needToDell = true;
            
            if(needToDell)
            {
                params.erase(params.begin() + i--);
                needToDell = false;
            }
        }
    }

    template <typename T>
    T safetyFromString(const std::string_view s, const T elseV = 0)
    {
        int res = std::atoi(s.data());
        if (res == 0 && (s.size() == 0 || s[0] != '0'))
            return elseV;
        return static_cast<T>(res);
    }
};