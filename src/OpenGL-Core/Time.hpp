#ifndef TIME_HPP
#define TIME_HPP

#include <GLFW/glfw3.h>

//By default, values are in second
class Time{
public:
    Time() {
        float currentTime = (float)glfwGetTime();
        m_timeStep = 1.0f/120.0f; // Start like if it was running at 120FPS 
        m_lastTime = currentTime;
        m_averageTimeStep = 1.0f/120.0f;
        m_lastRecordTime = currentTime;
        m_numberFrame = 0;
    }

    void updateTime() {
        float currentTime = (float)glfwGetTime(); // return value in secon
        m_timeStep = currentTime - m_lastTime;
        m_lastTime = currentTime;

        m_numberFrame++;
        if ( currentTime - m_lastRecordTime >= 1.0f) {
            m_averageTimeStep = 1.0f/m_numberFrame;
            m_lastRecordTime = currentTime;
            m_numberFrame = 0;
        }
        
    }

    float getTimeStep() const {
        return m_timeStep;
    }

    float getAverageTimeStep() const {
        return m_averageTimeStep;
    }

    float getInstantFPS() const {
        return 1.0f/getTimeStep();
    }

    float getAverageFPS() const {
        return 1.0f/getAverageTimeStep();
    }

    float getTotalTime() const {
        return (float)glfwGetTime(); //Timer measures time elapsed since GLFW was initialized, unless the timer has been set using glfwSetTime.
    }

private:
    float m_timeStep;
    float m_lastTime;

    float m_averageTimeStep;
    float m_lastRecordTime;
    int m_numberFrame;
};

#endif