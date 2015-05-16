//edges_reader.h: header of class MMXI to read MMX format in GRAPH
//				currently only reads matrix forma for configuration MCPS
//date: 19/11/2014
//author: pss

#include "stdio.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define EDGES_MAX_LINE_LENGTH 255

template<class T>
class EDGES{
	enum {PREMATURE_EOF=-4, COULD_NOT_READ_FILE, HEADER_ERROR, INVALID_EDGE};
public:	
	EDGES(string filename, T& gout);
	~EDGES(){f.close();}
	int read();
	

private:
	int read_number_of_vertices( int& nEdges);			//header comments %, # are removed (%% is discarded as MTX format)
	int remove_comments();
	void what(int error_code, ostream& =cerr);
	T& g;
	ifstream f;
};

template<class T>
EDGES<T>::EDGES(string filename, T& gout):g(gout){
	f.open(filename, ios::in);	
	if(!f){ 
		cerr<<filename.c_str()<<" not opened correctly"<<endl;
	}

	//set name without path
	g.set_name(filename, true);
}

template<class T>
int EDGES<T>::remove_comments(){
////////////////
// remove lines starting with # or % (accepted comments)
// lines starting with %% are not accepted as comment (format mtx)

  	char line[EDGES_MAX_LINE_LENGTH];
	char c;

	do{
		c=f.peek(); 
		if (c=='%' || c== '#'){
			f.getline(line,EDGES_MAX_LINE_LENGTH);
			if(f.fail()) return  MM_PREMATURE_EOF;
			if (line[0]=='%' && line[1]=='%') return HEADER_ERROR;
		}
		else break;
	}while(true);

 
 return 0;
}

template<class T>
int EDGES<T>::read(){
	int N, M, v, w;
	bool loops=false;
	
	//read size (opens and closes file)  and removes header comments: %,, #
	cout<<"determining size --------------"<<endl;
	int ret_code;
	if( (ret_code=read_number_of_vertices( M))<0){
		what(ret_code);
		return -1;
	}else{
		N=ret_code;		
	}

	//reads graph: no error is expected here
	string graphname(g.get_name());						//stores name of file, already without path before it is initialized
	cout<<"allocating memory for graph size:"<<N<<" --------------"<<endl;
	g.init(N);
	cout<<"reading graph from file-----------------------"<<endl;
	remove_comments();

	for (int i=0; i<M; i++){
		f>>v>>w;
		if(v==w){
			//cerr<<"loops found in vertex: "<<v<<endl;
			loops=true;
			continue;
		}
		g.add_edge(v-1, w-1);		//0 based
	}
	
	if(loops){
		cerr<<"loops found and removed"<<endl;
	}else {cout<<"graph read correctly-----------------------"<<endl;}

	//set name (without path)
	g.set_name(graphname, false);
	
return 0;
}


template<class T>
int EDGES<T>::read_number_of_vertices(int& M){
/////////////////
// It is assumed f is valid
// Header comments starting with % or # are removed
// The stream is reset to the beginning

	int nV=0, v, w;
	M=0;

	//remove header comments (%% is discarded as MTX format)
	int ret_code;
	if((ret_code=remove_comments())<0){
		return ret_code;
	}

	int lr=0;
	while(!f.eof()){
		f>>v>>w;
		if(f.fail()){
			cout<<"lines read:"<<M<<" nchar read last line:"<<lr<<" v:"<<v<<" w:"<<w<<endl;
			if(f.eof())  break;	//captures a possible empty line at the end

			M=0;
			return INVALID_EDGE;
		}
		
		if(v>nV) nV=v;
		if(w>nV) nV=w;
		M++;
	}
f.clear();
f.seekg(0, ios::beg);
return nV;
}

template<class T>
void EDGES<T>::what(int error_code, ostream& o){
	switch(error_code){
	case PREMATURE_EOF:
		o<<"premature end of file"<<endl;
		break;
	case COULD_NOT_READ_FILE:
		o<<"could not read file"<<endl;
		break;
	case HEADER_ERROR:	
		o<<"header not expected"<<endl;
		break;
	case INVALID_EDGE:
		o<<"invalid edge"<<endl;
		break;

	default:
		o<<"unknown error type"<<endl;

	}
}