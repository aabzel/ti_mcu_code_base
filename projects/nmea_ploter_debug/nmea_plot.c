
#include "nmea_plot.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "nmea_protocol.h"

bool plot_nmea(char* file_name, char* out_file_name) {
    printf("\n%s()\n", __FUNCTION__);
    char OutFileMM[80] = "";
    char OutFileDeg[80] = "";
    char curFileStr[2000] = "";
    bool res = false;
    FILE* pInFile = NULL;
    FILE* pOutFileDeg = NULL;
    FILE* pOutFileM = NULL;
    uint16_t len = 0;
    printf("\n>In file: [%s]", file_name);
    printf("\n>Out file: [%s]", out_file_name);
    snprintf(OutFileDeg, sizeof(OutFileDeg), "%s_deg.txt", out_file_name);
    snprintf(OutFileMM, sizeof(OutFileMM), "%s_m.txt", out_file_name);
    pOutFileDeg = fopen(OutFileDeg, "w");
    pOutFileM = fopen(OutFileMM, "w");
    pInFile = fopen(file_name, "r");
    if(pInFile && pOutFileM && pOutFileDeg) {
        int line = 0;
        while(NULL != fgets(curFileStr, sizeof(curFileStr), pInFile)) {
#ifdef HAS_NMEA_LOG_DIAG
            printf("\n>[%s] len %u", curFileStr, strlen(curFileStr));
#endif
            len = strlen(curFileStr) - 4 - 1;
            res = nmea_parse(curFileStr, len, &NmeaData);
            if(res) {
                res = is_valid_gnss_coordinates(NmeaData.coordinate_dd);
                if(res) {
                    fprintf(pOutFileDeg, "%f %f\n", NmeaData.coordinate_dd.longitude, NmeaData.coordinate_dd.latitude);
                    NmeaData.coordinate_dd = gnss_encode_deg2mm(NmeaData.coordinate_dd);

                    fprintf(pOutFileM, "%f %f\n", NmeaData.coordinate_dd.longitude, NmeaData.coordinate_dd.latitude);
                    line++;
                } else {
                    printf("\n[e] Infalid GNSS %f %f\n", NmeaData.coordinate_dd.longitude,
                           NmeaData.coordinate_dd.latitude);
                }
            } else {
                printf("\n[e] Unable to proc [%s] len %u", curFileStr, strlen(curFileStr));
            }
        }
        fclose(pInFile);
        fclose(pOutFileM);
        fclose(pOutFileDeg);
        res = true;
    } else {
        printf("\n[e] Unable to open file [%s]", file_name);
    }
    return res;
}
