// AirTransport.h
#pragma once
#include <Transport/Transport.h>

class RACING_API AirTransport : public Transport {
public:
    AirTransport(std::string name, double speed);
    double calculateTime(double distance) const override;
    
    // Каждое воздушное ТС само считает свой коэффициент на основе начальной дистанции
    virtual double getDistanceReductionFactor(double distance) const = 0;
};