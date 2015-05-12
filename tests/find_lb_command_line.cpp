#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../kcore.h"

using namespace std;

template <class Collection>
void printCol(const Collection& c, ostream& o=cout ){
	copy(c.begin(), c.end(), ostream_iterator<typename Collection::value_type>(o," " ));
}

int main(int argc, char** argv){
	if(argc!=3){
		cerr<<"incorrect number of parameters"<<endl;
		cerr<<"required <filename> <number of iterations or -1 for all possible iterations>"<<endl;
	}

	//read params
	string filename(argv[1]);
	int num_iter=atoi(argv[2]);

	//find initial clique
	sparse_ugraph ug(filename);
	KCore<sparse_ugraph> kc(ug);
	kc.kcore();
	vector<int> initial_clique;
	PrecisionTimer pt;
	pt.wall_tic();
	if(num_iter==EMPTY_ELEM)
		 initial_clique=kc.find_heur_clique_sparse();
	else  initial_clique=kc.find_heur_clique_sparse(num_iter);
	double time_sec=pt.wall_toc();
	cout<<endl<<"[t:"<<time_sec<<","<<" Smax:"<<initial_clique.size()<<"]"<<endl;
	printCol(initial_clique);
}

