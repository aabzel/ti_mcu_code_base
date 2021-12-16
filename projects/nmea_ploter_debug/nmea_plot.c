
#include "nmea_plot.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "nmea_protocol.h"


bool plot_nmea(char *file_name){
	printf ("\n%s()\n", __FUNCTION__);
    char curFileStr[500];
    bool res= false;
	FILE *inFilePrt = NULL;
	FILE *outFile = NULL;
	uint16_t len = 0;
    printf ("\n>In file: [%s]", file_name);
	outFile =fopen ("gnss_out.txt", "w");
	inFilePrt = fopen (file_name, "r");
	if (inFilePrt) {
		int line = 0;
        while (NULL != fgets (curFileStr, sizeof (curFileStr), inFilePrt)) {
#ifdef HAS_NMEA_LOG_DIAG
			printf ("\n>[%s] len %u", curFileStr, strlen(curFileStr));
#endif
            len = strlen(curFileStr)-4-1;
			res = nmea_parse(curFileStr,len, &NmeaData);
			if(res){
				NmeaData.rmc.coordinate_dd= encode_gnss_coordinates(NmeaData.rmc.coordinate_ddmm);
				NmeaData.rmc.coordinate_dd = gnss_encode_deg2mm(NmeaData.rmc.coordinate_dd);
				if(outFile){
    				fprintf(outFile,"%f %f\n", NmeaData.rmc.coordinate_dd.longitude, NmeaData.rmc.coordinate_dd.latitude);
				}
			}else{
				printf ("\n[e] Unable to proc [%s]", curFileStr);
			}
			line++;
		}
        fclose(inFilePrt);
		if(outFile){
    		fclose(outFile);
		}
        res = true;
	}else{
		printf ("\n[e] Unable to open file [%s]", file_name);
	}
    return res;
}