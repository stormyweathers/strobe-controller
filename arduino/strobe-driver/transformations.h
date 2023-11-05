#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "globals.h"
//Scratch space buffers to avoid many dynamic allocations
float mat1[3][3];
float mat2[3][3];
float mat3[3][3];
float mat4[3][3];
float mat_rot[3][3];
float mat_scale[3][3];

float id[3][3] = {{1,0,0},{0,1,0},{0,0,1}};


float input_rgb[3];
float output_rgb[3];
uint8_t normalized_rgb[3];


float TPOT = 3*PI/2;
float HALFPI = PI/2;

void matmul(float out[3][3],float left[3][3], float right[3][3])
{
  // 3x3 matrix multuplication
  for (uint8_t ii=0; ii<3;ii++){
    for (uint8_t jj=0; jj<3 ;jj++){
      out[ii][jj] = 0.0;
      for (uint8_t kk=0; kk<3; kk++)
      {
        out[ii][jj] += left[ii][kk] * right[kk][jj];
      }
    }
  }
}

void matmul(float out[3],float left[3][3], float right[3])
{
  // 3x3 matrix multuplication
  for (uint8_t ii=0; ii<3;ii++){
      out[ii] = 0.0;
      for (uint8_t kk=0; kk<3; kk++)
      {
        out[ii] += left[ii][kk] * right[kk];
      }
    }
}

void rotation_matrix(float angle, float mat[3][3])
{
  float c2 = pow(cos(angle),2);
  float s2 = pow(sin(angle),2);
  mat[0][0]=c2;
  mat[1][0]=0;
  mat[2][0]=s2;

  mat[0][1]=s2;
  mat[1][1]=c2;
  mat[2][1]=0;

  mat[0][2]=0;
  mat[1][2]=s2;
  mat[2][2]=c2;
}

void rotation_matrix_full(float theta, float transform_matrix[3][3])
{
  //the geometry of simplex space suggests a 3pi/2 periodicity
  // Remap so 2pi periodic function is exposed to the outside world
  theta = map(theta,0,TWO_PI,0,TPOT);

  // % mod doesnt work with floats, but I want it to be periodic
  // This is dumb, but should work
  if (theta <    0){rotation_matrix_full(theta+TPOT, transform_matrix) ;return;}
  if (theta > TPOT){rotation_matrix_full(theta-TPOT, transform_matrix); return;}

  float theta_1;
  float theta_2;
  float theta_3;
  if ( (0<=theta ) & (theta< HALFPI) )
  {
    theta_1 = theta;
    theta_2 = 0;
    theta_3 = 0;
  }
  else if ( (HALFPI<= theta) & (theta < PI ) )
  {
    theta_1 = HALFPI;
    theta_2 = theta-HALFPI;
    theta_3 = 0;
  }
  else if ( (PI<= theta) & (theta < TPOT  ) )
  {
    theta_1 = HALFPI;
    theta_2 = HALFPI;
    theta_3 = theta-PI;
  }
  rotation_matrix(theta_1, mat1);
  rotation_matrix(theta_2, mat2);
  rotation_matrix(theta_3, mat3);

  matmul(mat4,mat3,mat2);
  matmul(transform_matrix,mat4,mat1);
}

void scale_matrix(float radius, float mat[3][3])
{
  radius = constrain(radius,0,10);
  float t =(1-radius)/3;
  mat[0][0]=1-2*t;
  mat[1][0]=t;
  mat[2][0]=t;

  mat[0][1]=t;
  mat[1][1]=1-2*t;
  mat[2][1]=t;

  mat[0][2]=t;
  mat[1][2]=t;
  mat[2][2]=1-2*t;
}

void print_mat(float mat[3][3])
{

  Serial.printf("\n [ ");
  for (int i =0; i<3; i++)
  {
      Serial.printf(" [ %f,  %f,  %f ] ,]n", mat[i][0],mat[i][1],mat[i][2]);
  }
  Serial.printf("\n ] \n");

}

void matrix_from_xy(float x,  float y)
{
  // Build the RGB color transformation matrix from the X,Y joystick coordinates
  x=constrain(x,-1.0,1.0);
  y=constrain(y,-1.0,1.0);
  // Convert joystick to proper matrix params

  // How is this going above 1?
  float radius = pow(pow(x,2)+pow(y,2),0.5);
  radius = constrain(radius,0,1);
  float angle = atan2(y,x)+PI;
  
  rotation_matrix_full(angle,mat_rot);
  scale_matrix(radius,mat_scale);

  //print_mat(mat_scale);
  matmul(transform_matrix,mat_scale,mat_rot);
  //matmul(transform_matrix,mat_scale,id);
}


#endif