#include<iostream>
using namespace std;
#include <omp.h>
#include<windows.h>


int NUM_THREADS = 8;
void display(float **A, int n){

        for(int i = 0;i<n; i++){
	          for(int j=0; j<n; j++)
		           {cout<<A[i][j]<<' ';}
                          cout<<endl;}

}


int main(){

   //矩阵初始化
   for(int n=4;n<=1024;n=n*2)
   {

   float** A = new float* [n];
   for(int i=0;i<n;i++)
         A[i]= new float[n];

   for(int i=0;i<n;i++)
      { for(int j=0;j<i;j++)
                 A[i][j]=0;
                 A[i][i]=1.0;
        for(int j=i+1;j<n;j++)
                  A[i][j]=rand();
       }
    for(int k=0;k<n;k++)
       for(int i=k+1;i<n;i++)
              for(int j=0;j<n;j++)
                        A[i][j]+=A[k][j];


    //计时
   long long head, tail , freq ;
   QueryPerformanceFrequency((LARGE_INTEGER *)&freq );
   QueryPerformanceCounter((LARGE_INTEGER *)&head);

   for(int l=1;l<=10;l++){
   // 在外循环之外创建线程，避免线程反复创建销毁，注意共享变量和私有变量的设置
   int i,j,k;
   float tmp;
   #pragma omp parallel num_threads(NUM_THREADS) private(i, j, k, tmp)
    for(k = 1; k < n; ++k){
         // 串行部分，也可以尝试并行化
          #pragma omp single
       {
          tmp = A[k][k];
          for(int j = k + 1; j < n; ++j){
              A[k][j] = A[k][j] / tmp;
                    }
          A[k][k] = 1.0;
        }


    for(i = k + 1; i < n; ++i)
        {
         tmp = A[i][k];
         //并行部分 使用列划分
         //#pragma omp for
         #pragma omp for
         for(j = k + 1; j < n; ++j)
             {

              A[i][j] = A[i][j] - tmp*A[k][j];
              }
              A[i][k] = 0.0;
        }
  // 离开for循环时，各个线程默认同步，进入下一行的处理
    }
   }

    QueryPerformanceCounter((LARGE_INTEGER *)&tail );
    cout<<(tail - head)*1000.0/(freq)<<endl;//时间单位：毫秒
   }

    return 0;
}
