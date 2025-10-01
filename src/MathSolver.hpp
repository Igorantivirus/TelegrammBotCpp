#pragma once

#include <utility>
#include <vector>
#include <string>
#include <span>
#include <iostream>

#include <MathWorker/MathWorker.hpp>

#include "StringUtility.hpp"

class MathSolver
{
public:

    inline bool isInlineMathQuestion(const std::string& str) const
    {
        return str.empty() ? false : (str.back() == '=');
    }

    std::pair<std::string, bool> getMathInlineResponseResult(const std::string& str)
    {
        std::vector<std::string_view> params = StringUtility::split(str, '\n');
        if(params.empty() || params.back().size() < 2)
            return std::make_pair<std::string, bool>("Empty input.", false);
        params.back().remove_suffix(1);
        try
        {
            return std::make_pair<std::string, bool>(str + clearProcessMath(params.back(), std::span<std::string_view>(params.begin(), params.size() - 1)), true);
        }
        catch(const mathWorker::ParseException& e)
        {
            return std::make_pair<std::string, bool>(std::string("Math error: ") + e.what(), false);
        }
        catch(const std::exception& e)
        {
            return std::make_pair<std::string, bool>(std::string("Bot error: ") + e.what(), false);
        }
        catch(...)
        {
            return std::make_pair<std::string, bool>("Unknown error.", false);
        }
    }

private:

    std::string clearProcessMath(const std::string_view expression, const std::span<std::string_view> additional) noexcept(false)
    {
        mathWorker::Signature signature = mathWorker::generator::mathSignature();
        mathWorker::BaseTokenizer tokenizer{signature};
        mathWorker::MathParser parser(tokenizer);
        mathWorker::DirectSignatureConnector connector(signature);

        for(const auto& i : additional)
            connector.addTerm(std::string(i));

        std::string res = parser.parse(expression)->replace(signature.getVariableContext())->calculate(signature)->toString();
        return res;
    }


};