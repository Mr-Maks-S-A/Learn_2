// Broom.h
#pragma once
#include "AirTransport.h"

class RACING_API Broom : public AirTransport {
public:
    Broom() : AirTransport("Метла", 20) {}

    double getDistanceReductionFactor(double distance) const override {
        int thousands = static_cast<int>(distance) / 1000;
        double factor = thousands * 0.01; // +1% за каждые 1000 у.е.
        return factor;
    }
};