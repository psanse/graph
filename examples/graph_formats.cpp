//writes a simple udirected graph to file in different formats
//author:pss
//date: 12/12/2014

#include <iostream>
#include <string.h>
#include <string>
#include "../graph.h"

using namespace std;

enum file_t{DIMACS=1, MTX, EDGELIST};

int main(int argc, char** argv){
	if(argc!=3){
		cerr<<"please introduce filename and format: 1-DIMACS, 2-MTX, 3-EDGES"<<endl;
		return -1;
	}

	string filename=argv[1];
	int type=atoi(argv[2]);

	
	cout<<"READING: "<<filename<<"-----------------------"<<endl;
	sparse_ugraph g1(filename);
	g1.print_data();

	filename=filename.substr(0,filename.find_last_of("."));	
	filename=filename.substr(filename.find_last_of("\\ /")+1);
	cout<<"writing to:"<<filename<<endl;

	ofstream f;
	switch(type){
	case DIMACS:
		f.open(filename+"_u.clq");
		g1.write_dimacs(f);
		f.close();
		break;
	case MTX:
		f.open(filename+"_u.mtx");
		g1.write_mtx(f);
		f.close();
		break;
	case EDGELIST:
		f.open(filename+"_u.edges");
		g1.write_EDGES(f);
		f.close();
		break;
	default:
		cerr<<"incorrect file type"<<endl;
		return -1;
	}

}

