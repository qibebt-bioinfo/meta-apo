// Updated at Aug 23, 2019
// Updated by Xiaoquan Su
// Bioinformatics Group, Single-Cell Research Center, QIBEBT, CAS
// Key calibration

#include <iostream>
using namespace std;

#ifndef OLS_H
#define OLS_H

class _OLS{
      
      public:
             _OLS(){
                    N = 0;
                    Beta_0 = 0;
                    Beta_1 = 0;
                    }
             
             _OLS(int n, float beta_0, float beta_1){
                        Load_Beta(n, beta_0, beta_1);
                        }
             
             void Load_Beta(int n, float beta_0, float beta_1){
                  N = n;
                  Beta_0 = beta_0;
                  Beta_1 = beta_1;
                  }
             
             void Add_Pt(float x, float y){
                  N = X.size();
                  
                  X.push_back(x);
                  Y.push_back(y);
                  N ++;
                  }
             
             void Calc_Beta(){
                  //beta_0
                  double t1=0, t2=0, t3=0, t4=0;
		          for(int i = 0; i< X.size(); i++){
			              t1 += X[i] * X[i];
			              t2 += X[i];
			              t3 += X[i] * Y[i];
			              t4 += Y[i];
                          }
                 
                 if (t1 * X.size() - t2 * t2 == 0) return;
                 
                 Beta_1 = (t3 * X.size() - t2 * t4) / (t1 * X.size() - t2 * t2);
		         Beta_0 = (t1 * t4 - t2 * t3) / (t1 * X.size() - t2 * t2);
		         
		         N = X.size();
                 }
             
             float Get_Beta_0(){
                  return Beta_0;
                  }
             
             float Get_Beta_1(){
                  return Beta_1;
                  }
             
             float Predict_Y(float x){
                   return Beta_0 + Beta_1 * x;
                   }
             
             float Get_N(){
                   return N;
                   }
      
      private:
              float Beta_0;
              float Beta_1;
              
              int N;
              
              vector <float> X;
              vector <float> Y;
      
      };

#endif
