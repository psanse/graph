//mmx_reader.h: header of class MMXI to read MMX format in GRAPH
//				currently only reads matrix forma for configuration MCPS
//date: 6/10/2014
//author: pss

#include "mmio.h"
#include "stdio.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define ALLOW_DIRECTED_GRAPHS	//does not look for "symmetric" header


template<class T>
class MMI{
public:	
	MMI(T& gout):g(gout){}
	int read(const string& filename);

private:
	void what(int error_code, ostream& =cerr);
	MM_typecode matcode;			//4 char code which represents the format type
	T& g;
};

template<class T>
int MMI<T>::read(const string& filename){
	int M, N, nz, v, w;

	//open file
	FILE* f= fopen(filename.c_str(), "r"); 	
	if(f==NULL){
		cerr<<filename.c_str()<<" not found"<<endl;
		return -1;
	}

	//read banner	
	int ret_code=0;
    if ( (ret_code=mm_read_banner(f, &matcode)) != 0)    {
        cout<<"Could not process Matrix Market banner."<<endl;
		what(ret_code);
		fclose(f);
		return -1;
    }

	//filter out everything except MCPS
#ifdef ALLOW_DIRECTED_GRAPHS

	if( (!mm_is_sparse(matcode)) ||  (!mm_is_pattern(matcode)) /*|| (!mm_is_symmetric(matcode))*/ )   {
        cout<<"Sorry, this application does not support ";
		stringstream sstr("Market Market type: ");
		sstr<< mm_typecode_to_str(matcode)<<endl;
		fclose(f);
        return -1;
    }

#else

	if( (!mm_is_sparse(matcode)) ||  (!mm_is_pattern(matcode)) || (!mm_is_symmetric(matcode)) )   {
        cout<<"Sorry, this application does not support ";
		stringstream sstr("Market Market type: ");
		sstr<< mm_typecode_to_str(matcode)<<endl;
		fclose(f);
        return -1;
    }

#endif
	
	//find out size of sparse matrix 
	if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0){
		what(ret_code);
			fclose(f);
		return -1;
	}

	//check it is a square matrix
	if(M!=N){
		cerr<<"non-square adjacency matrix"<<endl;
			fclose(f);
		return -1;
	}
		
	//init size
	g.init(N);
	
	//read edges
	for (int i=0; i<nz; i++){
		fscanf(f, "%d %d", &v, &w);
		if(v==w){
			cerr<<"loops found in vertex: "<<v<<endl;
		}
		
		g.add_edge(v-1, w-1);		//0 based
    }

	//name (remove path)
	g.set_name(filename, true);	

			
	fclose(f);
return 0;
}

template<class T>
void MMI<T>::what(int error_code, ostream& o){
	switch(error_code){
	case MM_COULD_NOT_READ_FILE:
		o<<"could not read file"<<endl;
		break;
	case MM_PREMATURE_EOF:
		o<<"premature EOF"<<endl;
		break;
	case MM_NOT_MTX:
		o<<"no matrix format"<<endl;
		break;
	case MM_NO_HEADER:
		o<<"incorrect header"<<endl;
		break;
	case MM_UNSUPPORTED_TYPE:
		o<<"not supported type"<<endl;
		break;
	case MM_LINE_TOO_LONG:
		o<<"line too long"<<endl;
		break;
	case MM_COULD_NOT_WRITE_FILE:
		o<<"could not write to file"<<endl;		///this should not occur
		break;
	default:
		o<<"unknown error type"<<endl;

	}
}