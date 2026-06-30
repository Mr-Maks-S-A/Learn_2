// Ground.h
#pragma once
#include <Transport/Transport.h>

class RACING_API GroundTransport : public Transport {
protected:
    double moveTimeBeforeRest;

public:
    GroundTransport(std::string name, double speed, double moveTimeBeforeRest);
    
    double calculateTime(double distance) const override;
    
    // Виртуальный метод для определения длительности конкретной остановки
    virtual double getRestDuration(int restCount) const = 0;
};