// Updated at Aug 28, 2019
// Updated by Xiaoquan Su
// Bioinformatics Group, Single-Cell Research Center, QIBEBT, CAS
// Key calibration

//_Key_Calibrate.Tran(wgs, 16s)
//_Key_Calibrate.Cal(model, 16s, 16s.cal)

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#include "table_format.h"
#include "utility.h"
#include "ols.h"

#ifndef KEY_CALIBRATE_H
#define KEY_CALIBRATE_H

using namespace std;

class _Key_Calibrate{
	
	public:
		_Key_Calibrate(){
			Key_dim = 0; 
		}
		
		_Key_Calibrate(int dim){
			Key_dim = dim;
			Model_OLS = vector <_OLS> (Key_dim, _OLS());
		}
		
		_Key_Calibrate(const char * modelfilename);
		
		int Load_Model(const char * modelfilename);
		int Output_Model(const char * modelfilename);
		
		int Train(float ** ref_abd, float ** target_abd, int pair_count);
		void Calibrate(float * abd, float * cal_abd);
	
	private:
		int Key_dim;
		vector <_OLS> Model_OLS;
};

_Key_Calibrate::_Key_Calibrate(const char * modelfilename){
	
	Load_Model(modelfilename);
}

int _Key_Calibrate::Load_Model(const char * modelfilename){
	ifstream infile(modelfilename, ios::in);
		if (!infile){
			cerr << "Error: Cannot input model file: " << modelfilename << endl;
			return 0;
	}
	
	vector <_OLS> this_model;
	
	int line_count = 0;
	string buffer;
	while(getline(infile, buffer)){
		if(buffer[0] == '#') continue;
		stringstream strin(buffer);
		float float_temp_0 = 0;
		float float_temp_1 = 0;
		int int_temp = 0;
		
		strin >> int_temp >> float_temp_0 >> float_temp_1;
		
		this_model.push_back(_OLS(int_temp, float_temp_0, float_temp_1));
		line_count ++;
	}
	
	Model_OLS = this_model;
	Key_dim = Model_OLS.size();
	
	infile.close();
	infile.clear();
}

int _Key_Calibrate::Output_Model(const char * modelfilename){
	ofstream outfile(modelfilename, ios::out);
		if (!outfile){
			cerr << "Error: Cannot input model file: " << modelfilename << endl;
			return 0;
	}
	
	for (int i = 0; i < Key_dim; i++)
	//for (int i = 0; i < Model_OLS.size(); i ++)
		outfile << Model_OLS[i].Get_N() << "\t" << Model_OLS[i].Get_Beta_0() << "\t" << Model_OLS[i].Get_Beta_1() << endl;
	
	outfile.close();
	outfile.clear();
}

int _Key_Calibrate::Train(float ** ref_abd, float ** target_abd, int pair_count){
	
	//vector <_OLS> pos_OLS (Key_dim, _OLS());
	//vector <_OLS> neg_OLS (Key_dim, _OLS());
	vector <_OLS> this_model (Key_dim, _OLS());
	
	for (int i = 0; i < pair_count; i ++){
		//normalization
		float ref_sum = 0;
		float target_sum = 0;
		for (int j = 0; j < Key_dim; j ++){
			ref_sum += ref_abd[i][j];
			target_sum += target_abd[i][j];
		}
		
		if ((ref_sum <= 0) || (target_sum == 0)) continue;
		
		for (int j = 0; j < Key_dim; j ++){
			ref_abd[i][j] /= ref_sum ;
			target_abd[i][j] /= target_sum;
		}
		//training
		for (int j = 0; j < Key_dim; j ++){
			//if ((ref_abd[i][j] == 0) || (target_abd[i][j] == 0)) continue;
			//if (ref_abd[i][j] == 0) continue;			
			if ((ref_abd[i][j] == 0) && (target_abd[i][j] == 0)) continue;
			
			//if (ref_abd[i][j] > target_abd[i][j])
			//   pos_OLS[j].Add_Pt(target_abd[i][j], ref_abd[i][j]);
            //else if (ref_abd[i][j] < target_abd[i][j])
            //   neg_OLS[j].Add_Pt(target_abd[i][j], ref_abd[i][j]);                           			                  
            
            this_model[j].Add_Pt(target_abd[i][j], ref_abd[i][j]);
            }

	}
	
    for (int i = 0; i < Key_dim; i ++){	
	   /*
       if ((pos_OLS[i].Get_N() == 0) && (neg_OLS[i].Get_N() == 0)) continue;
	   
       if (pos_OLS[i].Get_N() >= neg_OLS[i].Get_N()){
                              pos_OLS[i].Calc_Beta();
                              this_model[i] = pos_OLS[i];
                              }
       else{
            neg_OLS[i].Calc_Beta();
            this_model[i] = neg_OLS[i];
            }
       */     
       if (this_model[i].Get_N() > 0)
          this_model[i].Calc_Beta();
	}
	
	Model_OLS = this_model;
	return 0;	
}

void _Key_Calibrate::Calibrate(float * abd, float * cal_abd){
	
	//norm
	float abd_sum = 0;
	for (int i = 0; i < Key_dim; i ++){
        cal_abd[i] = abd[i];
        abd_sum += cal_abd[i];
        }
    if (abd_sum <= 0) return;
    
    for(int i = 0; i < Key_dim; i ++){
            cal_abd[i] /= abd_sum;
            } 
	
	//calibrate
	for (int i = 0; i < Key_dim; i ++){
		if (Model_OLS[i].Get_N() == 0) continue;
		if (cal_abd[i] == 0) continue;
		//debug
	    //cout << abd[i] << "\t" << cal_abd[i] << "\t";
		
			cal_abd[i] = Model_OLS[i].Predict_Y(cal_abd[i]);
			cal_abd[i] = (cal_abd[i] > 0) ? cal_abd[i] : 0;		
	        cal_abd[i] *= abd_sum;
	    //debug
	    //cout << cal_abd[i] << "\t" <<  Model_OLS[i].Get_N() << "\t" << Model_OLS[i].Get_Beta_0() << "\t" << Model_OLS[i].Get_Beta_1() << endl;
    }
}

#endif
