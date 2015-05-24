//graph_gen.h header for Erdos-Renyi sparse and non sparse bitstring unidrected graph generation 
//author: pablo san segundo

#ifndef __GRAPH_GEN_H__
#define __GRAPH_GEN_H__


#include <iostream>
#include <sstream>
#include <random>					//uses uniform int random generator

#include "graph.h"
#include "pablodev/utils/common.h"

using namespace std;

//for random tests
struct random_attr_t {
	friend ostream & operator<<(ostream& o, const random_attr_t& r) {o<<"["<<r.nLB<<" "<<r.nUB<<" "<<r.pLB \
										<<" "<<r.pUB<<" "<<r.nRep<<" "<<r.incN<<" "<<r.incP<<"]"<<endl; return o;}
	random_attr_t(int nLB, int nUB, double pLB, double pUB, int nRep, int incN, double incP) { 
					set(nLB, nUB, pLB, pUB, nRep, incN, incP);}
	void set(int nLB, int nUB, double pLB, double pUB, int nRep, int incN, double incP){
		this->nLB=nLB; this->nUB=nUB; this->pLB=pLB; this->pUB=pUB;  this->nRep=nRep; this->incN=incN; 
		this->incP=incP;
	}

	int nLB;
	int nUB;
	double pLB;
	double pUB;
	int nRep;
	int incN;
	double incP;
};

template<class Graph_t>
class RandomGen{
public:
//non sparse generators
	static int create_ugraph (Graph_t& g, int size, double p);
	static int create_graph (Graph_t& g, int size, double p);
	
	static int create_ugraph_benchmark (const string& path, const random_attr_t& rd);	
	static int create_graph_benchmark (const string& path, const random_attr_t& rd);
	
////useful function
//private:
//	static void append_slash_to_string(std::string & path);
	
};

template<int logSizeTable=35000>
class SparseRandomGen{
////////////////////////
// Specific massive sparse uniform graph generator

	static const int MAX_RAND=logSizeTable;		
public:
	int create_ugraph (sparse_ugraph& g, int n, double p);
	SparseRandomGen(){init_log_table();}
			
private:
	void init_log_table();					//variable precission at compile time
	double m_log[MAX_RAND];
inline	static void decode	(BITBOARD edge_index, unsigned int & i, unsigned int &j,  unsigned int n);
};

template<int MAX_RAND>
inline
void SparseRandomGen<MAX_RAND>::decode	(BITBOARD edge_index, unsigned int & i, unsigned int &j, unsigned int n){
/////////////////////////////
// decodes undimensional edge index to upper triangle of adjacency matrix 
//
// REMARKS: From personal communication with [Fast Random graph generation, 2011] authors

	double a=sqrt((pow((2*n)-1.0,2)-8*(edge_index+1)));
	BITBOARD r=ceil( (((2*n)-1.0-a)/2-1.0 )    );
	BITBOARD c=edge_index+1+r-(r*(2*n-r-1)/2);			//type is important else c overflows unsigned int type for very large graphs

	//cast back to unsigned int
	i=(unsigned int)r;
	j=(unsigned int)c;
	
};

template<int logSizeTable>
void SparseRandomGen<logSizeTable>::init_log_table(){
//16_bit table for logarithms
	
	for(int i=0; i<MAX_RAND; i++){
			SparseRandomGen::m_log[i]=log(i/(double)MAX_RAND);
	}
}

template<int logSizeTable>
int SparseRandomGen<logSizeTable>::create_ugraph (sparse_ugraph& g, int n, double p){
		
	BITBOARD m=((BITBOARD)n*(n-1)/2);			//max num edges
	BITBOARD e=0;								//**check
	double logp=log(1-p);
	BITBOARD skip;
	unsigned int i=0, j=0;					
	int num=0;									//optional index which counts edges added to the graph

	default_random_engine generator;
	uniform_int_distribution<int> distribution(0,MAX_RAND-1);
	generator.seed((unsigned) time(NULL));
	
	g.init(n);
	do{
		int l=ceil(m_log[distribution(generator)]/logp)-1; 
		(l>0)? skip=l : skip=0;
		if((e+=++skip)>=m) 
			break;

		decode(e,i,j,n);					//perfectly valid option and similar in O
		/*while(skip>0){
		if( (m_n-1-j)>=skip ){
		j+=skip;
		break;
		}else{
		i++;
		skip-=(m_n-1-j);
		j=i;
		}
		}*/

		num++;
		g.add_edge(i,j);					//O(log) because it is a sparse graph
		
	}while(e<m);

	//opional
	/*stringstream sstr("");
	sstr<<m<<":"<<"["<<num<<"]"<<std::endl;
	cout<<sstr.str();*/
	
return 0;
}


template<class Graph_t>
int RandomGen<Graph_t>::create_ugraph (Graph_t& g, int n, double p){
///////////////////////////
// Generates uniform simple random undirected graph  (no self loops)
// with name r<N>_0.<P>.txt
//
// RETURNS: -1 if ERROR, 0 if OK
// Notes: no self loops

	//ASSERTS
	if(n<=0){
		cerr<<"wrong size for random graph"<<std::endl;
		return -1;
	}

	if(g.init(n)){
		cerr<<"error during allcoation: undirected random graph not generated"<<std::endl;
		return -1;
	}


	//gen undirected edges
	for(int i=0; i<n-1; i++){
		for(int j=i+1; j<n; j++){
			if(UniformBoolean(p)){
				g.add_edge(i,j);
			}
		}
	}

	//name random
	std::stringstream sstr;
	sstr<<"r"<<n<<"_"<<"0."<<setprecision(2)<<100*p<<".txt";
	g.set_name(sstr.str());

return 0;
}

template<class Graph_t>
int RandomGen<Graph_t>::create_graph (Graph_t& g, int n, double p){
///////////////////////////
// Generates uniform simple random directed graph  (no self loops)
// with name r<N>_0.<P>.txt
//
// RETURNS: -1 if ERROR, 0 if OK
// Notes: no self loops

	//ASSERTS
	if(n<=0){
		cerr<<"wrong size for random graph"<<std::endl;
		return -1;
	}

	if(g.init(n)){
		cerr<<"error during allocation: directed random graph not generated"<<std::endl;
		return -1;
	}
	
	//gen undirected edges
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(i==j) continue;				//remove self-loops
			if(UniformBoolean(p)){
				g.add_edge(i,j);
			}
		}
	}

	//name random
	std::stringstream sstr;
	sstr<<"r"<<n<<"_"<<"0."<<setprecision(2)<<100*p<<".txt";
	g.set_name(sstr.str());

return 0;
}

//template<class Graph_t>
//void RandomGen<Graph_t>::append_slash_to_string(std::string & path){
////update path with "/" at the end (por POSIX compatibility) if required
//
//	size_t pos=path.find_last_of("\\/");
//	if(pos==path.length()-1){
//		if(path[pos]=='\\')
//			path.replace(pos,path.length(),"/");
//	}else path+='/';
//}

template<class Graph_t>
int RandomGen<Graph_t>::create_ugraph_benchmark (const string& path, const random_attr_t& rd){
///////////////////////////
// Generates full benchmark, each file with notation r<N>_0.<P>.txt
//
// REMARKS: could be written directly to the stream without using graph object (*** TO DO)

	std::ostringstream o;
	Graph_t g;
	std::ofstream f;
	
	//add slash '/' at the end to the path if required
	string mypath(path);
	com::dir::append_slash(mypath);
	

	for(int i=rd.nLB ; i<=rd.nUB; i+=rd.incN){
		for(double j=rd.pLB ; j<=rd.pUB; j+=rd.incP){
			for(int k=0; k<rd.nRep; k++){
				create_ugraph(g,i,j);
				o.str("");
				o<<mypath.c_str()<<"r"<<i<<"_"<<j<<"_"<<k<<".txt";		
				f.open(o.str().c_str());
				if(!f){
					cerr<<"Error al generar el archivo"<<endl;
					return -1;
				}
				g.write_dimacs(f);		
				f.close();
			}
		}
	}
return 0;		//OK
}

template<class Graph_t>
int RandomGen<Graph_t>::create_graph_benchmark (const string& path, const random_attr_t& rd){
///////////////////////////
// Generates full benchmark, each file with notation r<N>_0.<P>.txt
//
// REMARKS: could be written directly to the stream without using graph object 

	std::ostringstream o;
	Graph_t g;
	std::ofstream f;
	
	//add slash '/' at the end to the path if required
	string mypath(path);
	com::dir::append_slash(mypath);
	

	for(int i=rd.nLB ; i<=rd.nUB; i+=rd.incN){
		for(double j=rd.pLB ; j<=rd.pUB; j+=rd.incP){
			for(int k=0; k<rd.nRep; k++){
				create_graph(g,i,j);
				o.str("");
				o<<mypath.c_str()<<"r"<<i<<"_"<<j<<"_"<<k<<".txt";			
				f.open(o.str().c_str());
				if(!f){
					cerr<<"Error when opening file to write"<<endl;
					return -1;
				}
				g.write_dimacs(f);		
				f.close();
			}
		}
	}
return 0;		//OK
}








#endif