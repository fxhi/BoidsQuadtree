#include "Time.hpp"

#include <GLFW/glfw3.h>

#include <iomanip>
#include <sstream>

#include <iostream>

// Public
// ------

Time::Time() {
    float currentTime = (float)glfwGetTime();
    m_timeStep = 1.0f/120.0f; // Start like if it was running at 120FPS 
    m_lastTime = currentTime;
    m_averageTimeStep = 1.0f/120.0f;
    m_lastRecordTime = currentTime;
    m_numberFrame = 0;
}

void Time::updateTime() {
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

void Time::displayFPS() {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << getAverageTimeStep()*1000;
    std::cout << "\r Time step (average) : " << stream.str() << " ms/frame" << " - " << getAverageFPS()  << " FPS"<< std::flush; // the last space is need for visibility, issue (on vs code at least) when flushing
}

float Time::getTimeStep() const {
    return m_timeStep;
}

float Time::getAverageTimeStep() const {
    return m_averageTimeStep;
}

float Time::getInstantFPS() const {
    return 1.0f/getTimeStep();
}

float Time::getAverageFPS() const {
    return 1.0f/getAverageTimeStep();
}

float Time::getTotalTime() const {
    return (float)glfwGetTime(); //Timer measures time elapsed since GLFW was initialized, unless the timer has been set using glfwSetTime.
}