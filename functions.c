#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

float getZoneFactor(int zoneId) {
    FILE *f = fopen("Arquivos/zonas.txt", "r");
    if (f == NULL) {
        fprintf(stderr, "Erro ao abrir 'zonas.txt'. Usando fator padrao...\n");
        return 1.0f;
    }

    int code;
    float factor;
    char zoneName[50];

    while (fscanf(f, "%d %f %49[^\n]", &code, &factor, zoneName) == 3) {
        if (code == zoneId) {
            fclose(f);
            return factor;
        }
    }

    fclose(f);
    return 1.0f;
}

float getDistance(int originId, int destinationId) {
    FILE *f = fopen("Arquivos/matriz.txt", "r");
    if (f == NULL) {
        fprintf(stderr, "Erro ao abrir 'matriz.txt'. Usando distancia de fallback...\n");
        return -1.0f;
    }

    int n;
    if (fscanf(f, "%d", &n) != 1 || n <= 0) {
        fclose(f);
        return -1.0f;
    }

    float distance = -1.0f;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            float value;
            if (fscanf(f, "%f", &value) != 1) {
                fclose(f);
                return -1.0f;
            }
            if (i == originId && j == destinationId) {
                distance = value;
            }
        }
    }

    fclose(f);
    return distance;
}

RideData readFile() {
    RideData data;
    FILE *f = fopen("Arquivos/data.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir 'data.txt'\n");
        exit(1);
    }

    int zoneId;
    int originZoneId;
    int destinationZoneId;

    fscanf(f, "distanceInKM = %f\n", &data.distanceInKM);
    fscanf(f, "timeInMinutes = %f\n", &data.timeInMinutes);
    fscanf(f, "demand = %f\n", &data.demand);
    fscanf(f, "offer = %f\n", &data.offer);

    fscanf(f, "geographicZoneID = %d\n", &zoneId);
    data.geographicZoneFactor = getZoneFactor(zoneId);

    fscanf(f, "originZoneID = %d\n", &originZoneId);
    fscanf(f, "destinationZoneID = %d\n", &destinationZoneId);

    float matrixDistance = getDistance(originZoneId, destinationZoneId);
    if (matrixDistance > 0.0f) {
        data.distanceInKM = matrixDistance;
    }

    fscanf(f, "peakHourFactor = %f\n", &data.peakHourFactor);
    fscanf(f, "eventOrHolidayFactor = %f\n", &data.eventOrHolidayFactor);
    fscanf(f, "weatherFactor = %f\n", &data.weatherFactor);
    fscanf(f, "trafficFactor = %f\n", &data.trafficFactor);
    fscanf(f, "fuelPrice = %f\n", &data.fuelPrice);

    fclose(f);
    return data;
}

double getDynamicFactor(RideData data) {
    float factor = 1.0;

    // Demanda e Oferta
    if (data.offer <= 0) {
        factor = 3.0; // Valor maximo se nao tiver oferta
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

    // A distancia da rota (vinda da matriz A->B) modula o surge:
    // rotas mais longas reduzem suavemente o multiplicador, de 1.0 (ate 30 km)
    // ate 0.75 (a partir de 150 km), evitando precos exorbitantes.
    float distanceTaper = 1.0f;
    if (data.distanceInKM > 30.0f) {
        distanceTaper = 1.0f - 0.25f * ((data.distanceInKM - 30.0f) / 120.0f);
        if (distanceTaper < 0.75f) {
            distanceTaper = 0.75f;
        }
    }
    factor *= distanceTaper;

    if (factor > 3.0f) {
        return 3.0;
    } else {
        return factor;
    }
}

double getFinalPrice(RideData data) {
    const float COST_PER_KM = 100.0f;
    const float COST_PER_MINUTE = 50.0f;
    const float BASE_PRICE = 300.0f;
    const float MINIMUM_PRICE = 450.0f;

    float finalFactor = getDynamicFactor(data);

    // Calculo feito em 'double' para garantir precisao no preco final
    double calculatedPrice = (double)(BASE_PRICE + data.distanceInKM * COST_PER_KM + data.timeInMinutes * COST_PER_MINUTE) * finalFactor;

    if (calculatedPrice < MINIMUM_PRICE) {
        return MINIMUM_PRICE;
    } else {
        return calculatedPrice;
    }
}
