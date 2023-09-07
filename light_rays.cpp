#include <iostream>
#include <functional>
#include <cmath>
#include <math.h>
#include <vector>
#include <fstream>

#define max_colour 255
#define minWL 380.
#define maxWL 750.

struct vec{
    vec(double X, double Y): x(X), y(Y) {}
    double x,y;
};

struct colour{
    colour operator+ (const colour& c) {return colour(r+c.r,g+c.g,b+c.b);}
    void correct () {
        int max = r;
        if (g>max) max = g;
        if (b>max) max = b;
        if (max > max_colour){
            r = max_colour*r/max;
            g = max_colour*g/max;
            b = max_colour*b/max;
        }
    }
    colour(double R, double G, double B): r(R), g(G), b(B) {}
    int r,g,b;
};

struct light{
    light (vec R , double lambda, double theta, double N) : pos(R), wavelength(lambda), angle(theta), origin_N(N) {}
    vec pos;
    double wavelength;
    double angle;
    double origin_N;
};

double Sin( double degrees ){ 
    return sin(degrees * M_PI / 180.); 
} 
double Cos( double degrees ){ 
    return cos(degrees * M_PI / 180.); 
}
double Arcsin( double angle ){
    return asin(angle)*180./M_PI;
}

double F_colour(double lambda, double a, double b){
    return 256*exp( -pow(((lambda-(b+a)/2)/((b-a)/2)),2) );
}

colour lambda_to_colour (double lambda){
    return colour ( F_colour(lambda,520,630) , F_colour(lambda,500,590) , F_colour(lambda,410,480) );
}

int main(){

    // Simulation variables
    std::vector<std::vector<light>> curves (maxWL-minWL+1);
    int N_Iter = 5000;

    // Initial conditions
    double surfaceY0 = 0;
    vec R0(0,-5);
    double dR = 0.01;
    double theta_0 = 60;
    double N0 = 2;

    // Constraints
    std::vector<double> A = {0.001};
    auto N_index = [N0,A](double x, double y, double wavelength){
        return N0 + sin(x)*sin(y)*sin(A[0]*wavelength);             
    }; 

    // Resolution
    double new_N = N0;
    for (double lambda = 0; lambda <= maxWL-minWL; lambda++){
        curves[lambda].push_back( light(R0,minWL+lambda,theta_0,N0) );
    }
    for (int n = 0; n < N_Iter; n++){
        for (auto& ray : curves){
            if (ray.back().pos.y >= surfaceY0){
                new_N = N_index(ray.back().pos.x,ray.back().pos.y,ray.back().wavelength);
                double sin_theta = ray.back().origin_N*Sin(ray.back().angle)/new_N;
                if (sin_theta<=1 && sin_theta>=-1){
                    ray.back().angle = Arcsin(sin_theta);
                }else{
                    ray.back().angle = 180-ray.back().angle;
                }
            }
            vec new_point(ray.back().pos.x+dR*Sin(ray.back().angle), ray.back().pos.y+dR*Cos(ray.back().angle));
            ray.push_back( light(new_point,ray.back().wavelength,ray.back().angle,new_N) );
        }
    }

    // Data Extraction
    double Xmax = curves[0][0].pos.x , Xmin=curves[0][0].pos.x , Ymax=curves[0][0].pos.y , Ymin=curves[0][0].pos.y ;
    for(int i = 0; i < curves.size(); i++){
        for(int j = 0; j < curves[i].size(); j++){
            if(curves[i][j].pos.x>Xmax) Xmax = curves[i][j].pos.x;
            else if(curves[i][j].pos.x<Xmin) Xmin = curves[i][j].pos.x;
            if(curves[i][j].pos.y>Ymax) Ymax = curves[i][j].pos.y;
            else if(curves[i][j].pos.y<Ymin) Ymin = curves[i][j].pos.y;
        }
    }
    double D_X = Xmax-Xmin;
    double D_Y = Ymax-Ymin;
    int matrix_WID = round(D_X/dR)+1;
    int matrix_HEI = round(D_Y/dR)+1;
    int matrix_MIN_X = round(Xmin/dR);
    int matrix_MIN_Y = round(Ymin/dR);
    std::vector<std::vector<colour>> matrix(matrix_HEI,std::vector<colour>(matrix_WID,colour(0,0,0)));
    if (-matrix_MIN_Y < matrix_HEI) for(int i = 0; i < matrix_WID; i++) {
        matrix[-matrix_MIN_Y-1][i] = colour(max_colour,max_colour,max_colour);
        matrix[-matrix_MIN_Y][i] = colour(max_colour,max_colour,max_colour);
        matrix[-matrix_MIN_Y+1][i] = colour(max_colour,max_colour,max_colour);
    }
    for (int k = 0; k < curves.size(); k++){
        for (int l = 0; l < curves[k].size(); l++){
            int i = round(curves[k][l].pos.y/dR) - matrix_MIN_Y;
            int j = round(curves[k][l].pos.x/dR) - matrix_MIN_X;
            matrix[i][j] = matrix[i][j] + lambda_to_colour(curves[k][l].wavelength);
        }
    }
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[i].size(); j++){
            matrix[i][j].correct();
        }
    }
    
    // Data Saving
    std::ofstream save("light_ray.ppm");
    save << "P3\n";
    save << matrix_WID << " " << matrix_HEI << "\n";
    save << max_colour << "\n";
    for (int i = 0; i < matrix_HEI; i++) for(int j = 0; j < matrix_WID; j++) save << matrix[i][j].r << " " << matrix[i][j].g << " " << matrix[i][j].b << "\n";
    
}