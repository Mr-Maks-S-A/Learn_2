// CamelFast.h
#pragma once
#include <Transport/Ground.hpp>

class RACING_API CamelFast : public GroundTransport {
public:
    CamelFast() : GroundTransport("Верблюд-быстроход", 40, 10) {}
    
    double getRestDuration(int restCount) const override {
        if (restCount == 1) return 5.0;
        if (restCount == 2) return 6.5;
        return 8.0;
    }
};