#ifndef RANDOMNUMBER_HPP
#define RANDOMNUMBER_HPP

#include <random>



inline float randomFloat()
{
    std::random_device randomDevice;
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator(randomDevice());
    return distribution(generator);
}

inline float randomFloat(float min, float max)
{
    return min + (max - min) * randomFloat();
}

// Return random angle(float) between 0 and 2*PI
inline float randomAngle() 
{
    return randomFloat(0.0f, 2.0*M_PI);
}

inline float randomAngle(float min, float max) 
{
    return randomFloat(min, max);
}

inline int randomInt(int min, int max) // check if range if can return max ? I think
{
    return static_cast<int>(std::round(min + (max - min) * randomFloat()));
}

#endif