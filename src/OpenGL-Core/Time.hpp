#ifndef TIME_HPP
#define TIME_HPP

//By default, values are in second

class Time{
public:
    Time();
    void updateTime();
    void displayFPS();
    float getTimeStep() const;
    float getAverageTimeStep() const;
    float getInstantFPS() const;
    float getAverageFPS() const;
    float getTotalTime() const;

private:
    float m_timeStep;
    float m_lastTime;

    float m_averageTimeStep;
    float m_lastRecordTime;
    int m_numberFrame;
};

#endif