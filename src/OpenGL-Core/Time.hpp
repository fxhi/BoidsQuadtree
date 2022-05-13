#ifndef TIME_HPP
#define TIME_HPP

//By default, values are in second

class Time{
public:
    Time();
    void updateTime();
    void displayFPS();
    float getTimeStep() const;
    float getTimeStepFrame() const;
    float getAverageTimeStep() const;
    float getInstantFPS() const;
    float getAverageFPS() const;
    float getTotalTime() const;

private:
    float m_timeStepFrame; // Time step between two frames

    float m_maxTimeStep = 1.0f/10.0f; // Like if the simulation runs at 10FPS
    float m_timeStep; // Time step of the simulation ( can be different of m_timeStepFrame)

    float m_lastTime;

    float m_averageTimeStep;
    float m_lastRecordTime;
    int m_numberFrame;
};

#endif