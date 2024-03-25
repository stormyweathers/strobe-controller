#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

namespace colorspace_operations{
//Scratch space buffers to avoid many dynamic allocations
float mat1[3][3];
float mat2[3][3];
float mat3[3][3];
float mat4[3][3];
float mat_rot[3][3];
float mat_scale[3][3];

float input_rgb[3];
float output_rgb[3];
uint8_t normalized_rgb[3];

// identity matrix
float id[3][3] = {{1,0,0},{0,1,0},{0,0,1}};

// Useful constants
float TPOT = 3*PI/2;
float HALFPI = PI/2;

// Turn the 0xRRGGBBWW into an array
void extract_pulse_code(uint32_t pulse_code, float rgb_vals[3]);

// Turn the integer array back to 0xRRGGBB00
uint32_t reconstruct_pulse_code(uint8_t rgb[3]);

// 3x3 matrix multiplation
void matmul(float out[3][3],float left[3][3], float right[3][3]);
// 3x3 Matrix-vector multiplication
void matmul(float out[3],float left[3][3], float right[3]);

// Construct primitive simplex rotation matrix
// This is NOT an SO(3) matrix
void rotation_matrix(float angle, float mat[3][3]);

// 3 concatenated primitive simplex rotations to get a full 3Pi/2 range
void rotation_matrix_full(float theta, float transform_matrix[3][3]);

// Construct the simplex scaling matrix
void scale_matrix(float radius, float mat[3][3]);

// serial print a 3x3 matrix with nice format
void print_mat(float mat[3][3]);

// Construct 3x3 scale - rotation matrix from XY coordinates
void matrix_from_xy(float x,  float y, float matrix[3][3]);

// Construct the hue-shift step in simplext space
void compute_hue_delta(float rgb[3], float z, float delta[3]);

// Convert float to 8-bit int, all components positive and sum to 255.
void normalize_rgb( float rgb[3], uint8_t normalized_rgb[3] );
}

uint32_t colorspace_operations::reconstruct_pulse_code(uint8_t rgb[3])
      {
        uint32_t r_shifted = rgb[0] << 24;
        uint32_t g_shifted = rgb[1] << 16;
        uint32_t b_shifted = rgb[2] << 8;
        return r_shifted+g_shifted+b_shifted;
      }

void colorspace_operations::extract_pulse_code(uint32_t pulse_code, float rgb_vals[3])
{
  /* Get the 8-bit pulse code from a full 4-channel 32-bit pulse code word
   *In hex:   0xRRGGBBWW
   * extract_pulse_code(0xRRGGBBWW,0) = 0xRR
   * extract_pulse_code(0xRRGGBBWW,1) = 0xGG
   * extract_pulse_code(0xRRGGBBWW,2) = 0xBB
   * extract_pulse_code(0xRRGGBBWW,3) = 0xWW 
   * extract_pulse_code(0xRRGGBBWW,>3)= 0x00
   */
  for (uint8_t position = 0; position < 3; position++)
  {
    uint32_t mask = 0xFF << (8*(3-position));
    rgb_vals[position] = ( 1.0*(  ( pulse_code & mask ) >> (8* (3-position) )  ) );
  } 
}

void colorspace_operations::matmul(float out[3][3],float left[3][3], float right[3][3])
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

void colorspace_operations::matmul(float out[3],float left[3][3], float right[3])
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

void colorspace_operations::rotation_matrix(float angle, float mat[3][3])
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

void colorspace_operations::rotation_matrix_full(float theta, float transform_matrix[3][3])
{
  //the geometry of simplex space suggests a 3pi/2 periodicity
  // Remap so 2pi periodic function is exposed to the outside world
  theta = map(theta,0,TWO_PI,0,TPOT);

  // % mod doesnt work with floats, but I want it to be periodic
  // This is dumb, but should work
  if (theta <    0){colorspace_operations::rotation_matrix_full(theta+TPOT, transform_matrix) ;return;}
  if (theta > TPOT){colorspace_operations::rotation_matrix_full(theta-TPOT, transform_matrix) ;return;}

  float theta_1;
  float theta_2;
  float theta_3;
  if ( (0<=theta ) & (theta< colorspace_operations::HALFPI) )
  {
    theta_1 = theta;
    theta_2 = 0;
    theta_3 = 0;
  }
  else if ( (colorspace_operations::HALFPI<= theta) & (theta < PI ) )
  {
    theta_1 = colorspace_operations::HALFPI;
    theta_2 = theta-colorspace_operations::HALFPI;
    theta_3 = 0;
  }
  else if ( (PI<= theta) & (theta < colorspace_operations::TPOT  ) )
  {
    theta_1 = colorspace_operations::HALFPI;
    theta_2 = colorspace_operations::HALFPI;
    theta_3 = theta-PI;
  }
  colorspace_operations::rotation_matrix(theta_1, mat1);
  colorspace_operations::rotation_matrix(theta_2, mat2);
  colorspace_operations::rotation_matrix(theta_3, mat3);

  colorspace_operations::matmul(mat4,mat3,mat2);
  colorspace_operations::matmul(transform_matrix,mat4,mat1);
}

void colorspace_operations::scale_matrix(float radius, float mat[3][3])
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

void colorspace_operations::print_mat(float mat[3][3])
{

  Serial.printf("\n [ ");
  for (int i =0; i<3; i++)
  {
      Serial.printf(" [ %f,  %f,  %f ] ,]n", mat[i][0],mat[i][1],mat[i][2]);
  }
  Serial.printf("\n ] \n");

}

void colorspace_operations::matrix_from_xy(float x,  float y, float matrix[3][3])
{
  // Build the RGB color transformation matrix from the X,Y joystick coordinates
  x=constrain(x,-1.0,1.0);
  y=constrain(y,-1.0,1.0);
  // Convert joystick to proper matrix params

  // How is this going above 1?
  float radius = pow(pow(x,2)+pow(y,2),0.5);
  radius = constrain(radius,0,1);
  float angle = atan2(y,x)+PI;
  
  colorspace_operations::rotation_matrix_full(angle,mat_rot);
  colorspace_operations::scale_matrix(radius,mat_scale);

  //print_mat(mat_scale);
  colorspace_operations::matmul(matrix,mat_scale,mat_rot);
}

void colorspace_operations::compute_hue_delta(float rgb[3], float z, float delta[3])
{
  // Compute the RGB step in hue-space based on the joystick z-coordinate
  // 3 critical points:
  //    z= 0: (R,G,B) --> (R,G,B)
  //    z=+1: (R,G,B) --> (255,0,0)
  //    z=-1: (R,G,B) --> (0, G/(G+B), B/(G+B))
  // return: delta (float[3])
  z=constrain(z,-1.0,1.0);

  uint8_t r = rgb[0];
  uint8_t g = rgb[1];
  uint8_t b = rgb[2];

  //Edge case of pure r, add a little g+b to avoid div by 0
  if (0== g+b)
  {
    r=r-1;
    g=g+0.5;
    b=b+0.5;
  }

  if (z>=0)
  {
    delta[0] = (g+b)*z;
    delta[1] = -g*z;
    delta[2] = -b*z;
  }
  else if (z<0)
  {
    delta[0] = r*z;
    delta[1] = -g*r*z / (g+b);
    delta[2] = -b*r*z / (g+b);
  }
  return;
}

void colorspace_operations::normalize_rgb( float rgb[3], uint8_t normalized_rgb[3] )
{
  //Renormalize results back to ints
  //The new num may not be exactly 255, it may be off by 1 thats ok
  // Using absolute value is an artistic choice to get reflecting boundary conditions on the simplex
  float rescale = 255/(rgb[0] + rgb[1]+rgb[2]);
  for (int i =0; i<3; i++)
  {
    normalized_rgb[i]=static_cast<uint8_t> round(rescale*abs(rgb[i]));
  }
}

#endif