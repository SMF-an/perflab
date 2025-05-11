/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Mufan Shi",              /* Team name */

    "Mufan Shi",     /* First member full name */
    "23307130075@m.fudan.edu.cn",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate1 - A little improvement
 */
char rotate1_descr[] = "rotate1: Eliminate the inefficiency of loops";
void rotate1(int dim, pixel *src, pixel *dst) 
{
    int i, j, index;

    for (j = 0; j < dim; j++)
    {
        index = dim-1-j;
        for (i = 0; i < dim; i++)
            dst[RIDX(index, i, dim)] = src[RIDX(i, j, dim)];
    }
}

/* 
 * rotate2 
 */
char rotate2_descr[] = "rotate2: Change the way of addressing";
void rotate2(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    pixel* index_d = NULL, *index_s = NULL;

    for (j = 0; j < dim; j++)
    {
        index_d = &dst[RIDX(dim-1-j, 0, dim)];
        index_s = &src[RIDX(0, j, dim)];
        for (i = 0; i < dim; i++)
        {
            *index_d++ = *index_s;
            index_s += dim;
        }
    }
}

/* 
 * rotate3
 */
char rotate3_descr[] = "rotate3: Loop unrolling";
void rotate3(int dim, pixel *src, pixel *dst) 
{
    int i, j, border = dim - 3, update2 = dim*2, update3 = update2 + dim, update4 = update2 + update2;
    pixel *index_d1 = NULL, *index_s1 = NULL, 
          *index_d2 = NULL, *index_s2 = NULL, 
          *index_d3 = NULL, *index_s3 = NULL, 
          *index_d4 = NULL, *index_s4 = NULL;

    for (j = 0; j < dim; j++)
    {
        index_d1 = &dst[RIDX(dim-1-j, 0, dim)];
        index_s1 = &src[RIDX(0, j, dim)];
        index_d2 = index_d1 + 1;
        index_d3 = index_d1 + 2;
        index_d4 = index_d1 + 3;
        index_s2 = index_s1 + dim;
        index_s3 = index_s1 + update2;
        index_s4 = index_s1 + update3;
        for (i = 0; i < border; i += 4)
        {
            *index_d1 = *index_s1;
            *index_d2 = *index_s2;
            *index_d3 = *index_s3;
            *index_d4 = *index_s4;
                 
            index_d1 += 4;
            index_d2 += 4;
            index_d3 += 4;
            index_d4 += 4;
            index_s1 += update4;
            index_s2 += update4;
            index_s3 += update4;
            index_s4 += update4;
        }

        for (; i < dim; i++)
            dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
    }
}

/* 
 * rotate_block
 */
char rotate_block_descr[] = "rotate_block: Block matrix";
void rotate_block(int dim, pixel *src, pixel *dst) 
{
    int sub_dim = 16;
    int r, c, i, j, i_border, j_border, border = dim - sub_dim + 1, index;

    for (r = 0; r < border; r += sub_dim)
    {
        i_border = r + sub_dim;
        for (c = 0; c < border; c += sub_dim)
        {
            j_border = c + sub_dim;
            for (j = c; j < j_border; j++)
            {
                index = dim-1-j;
                for (i = r; i < i_border; i++)
                    dst[RIDX(index, i, dim)] = src[RIDX(i, j, dim)];
            }
        }

        j_border = dim;
        for (j = c; j < j_border; j++)
        {
            index = dim-1-j;
            for (i = r; i < i_border; i++)
                dst[RIDX(index, i, dim)] = src[RIDX(i, j, dim)];
        }
    }

    i_border = dim;
    for (c = 0; c < border; c += sub_dim)
    {
        j_border = c + sub_dim;
        for (j = c; j < j_border; j++)
        {
            index = dim-1-j;
            for (i = r; i < i_border; i++)
                dst[RIDX(index, i, dim)] = src[RIDX(i, j, dim)];
        }
    }

    j_border = dim;
    for (j = c; j < j_border; j++)
    {
        index = dim-1-j;
        for (i = r; i < i_border; i++)
            dst[RIDX(index, i, dim)] = src[RIDX(i, j, dim)];
    }
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int sub_dim = 16;
    int r, c, i, j, i_border, j_border, border = dim - sub_dim + 1, index;

    for (r = 0; r < border; r += sub_dim)
    {
        i_border = r + sub_dim;
        for (c = 0; c < border; c += sub_dim)
        {
            j_border = c + sub_dim;
            for (j = c; j < j_border; j++)
            {
                index = dim-1-j;
                for (i = r; i < i_border; i++)
                    dst[RIDX(index, i, dim)] = src[RIDX(i, j, dim)];
            }
        }

        j_border = dim;
        for (j = c; j < j_border; j++)
        {
            index = dim-1-j;
            for (i = r; i < i_border; i++)
                dst[RIDX(index, i, dim)] = src[RIDX(i, j, dim)];
        }
    }

    i_border = dim;
    for (c = 0; c < border; c += sub_dim)
    {
        j_border = c + sub_dim;
        for (j = c; j < j_border; j++)
        {
            index = dim-1-j;
            for (i = r; i < i_border; i++)
                dst[RIDX(index, i, dim)] = src[RIDX(i, j, dim)];
        }
    }

    j_border = dim;
    for (j = c; j < j_border; j++)
    {
        index = dim-1-j;
        for (i = r; i < i_border; i++)
            dst[RIDX(index, i, dim)] = src[RIDX(i, j, dim)];
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate1, rotate1_descr);  
    add_rotate_function(&rotate2, rotate2_descr);  
    add_rotate_function(&rotate3, rotate3_descr);    
    add_rotate_function(&rotate_block, rotate_block_descr);  
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static inline void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static inline void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static inline void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

static inline pixel AVG(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    int i_low = max(i-1, 0), i_high = min(i+1, dim-1), j_low = max(j-1, 0), j_high = min(j+1, dim-1);
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = i_low; ii <= i_high; ii++) 
	for(jj = j_low; jj <= j_high; jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth1 - A little improvement 
 */
char smooth1_descr[] = "smooth1: Eliminate the inefficiency of loops and reduce expenses of function calls";
void smooth1(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = AVG(dim, i, j, src);
}

/*
 * smooth_dp 
 */
char smooth_dp_descr[] = "smooth_dp: Better algorithm";
void smooth_dp(int dim, pixel *src, pixel *dst) 
{
    pixel_sum *sum = (pixel_sum*)calloc(dim, sizeof(pixel_sum));
    int i, j;

    for(i = 0; i < dim; ++i)
    {
        // initailize and update the sum array
        switch(i)
        {
            case 0:
                for(j = 0; j < dim; ++j)
                {
                    sum[j].red = src[RIDX(0, j, dim)].red + src[RIDX(1, j, dim)].red;
                    sum[j].green = src[RIDX(0, j, dim)].green + src[RIDX(1, j, dim)].green;
                    sum[j].blue = src[RIDX(0, j, dim)].blue + src[RIDX(1, j, dim)].blue;        
                }
                break;
            default:
                if(i == dim - 1)
                {
                    for(j = 0; j < dim; ++j)
                    {
                        sum[j].red = src[RIDX(dim-1, j, dim)].red + src[RIDX(dim-2, j, dim)].red;
                        sum[j].green = src[RIDX(dim-1, j, dim)].green + src[RIDX(dim-2, j, dim)].green;
                        sum[j].blue = src[RIDX(dim-1, j, dim)].blue + src[RIDX(dim-2, j, dim)].blue;
                    }
                }
                else
                {
                    for(j = 0; j < dim; ++j)
                    {
                        sum[j].red = src[RIDX(i-1, j, dim)].red + src[RIDX(i, j, dim)].red + src[RIDX(i+1, j, dim)].red;
                        sum[j].green = src[RIDX(i-1, j, dim)].green + src[RIDX(i, j, dim)].green + src[RIDX(i+1, j, dim)].green;
                        sum[j].blue = src[RIDX(i-1, j, dim)].blue + src[RIDX(i, j, dim)].blue + src[RIDX(i+1, j, dim)].blue;
                    }
                }        
        }

        // accumulate the sum array
        for(j = 0; j < dim; ++j)
        {
            if(i == 0 || i == dim - 1)
            {
                if(j == 0)
                {
                    dst[RIDX(i, 0, dim)].red = (sum[0].red + sum[1].red)/4;
                    dst[RIDX(i, 0, dim)].green = (sum[0].green + sum[1].green)/4;
                    dst[RIDX(i, 0, dim)].blue = (sum[0].blue + sum[1].blue)/4;
                }
                else if(j == dim - 1)
                {
                    dst[RIDX(i, dim-1, dim)].red = (sum[dim-1].red + sum[dim-2].red)/4;
                    dst[RIDX(i, dim-1, dim)].green = (sum[dim-1].green + sum[dim-2].green)/4;
                    dst[RIDX(i, dim-1, dim)].blue = (sum[dim-1].blue + sum[dim-2].blue)/4;
                }
                else
                {
                    dst[RIDX(i, j, dim)].red = (sum[j].red + sum[j-1].red + sum[j+1].red)/6;
                    dst[RIDX(i, j, dim)].green = (sum[j].green + sum[j-1].green + sum[j+1].green)/6;
                    dst[RIDX(i, j, dim)].blue = (sum[j].blue + sum[j-1].blue + sum[j+1].blue)/6;
                }
            }
            else
            {
                if(j == 0)
                {
                    dst[RIDX(i, 0, dim)].red = (sum[0].red + sum[1].red)/6;
                    dst[RIDX(i, 0, dim)].green = (sum[0].green + sum[1].green)/6;
                    dst[RIDX(i, 0, dim)].blue = (sum[0].blue + sum[1].blue)/6;
                }
                else if(j == dim - 1)
                {
                    dst[RIDX(i, dim-1, dim)].red = (sum[dim-1].red + sum[dim-2].red)/6;
                    dst[RIDX(i, dim-1, dim)].green = (sum[dim-1].green + sum[dim-2].green)/6;
                    dst[RIDX(i, dim-1, dim)].blue = (sum[dim-1].blue + sum[dim-2].blue)/6;
                }
                else
                {
                    dst[RIDX(i, j, dim)].red = (sum[j].red + sum[j-1].red + sum[j+1].red)/9;
                    dst[RIDX(i, j, dim)].green = (sum[j].green + sum[j-1].green + sum[j+1].green)/9;
                    dst[RIDX(i, j, dim)].blue = (sum[j].blue + sum[j-1].blue + sum[j+1].blue)/9;
                }
            }  
        }
    }
}

/*
 * smooth_dp1
 */
char smooth_dp1_descr[] = "smooth_dp1: Optimization disaster";
void smooth_dp1(int dim, pixel *src, pixel *dst) 
{
    pixel_sum *sum = (pixel_sum*)calloc(dim, sizeof(pixel_sum));
    int i, j, col_size, total_size;

    for(i = 0; i < dim; ++i)
    {
        // initailize and update the sum array
        switch(i)
        {
            case 0:
                for(j = 0; j < dim; ++j)
                {
                    sum[j].red = src[RIDX(0, j, dim)].red + src[RIDX(1, j, dim)].red;
                    sum[j].green = src[RIDX(0, j, dim)].green + src[RIDX(1, j, dim)].green;
                    sum[j].blue = src[RIDX(0, j, dim)].blue + src[RIDX(1, j, dim)].blue;        
                }
                col_size = 2;
                break;
            default:
                if(i == dim - 1)
                {
                    for(j = 0; j < dim; ++j)
                    {
                        sum[j].red = src[RIDX(dim-1, j, dim)].red + src[RIDX(dim-2, j, dim)].red;
                        sum[j].green = src[RIDX(dim-1, j, dim)].green + src[RIDX(dim-2, j, dim)].green;
                        sum[j].blue = src[RIDX(dim-1, j, dim)].blue + src[RIDX(dim-2, j, dim)].blue;
                    }
                    col_size = 2;
                }
                else
                {
                    for(j = 0; j < dim; ++j)
                    {
                        sum[j].red = src[RIDX(i-1, j, dim)].red + src[RIDX(i, j, dim)].red + src[RIDX(i+1, j, dim)].red;
                        sum[j].green = src[RIDX(i-1, j, dim)].green + src[RIDX(i, j, dim)].green + src[RIDX(i+1, j, dim)].green;
                        sum[j].blue = src[RIDX(i-1, j, dim)].blue + src[RIDX(i, j, dim)].blue + src[RIDX(i+1, j, dim)].blue;
                    }
                    col_size = 3;
                }        
        }

        // accumulate the sum array
        for(j = 0; j < dim; ++j)
        {
            if(j == 0)
            {
                total_size = col_size << 1;
                dst[RIDX(i, 0, dim)].red = (sum[0].red + sum[1].red)/total_size;
                dst[RIDX(i, 0, dim)].green = (sum[0].green + sum[1].green)/total_size;
                dst[RIDX(i, 0, dim)].blue = (sum[0].blue + sum[1].blue)/total_size;
            }
            else if(j == dim - 1)
            {
                total_size = col_size << 1;
                dst[RIDX(i, dim-1, dim)].red = (sum[dim-1].red + sum[dim-2].red)/total_size;
                dst[RIDX(i, dim-1, dim)].green = (sum[dim-1].green + sum[dim-2].green)/total_size;
                dst[RIDX(i, dim-1, dim)].blue = (sum[dim-1].blue + sum[dim-2].blue)/total_size;
            }
            else
            {
                total_size = (col_size << 1) + col_size;
                dst[RIDX(i, j, dim)].red = (sum[j].red + sum[j-1].red + sum[j+1].red)/total_size;
                dst[RIDX(i, j, dim)].green = (sum[j].green + sum[j-1].green + sum[j+1].green)/total_size;
                dst[RIDX(i, j, dim)].blue = (sum[j].blue + sum[j-1].blue + sum[j+1].blue)/total_size;
            } 
        }
    }
}

/*
 * smooth_dp2
 */
char smooth_dp2_descr[] = "smooth_dp2";
void smooth_dp2(int dim, pixel *src, pixel *dst) 
{
    pixel_sum *sum = (pixel_sum*)calloc(dim, sizeof(pixel_sum));
    int i, j;

    for(i = 0; i < dim; ++i)
    {
        // initailize and update the sum array
        switch(i)
        {
            case 0:
                for(j = 0; j < dim; ++j)
                {
                    sum[j].red = src[RIDX(0, j, dim)].red + src[RIDX(1, j, dim)].red;
                    sum[j].green = src[RIDX(0, j, dim)].green + src[RIDX(1, j, dim)].green;
                    sum[j].blue = src[RIDX(0, j, dim)].blue + src[RIDX(1, j, dim)].blue;        
                }
                break;
            case 1:
                for(j = 0; j < dim; ++j)
                {
                    sum[j].red = sum[j].red + src[RIDX(2, j, dim)].red;
                    sum[j].green = sum[j].green + src[RIDX(2, j, dim)].green;
                    sum[j].blue = sum[j].blue + src[RIDX(2, j, dim)].blue;
                }
                break;
            default:
                if(i == dim - 1)
                {
                    for(j = 0; j < dim; ++j)
                    {
                        sum[j].red = sum[j].red - src[RIDX(dim-3, j, dim)].red;
                        sum[j].green = sum[j].green - src[RIDX(dim-3, j, dim)].green;
                        sum[j].blue = sum[j].blue - src[RIDX(dim-3, j, dim)].blue;
                    }
                }
                else
                {
                    for(j = 0; j < dim; ++j)
                    {
                        sum[j].red = sum[j].red - src[RIDX(i-2, j, dim)].red + src[RIDX(i+1, j, dim)].red;
                        sum[j].green = sum[j].green - src[RIDX(i-2, j, dim)].green + src[RIDX(i+1, j, dim)].green;
                        sum[j].blue = sum[j].blue - src[RIDX(i-2, j, dim)].blue + src[RIDX(i+1, j, dim)].blue;
                    }
                }        
        }

        // accumulate the sum array
        for(j = 0; j < dim; ++j)
        {
            if(i == 0 || i == dim - 1)
            {
                if(j == 0)
                {
                    dst[RIDX(i, 0, dim)].red = (sum[0].red + sum[1].red)/4;
                    dst[RIDX(i, 0, dim)].green = (sum[0].green + sum[1].green)/4;
                    dst[RIDX(i, 0, dim)].blue = (sum[0].blue + sum[1].blue)/4;
                }
                else if(j == dim - 1)
                {
                    dst[RIDX(i, dim-1, dim)].red = (sum[dim-1].red + sum[dim-2].red)/4;
                    dst[RIDX(i, dim-1, dim)].green = (sum[dim-1].green + sum[dim-2].green)/4;
                    dst[RIDX(i, dim-1, dim)].blue = (sum[dim-1].blue + sum[dim-2].blue)/4;
                }
                else
                {
                    dst[RIDX(i, j, dim)].red = (sum[j].red + sum[j-1].red + sum[j+1].red)/6;
                    dst[RIDX(i, j, dim)].green = (sum[j].green + sum[j-1].green + sum[j+1].green)/6;
                    dst[RIDX(i, j, dim)].blue = (sum[j].blue + sum[j-1].blue + sum[j+1].blue)/6;
                }
            }
            else
            {
                if(j == 0)
                {
                    dst[RIDX(i, 0, dim)].red = (sum[0].red + sum[1].red)/6;
                    dst[RIDX(i, 0, dim)].green = (sum[0].green + sum[1].green)/6;
                    dst[RIDX(i, 0, dim)].blue = (sum[0].blue + sum[1].blue)/6;
                }
                else if(j == dim - 1)
                {
                    dst[RIDX(i, dim-1, dim)].red = (sum[dim-1].red + sum[dim-2].red)/6;
                    dst[RIDX(i, dim-1, dim)].green = (sum[dim-1].green + sum[dim-2].green)/6;
                    dst[RIDX(i, dim-1, dim)].blue = (sum[dim-1].blue + sum[dim-2].blue)/6;
                }
                else
                {
                    dst[RIDX(i, j, dim)].red = (sum[j].red + sum[j-1].red + sum[j+1].red)/9;
                    dst[RIDX(i, j, dim)].green = (sum[j].green + sum[j-1].green + sum[j+1].green)/9;
                    dst[RIDX(i, j, dim)].blue = (sum[j].blue + sum[j-1].blue + sum[j+1].blue)/9;
                }
            }  
        }
    }
}

/*
 * smooth_dp3 - 2-dimension dp
 */
char smooth_dp3_descr[] = "smooth_dp3";
void smooth_dp3(int dim, pixel *src, pixel *dst) 
{
    pixel_sum *sum_col = (pixel_sum*)calloc(dim, sizeof(pixel_sum));
    pixel_sum *sum_row = (pixel_sum*)calloc(dim, sizeof(pixel_sum));
    int i, j, col_size;

    for(i = 0; i < dim; ++i)
    {
        // initailize and update the sum_col array
        switch(i)
        {
            case 0:
                for(j = 0; j < dim; ++j)
                {
                    sum_col[j].red = src[RIDX(0, j, dim)].red + src[RIDX(1, j, dim)].red;
                    sum_col[j].green = src[RIDX(0, j, dim)].green + src[RIDX(1, j, dim)].green;
                    sum_col[j].blue = src[RIDX(0, j, dim)].blue + src[RIDX(1, j, dim)].blue;
                }
                col_size = 2;
                break;
            case 1:
                for(j = 0; j < dim; ++j)
                {
                    sum_col[j].red += src[RIDX(2, j, dim)].red;
                    sum_col[j].green += src[RIDX(2, j, dim)].green;
                    sum_col[j].blue += src[RIDX(2, j, dim)].blue;
                }
                col_size = 3;
                break;
            default:
                if(i == dim - 1)
                {
                    for(j = 0; j < dim; ++j)
                    {
                        sum_col[j].red -= src[RIDX(dim-3, j, dim)].red;
                        sum_col[j].green -= src[RIDX(dim-3, j, dim)].green;
                        sum_col[j].blue -= src[RIDX(dim-3, j, dim)].blue;
                    }
                    col_size = 2;
                }
                else
                {
                    for(j = 0; j < dim; ++j)
                    {
                        sum_col[j].red = sum_col[j].red - src[RIDX(i-2, j, dim)].red + src[RIDX(i+1, j, dim)].red;
                        sum_col[j].green = sum_col[j].green - src[RIDX(i-2, j, dim)].green + src[RIDX(i+1, j, dim)].green;
                        sum_col[j].blue = sum_col[j].blue - src[RIDX(i-2, j, dim)].blue + src[RIDX(i+1, j, dim)].blue;
                    }
                    col_size = 3;
                }        
        }

        // accumulate the sum array
        for(j = 0; j < dim; ++j)
        {
            switch(j)
            {
                case 0:
                    sum_row[0].red = sum_col[0].red + sum_col[1].red;
                    sum_row[0].green = sum_col[0].green + sum_col[1].green;
                    sum_row[0].blue = sum_col[0].blue + sum_col[1].blue;
                    sum_row[0].num = col_size << 1;
                    break;
                case 1:
                    sum_row[1].red = sum_row[0].red + sum_col[2].red;
                    sum_row[1].green = sum_row[0].green + sum_col[2].green;
                    sum_row[1].blue = sum_row[0].blue + sum_col[2].blue;
                    sum_row[1].num = (col_size << 1) + col_size;
                    break;
                default:
                    if(j == dim - 1)
                    {
                        sum_row[dim-1].red = sum_row[dim-2].red - sum_col[dim-3].red;
                        sum_row[dim-1].green = sum_row[dim-2].green - sum_col[dim-3].green;
                        sum_row[dim-1].blue = sum_row[dim-2].blue - sum_col[dim-3].blue;
                        sum_row[dim-1].num = col_size << 1;
                    }
                    else
                    {
                        sum_row[j].red = sum_row[j-1].red + sum_col[j+1].red - sum_col[j-2].red;
                        sum_row[j].green = sum_row[j-1].green + sum_col[j+1].green - sum_col[j-2].green;
                        sum_row[j].blue = sum_row[j-1].blue + sum_col[j+1].blue - sum_col[j-2].blue;
                        sum_row[j].num = sum_row[j-1].num;
                    }
            }
        }

        for(j = 0; j < dim; ++j)
        {
            if(sum_row[j].num == 4)
            { 
                dst[RIDX(i, j, dim)].red = sum_row[j].red/4;
                dst[RIDX(i, j, dim)].green = sum_row[j].green/4;
                dst[RIDX(i, j, dim)].blue = sum_row[j].blue/4;
            }
            else if(sum_row[j].num == 6)
            { 
                dst[RIDX(i, j, dim)].red = sum_row[j].red/6;
                dst[RIDX(i, j, dim)].green = sum_row[j].green/6;
                dst[RIDX(i, j, dim)].blue = sum_row[j].blue/6;
            }
            else
            { 
                dst[RIDX(i, j, dim)].red = sum_row[j].red/9;
                dst[RIDX(i, j, dim)].green = sum_row[j].green/9;
                dst[RIDX(i, j, dim)].blue = sum_row[j].blue/9;
            }
        }
    }
}

/*
 * smooth_block 
 */
char smooth_block_descr[] = "smooth_block: Block matrix";
void smooth_block(int dim, pixel *src, pixel *dst) 
{
    int sub_dim = 16;
    int r, c, i, j, k, i_border, j_border, border = dim - sub_dim + 1, i_low, i_high, j_low, j_high;
    pixel_sum *sum = (pixel_sum*)calloc(dim, sizeof(pixel_sum));

    for (r = 0; r < border; r += sub_dim)
    {
        i_border = r + sub_dim;
        for (c = 0; c < border; c += sub_dim)
        {
            j_border = c + sub_dim;
            for(i = r; i < i_border; ++i)
            {
                // initailize and update the sum array
                i_low = max(i-1, 0);
                i_high = min(i+1, dim-1);
                j_low = max(c-1, 0);
                j_high = min(j_border, dim-1);
                for(j = j_low; j <= j_high; ++j)
                {
                    sum[j].red = sum[j].green = sum[j].blue = 0;
                    for(k = i_low; k <= i_high; ++k)
                    {
                        sum[j].red += src[RIDX(k, j, dim)].red;
                        sum[j].green += src[RIDX(k, j, dim)].green;
                        sum[j].blue += src[RIDX(k, j, dim)].blue;
                    }
                }

                // accumulate the sum array
                for(j = c; j < j_border; ++j)
                {
                    if(i == 0 || i == dim - 1)
                    {
                        if(j == 0)
                        {
                            dst[RIDX(i, 0, dim)].red = (sum[0].red + sum[1].red)/4;
                            dst[RIDX(i, 0, dim)].green = (sum[0].green + sum[1].green)/4;
                            dst[RIDX(i, 0, dim)].blue = (sum[0].blue + sum[1].blue)/4;
                        }
                        else if(j == dim - 1)
                        {
                            dst[RIDX(i, dim-1, dim)].red = (sum[dim-1].red + sum[dim-2].red)/4;
                            dst[RIDX(i, dim-1, dim)].green = (sum[dim-1].green + sum[dim-2].green)/4;
                            dst[RIDX(i, dim-1, dim)].blue = (sum[dim-1].blue + sum[dim-2].blue)/4;
                        }
                        else
                        {
                            dst[RIDX(i, j, dim)].red = (sum[j].red + sum[j-1].red + sum[j+1].red)/6;
                            dst[RIDX(i, j, dim)].green = (sum[j].green + sum[j-1].green + sum[j+1].green)/6;
                            dst[RIDX(i, j, dim)].blue = (sum[j].blue + sum[j-1].blue + sum[j+1].blue)/6;
                        }
                    }
                    else
                    {
                        if(j == 0)
                        {
                            dst[RIDX(i, 0, dim)].red = (sum[0].red + sum[1].red)/6;
                            dst[RIDX(i, 0, dim)].green = (sum[0].green + sum[1].green)/6;
                            dst[RIDX(i, 0, dim)].blue = (sum[0].blue + sum[1].blue)/6;
                        }
                        else if(j == dim - 1)
                        {
                            dst[RIDX(i, dim-1, dim)].red = (sum[dim-1].red + sum[dim-2].red)/6;
                            dst[RIDX(i, dim-1, dim)].green = (sum[dim-1].green + sum[dim-2].green)/6;
                            dst[RIDX(i, dim-1, dim)].blue = (sum[dim-1].blue + sum[dim-2].blue)/6;
                        }
                        else
                        {
                            dst[RIDX(i, j, dim)].red = (sum[j].red + sum[j-1].red + sum[j+1].red)/9;
                            dst[RIDX(i, j, dim)].green = (sum[j].green + sum[j-1].green + sum[j+1].green)/9;
                            dst[RIDX(i, j, dim)].blue = (sum[j].blue + sum[j-1].blue + sum[j+1].blue)/9;
                        }
                    }  
                }
            }
        }
    }
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    pixel_sum *sum = (pixel_sum*)calloc(dim, sizeof(pixel_sum));
    int i, j;

    for(i = 0; i < dim; ++i)
    {
        // initailize and update the sum array
        switch(i)
        {
            case 0:
                for(j = 0; j < dim; ++j)
                {
                    sum[j].red = src[RIDX(0, j, dim)].red + src[RIDX(1, j, dim)].red;
                    sum[j].green = src[RIDX(0, j, dim)].green + src[RIDX(1, j, dim)].green;
                    sum[j].blue = src[RIDX(0, j, dim)].blue + src[RIDX(1, j, dim)].blue;        
                }
                break;
            case 1:
                for(j = 0; j < dim; ++j)
                {
                    sum[j].red = sum[j].red + src[RIDX(2, j, dim)].red;
                    sum[j].green = sum[j].green + src[RIDX(2, j, dim)].green;
                    sum[j].blue = sum[j].blue + src[RIDX(2, j, dim)].blue;
                }
                break;
            default:
                if(i == dim - 1)
                {
                    for(j = 0; j < dim; ++j)
                    {
                        sum[j].red = sum[j].red - src[RIDX(dim-3, j, dim)].red;
                        sum[j].green = sum[j].green - src[RIDX(dim-3, j, dim)].green;
                        sum[j].blue = sum[j].blue - src[RIDX(dim-3, j, dim)].blue;
                    }
                }
                else
                {
                    for(j = 0; j < dim; ++j)
                    {
                        sum[j].red = sum[j].red - src[RIDX(i-2, j, dim)].red + src[RIDX(i+1, j, dim)].red;
                        sum[j].green = sum[j].green - src[RIDX(i-2, j, dim)].green + src[RIDX(i+1, j, dim)].green;
                        sum[j].blue = sum[j].blue - src[RIDX(i-2, j, dim)].blue + src[RIDX(i+1, j, dim)].blue;
                    }
                }        
        }

        // accumulate the sum array
        for(j = 0; j < dim; ++j)
        {
            if(i == 0 || i == dim - 1)
            {
                if(j == 0)
                {
                    dst[RIDX(i, 0, dim)].red = (sum[0].red + sum[1].red)/4;
                    dst[RIDX(i, 0, dim)].green = (sum[0].green + sum[1].green)/4;
                    dst[RIDX(i, 0, dim)].blue = (sum[0].blue + sum[1].blue)/4;
                }
                else if(j == dim - 1)
                {
                    dst[RIDX(i, dim-1, dim)].red = (sum[dim-1].red + sum[dim-2].red)/4;
                    dst[RIDX(i, dim-1, dim)].green = (sum[dim-1].green + sum[dim-2].green)/4;
                    dst[RIDX(i, dim-1, dim)].blue = (sum[dim-1].blue + sum[dim-2].blue)/4;
                }
                else
                {
                    dst[RIDX(i, j, dim)].red = (sum[j].red + sum[j-1].red + sum[j+1].red)/6;
                    dst[RIDX(i, j, dim)].green = (sum[j].green + sum[j-1].green + sum[j+1].green)/6;
                    dst[RIDX(i, j, dim)].blue = (sum[j].blue + sum[j-1].blue + sum[j+1].blue)/6;
                }
            }
            else
            {
                if(j == 0)
                {
                    dst[RIDX(i, 0, dim)].red = (sum[0].red + sum[1].red)/6;
                    dst[RIDX(i, 0, dim)].green = (sum[0].green + sum[1].green)/6;
                    dst[RIDX(i, 0, dim)].blue = (sum[0].blue + sum[1].blue)/6;
                }
                else if(j == dim - 1)
                {
                    dst[RIDX(i, dim-1, dim)].red = (sum[dim-1].red + sum[dim-2].red)/6;
                    dst[RIDX(i, dim-1, dim)].green = (sum[dim-1].green + sum[dim-2].green)/6;
                    dst[RIDX(i, dim-1, dim)].blue = (sum[dim-1].blue + sum[dim-2].blue)/6;
                }
                else
                {
                    dst[RIDX(i, j, dim)].red = (sum[j].red + sum[j-1].red + sum[j+1].red)/9;
                    dst[RIDX(i, j, dim)].green = (sum[j].green + sum[j-1].green + sum[j+1].green)/9;
                    dst[RIDX(i, j, dim)].blue = (sum[j].blue + sum[j-1].blue + sum[j+1].blue)/9;
                }
            }  
        }
    }
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    add_smooth_function(&smooth1, smooth1_descr);
    add_smooth_function(&smooth_dp, smooth_dp_descr);
    add_smooth_function(&smooth_dp1, smooth_dp1_descr);
    add_smooth_function(&smooth_dp2, smooth_dp2_descr);
    add_smooth_function(&smooth_dp3, smooth_dp3_descr);
    add_smooth_function(&smooth_block, smooth_block_descr);
    add_smooth_function(&smooth, smooth_descr);
    /* ... Register additional test functions here */
}

