#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include <arm_neon.h>

using namespace std;

int menu(int argc, char **argv, char **RGB, char **YUV, int *exit){
    int authors = 0;
    int help = 0;
    *RGB = NULL;
    *YUV = NULL;
    *exit = 0;

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
                *RGB = optarg;
                break;
            case 'o':
                *YUV = optarg;
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    printf("      _____   _____ ____     _         __     ___    ___      __ \n ");
    printf("    |  __ \\ / ____|  _ \\   | |        \\ \\   / / |  | \\ \\    / / \n ");
    printf("    | |__) | |  __| |_) |  | |_ ___    \\ \\_/ /| |  | |\\ \\  / /  \n ");
    printf("    |  _  /| | |_ |  _ <   | __/ _ \\    \\   / | |  | | \\ \\/ /   \n ");
    printf("    | | \\ \\| |__| | |_) |  | || (_) |    | |  | |__| |  \\  /    \n ");
    printf("    |_|  \\_\\\\_____|____/    \\__\\___/     |_|   \\____/    \\/     \n\n\n" );

    if (authors) {
        printf("Authors: Boris Altamirano - Daniel Jimenez - Jose Andres Pacheco\n\n");
        *exit = 1;
    } else if (help == 1) {
        printf("RGB to YUV images files converter.\n\n");
        printf("Options: \n\t -a \t\t Displays authors names. \n\t -h \t\t Displays this menu. \n");
        printf("\t -i 'RGBFile' \t Specifies the RGB file to be converted. \n\t -o 'YUVFile' \t Specifies the output YUV file name.\n\n");
        *exit = 1;
    } else {
        if (*RGB != NULL) {
            printf("Using RGB File: %s.\n",*RGB);
        } else {
            printf("No RGB File specified.\n\n");
            return 1;
        }

        if (*YUV != NULL) {
            printf("Writing output YUV file name as: %s.\n\n",*YUV);
        } else {
            printf("No output YUV File name specified.\n\n");
            return 1;
        }
    }
    return 0;

}

void rgb2yuv (char *input_image, char *output_image){
    ifstream in_image;
    ofstream out_image;

    //char pixels_rgb[640*480*3];
    char pixels_yuv[640*480*3];
    char pixels_rgb[640*480*3];

    in_image.open(input_image, ios::in | ios::binary); //Example image input: "sample_640x480.rgb"
    out_image.open(output_image, ios::out | ios::binary); //Example image output: "sample_639x480.yuv"

    char* buf_yuv;

    in_image.read(pixels_rgb, sizeof(pixels_rgb));

    for (int i=0; i<sizeof(pixels_rgb); i=i+12){
        float tmp_r[4];
        float tmp_g[4];
        float tmp_b[4];

        float tmp_y[4];
        float tmp_u[4];
        float tmp_v[4];

        float32x4_t buf_r, buf_g, buf_b;
        float32x4_t buf_y, buf_u, buf_v;
        float32x4_t R1, R2, R3;
        float32x4_t G1, G2, G3;
        float32x4_t B1, B2, B3;

        float32x4_t constR1 = {0.257,0.257,0.257,0.257};
        float32x4_t constR2 = {0.504,0.504,0.504,0.504};
        float32x4_t constR3 = {0.098,0.098,0.098,0.098};
        float32x4_t constR4 = {16,16,16,16};

        float32x4_t constG1 = {-0.148,-0.148,-0.148,-0.148};
        float32x4_t constG2 = {-0.291,-0.291,-0.291,-0.291};
        float32x4_t constG3 = {0.439,0.439,0.439,0.439};
        float32x4_t constG4 = {128,128,128,128};

        float32x4_t constB1 = {0.439,0.439,0.439,0.439};
        float32x4_t constB2 = {-0.368,-0.368,-0.368,-0.368};
        float32x4_t constB3 = {-0.071,-0.071,-0.071,-0.071};
        float32x4_t constB4 = {128,128,128,128};

        for (int j=0; j<4; j++) {
            tmp_r[j] = float(static_cast<unsigned char>(pixels_rgb[i+(j*3)]));
            tmp_g[j] = float(static_cast<unsigned char>(pixels_rgb[i+(j*3)+1]));
            tmp_b[j] = float(static_cast<unsigned char>(pixels_rgb[i+(j*3)+2]));
        }

        buf_r = vld1q_f32(tmp_r);
        buf_g = vld1q_f32(tmp_g);
        buf_b = vld1q_f32(tmp_b);

        R1 = vmulq_f32(buf_r,constR1);
        R2 = vmulq_f32(buf_g,constR2);
        R3 = vmulq_f32(buf_b,constR3);

        buf_y = vaddq_f32(R1,R2);
        buf_y = vaddq_f32(buf_y,R3);
        buf_y = vaddq_f32(buf_y,constR4);

        G1 = vmulq_f32(buf_r,constG1);
        G2 = vmulq_f32(buf_g,constG2);
        G3 = vmulq_f32(buf_b,constG3);

        buf_u = vaddq_f32(G1,G2);
        buf_u = vaddq_f32(buf_y,G3);
        buf_u = vaddq_f32(buf_y,constG4);

        B1 = vmulq_f32(buf_r,constB1);
        B2 = vmulq_f32(buf_g,constB2);
        B3 = vmulq_f32(buf_b,constB3);

        buf_v = vaddq_f32(B1,B2);
        buf_v = vaddq_f32(buf_y,B3);
        buf_v = vaddq_f32(buf_y,constB4);

        vst1q_f32(tmp_y,buf_y);
        vst1q_f32(tmp_u,buf_u);
        vst1q_f32(tmp_v,buf_v);

        for (int j=0; j<4; j++) {
            pixels_yuv[i+(j*3)] = char(tmp_y[j]);
            pixels_yuv[i+(j*3)+1] = char(tmp_u[j]);
            pixels_yuv[i+(j*3)+2] = char(tmp_v[j]);
        }
    }

    in_image.close();
    out_image.write(pixels_yuv, sizeof(pixels_yuv));
    out_image.close();
}

int main (int argc, char **argv) {

    char *RGB_file_name;
    char *YUV_file_name;
    int exit;

    struct timeval time1;
    struct timeval time2;
    long int elapsed_time;

    if (menu(argc,argv,&RGB_file_name,&YUV_file_name,&exit)){
        return 1;
    }
    if (exit == 1) {
        return 0;
    }

    printf("Executing conversion!!\n");

    gettimeofday(&time1, NULL);
    rgb2yuv(RGB_file_name,YUV_file_name);
    gettimeofday(&time2, NULL);

    elapsed_time = time2.tv_usec - time1.tv_usec;

    printf("Elapsed time: %ld us\n", elapsed_time);

    return 0;
}
