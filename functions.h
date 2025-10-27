#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

typedef struct rideData RideData;

float getZoneFactor(int zoneId);
RideData readFile();
double getDynamicFactor(RideData data);
double getFinalPrice(RideData data);

#endif
