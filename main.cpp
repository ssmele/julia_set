#include <iostream>
#include <fstream>
#include <stdio.h>
#include <map>

using namespace std;

const auto WIDTH_DIM = 4280;
const auto HEIGHT_DIM = 4280;
const auto CLR_CHANNELS = 3;

const auto X_LIM_LOW = -1.2;
const auto X_LIM_HIGH = 1.2;
const auto Y_LIM_LOW = -1.2;
const auto Y_LIM_HIGH = 1.2;

const auto MAX_ITER = 1000;

const auto C_REAL = 0.3;
const auto C_IMG = 0.098;


void print_img_array(int * img_array){
    for (int y = 0; y<HEIGHT_DIM; y++){
        cout << '[';
        for(int x = 0; x<WIDTH_DIM; x++){
            printf("[%d, %d, %d], ",
                    img_array[(y*HEIGHT_DIM)+x+0],
                    img_array[(y*HEIGHT_DIM)+x+1],
                    img_array[(y*HEIGHT_DIM)+x+2]);
        }
        cout << ']' << endl;
    }
}


void generate_img(int * img, char * file_name){
    // Save result to a PPM image
    ofstream file(file_name, std::ios::out | std::ios::binary);
    file << "P6\n" << WIDTH_DIM << " " << HEIGHT_DIM << "\n255\n";

    for(int y = 0; y<HEIGHT_DIM; y++){
        for(int x = 0; x<WIDTH_DIM; x++){
            file << (unsigned char) img[(y*HEIGHT_DIM)+x+0]
                 << (unsigned char) img[(y*HEIGHT_DIM)+x+1]
                 << (unsigned char) img[(y*HEIGHT_DIM)+x+2];
        }
    }

    file.close();
}

void N_TO_RGB(int * N, int * IMG){
    for(int y = 0; y<HEIGHT_DIM; y++){
        for(int x = 0; x<WIDTH_DIM; x++){
            IMG[(y*HEIGHT_DIM)+x+0] = N[(y*HEIGHT_DIM)+x];
            IMG[(y*HEIGHT_DIM)+x+1] = N[(y*HEIGHT_DIM)+x];
            IMG[(y*HEIGHT_DIM)+x+2] = N[(y*HEIGHT_DIM)+x];
        }
    }
}

void julia_set(int * N){
    // For each pixel.
    for(int y = 0; y<HEIGHT_DIM; y++){
        for(int x = 0; x<WIDTH_DIM; x++){
            float x_step = (X_LIM_HIGH - X_LIM_LOW)/(WIDTH_DIM-1);
            float y_step = (Y_LIM_HIGH - Y_LIM_LOW)/(HEIGHT_DIM-1);
            float zx = X_LIM_LOW + (x_step*x), zy = Y_LIM_LOW + (y_step*y);
            int iter_count = 0;
            while ((zx*zx) + (zy*zy) < 4 && iter_count < MAX_ITER){
                float tmp = (zx*zx) - (zy*zy) + C_REAL;
                zy = 2.0*zx*zy + C_IMG;
                zx = tmp;
                iter_count++;
            }

            N[(y*HEIGHT_DIM)+x] = iter_count;
        }
    }
}

int main() {
    // Make new array to store iteration information.
    int * N = new int[HEIGHT_DIM*WIDTH_DIM];
    julia_set(N);
    //print_img_array(N);

    map<int,int> unique_counter;
    for(int y = 0; y<HEIGHT_DIM; y++) {
        for (int x = 0; x < WIDTH_DIM; x++) {
            if (unique_counter.find(N[(y*HEIGHT_DIM)+x]) == unique_counter.end()){
                unique_counter.insert({N[(y*HEIGHT_DIM)+x], 1});
            } else{
                unique_counter.at(N[(y*HEIGHT_DIM)+x])++;
            }
        }
    }


    // Generate the image from iteration values.
    int * img = new int[HEIGHT_DIM*WIDTH_DIM*CLR_CHANNELS];
    N_TO_RGB(N, img);

    // Save image.
    generate_img(img, "cool.ppm");

    // Clean up stuff.
    delete N, img;
    exit(0);
}

