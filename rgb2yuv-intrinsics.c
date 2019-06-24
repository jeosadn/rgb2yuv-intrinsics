#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv) {

    //----------------------//	
    //    Executing menu    //
    //----------------------//

    char *RGB = NULL;
    char *YUV = NULL;
    int authors = 0;
    int help = 0;

    int opt;

    while ((opt = getopt(argc, argv, "i:o:ha")) != -1) {
    
        switch (opt) {
	    case 'h':
                help = 1;
        	break;
       	    case 'a':
    		authors = 1;
                break;
	    case 'i':
	        RGB = optarg;
		break;
	    case 'o':
		YUV = optarg;
		break;
       	    case '?':
   	    	printf("unknown option: %c\n", optopt);
   	    	break;
        }
    }

    printf("         _____   _____ ____     _         __     ___    ___      __ \n ");
    printf("	|  __ \\ / ____|  _ \\   | |        \\ \\   / / |  | \\ \\    / / \n ");
    printf("	| |__) | |  __| |_) |  | |_ ___    \\ \\_/ /| |  | |\\ \\  / /  \n ");
    printf("	|  _  /| | |_ |  _ <   | __/ _ \\    \\   / | |  | | \\ \\/ /   \n ");
    printf("	| | \\ \\| |__| | |_) |  | || (_) |    | |  | |__| |  \\  /    \n ");
    printf("	|_|  \\_\\\\_____|____/    \\__\\___/     |_|   \\____/    \\/     \n\n\n" );

    if (help == 0) {
        if (authors) {
	    printf("Authors: Boris Altamirano - Daniel Jimenez - Jose Andres Pacheco\n\n");
	} else {
	    if (RGB != NULL) {
	        printf("Using RGB File: %s.\n",RGB);
	    } else {
		printf("No RGB File specified.\n\n");
		return 0;
	    }

	    if (YUV != NULL) {
	        printf("Writing output YUV file name as: %s.\n\n",YUV);
	    } else {
		printf("No output YUV File name specified.\n\n");
		return 0;
	    }
	}
    } else {
   	printf("RGB to YUV images files converter.\n\n");
	printf("Options: \n\t -a \t\t Displays authors names. \n\t -h \t\t Displays this menu. \n");
	printf("\t -i 'RGBFile' \t Specifies the RGB file to be converted. \n\t -o 'YUVFile' \t Specifies the output YUV file name.\n\n");
    }

    return 1;
}

