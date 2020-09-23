// Updated at Nov 8, 2019
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

string Input_list;
string Listprefix;

string Input_file;

string Input_table;
string Modelfilename;

string Outpath = "functions.calibrated";
string Outpath_list;

int Mode = 0; //0: single, 1: multi, 2: multi_table
bool Reversed_table = false;

_Comp_Tree_Func comp_tree_func;
_Key_Calibrate key_calibrate;

int printhelp(){
    
    cout << "meta-apo-calibrate version : " << Version << endl;
    cout << "\tCalibration of predicted gene profiles of amplicon microbiomes" << endl;
    cout << "Usage: " << endl;
    cout << "meta-apo-calibrate [Option] Value" << endl;
    cout << "Options: " << endl;
    
    cout << "\t[Input options, required]" << endl;
	cout << "\t  -i Input a gene profile file for a single sample" << endl;
	cout << "\tor" << endl;
    cout << "\t  -l Input files list for multiple samples" << endl;
    cout << "\t  -p List files path prefix [Optional for -l]" << endl;
    cout << "\tor" << endl;
	cout << "\t  -t Input KO table (*.ko.abd) for multiple samples" << endl;
	cout << "\t  -R (upper) If the input table is reversed, T(rue) or F(alse), default is false [Optional for -t]" << endl;
	
    cout << "\t  -m Input model file" << endl;
	
    
    
    cout << "\t[Output options]" << endl;
    cout << "\t  -o Output path, default is \"functions.calibrated\"" << endl;
	
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
	       	case 'i': Input_file = argv[i+1]; Mode = 0; break;
            	
			case 'l': Input_list = argv[i+1]; Mode = 1; break;
			case 'p': Listprefix = argv[i+1]; break;
            
            case 't': Input_table = argv[i+1]; Mode = 2; break;
			case 'R': if ((argv[i+1][0] == 't') || (argv[i+1][0] == 'T')) Reversed_table = true; break;
            
            case 'm': Modelfilename = argv[i+1]; break;
            
			case 'o': Outpath = argv[i+1]; break;
                                                 
            case 'h': printhelp(); break;
            default : cerr << "Error: Unrec argument " << argv[i] << endl; printhelp(); break; 
            }
        i+=2;
    	}
    
    key_calibrate = _Key_Calibrate(Modelfilename.c_str());
    if(Outpath[Outpath.size()-1] != '/')
        Outpath_list = Outpath + ".list";
    else
        Outpath_list = Outpath.substr(0, Outpath.size()-1) + ".list";
    }

int Output_Key(const char * outfilename, float * abd, int dim){
	ofstream outfile(outfilename, ios::out);
	if (!outfile){
		cerr << "Error: Cannot open output file: " << outfilename << endl;
		return 0;
	}
	
	int count = 0;
	//title
	outfile << "#functions" << endl;
	for (int i = 0; i < dim; i ++)
		if (abd[i] > 0){
			outfile << comp_tree_func.Get_Gene_Name(i) << "\t" << abd[i] << endl;
			count ++;
			}
	
	outfile.close();
	outfile.clear();
	
	return count;
}

int main(int argc, char * argv[]){
		
	Parse_Para(argc, argv); 
	cout << "Meta-Apo Calibration Starts" << endl;
    switch (Mode) {
       case 0: {
				float * abd = new float [comp_tree_func.Get_GeneN()];
				float * cal_abd = new float [comp_tree_func.Get_GeneN()];
				comp_tree_func.Load_Gene_Count(Input_file.c_str(), abd);
				key_calibrate.Calibrate(abd, cal_abd);
				//output
				Output_Key(Outpath.c_str(), cal_abd, comp_tree_func.Get_GeneN()); 
				break;
	   }
       case 1:{
	   			vector <string> input_list;
	   			vector <string> ids;
	   			
	   			Load_List(Input_list.c_str(), input_list, ids, Listprefix);
	   			
	   			float ** abd = new float * [input_list.size()]; 
	   			float ** cal_abd = new float * [input_list.size()]; 
				 
				for (int i = 0; i < input_list.size(); i ++){
        			abd[i] = new float [comp_tree_func.Get_GeneN()];
        			comp_tree_func.Load_Gene_Count(input_list[i].c_str(), abd[i]);	
        		}
        		
        		for (int i = 0; i < input_list.size(); i ++){
        			cal_abd[i] = new float [comp_tree_func.Get_GeneN()];
        			key_calibrate.Calibrate(abd[i], cal_abd[i]);
				}

				Check_Path(Outpath.c_str(), 1);
				
				for (int i = 0; i < input_list.size(); i ++){
					//output
					Output_Key((Outpath + "/" + ids[i] + ".txt").c_str(), cal_abd[i], comp_tree_func.Get_GeneN());
				}	
	   			
	   			Make_list(Outpath_list.c_str(), Outpath.c_str(), ids, 4);
				break;	
	   }
       case 2:{ 
           	    //Table for input
                _Table_Format input_table(Input_table.c_str(),!Reversed_table);
           	    //Table for output
				vector <string> genes;
				for (int i = 0; i < comp_tree_func.Get_GeneN(); i ++)
				    genes.push_back(comp_tree_func.Get_Gene_Name(i));				
                _Table_Format output_table(genes);
				
            	int input_count = input_table.Get_Sample_Size();
            	float ** input_abd = new float * [input_count];
            	float ** cal_abd = new float * [input_count]; 
            	for (int i = 0; i < input_count; i ++){
            		input_abd[i] = new float [comp_tree_func.Get_GeneN()];
			        comp_tree_func.Load_Gene_Count(&input_table, input_abd[i], i);
            	}
            	for (int i = 0; i < input_count; i ++){
        			cal_abd[i] = new float [comp_tree_func.Get_GeneN()];
        			key_calibrate.Calibrate(input_abd[i], cal_abd[i]);
        			
        			//normalize
        			float sum = 0;
        			for (int j = 0; j < comp_tree_func.Get_GeneN(); j ++)
        			    sum += cal_abd[i][j];
    			    if (sum <=0) continue;
    			    for (int j = 0; j < comp_tree_func.Get_GeneN(); j ++)
        			    cal_abd[i][j] /= sum;  
                    
                    vector <float> cal_abd_vector(cal_abd[i], cal_abd[i] + comp_tree_func.Get_GeneN());
                    
                    output_table.Add_Abd(cal_abd_vector, input_table.Get_Sample_Names()[i]); 			    
				}
				
				//output
				output_table.Filter_Empty();
				
				if (!Reversed_table)
				   output_table.Output_Table(Outpath.c_str());
	            else output_table.Output_Table_Rev(Outpath.c_str());
	
				break;
            }
            
       default: break;
       }
    cout << "Meta-Apo Calibration Finished" << endl;
	
    return 0;
	
}
