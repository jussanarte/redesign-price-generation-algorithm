#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "functions.h"

struct rideData{
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
};


float getZoneFactor(int zoneId) {
    FILE *f = fopen("Arquivos/zonas.txt", "r");
    if (f == NULL) {
        fprintf(stderr, "Erro ao abrir 'zonas.txt'. Retornando fator padrao...\n");
        return 1.0f; 
    }

    int code;
    float factor;
    char zoneName[50];
    
    // Loop para ler linha por linha do arquivo de zonas
    // %d (código), %f (fator), %49[^\n] (lê o nome da zona até o final da linha)
    while (fscanf(f, "%d %f %49[^\n]", &code, &factor, zoneName) == 3) {
        if (code == zoneId) {
            fclose(f);
            return factor;
        }
    }

    fclose(f);
    return 1.0f; 
}


RideData readFile() {
    RideData data;
    FILE *f = fopen("Arquivos/data.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    fscanf(f, "distanceInKM = %f\n", &data.distanceInKM);
    fscanf(f, "timeInMinutes = %f\n", &data.timeInMinutes);
    fscanf(f, "demand = %f\n", &data.demand);
    fscanf(f, "offer = %f\n", &data.offer);

    int zoneId;
    fscanf(f, "geographicZoneID = %d\n", &zoneId); 
    data.geographicZoneFactor = getZoneFactor(zoneId); 

    fscanf(f, "peakHourFactor = %f\n", &data.peakHourFactor);
    fscanf(f, "eventOrHolidayFactor = %f\n", &data.eventOrHolidayFactor);
    fscanf(f, "weatherFactor = %f\n", &data.weatherFactor);
    fscanf(f, "trafficFactor = %f\n", &data.trafficFactor);
    fscanf(f, "fuelPrice = %f\n", &data.fuelPrice);

    fclose(f);
    return data;
}


double getDynamicFactor(RideData data){
    float factor = 1.0;
    
    // Demanda e Oferta
    if (data.offer <= 0) {
        factor = 3.0; //Valor maximo se não tiver oferta
    } else {
        factor = data.demand / data.offer;
    }
    
    // Horas de Ponta (ex., 7.0 para 7:00 da manha)
    if ((data.peakHourFactor >= 7.0 && data.peakHourFactor <= 9.0) || 
        (data.peakHourFactor >= 17.0 && data.peakHourFactor <= 20.0))
    {
        factor *= 1.3;
    }
    
    if (data.weatherFactor < 0.5f) {
        factor *= 1.0;
    } else if (data.weatherFactor >= 0.5f && data.weatherFactor <= 0.75f) {
        factor *= 1.2;
    } else {
        factor *= 1.5;
    }
    
    if (data.fuelPrice == 300.0f || data.fuelPrice == 400.0f) {
        factor *= 1.2;
    } else {
        factor *= 1.0; 
    }
    
    // Existencia de um evento
    if (data.eventOrHolidayFactor > 0.0f) {
        factor *= 1.5;
    }
    
    // Existencia de congestionamento
    if (data.trafficFactor > 0.0f) {
        factor *= 1.3;
    }
    factor *= data.geographicZoneFactor;
    
    if (factor > 3.0f) {
        return 3.0;
    } else {
        return factor;    
    }
}

double getFinalPrice(RideData data){
    
    const float COST_PER_KM = 100.0f;
    const float COST_PER_MINUTE = 50.0f;
    const float BASE_PRICE = 300.0f;
    const float MINIMUM_PRICE = 450.0f;
    
    float finalFactor = getDynamicFactor(data);
    
    // Calculation performed in 'double' to ensure precision for the final price
    double calculatedPrice = (double)(BASE_PRICE + data.distanceInKM * COST_PER_KM + data.timeInMinutes * COST_PER_MINUTE) * finalFactor;
    
    if (calculatedPrice < MINIMUM_PRICE) {
        return MINIMUM_PRICE;
    } else {
        return calculatedPrice;
    }
}
