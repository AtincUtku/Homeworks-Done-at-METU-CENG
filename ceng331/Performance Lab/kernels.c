/********************************************************
 * Kernels to be optimized for the Metu Ceng Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Team",                     /* Team name */

    "Atınç Utku Alparslan",             /* First member full name */
    "2380061",                 /* First member id */

    "Berke Sina Ahlatcı",                         /* Second member full name (leave blank if none) */
    "2468502",                         /* Second member id (leave blank if none) */

    "Murat Akkoyun",                         /* Third member full name (leave blank if none) */
    "2380020"                          /* Third member id (leave blank if none) */
};

/****************
 * EXPOSURE FUSION KERNEL *
 ****************/

/*******************************************************
 * Your different versions of the exposure fusion kernel go here 
 *******************************************************/

/* 
 * naive_fusion - The naive baseline version of exposure fusion
 */
char naive_fusion_descr[] = "naive_fusion: Naive baseline exposure fusion";
void naive_fusion(int dim, int *img, int *w, int *dst) {
  
    int i, j, k; 
    for(k = 0; k < 4; k++){
        for(i = 0; i < dim; i++) {
            for(j = 0; j < dim; j++) {
                    dst[i*dim+j] += w[k*dim*dim+i*dim+j] * img[k*dim*dim+i*dim+j];
            }
        }
    }
}

/* 
 * fusion - Your current working version of fusion
 * IMPORTANT: This is the version you will be graded on
 */
char fusion_descr[] = "fusion: Current working version";
void fusion(int dim, int *img, int *w, int *dst) 
{
    // naive_fusion(dim, img, w, dst);
    
    int j,k;
    int dimsquare;
    dimsquare = dim*dim;
    int k_dimsquare = 0;
    for (k=0; k<4; k++) {
        for (j=0; j<dimsquare;j+=8)
            {

                    dst[j] += w[k_dimsquare+j] * img[k_dimsquare+j];
                    dst[j+1] += w[k_dimsquare+j+1] * img[k_dimsquare+j+1];
                    dst[j+2] += w[k_dimsquare+j+2] * img[k_dimsquare+j+2];
                    dst[j+3] += w[k_dimsquare+j+3] * img[k_dimsquare+j+3];
                    dst[j+4] += w[k_dimsquare+j+4] * img[k_dimsquare+j+4];
                    dst[j+5] += w[k_dimsquare+j+5] * img[k_dimsquare+j+5];
                    dst[j+6] += w[k_dimsquare+j+6] * img[k_dimsquare+j+6];
                    dst[j+7] += w[k_dimsquare+j+7] * img[k_dimsquare+j+7];


            }

        k_dimsquare += dimsquare;
        
    }
}


/*********************************************************************
 * register_fusion_functions - Register all of your different versions
 *     of the fusion kernel with the driver by calling the
 *     add_fusion_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_fusion_functions() 
{
    add_fusion_function(&naive_fusion, naive_fusion_descr);  
    add_fusion_function(&fusion, fusion_descr);  
    /* ... Register additional test functions here */
}

/***************************
 * GAUSSIAN BLUR KERNEL *
 ***************************/

/******************************************************
 * Your different versions of the Gaussian blur functions go here
 ******************************************************/

/* 
 * naive_blur - The naive baseline version of Gussian blur
 */
char naive_blur_descr[] = "naive_blur The naive baseline version of Gaussian blur";
void naive_blur(int dim, float *img, float *flt, float *dst) {
  
    int i, j, k, l;

    for(i = 0; i < dim-5+1; i++){
        for(j = 0; j < dim-5+1; j++) {
            for(k = 0; k < 5; k++){
                for(l = 0; l < 5; l++) {
                    dst[i*dim+j] = dst[i*dim+j] + img[(i+k)*dim+j+l] * flt[k*dim+l];
                }
            }
        }
    }
}


char eight_blur_descr[] = "eight_blur: different working version";
void eight_blur(int dim, float *img, float *flt, float *dst) 
{
  
    int i, j, k;
    int dimi, dimk, idimk,dimij;
    int N;
    N=dim-4;
    dimk = 0;
    dimi=0;
    for(k=0; k<5; k++) { 
        
        

        dimi = 0;
        for ( i = 0; i < N; i++) {
            
            
            idimk = dimi+dimk;
            
            for (j=0; j<N; j+=4) {
                
                
                dimij = dimi+j; 
                dst[dimij] += img[idimk+j] * flt[dimk];
                dst[dimij] += img[idimk+j+1] * flt[dimk+1];
                dst[dimij] += img[idimk+j+2] * flt[dimk+2];
                dst[dimij] += img[idimk+j+3] * flt[dimk+3];
                dst[dimij] += img[idimk+j+4] * flt[dimk+4];

                dst[dimij+1] += img[idimk+j+1] * flt[dimk];
                dst[dimij+1] += img[idimk+j+2] * flt[dimk+1];
                dst[dimij+1] += img[idimk+j+3] * flt[dimk+2];
                dst[dimij+1] += img[idimk+j+4] * flt[dimk+3];
                dst[dimij+1] += img[idimk+j+5] * flt[dimk+4];

                dst[dimij+2] += img[idimk+j+2] * flt[dimk];
                dst[dimij+2] += img[idimk+j+3] * flt[dimk+1];
                dst[dimij+2] += img[idimk+j+4] * flt[dimk+2];
                dst[dimij+2] += img[idimk+j+5] * flt[dimk+3];
                dst[dimij+2] += img[idimk+j+6] * flt[dimk+4];

                dst[dimij+3] += img[idimk+j+3] * flt[dimk];
                dst[dimij+3] += img[idimk+j+4] * flt[dimk+1];
                dst[dimij+3] += img[idimk+j+5] * flt[dimk+2];
                dst[dimij+3] += img[idimk+j+6] * flt[dimk+3];
                dst[dimij+3] += img[idimk+j+7] * flt[dimk+4];

       

            }
            

            dimi+=dim;

        }

        dimk+=dim;
    }


    

        
}

char blur_wo_funCall_descr[] = "blur_wo_funCall: Current working version wo calling function";
void blur_wo_funCall(int dim, float *img, float *flt, float *dst) 
{
  
    int i, j, k;
    int dimi, dimk, idimk, dimij;
    int N;

    // if (dim <= 128) {
    //     eight_blur(dim, img, flt, dst);
    //     return;
    // }

    N=dim-4;
    dimi = 0;
    
    for(i = 0; i < N; i++) {

        
        for (j=0; j<N; j+=16) {
            
            dimij = dimi+j;

            dimk = 0;
            for (k=0; k<5; k++) {
                
                
                idimk = dimi+dimk;

                

                dst[dimij] += img[idimk+j] * flt[dimk];
                dst[dimij] += img[idimk+j+1] * flt[dimk+1];
                dst[dimij] += img[idimk+j+2] * flt[dimk+2];
                dst[dimij] += img[idimk+j+3] * flt[dimk+3];
                dst[dimij] += img[idimk+j+4] * flt[dimk+4];

                dst[dimij+1] += img[idimk+j+1] * flt[dimk];
                dst[dimij+1] += img[idimk+j+2] * flt[dimk+1];
                dst[dimij+1] += img[idimk+j+3] * flt[dimk+2];
                dst[dimij+1] += img[idimk+j+4] * flt[dimk+3];
                dst[dimij+1] += img[idimk+j+5] * flt[dimk+4];

                dst[dimij+2] += img[idimk+j+2] * flt[dimk];
                dst[dimij+2] += img[idimk+j+3] * flt[dimk+1];
                dst[dimij+2] += img[idimk+j+4] * flt[dimk+2];
                dst[dimij+2] += img[idimk+j+5] * flt[dimk+3];
                dst[dimij+2] += img[idimk+j+6] * flt[dimk+4];

                dst[dimij+3] += img[idimk+j+3] * flt[dimk];
                dst[dimij+3] += img[idimk+j+4] * flt[dimk+1];
                dst[dimij+3] += img[idimk+j+5] * flt[dimk+2];
                dst[dimij+3] += img[idimk+j+6] * flt[dimk+3];
                dst[dimij+3] += img[idimk+j+7] * flt[dimk+4];

                dst[dimij+4] += img[idimk+j+4] * flt[dimk];
                dst[dimij+4] += img[idimk+j+5] * flt[dimk+1];
                dst[dimij+4] += img[idimk+j+6] * flt[dimk+2];
                dst[dimij+4] += img[idimk+j+7] * flt[dimk+3];
                dst[dimij+4] += img[idimk+j+8] * flt[dimk+4];

                dst[dimij+5] += img[idimk+j+5] * flt[dimk];
                dst[dimij+5] += img[idimk+j+6] * flt[dimk+1];
                dst[dimij+5] += img[idimk+j+7] * flt[dimk+2];
                dst[dimij+5] += img[idimk+j+8] * flt[dimk+3];
                dst[dimij+5] += img[idimk+j+9] * flt[dimk+4];

                dst[dimij+6] += img[idimk+j+6] * flt[dimk];
                dst[dimij+6] += img[idimk+j+7] * flt[dimk+1];
                dst[dimij+6] += img[idimk+j+8] * flt[dimk+2];
                dst[dimij+6] += img[idimk+j+9] * flt[dimk+3];
                dst[dimij+6] += img[idimk+j+10] * flt[dimk+4];

                dst[dimij+7] += img[idimk+j+7] * flt[dimk];
                dst[dimij+7] += img[idimk+j+8] * flt[dimk+1];
                dst[dimij+7] += img[idimk+j+9] * flt[dimk+2];
                dst[dimij+7] += img[idimk+j+10] * flt[dimk+3];
                dst[dimij+7] += img[idimk+j+11] * flt[dimk+4];

                dst[dimij+8] += img[idimk+j+8] * flt[dimk];
                dst[dimij+8] += img[idimk+j+9] * flt[dimk+1];
                dst[dimij+8] += img[idimk+j+10] * flt[dimk+2];
                dst[dimij+8] += img[idimk+j+11] * flt[dimk+3];
                dst[dimij+8] += img[idimk+j+12] * flt[dimk+4];

                dst[dimij+9] += img[idimk+j+9] * flt[dimk];
                dst[dimij+9] += img[idimk+j+10] * flt[dimk+1];
                dst[dimij+9] += img[idimk+j+11] * flt[dimk+2];
                dst[dimij+9] += img[idimk+j+12] * flt[dimk+3];
                dst[dimij+9] += img[idimk+j+13] * flt[dimk+4];

                dst[dimij+10] += img[idimk+j+10] * flt[dimk];
                dst[dimij+10] += img[idimk+j+11] * flt[dimk+1];
                dst[dimij+10] += img[idimk+j+12] * flt[dimk+2];
                dst[dimij+10] += img[idimk+j+13] * flt[dimk+3];
                dst[dimij+10] += img[idimk+j+14] * flt[dimk+4];

                dst[dimij+11] += img[idimk+j+11] * flt[dimk];
                dst[dimij+11] += img[idimk+j+12] * flt[dimk+1];
                dst[dimij+11] += img[idimk+j+13] * flt[dimk+2];
                dst[dimij+11] += img[idimk+j+14] * flt[dimk+3];
                dst[dimij+11] += img[idimk+j+15] * flt[dimk+4];

                dst[dimij+12] += img[idimk+j+12] * flt[dimk];
                dst[dimij+12] += img[idimk+j+13] * flt[dimk+1];
                dst[dimij+12] += img[idimk+j+14] * flt[dimk+2];
                dst[dimij+12] += img[idimk+j+15] * flt[dimk+3];
                dst[dimij+12] += img[idimk+j+16] * flt[dimk+4];

                dst[dimij+13] += img[idimk+j+13] * flt[dimk];
                dst[dimij+13] += img[idimk+j+14] * flt[dimk+1];
                dst[dimij+13] += img[idimk+j+15] * flt[dimk+2];
                dst[dimij+13] += img[idimk+j+16] * flt[dimk+3];
                dst[dimij+13] += img[idimk+j+17] * flt[dimk+4];

                dst[dimij+14] += img[idimk+j+14] * flt[dimk];
                dst[dimij+14] += img[idimk+j+15] * flt[dimk+1];
                dst[dimij+14] += img[idimk+j+16] * flt[dimk+2];
                dst[dimij+14] += img[idimk+j+17] * flt[dimk+3];
                dst[dimij+14] += img[idimk+j+18] * flt[dimk+4];

                dst[dimij+15] += img[idimk+j+15] * flt[dimk];
                dst[dimij+15] += img[idimk+j+16] * flt[dimk+1];
                dst[dimij+15] += img[idimk+j+17] * flt[dimk+2];
                dst[dimij+15] += img[idimk+j+18] * flt[dimk+3];
                dst[dimij+15] += img[idimk+j+19] * flt[dimk+4];

                
                dimk+=dim;
                

                


            }
            



        }

        dimi+=dim;
    }


    

        
}

/* 
 * blur - Your current working version of Gaussian blur
 * IMPORTANT: This is the version you will be graded on
 */
char blur_descr[] = "blur: Current working version";
void blur(int dim, float *img, float *flt, float *dst) 
{
  


    if (dim < 256) {
        eight_blur(dim, img, flt, dst);
        
    }

    else {
        blur_wo_funCall(dim, img, flt, dst);
    }



        
}






char new_blur_descr[] = "new_blur: Current working version";
void new_blur(int dim, float *img, float *flt, float *dst) 
{
  
    int i, j;
    int dimi, dimk, idimk, dimij;
    int N;
    N=dim-4;
    dimi = 0;
    
    for(i = 0; i < N; i++) {

        
        for (j=0; j<N; j+=8) {
            
            dimij = dimi+j;

            dimk = 0;
            
                
                
                idimk = i*dim;

                dst[dimij] += img[idimk+j] * flt[dimk];
                dst[dimij] += img[idimk+j+1] * flt[dimk+1];
                dst[dimij] += img[idimk+j+2] * flt[dimk+2];
                dst[dimij] += img[idimk+j+3] * flt[dimk+3];
                dst[dimij] += img[idimk+j+4] * flt[dimk+4];

                dst[dimij+1] += img[idimk+j+1] * flt[dimk];
                dst[dimij+1] += img[idimk+j+1+1] * flt[dimk+1];
                dst[dimij+1] += img[idimk+j+1+2] * flt[dimk+2];
                dst[dimij+1] += img[idimk+j+1+3] * flt[dimk+3];
                dst[dimij+1] += img[idimk+j+1+4] * flt[dimk+4];

                dst[dimij+2] += img[idimk+j+2] * flt[dimk];
                dst[dimij+2] += img[idimk+j+2+1] * flt[dimk+1];
                dst[dimij+2] += img[idimk+j+2+2] * flt[dimk+2];
                dst[dimij+2] += img[idimk+j+2+3] * flt[dimk+3];
                dst[dimij+2] += img[idimk+j+2+4] * flt[dimk+4];

                dst[dimij+3] += img[idimk+j+3] * flt[dimk];
                dst[dimij+3] += img[idimk+j+3+1] * flt[dimk+1];
                dst[dimij+3] += img[idimk+j+3+2] * flt[dimk+2];
                dst[dimij+3] += img[idimk+j+3+3] * flt[dimk+3];
                dst[dimij+3] += img[idimk+j+3+4] * flt[dimk+4];

                dst[dimij+4] += img[idimk+j+4] * flt[dimk];
                dst[dimij+4] += img[idimk+j+4+1] * flt[dimk+1];
                dst[dimij+4] += img[idimk+j+4+2] * flt[dimk+2];
                dst[dimij+4] += img[idimk+j+4+3] * flt[dimk+3];
                dst[dimij+4] += img[idimk+j+4+4] * flt[dimk+4];

                dst[dimij+5] += img[idimk+j+5] * flt[dimk];
                dst[dimij+5] += img[idimk+j+5+1] * flt[dimk+1];
                dst[dimij+5] += img[idimk+j+5+2] * flt[dimk+2];
                dst[dimij+5] += img[idimk+j+5+3] * flt[dimk+3];
                dst[dimij+5] += img[idimk+j+5+4] * flt[dimk+4];

                dst[dimij+6] += img[idimk+j+6] * flt[dimk];
                dst[dimij+6] += img[idimk+j+6+1] * flt[dimk+1];
                dst[dimij+6] += img[idimk+j+6+2] * flt[dimk+2];
                dst[dimij+6] += img[idimk+j+6+3] * flt[dimk+3];
                dst[dimij+6] += img[idimk+j+6+4] * flt[dimk+4];

                dst[dimij+7] += img[idimk+j+7] * flt[dimk];
                dst[dimij+7] += img[idimk+j+7+1] * flt[dimk+1];
                dst[dimij+7] += img[idimk+j+7+2] * flt[dimk+2];
                dst[dimij+7] += img[idimk+j+7+3] * flt[dimk+3];
                dst[dimij+7] += img[idimk+j+7+4] * flt[dimk+4];

              

                
                dimk+=dim;

                
                idimk = (i+1)*dim;

                dst[dimij] += img[idimk+j] * flt[dimk];
                dst[dimij] += img[idimk+j+1] * flt[dimk+1];
                dst[dimij] += img[idimk+j+2] * flt[dimk+2];
                dst[dimij] += img[idimk+j+3] * flt[dimk+3];
                dst[dimij] += img[idimk+j+4] * flt[dimk+4];

                dst[dimij+1] += img[idimk+j+1] * flt[dimk];
                dst[dimij+1] += img[idimk+j+1+1] * flt[dimk+1];
                dst[dimij+1] += img[idimk+j+1+2] * flt[dimk+2];
                dst[dimij+1] += img[idimk+j+1+3] * flt[dimk+3];
                dst[dimij+1] += img[idimk+j+1+4] * flt[dimk+4];

                dst[dimij+2] += img[idimk+j+2] * flt[dimk];
                dst[dimij+2] += img[idimk+j+2+1] * flt[dimk+1];
                dst[dimij+2] += img[idimk+j+2+2] * flt[dimk+2];
                dst[dimij+2] += img[idimk+j+2+3] * flt[dimk+3];
                dst[dimij+2] += img[idimk+j+2+4] * flt[dimk+4];

                dst[dimij+3] += img[idimk+j+3] * flt[dimk];
                dst[dimij+3] += img[idimk+j+3+1] * flt[dimk+1];
                dst[dimij+3] += img[idimk+j+3+2] * flt[dimk+2];
                dst[dimij+3] += img[idimk+j+3+3] * flt[dimk+3];
                dst[dimij+3] += img[idimk+j+3+4] * flt[dimk+4];

                dst[dimij+4] += img[idimk+j+4] * flt[dimk];
                dst[dimij+4] += img[idimk+j+4+1] * flt[dimk+1];
                dst[dimij+4] += img[idimk+j+4+2] * flt[dimk+2];
                dst[dimij+4] += img[idimk+j+4+3] * flt[dimk+3];
                dst[dimij+4] += img[idimk+j+4+4] * flt[dimk+4];

                dst[dimij+5] += img[idimk+j+5] * flt[dimk];
                dst[dimij+5] += img[idimk+j+5+1] * flt[dimk+1];
                dst[dimij+5] += img[idimk+j+5+2] * flt[dimk+2];
                dst[dimij+5] += img[idimk+j+5+3] * flt[dimk+3];
                dst[dimij+5] += img[idimk+j+5+4] * flt[dimk+4];

                dst[dimij+6] += img[idimk+j+6] * flt[dimk];
                dst[dimij+6] += img[idimk+j+6+1] * flt[dimk+1];
                dst[dimij+6] += img[idimk+j+6+2] * flt[dimk+2];
                dst[dimij+6] += img[idimk+j+6+3] * flt[dimk+3];
                dst[dimij+6] += img[idimk+j+6+4] * flt[dimk+4];

                dst[dimij+7] += img[idimk+j+7] * flt[dimk];
                dst[dimij+7] += img[idimk+j+7+1] * flt[dimk+1];
                dst[dimij+7] += img[idimk+j+7+2] * flt[dimk+2];
                dst[dimij+7] += img[idimk+j+7+3] * flt[dimk+3];
                dst[dimij+7] += img[idimk+j+7+4] * flt[dimk+4];

             

                
                dimk+=dim;

                idimk = (i+2)*dim;

                dst[dimij] += img[idimk+j] * flt[dimk];
                dst[dimij] += img[idimk+j+1] * flt[dimk+1];
                dst[dimij] += img[idimk+j+2] * flt[dimk+2];
                dst[dimij] += img[idimk+j+3] * flt[dimk+3];
                dst[dimij] += img[idimk+j+4] * flt[dimk+4];

                dst[dimij+1] += img[idimk+j+1] * flt[dimk];
                dst[dimij+1] += img[idimk+j+1+1] * flt[dimk+1];
                dst[dimij+1] += img[idimk+j+1+2] * flt[dimk+2];
                dst[dimij+1] += img[idimk+j+1+3] * flt[dimk+3];
                dst[dimij+1] += img[idimk+j+1+4] * flt[dimk+4];

                dst[dimij+2] += img[idimk+j+2] * flt[dimk];
                dst[dimij+2] += img[idimk+j+2+1] * flt[dimk+1];
                dst[dimij+2] += img[idimk+j+2+2] * flt[dimk+2];
                dst[dimij+2] += img[idimk+j+2+3] * flt[dimk+3];
                dst[dimij+2] += img[idimk+j+2+4] * flt[dimk+4];

                dst[dimij+3] += img[idimk+j+3] * flt[dimk];
                dst[dimij+3] += img[idimk+j+3+1] * flt[dimk+1];
                dst[dimij+3] += img[idimk+j+3+2] * flt[dimk+2];
                dst[dimij+3] += img[idimk+j+3+3] * flt[dimk+3];
                dst[dimij+3] += img[idimk+j+3+4] * flt[dimk+4];

                dst[dimij+4] += img[idimk+j+4] * flt[dimk];
                dst[dimij+4] += img[idimk+j+4+1] * flt[dimk+1];
                dst[dimij+4] += img[idimk+j+4+2] * flt[dimk+2];
                dst[dimij+4] += img[idimk+j+4+3] * flt[dimk+3];
                dst[dimij+4] += img[idimk+j+4+4] * flt[dimk+4];

                dst[dimij+5] += img[idimk+j+5] * flt[dimk];
                dst[dimij+5] += img[idimk+j+5+1] * flt[dimk+1];
                dst[dimij+5] += img[idimk+j+5+2] * flt[dimk+2];
                dst[dimij+5] += img[idimk+j+5+3] * flt[dimk+3];
                dst[dimij+5] += img[idimk+j+5+4] * flt[dimk+4];

                dst[dimij+6] += img[idimk+j+6] * flt[dimk];
                dst[dimij+6] += img[idimk+j+6+1] * flt[dimk+1];
                dst[dimij+6] += img[idimk+j+6+2] * flt[dimk+2];
                dst[dimij+6] += img[idimk+j+6+3] * flt[dimk+3];
                dst[dimij+6] += img[idimk+j+6+4] * flt[dimk+4];

                dst[dimij+7] += img[idimk+j+7] * flt[dimk];
                dst[dimij+7] += img[idimk+j+7+1] * flt[dimk+1];
                dst[dimij+7] += img[idimk+j+7+2] * flt[dimk+2];
                dst[dimij+7] += img[idimk+j+7+3] * flt[dimk+3];
                dst[dimij+7] += img[idimk+j+7+4] * flt[dimk+4];

              
                
                dimk+=dim;

                idimk = (i+3)*dim;

                dst[dimij] += img[idimk+j] * flt[dimk];
                dst[dimij] += img[idimk+j+1] * flt[dimk+1];
                dst[dimij] += img[idimk+j+2] * flt[dimk+2];
                dst[dimij] += img[idimk+j+3] * flt[dimk+3];
                dst[dimij] += img[idimk+j+4] * flt[dimk+4];

                dst[dimij+1] += img[idimk+j+1] * flt[dimk];
                dst[dimij+1] += img[idimk+j+1+1] * flt[dimk+1];
                dst[dimij+1] += img[idimk+j+1+2] * flt[dimk+2];
                dst[dimij+1] += img[idimk+j+1+3] * flt[dimk+3];
                dst[dimij+1] += img[idimk+j+1+4] * flt[dimk+4];

                dst[dimij+2] += img[idimk+j+2] * flt[dimk];
                dst[dimij+2] += img[idimk+j+2+1] * flt[dimk+1];
                dst[dimij+2] += img[idimk+j+2+2] * flt[dimk+2];
                dst[dimij+2] += img[idimk+j+2+3] * flt[dimk+3];
                dst[dimij+2] += img[idimk+j+2+4] * flt[dimk+4];

                dst[dimij+3] += img[idimk+j+3] * flt[dimk];
                dst[dimij+3] += img[idimk+j+3+1] * flt[dimk+1];
                dst[dimij+3] += img[idimk+j+3+2] * flt[dimk+2];
                dst[dimij+3] += img[idimk+j+3+3] * flt[dimk+3];
                dst[dimij+3] += img[idimk+j+3+4] * flt[dimk+4];

                dst[dimij+4] += img[idimk+j+4] * flt[dimk];
                dst[dimij+4] += img[idimk+j+4+1] * flt[dimk+1];
                dst[dimij+4] += img[idimk+j+4+2] * flt[dimk+2];
                dst[dimij+4] += img[idimk+j+4+3] * flt[dimk+3];
                dst[dimij+4] += img[idimk+j+4+4] * flt[dimk+4];

                dst[dimij+5] += img[idimk+j+5] * flt[dimk];
                dst[dimij+5] += img[idimk+j+5+1] * flt[dimk+1];
                dst[dimij+5] += img[idimk+j+5+2] * flt[dimk+2];
                dst[dimij+5] += img[idimk+j+5+3] * flt[dimk+3];
                dst[dimij+5] += img[idimk+j+5+4] * flt[dimk+4];

                dst[dimij+6] += img[idimk+j+6] * flt[dimk];
                dst[dimij+6] += img[idimk+j+6+1] * flt[dimk+1];
                dst[dimij+6] += img[idimk+j+6+2] * flt[dimk+2];
                dst[dimij+6] += img[idimk+j+6+3] * flt[dimk+3];
                dst[dimij+6] += img[idimk+j+6+4] * flt[dimk+4];

                dst[dimij+7] += img[idimk+j+7] * flt[dimk];
                dst[dimij+7] += img[idimk+j+7+1] * flt[dimk+1];
                dst[dimij+7] += img[idimk+j+7+2] * flt[dimk+2];
                dst[dimij+7] += img[idimk+j+7+3] * flt[dimk+3];
                dst[dimij+7] += img[idimk+j+7+4] * flt[dimk+4];

             

                
                dimk+=dim;

                idimk = (i+4)*dim;

                dst[dimij] += img[idimk+j] * flt[dimk];
                dst[dimij] += img[idimk+j+1] * flt[dimk+1];
                dst[dimij] += img[idimk+j+2] * flt[dimk+2];
                dst[dimij] += img[idimk+j+3] * flt[dimk+3];
                dst[dimij] += img[idimk+j+4] * flt[dimk+4];

                dst[dimij+1] += img[idimk+j+1] * flt[dimk];
                dst[dimij+1] += img[idimk+j+1+1] * flt[dimk+1];
                dst[dimij+1] += img[idimk+j+1+2] * flt[dimk+2];
                dst[dimij+1] += img[idimk+j+1+3] * flt[dimk+3];
                dst[dimij+1] += img[idimk+j+1+4] * flt[dimk+4];

                dst[dimij+2] += img[idimk+j+2] * flt[dimk];
                dst[dimij+2] += img[idimk+j+2+1] * flt[dimk+1];
                dst[dimij+2] += img[idimk+j+2+2] * flt[dimk+2];
                dst[dimij+2] += img[idimk+j+2+3] * flt[dimk+3];
                dst[dimij+2] += img[idimk+j+2+4] * flt[dimk+4];

                dst[dimij+3] += img[idimk+j+3] * flt[dimk];
                dst[dimij+3] += img[idimk+j+3+1] * flt[dimk+1];
                dst[dimij+3] += img[idimk+j+3+2] * flt[dimk+2];
                dst[dimij+3] += img[idimk+j+3+3] * flt[dimk+3];
                dst[dimij+3] += img[idimk+j+3+4] * flt[dimk+4];

                dst[dimij+4] += img[idimk+j+4] * flt[dimk];
                dst[dimij+4] += img[idimk+j+4+1] * flt[dimk+1];
                dst[dimij+4] += img[idimk+j+4+2] * flt[dimk+2];
                dst[dimij+4] += img[idimk+j+4+3] * flt[dimk+3];
                dst[dimij+4] += img[idimk+j+4+4] * flt[dimk+4];

                dst[dimij+5] += img[idimk+j+5] * flt[dimk];
                dst[dimij+5] += img[idimk+j+5+1] * flt[dimk+1];
                dst[dimij+5] += img[idimk+j+5+2] * flt[dimk+2];
                dst[dimij+5] += img[idimk+j+5+3] * flt[dimk+3];
                dst[dimij+5] += img[idimk+j+5+4] * flt[dimk+4];

                dst[dimij+6] += img[idimk+j+6] * flt[dimk];
                dst[dimij+6] += img[idimk+j+6+1] * flt[dimk+1];
                dst[dimij+6] += img[idimk+j+6+2] * flt[dimk+2];
                dst[dimij+6] += img[idimk+j+6+3] * flt[dimk+3];
                dst[dimij+6] += img[idimk+j+6+4] * flt[dimk+4];

                dst[dimij+7] += img[idimk+j+7] * flt[dimk];
                dst[dimij+7] += img[idimk+j+7+1] * flt[dimk+1];
                dst[dimij+7] += img[idimk+j+7+2] * flt[dimk+2];
                dst[dimij+7] += img[idimk+j+7+3] * flt[dimk+3];
                dst[dimij+7] += img[idimk+j+7+4] * flt[dimk+4];

               

                
                dimk+=dim;
                

                


            



        }

        dimi+=dim;
    }


    

        
}


char blur_woJ_descr[] = "blur_woJ: different working version withou +j";
void blur_woJ(int dim, float *img, float *flt, float *dst) 
{
  
    int i, j, k;
    int dimi, dimk, idimk, dimij;
    int N;
    N=dim-4;
    dimi = 0;
    for(i = 0; i < N; i++) {

        
        for (j=0; j<N; j+=16) {
            
            dimij = dimi+j;

            dimk = 0;
            for (k=0; k<5; k++) {
                
                
                idimk = (i+k)*dim+j;


                dst[dimij] += img[idimk] * flt[dimk];
                dst[dimij] += img[idimk+1] * flt[dimk+1];
                dst[dimij] += img[idimk+2] * flt[dimk+2];
                dst[dimij] += img[idimk+3] * flt[dimk+3];
                dst[dimij] += img[idimk+4] * flt[dimk+4];

                dst[dimij+1] += img[idimk+1] * flt[dimk];
                dst[dimij+1] += img[idimk+1+1] * flt[dimk+1];
                dst[dimij+1] += img[idimk+1+2] * flt[dimk+2];
                dst[dimij+1] += img[idimk+1+3] * flt[dimk+3];
                dst[dimij+1] += img[idimk+1+4] * flt[dimk+4];

                dst[dimij+2] += img[idimk+2] * flt[dimk];
                dst[dimij+2] += img[idimk+2+1] * flt[dimk+1];
                dst[dimij+2] += img[idimk+2+2] * flt[dimk+2];
                dst[dimij+2] += img[idimk+2+3] * flt[dimk+3];
                dst[dimij+2] += img[idimk+2+4] * flt[dimk+4];

                dst[dimij+3] += img[idimk+3] * flt[dimk];
                dst[dimij+3] += img[idimk+3+1] * flt[dimk+1];
                dst[dimij+3] += img[idimk+3+2] * flt[dimk+2];
                dst[dimij+3] += img[idimk+3+3] * flt[dimk+3];
                dst[dimij+3] += img[idimk+3+4] * flt[dimk+4];

                dst[dimij+4] += img[idimk+4] * flt[dimk];
                dst[dimij+4] += img[idimk+4+1] * flt[dimk+1];
                dst[dimij+4] += img[idimk+4+2] * flt[dimk+2];
                dst[dimij+4] += img[idimk+4+3] * flt[dimk+3];
                dst[dimij+4] += img[idimk+4+4] * flt[dimk+4];

                dst[dimij+5] += img[idimk+5] * flt[dimk];
                dst[dimij+5] += img[idimk+5+1] * flt[dimk+1];
                dst[dimij+5] += img[idimk+5+2] * flt[dimk+2];
                dst[dimij+5] += img[idimk+5+3] * flt[dimk+3];
                dst[dimij+5] += img[idimk+5+4] * flt[dimk+4];

                dst[dimij+6] += img[idimk+6] * flt[dimk];
                dst[dimij+6] += img[idimk+6+1] * flt[dimk+1];
                dst[dimij+6] += img[idimk+6+2] * flt[dimk+2];
                dst[dimij+6] += img[idimk+6+3] * flt[dimk+3];
                dst[dimij+6] += img[idimk+6+4] * flt[dimk+4];

                dst[dimij+7] += img[idimk+7] * flt[dimk];
                dst[dimij+7] += img[idimk+7+1] * flt[dimk+1];
                dst[dimij+7] += img[idimk+7+2] * flt[dimk+2];
                dst[dimij+7] += img[idimk+7+3] * flt[dimk+3];
                dst[dimij+7] += img[idimk+7+4] * flt[dimk+4];

                dst[dimij+8] += img[idimk+8] * flt[dimk];
                dst[dimij+8] += img[idimk+8+1] * flt[dimk+1];
                dst[dimij+8] += img[idimk+8+2] * flt[dimk+2];
                dst[dimij+8] += img[idimk+8+3] * flt[dimk+3];
                dst[dimij+8] += img[idimk+8+4] * flt[dimk+4];

                dst[dimij+9] += img[idimk+9] * flt[dimk];
                dst[dimij+9] += img[idimk+9+1] * flt[dimk+1];
                dst[dimij+9] += img[idimk+9+2] * flt[dimk+2];
                dst[dimij+9] += img[idimk+9+3] * flt[dimk+3];
                dst[dimij+9] += img[idimk+9+4] * flt[dimk+4];

                dst[dimij+10] += img[idimk+10] * flt[dimk];
                dst[dimij+10] += img[idimk+10+1] * flt[dimk+1];
                dst[dimij+10] += img[idimk+10+2] * flt[dimk+2];
                dst[dimij+10] += img[idimk+10+3] * flt[dimk+3];
                dst[dimij+10] += img[idimk+10+4] * flt[dimk+4];

                dst[dimij+11] += img[idimk+11] * flt[dimk];
                dst[dimij+11] += img[idimk+11+1] * flt[dimk+1];
                dst[dimij+11] += img[idimk+11+2] * flt[dimk+2];
                dst[dimij+11] += img[idimk+11+3] * flt[dimk+3];
                dst[dimij+11] += img[idimk+11+4] * flt[dimk+4];

                dst[dimij+12] += img[idimk+12] * flt[dimk];
                dst[dimij+12] += img[idimk+12+1] * flt[dimk+1];
                dst[dimij+12] += img[idimk+12+2] * flt[dimk+2];
                dst[dimij+12] += img[idimk+12+3] * flt[dimk+3];
                dst[dimij+12] += img[idimk+12+4] * flt[dimk+4];

                dst[dimij+13] += img[idimk+13] * flt[dimk];
                dst[dimij+13] += img[idimk+13+1] * flt[dimk+1];
                dst[dimij+13] += img[idimk+13+2] * flt[dimk+2];
                dst[dimij+13] += img[idimk+13+3] * flt[dimk+3];
                dst[dimij+13] += img[idimk+13+4] * flt[dimk+4];

                dst[dimij+14] += img[idimk+14] * flt[dimk];
                dst[dimij+14] += img[idimk+14+1] * flt[dimk+1];
                dst[dimij+14] += img[idimk+14+2] * flt[dimk+2];
                dst[dimij+14] += img[idimk+14+3] * flt[dimk+3];
                dst[dimij+14] += img[idimk+14+4] * flt[dimk+4];

                dst[dimij+15] += img[idimk+15] * flt[dimk];
                dst[dimij+15] += img[idimk+15+1] * flt[dimk+1];
                dst[dimij+15] += img[idimk+15+2] * flt[dimk+2];
                dst[dimij+15] += img[idimk+15+3] * flt[dimk+3];
                dst[dimij+15] += img[idimk+15+4] * flt[dimk+4];

                
                dimk+=dim;
                

                


            }
            



        }

        dimi+=dim;
    }


    

        
}

/*********************************************************************
 * register_blur_functions - Register all of your different versions
 *     of the gaussian blur kernel with the driver by calling the
 *     add_blur_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_blur_functions() 
{
    add_blur_function(&naive_blur, naive_blur_descr); 
    add_blur_function(&blur, blur_descr);
    add_blur_function(&blur_wo_funCall, blur_wo_funCall_descr);
    add_blur_function(&eight_blur, eight_blur_descr);
    add_blur_function(&blur_woJ, blur_woJ_descr);
    add_blur_function(&new_blur, new_blur_descr);
    /* ... Register additional test functions here */
}

