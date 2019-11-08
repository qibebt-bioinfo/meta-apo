// Updated at Aug 23, 2019
// Updated by Xiaoquan Su
// Bioinformatics Group, Single-Cell Research Center, QIBEBT, CAS
// Key calibration

//_Key_Calibrate.Tran(wgs, 16s)
//_Key_Calibrate.Cal(model, 16s, 16s.cal)

#include "key_calibrate.h"
#include "comp_sam_func.h"
#include "version.h"

using namespace std;

char Ref_db;

string Ref_list;
string Target_list;
string Listprefix;

string Ref_file;
string Target_file;

string Ref_table;
string Target_table;
string Modelfilename = "meta-apo.model";

int Mode = 0; //0: single, 1: multi, 2: multi_table
bool Reversed_table = false;


int printhelp(){
    
    cout << "meta-apo-train version : " << Version << endl;
    cout << "\tModel training using gene profiles of paired WGS-amplicon microbiomes" << endl;
    cout << "Usage: " << endl;
    cout << "meta-apo-train [Option] Value" << endl;
    cout << "Options: " << endl;
    
    cout << "\t[Input options, required]" << endl;
    cout << "\t  -L (upper) Input files list of training WGS samples" << endl;
    cout << "\t  -l Input files list of training amplicon samples" << endl;
    cout << "\t  -p List files path prefix [Optional for -l and -L]" << endl;
    cout << "\tor" << endl;
    cout << "\t  -T (upper) Input KO table (*.ko.abd) of training WGS samples" << endl;
    cout << "\t  -t Input KO table (*.ko.abd) of training amplicon samples" << endl;
    cout << "\t  -R (upper) If the input table is reversed, T(rue) or F(alse), default is false [Optional for -T and -t]" << endl;
    
    cout << "\t[Output options]" << endl;
    cout << "\t  Output mode file, default is \"meta-apo.model\" " << endl;
	
    cout << "\t[Other options]" << endl;
    cout << "\t  -h Help" << endl;
    
    exit(0);
    
    return 0;
    
    };

int Parse_Para(int argc, char * argv[]){
    
    Ref_db = 'B';
    
    int i = 1;
      
	if (argc ==1) 
		printhelp();
  
  	while(i<argc){
     	if (argv[i][0] != '-') {
            cerr << "Argument # " << i << " Error : Arguments must start with -" << endl;
            exit(0);
           };           
        switch(argv[i][1]){
            //case 'D': Ref_db = argv[i+1][0]; break;
	    //case 'i': Ref_file = argv[i+1]; Mode = 0; break;
            //case 'I': Target_file = argv[i+1]; Mode = 0; break; 
            case 'L': Ref_list = argv[i+1]; Mode = 1; break;
	    case 'l': Target_list = argv[i+1]; Mode = 1; break;
            case 'p': Listprefix = argv[i+1]; break;
            
            case 'T': Ref_table = argv[i+1]; Mode = 2; break;
			case 't': Target_table = argv[i+1]; Mode = 2; break;
            case 'R': if ((argv[i+1][0] == 't') || (argv[i+1][0] == 'T')) Reversed_table = true; break;
            
			case 'o': Modelfilename = argv[i+1]; break;
                                                 
            case 'h': printhelp(); break;
            default : cerr << "Error: Unrec argument " << argv[i] << endl; printhelp(); break; 
            }
        i+=2;
    	}
    }

int main(int argc, char * argv[]){
	
	Parse_Para(argc, argv); 
    _Comp_Tree_Func comp_tree_func;
    _Key_Calibrate key_calibrate(comp_tree_func.Get_GeneN());
                  
    switch (Mode) {
       case 0: {
				float ** ref_abd = new float * [1]; 
				float ** target_abd = new float * [1]; 
				
				ref_abd[0] = new float [comp_tree_func.Get_GeneN()];
				target_abd[0]= new float [comp_tree_func.Get_GeneN()];
				
				comp_tree_func.Load_Gene_Count(Ref_file.c_str(), ref_abd[0]);
				comp_tree_func.Load_Gene_Count(Target_file.c_str(), target_abd[0]);
				
				key_calibrate.Train(ref_abd, target_abd, 1);
				break;
	   }
       case 1:{
	   			vector <string> ref_list;
	   			vector <string> target_list;
	   			Load_List(Ref_list.c_str(), ref_list, Listprefix);
	   			Load_List(Target_list.c_str(), target_list, Listprefix);
	   			
	   			if (ref_list.size() != target_list.size()){
	   				cerr << "Error: Uneven reference and target traning set" << endl;
	   				exit(0);
				   }
	   			
	   			float ** ref_abd = new float * [ref_list.size()]; 
				float ** target_abd = new float * [ref_list.size()]; 
				
				for (int i = 0; i < ref_list.size(); i ++){
        			ref_abd[i] = new float [comp_tree_func.Get_GeneN()];
        			target_abd[i] = new float [comp_tree_func.Get_GeneN()];
        			
        			comp_tree_func.Load_Gene_Count(ref_list[i].c_str(), ref_abd[i]);
        			comp_tree_func.Load_Gene_Count(target_list[i].c_str(), target_abd[i]);
        		}
				
				key_calibrate.Train(ref_abd, target_abd, ref_list.size());		
	   			
				break;	
	   }
       case 2:{ 
		_Table_Format ref_table(Ref_table.c_str(), !Reversed_table);
            	_Table_Format target_table(Target_table.c_str(), !Reversed_table);
            	
            	int ref_count = ref_table.Get_Sample_Size();
            	int target_count = target_table.Get_Sample_Size();
            	if (ref_count != target_count){
	   				cerr << "Error: Uneven reference and target traning set" << endl;
	   				exit(0);
				   }
		float ** ref_abd = new float * [ref_count]; 
		float ** target_abd = new float * [ref_count];
            	for (int i = 0; i < ref_count; i ++){
						ref_abd[i] = new float [comp_tree_func.Get_GeneN()];
                                		target_abd[i] = new float [comp_tree_func.Get_GeneN()];
						comp_tree_func.Load_Gene_Count(&ref_table, ref_abd[i], i);
						comp_tree_func.Load_Gene_Count(&target_table, target_abd[i], i);	
        			
        			}

            	
				key_calibrate.Train(ref_abd, target_abd, ref_count);
            	
            	
				break;
            	
            	
				
            }
            
       default: break;
       }
	
	key_calibrate.Output_Model(Modelfilename.c_str());
	
    return 0;
	
}
