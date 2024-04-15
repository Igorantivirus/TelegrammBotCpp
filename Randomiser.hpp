#pragma once

#include<random>

template<typename ResType>
class Randomiser
{
public:
    Randomiser() : rng(dev()) {}
    Randomiser(const unsigned long long seed) : rng(seed) {}

    ResType generate(const ResType minValue, const ResType maxValue)
    {
        dist.param(typename decltype(dist)::param_type(minValue, maxValue));
        return dist(rng);
    }
    ResType generate()
    {
        dist.param(typename decltype(dist)::param_type(MinValue(), MaxValue()));
        return dist(rng);
    }


    void setSeed(unsigned long long seed)
    {
        rng.seed(seed);
    }
   
    constexpr static ResType MaxValue()
    {
        return (std::numeric_limits<ResType>::max)();
    }
    constexpr static ResType MinValue()
    {
        return (std::numeric_limits<ResType>::min)();
    }

private:
    std::random_device dev;
    std::mt19937_64 rng;
    std::uniform_int_distribution<ResType> dist;
};

template<>
class Randomiser<char>
{
public:
    Randomiser() : rng(dev()) {}
    Randomiser(const unsigned long long seed) : rng(seed) {}

    char generate(const char minValue, const char maxValue)
    {
        dist.param(typename decltype(dist)::param_type(minValue, maxValue));
        static_cast<char>(dist(rng));
    }
    char generate()
    {
        dist.param(typename decltype(dist)::param_type(MinValue(), MaxValue()));
        static_cast<char>(dist(rng));
    }


    void setSeed(unsigned long long seed)
    {
        rng.seed(seed);
    }

    constexpr static char MaxValue()
    {
        return (std::numeric_limits<char>::max)();
    }
    constexpr static char MinValue()
    {
        return (std::numeric_limits<char>::min)();
    }

private:
    std::random_device dev;
    std::mt19937_64 rng;
    std::uniform_int_distribution<short> dist;
};

template<>
class Randomiser<unsigned char>
{
public:
    Randomiser() : rng(dev()) {}
    Randomiser(const unsigned long long seed) : rng(seed) {}

    unsigned char generate(const unsigned char minValue, const unsigned char maxValue)
    {
        dist.param(typename decltype(dist)::param_type(minValue, maxValue));
        return static_cast<unsigned char>(dist(rng));
    }
    unsigned char generate()
    {
        dist.param(typename decltype(dist)::param_type(MinValue(), MaxValue()));
        static_cast<unsigned char>(dist(rng));
    }


    void setSeed(unsigned long long seed)
    {
        rng.seed(seed);
    }

    constexpr static unsigned char MaxValue()
    {
        return (std::numeric_limits<unsigned char>::max)();
    }
    constexpr static unsigned char MinValue()
    {
        return (std::numeric_limits<unsigned char>::min)();
    }

private:
    std::random_device dev;
    std::mt19937_64 rng;
    std::uniform_int_distribution<short> dist;
};

template<>
class Randomiser<bool>
{
public:
    Randomiser() : rng(dev()), dist(0, 1) {}
    Randomiser(const unsigned long long seed) : rng(seed), dist(0, 1) {}

    bool generate()
    {
        static_cast<bool>(dist(rng));
    }

    void setSeed(unsigned long long seed)
    {
        rng.seed(seed);
    }

    constexpr static bool MaxValue()
    {
        return true;
    }
    constexpr static bool MinValue()
    {
        return false;
    }

private:
    std::random_device dev;
    std::mt19937_64 rng;
    std::uniform_int_distribution<short> dist;
};

template<>
class Randomiser<float>
{
public:
    Randomiser() : rng(dev()) {}
    Randomiser(const unsigned long long seed) : rng(seed) {}

    float generate(const float minValue, const float maxValue)
    {
        dist.param(typename decltype(dist)::param_type(minValue, maxValue));
        return static_cast<float>(dist(rng));
    }
    float generate()
    {
        dist.param(typename decltype(dist)::param_type(MinValue(), MaxValue()));
        return static_cast<float>(dist(rng));
    }


    void setSeed(unsigned long long seed)
    {
        rng.seed(seed);
    }

    constexpr static float MaxValue()
    {
        return (std::numeric_limits<float>::max)();
    }
    constexpr static float MinValue()
    {
        return (std::numeric_limits<float>::min)();
    }

private:
    std::random_device dev;
    std::mt19937_64 rng;
    std::uniform_real_distribution<float> dist;
};

template<>
class Randomiser<double>
{
public:
    Randomiser() : rng(dev()) {}
    Randomiser(const unsigned long long seed) : rng(seed) {}

    double generate(const double minValue, const double maxValue)
    {
        dist.param(typename decltype(dist)::param_type(minValue, maxValue));
        return static_cast<double>(dist(rng));
    }
    double generate()
    {
        dist.param(typename decltype(dist)::param_type(MinValue(), MaxValue()));
        return static_cast<double>(dist(rng));
    }


    void setSeed(unsigned long long seed)
    {
        rng.seed(seed);
    }

    constexpr static double MaxValue()
    {
        return (std::numeric_limits<double>::max)();
    }
    constexpr static double MinValue()
    {
        return (std::numeric_limits<double>::min)();
    }

private:
    std::random_device dev;
    std::mt19937_64 rng;
    std::uniform_real_distribution<double> dist;
};

template<>
class Randomiser<long double>
{
public:
    Randomiser() : rng(dev()) {}
    Randomiser(const unsigned long long seed) : rng(seed) {}

    long double generate(const long double minValue, const long double maxValue)
    {
        dist.param(typename decltype(dist)::param_type(minValue, maxValue));
        return static_cast<long double>(dist(rng));
    }
    long double generate()
    {
        dist.param(typename decltype(dist)::param_type(MinValue(), MaxValue()));
        return static_cast<long double>(dist(rng));
    }


    void setSeed(unsigned long long seed)
    {
        rng.seed(seed);
    }

    constexpr static long double MaxValue()
    {
        return (std::numeric_limits<long double>::max)();
    }
    constexpr static long double MinValue()
    {
        return (std::numeric_limits<long double>::min)();
    }

private:
    std::random_device dev;
    std::mt19937_64 rng;
    std::uniform_real_distribution<long double> dist;
};