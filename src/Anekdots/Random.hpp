#pragma once

#include <random>
#include <type_traits>
#include <limits>
#include <span>
#include <ranges>

namespace rnd
{
    template<typename T>
    constexpr bool use_short_distribution_v = std::is_same_v<T, bool> || std::is_same_v<T, char> || std::is_same_v<T, unsigned char>;

    template<typename ResType, typename EngineType = std::mt19937_64>
    class Random
    {
    private:

        using dist_type = std::conditional_t<
            std::is_floating_point_v<ResType>,
            std::uniform_real_distribution<ResType>,//Если тип floating - uniform_real_distribution
            std::conditional_t<//иначе
            use_short_distribution_v<ResType>,
            std::uniform_int_distribution<short>,//если тип bool||char - std::uniform_int_distribution<short>
            std::uniform_int_distribution<ResType>//в стандартном случае
            >
        >;

    public:
        Random() : rng(dev()) {}
        explicit Random(const unsigned long long seed) : rng(seed) {}

        ResType generate(const ResType minVal, const ResType maxVal)
        {
            dist_type dist(minVal, maxVal);
            return dist(rng);
        }
        ResType generate()
        {
            dist_type dist(minValue(), maxValue());
            return dist(rng);
        }

        ResType operator()(const ResType minVal, const ResType maxVal)
        {
            return generate(minVal, maxVal);
        }
        ResType operator()()
        {
            return generate();
        }

        template<typename T>
        T& randOf(std::span<T> s)
        {
            if (s.empty())
                throw std::out_of_range("randOf: empty span");
            std::uniform_int_distribution<std::size_t> d(0, s.size() - 1);
            return s[d(rng)];
        }

        template<typename T>
        const T& randOf(std::span<const T> s) const
        {
            if (s.empty())
                throw std::out_of_range("randOf: empty span");
            std::uniform_int_distribution<std::size_t> d(0, s.size() - 1);
            return s[d(const_cast<EngineType&>(rng))];
        }

        template<typename T>
        void shuffle(std::span<T> s)
        {
            std::ranges::shuffle(s, rng);
        }


        void setSeed(unsigned long long seed)
        {
            rng.seed(seed);
        }

        std::random_device& getDevice()
        {
            return dev;
        }
        EngineType& getEngine()
        {
            return rng;
        }

        constexpr static ResType maxValue()
        {
            return (std::numeric_limits<ResType>::max)();
        }
        constexpr static ResType minValue()
        {
            return (std::numeric_limits<ResType>::min)();
        }

    private:

        std::random_device dev;
        EngineType rng;

    };
}