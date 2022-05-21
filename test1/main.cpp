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

   //�����ʼ��
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


    //��ʱ
   long long head, tail , freq ;
   QueryPerformanceFrequency((LARGE_INTEGER *)&freq );
   QueryPerformanceCounter((LARGE_INTEGER *)&head);

   for(int l=1;l<=10;l++){
   // ����ѭ��֮�ⴴ���̣߳������̷߳����������٣�ע�⹲�������˽�б���������
   int i,j,k;
   float tmp;
   #pragma omp parallel num_threads(NUM_THREADS) private(i, j, k, tmp)
    for(k = 1; k < n; ++k){
         // ���в��֣�Ҳ���Գ��Բ��л�
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
         //���в��� ʹ���л���
         //#pragma omp for
         #pragma omp for
         for(j = k + 1; j < n; ++j)
             {

              A[i][j] = A[i][j] - tmp*A[k][j];
              }
              A[i][k] = 0.0;
        }
  // �뿪forѭ��ʱ�������߳�Ĭ��ͬ����������һ�еĴ���
    }
   }

    QueryPerformanceCounter((LARGE_INTEGER *)&tail );
    cout<<(tail - head)*1000.0/(freq)<<endl;//ʱ�䵥λ������
   }

    return 0;
}
