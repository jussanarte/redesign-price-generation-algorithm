#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct {
    float distanceInKM;
    float timeInMinutes;
    float demand;
    float offer;
    float geographicZoneFactor;
    float peakHourFactor;
    float eventOrHolidayFactor;
    float weatherFactor;
    float trafficFactor;
    float fuelPrice;
} RideData;

float getZoneFactor(int zoneId);
float getDistance(int originId, int destinationId);
RideData readFile();
double getDynamicFactor(RideData data);
double getFinalPrice(RideData data);

#endif
