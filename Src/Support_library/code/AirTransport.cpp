#include <Transport/AirTransport.hpp>


double AirTransport::calculateTime(double distance) const {
    double factor = getDistanceReductionFactor(distance); // Например, 0.05 для 5%
    double reducedDistance = distance * (1.0 - factor);
    return reducedDistance / speed;
}