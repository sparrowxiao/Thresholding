//
//  Thresholding.c
//  
//
//  Created by ting xiao on 1/31/13.
//  Copyright (c) 2013 ting xiao. All rights reserved.
//

#include <stdio.h>
#include <stdint.h> 
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])/* arg0: T value; arg1: pic path */
{
    int fd_in,fd_out;
    char inTvalue[3];
    char infile[36];
    char outfile[36];

    //get the input value
    strcpy(inTvalue,argv[1]);
    strcpy(infile,argv[2]);
    strcpy(outfile,argv[3]);
    
    uint8_t pixels[1024];
    char *T_pixels;//is one pixel here since this a gray picture you should only set the char* since it is 8bits
    uint8_t* i_pixel;
    char head[800];
    unsigned int ct = 0;
    int dummy = 0;
    int even=1;
   
    //chang the Tvalue to int type
    /*
     * for example "128" --> 128
     */
    int index= 0;
    int Tvalue = 0;
    for (index= 0; index< 3; index++)
    {
        if(inTvalue[index] >= 48 && inTvalue[index] < 58)
        {
            Tvalue = (Tvalue * 10) + (inTvalue[index] - 48);
        }
  
    }
    
    printf("%d",Tvalue);
    printf("\n %s",infile);
    
    
    //open the file to prepare read and write
    if ((fd_in=open(infile,0))<2) {
        printf("can't open %s\n",infile);
        return 1;
	}
    
    if ((fd_out=open(outfile,O_WRONLY|O_CREAT,0600))<2) {
        printf("can't open %s\n",outfile);
        return 1;
	}
    
    
    
    //read ras file,skip the header 800
    
    if (read(fd_in,&head,800)<0)  {
        printf("can't read the header of %s\n",infile);
        return 1;
	}
    
    
    if ((dummy=write(fd_out,head,800))!=800)  {
        printf("file descriptor is %d	",fd_out);
        printf("error number is %d\n",errno);
        printf("can't write out pixels, line %d\n",ct);
        printf("dummy value is %d\n",dummy);
        return 1;
    }
    
    uint8_t test_pixel[1024]={0};
    uint8_t* ptempPixel;
    int ncount = 0;
    int npixel = 0;
    for (ct=0; ct<512; ct++)
    {
        if (even)//*1 3 5 1024???
        {
            i_pixel=test_pixel;
            npixel = 1024;
        }
        else//2.4.6. 512
        {
            i_pixel=(uint8_t *)(&test_pixel[512]);
            npixel =512;
        }
        even=!even;
        
        if (read(fd_in,i_pixel,512)<0)  {
            printf("can't read in pixels, line %d\n",ct);
            return 1;
		}
        
        ptempPixel = i_pixel;
        for (index= 0; index< npixel; index++)
        {
           ncount++;
            
            // i_pixel = (uint8_t*)(T_pixels);
            printf("%0x \n",*(ptempPixel));
            if (*(ptempPixel)>=128) {
                ncount++;
                
                *(ptempPixel)=1;
            }
            else
            {
                *(ptempPixel)=0;
            }
            
            ptempPixel++;
        }
        
        if ((dummy=write(fd_out,i_pixel,512))<0)  {
            printf("file descriptor is %d	",fd_out);
            printf("error number is %d\n",errno);
            printf("can't write out pixels, line %d\n",ct);
            printf("dummy value is %d\n",dummy);
            return 1;
		}
    }
    //write
    printf("%d",ncount);
    close(fd_in);
    close(fd_out);
    
    return 0;
}