#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>
#include <math.h>
#include <chrono>
#include <ctime>
#include <stdio.h>
#include <dirent.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "mod_fonction.h"
#include "mod_tool.h"
#include "mod_XYZ.h"
#include "mod_xmat.h"

using namespace std;


int main(int argc,char* argv[])
{
        cout<<""<<endl;
        cout<<"==================================================================="<<endl;
        cout<<"=====================   XMAT   CALCULATIONS   ====================="<<endl;
        cout<<"==================================================================="<<endl<<endl<<endl;

	std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();	
	string init_file="input.txt";
///////////////////////////////////////////////////////////////////////////
	
	ifstream myfile(init_file, ios::in);
	double Rcut;	
	double Rshort[3];	
	string input_folder;
	string output_folder;

        string line;
	std::getline( myfile, line );
	string tmp; 
	myfile >> tmp>>Rcut>>tmp>>input_folder>>tmp>>Rshort[0]>>tmp>>Rshort[1]>>tmp>>Rshort[2];
	myfile.close();

        cout<<"=====================     INITIALIZATION     ====================="<<endl;
	cout<<"Input_folder="<<input_folder<<endl;
	cout<<"Rcut="<<Rcut<<endl;
	cout<<"R_AA="<<Rshort[0]<<endl;
	cout<<"R_BB="<<Rshort[1]<<endl;
	cout<<"R_AB="<<Rshort[2]<<endl;



//////////////////////////////////////////////////////////////////////////////
	DIR* rep = opendir(input_folder.c_str ());
	if ( rep != NULL )
        {
		struct dirent* ent;
		while ( (ent = readdir(rep) ) != NULL )
		{
			string input_file=ent->d_name;
    			size_t start_pos = input_file.find("lammpstrj");
    			if(start_pos != std::string::npos)
			{
        			input_file=input_folder+"/"+input_file;
			        cout<<"=====================  "<<input_file<<"  ====================="<<endl;

				// Read positions from input_file
				strucXYZ xyz;
                		xyz.doReadXYZ(input_file);
                		xyz.doComputeDistances(Rcut);
                		xyz.doComputeAnglesOLD(Rcut);
				cout<<"Distances and angles DONE"<<endl;

				ifstream myfile2(init_file, ios::in);	
				int nn = 10; // Jump nn lines to get to the function list
			        int ii = 0;
				string line2;
        			while( std::getline( myfile2,line2 ))
        			{
            				if(ii > nn )
        		    		{
						int i_nature,i_include,i_bodies;
						double a_min,a_step,a_max;
						double b_min,b_step,b_max;
						int check2B, N_l3B,N_lNB;
			                        stringstream in(line2);
                			        string tmp;
                	       			in >> tmp>> i_nature;
						in >> tmp>> i_include;
						if (i_include!=0)
						{	
							in >> tmp>> a_min>>a_max>>a_step;
							in >> tmp>> b_min>>b_max>>b_step;
							in >> tmp>> check2B;
							in >> tmp>> N_l3B;
							in >> tmp>> N_lNB;

//							cout<<whichPot(i_nature)<<"     ";
//							cout<<a_min<<"    "<<b_step<<"     "<<a_max<<"  ";
//							cout<<b_min<<"    "<<b_step<<"     "<<b_max<<"  ";
//							cout<<check2B<<"    "<<N_l3B<<"     "<<N_lNB<<endl;

							string output_file=input_file; //replace(input_file,input_folder,output_folder);
							output_file=replace(output_file,".lammpstrj","");
							output_file=output_file.append("_f");
							output_file=output_file.append(to_string(i_nature));
							XMAT XMAT(xyz, a_min,a_max,a_step,b_min,b_max,b_step,i_nature,check2B,N_l3B,N_lNB);	
							XMAT.doXMAT(xyz,output_file,Rcut,Rshort);
						}
            				}
                		ii++;
        			}
				myfile2.close();
			}

	        }
        closedir(rep);
        }
        cout<<"==================================================================="<<endl<<endl;

	end = std::chrono::system_clock::now();
        int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
	cout<<"time_run="<<elapsed_seconds<<"µs"<<endl<<endl;
        cout<<"==================================================================="<<endl;
        cout<<"==================================================================="<<endl;
        cout<<"==================================================================="<<endl;
	return 0;
}

