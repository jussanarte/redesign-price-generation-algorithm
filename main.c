#include <stdio.h>
#include <stdlib.h>
#include "functions.c"

int main() {

	RideData data = readFile();
	printf("Distance: %.2f km\n", data.distanceInKM);
	printf("Time: %.2f min\n", data.timeInMinutes);
	printf("Demand: %.2f\n", data.demand);
	printf("Fuel price: %.2f AOA\n", data.fuelPrice);
	printf("\nTotal price: %.2f AOA\n", getFinalPrice(data));
   
    return 0;
}
