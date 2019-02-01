#include "cublas_v2.h"
#include "cuda_runtime.h"
#include "memory.h"
#include <iostream>
#include <ctime>
#include<stdio.h>
#include <string.h>
#include <iomanip>
#include <fstream>
#include <stack>
#include<sstream>
#include<math.h>
#include <thrust/device_vector.h> 

using namespace std;
int CorMat_2(float * , int , int,int,int,char);
int CorMat_3(float * , int , int,int,int,float,long long);
int CorMat_2_decomposition(float* , int , int ,int ,int ,int,char);
int CorMat_sparse(float * , int , int ,int ,int ,float,char);
long long remaining_N2(int N, int L,long long available_memory)
{
    long long x=available_memory;
    long long temp=N;
    temp*=2;
    x/=temp;
    return x;
}


long long remaining_N(int N, int L,int fflag)
{
        size_t free;
        size_t total_mem;
        cudaMemGetInfo(&free,&total_mem);
        long long available_mem = free;
        available_mem/=sizeof(float);
        long long NL=N;
        NL*=L;
        if (fflag==0)
        available_mem-=(NL);
        long long x=available_mem;
        long long temp=N;
        temp*=2;
        x/=temp;
        return x;
}


long long remaining_mem(int N, int L,int fflag)
{
        size_t free;
        size_t total_mem;
        cudaMemGetInfo(&free,&total_mem);
        long long available_mem = free;
        available_mem/=sizeof(float);
     cout<<" available_mem: "<<available_mem<<"\n";
        long long NL=N;
        NL*=L;
        if (fflag==0)
        available_mem-=(2*NL);
        long long x=available_mem;
        long long temp=N;
        temp*=2;
        temp *= 1.1;
        x/=temp;
        return x;
}


long long remaining_check(int N, int L,int slide)
{
        size_t free;
        size_t total_mem;
        cudaMemGetInfo(&free,&total_mem);
        long long available_mem = free;
        available_mem/=sizeof(float);
        long long NL=N;
        NL*=L;
        available_mem-=(NL);
        cout<<"*** "<<available_mem<<"\n\n";
        available_mem = available_mem*0.8;
        long long cor_size = N;
        cor_size *= (N-1);
        cor_size = cor_size/2;
        cor_size *= slide;
        available_mem -=cor_size;
         
        if (available_mem <=0)
                 return -1;
        if (available_mem >0)
            return 1;          

}





int main(int argc, char *argv[])
{

     if (argc<7)
     {
            cout<<"Please enter the following arguments: \nNumber of voxels/regions \n Lenght of time series for each region/voxel \n Window size \n Window step\n The approach: 1, 2 or 3? \n print the results: y/n";
     return 0;
    }
    	int  k = 0, l = 0, N = 0, L = 0;
    
    	N = atoi(argv[1]);
    	L = atoi(argv[2]);
    	int windowsize=atoi(argv[3]);
    	int windowstep=atoi(argv[4]);
	int method = atoi(argv[5]);
	char print = argv[6][0];
    	srand(time(0));
   	clock_t first,second;

        int slide_num;
        if(windowstep!=0)
                slide_num = ((L-windowsize)/windowstep) +1;
        cout<<" slide number is: "<<slide_num<<"\n";


    	clock_t kho1,kho2;
 ifstream myReadFile;
    	kho1=clock();
    	string ad = "time_series_30697_257";//sstm.str();
         myReadFile.open(ad);//.c_str());

        float * BOLD = new float [L * N];
        for (  k = 0; k < N; k++){
           for ( l = 0; l < L; l++)
        {
                myReadFile>>BOLD[k*L+l];//BOLD[l*N+k];
        }
        }

        myReadFile.close();
     	kho2=clock();
        long long M11 = (N-1);
        M11 *= N;
        M11 /= 2;
        if (method == 1 && remaining_check(N,L,slide_num)==-1)
           {     cout<<remaining_check(N,L,slide_num);
		 cout<<"\n not enough memory for method 1";
		return 0;
	}
	if (method == 1)
	{
    		first = clock();
   	 	int u = CorMat_2(BOLD,N, L,windowsize,windowstep,print);
    		second = clock();
	}

	if (method == 2)
	{
		int LL;
		cout<<"\n please enter the rank: ";
		cin>>LL;
		first = clock();
	      	CorMat_2_decomposition( BOLD, N, L,windowsize,windowstep,LL,print);
	        second = clock();

	}

        if (method == 3)
	{

        float thresh;
		cout<<"\n please enter the threshold: ";
		cin>>thresh;
 
        first = clock();
        int res = CorMat_sparse(BOLD, N, L,windowsize, windowstep,thresh,print);
        second = clock();

	}
	cout<<"\n time is: "<<(double)(second-first)/CLOCKS_PER_SEC;


return 0;

}
