/*  
 * ugraph.h file from the GRAPH library, a C++ library for bit encoded 
 * simple unweighted graphs. GRAPH stores the adjacency matrix un full 
 * but each row is encoded as a bitsrting. GRAPH is at the core of  BBMC, a 
 * state of the art leading exact maximum clique algorithm. 
 * (see license file (legal.txt) for references)
 *
 * Copyright (C)
 * Author: Pablo San Segundo
 * Intelligent Control Research Group CAR(UPM-CSIC) 
 *
 * Permission to use, modify and distribute this software is
 * granted provided that this copyright notice appears in all 
 * copies, in source code or in binaries. For precise terms 
 * see the accompanying LICENSE file.
 *
 * This software is provided "AS IS" with no warranty of any 
 * kind, express or implied, and with no claim as to its
 * suitability for any purpose.
 *
 */



#ifndef __SIMPLE_UGRAPH_H__
#define __SIMPLE_UGRAPH_H__

#include "simple_graph.h"
#include "pablodev/utils/prec_timer.h"

template<class T>
class Ugraph : public Graph<T>{
public:
	typedef T bb_type;
	template <class B>
	friend bool operator == (const Ugraph<B>& lhs, const Ugraph<B>& rhs);

	Ugraph						():Graph<T>(){}									//does not allocate memory
	Ugraph						(int nVert):Graph<T>(nVert){}					//creates empty graph with size vertices	
	Ugraph						(const Ugraph& ug):Graph<T>(ug){}
	Ugraph						(std::string filename);		
    ~Ugraph						(){Graph<T>::clear();};

	void add_edge				(int v, int w);									//sets bidirected edge
	void remove_edge			(int v, int w);
	void remove_vertices		(const BitBoardN& bbn);

			
	//degree
	int degree					(int v)							const;
	int degree					(int v, const BitBoardN& bbn)	const;
	int degree					(int v, const BitBoardS& bbs)	const;
	int max_degree_of_graph		()								const;	
	int max_degree_of_subgraph	(T& sg)							const;

	BITBOARD number_of_edges	(bool lazy=true);								
	BITBOARD number_of_edges	(const BitBoardN& )				const;			//on induced graph by list of vertices

	//graph creation	
	int create_complement		(Ugraph& g)							;			//returns complement graph in g

	//density	
	double density				();
	
	//I/O
	void print_degrees			(std::ostream& = cout)			const;
	void print_edges			(std::ostream& = cout)			const;
	void print_data				(bool lazy=true, std::ostream& = cout);
		
	void write_dimacs			(ostream & );
	void write_EDGES			(ostream &);
	void write_mtx				(ostream &);
};


template <class T>
bool operator == (const Ugraph<T>& lhs, const Ugraph<T>& rhs){
	for(int i=0; i<lhs.m_size-1; i++)
		for(int j=i+1; j<lhs.m_size; j++){
			if( lhs.is_adjacent(i,j) && !rhs.is_adjacent(i,j) ){
				return false;
			}
		}
		return true;
}

template<class T>
Ugraph<T>::Ugraph(string filename):Graph<T>(){
	
	Graph<T>::set_graph(filename);								//must be called from Ugraph
}

template<class T>
void Ugraph<T>::add_edge (int v, int w){
///////////////
// sets v-->w (no self loops allowed)
	if(v!=w){
		Graph<T>::m_g[v].set_bit(w);
		Graph<T>::m_g[w].set_bit(v);
		Graph<T>::m_edges++;
	}
}

template<class T>
void Ugraph<T>::remove_edge (int v, int w){
///////////////
// removes bidirected edge
	if(v!=w){
		Graph<T>::m_g[v].erase_bit(w);
		Graph<T>::m_g[w].erase_bit(v);
		Graph<T>::m_edges--;
	}
}

template<class T>
int Ugraph<T>::max_degree_of_graph () const{
/////////////////////
// degree of graph 

	int max_degree=0, temp=0; 

	for(int i=0; i<Graph<T>::m_size;i++){
		temp=degree(i);
		if(temp>max_degree)
				max_degree=temp;
	}
return max_degree;
}

template<class T>
int Ugraph<T>::max_degree_of_subgraph (T& sg) const{
/////////////////////
// degree of subgraph 
	int max_degree=0, temp=0; 

	int v=EMPTY_ELEM;
	if(sg.init_scan(bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM){
		while(true){
			v=sg.next_bit();
			if(v==EMPTY_ELEM) break;

			//compute max_degree circumscribed to subgraph
			temp=degree(v, sg);
			if(temp>max_degree)
				max_degree=temp;
		}
	}
		
return max_degree;
}

template<class T>
void Ugraph<T>::print_degrees (std::ostream& o) const {
	for(int i=0; i<<Graph<T>::m_size; i++){
		o<<"deg("<<i<<")"<<":"<<degree(i)<<" ";
	}
}

template<class T>
BITBOARD Ugraph<T>::number_of_edges (const BitBoardN& bbn) const{
////////////////
// Reads upper triangle of the adjacency matrix
// Date of creation: 17/6/10 
// Last Updated: 12/4/12  
	
	unsigned long long  edges=0;
	for(int i=0; i<Graph<T>::m_size-1; i++){
		if(bbn.is_bit(i)){
			for(int j=i+1; j<Graph<T>::m_size; j++)
				if(bbn.is_bit(j)){
					if( Graph<T>::m_g[i].is_bit(j)) edges++;
				}
		}
	}
return edges;
}

template<class T>
double Ugraph<T>::density	()	{
	BITBOARD max_edges=Graph<T>::m_size;
	max_edges*=(max_edges-1);
	return (2*number_of_edges()/(double)max_edges);			
	
}


template<class T>
inline
BITBOARD Ugraph<T>::number_of_edges	(bool lazy) {
///////////////////
// Should be specialized for sparse graphs
	if(lazy && Graph<T>::m_edges !=0) 
				return Graph<T>::m_edges;

	BITBOARD  edges=0;
	for(int i=0; i<Graph<T>::m_size-1; i++){
		for(int j=i+1; j<Graph<T>::m_size; j++){
			if(Graph<T>::is_edge(i,j)){				//O(log) for sparse graphs 
				edges++;
			}
		}
	}
	Graph<T>::m_edges=edges;
	
return Graph<T>::m_edges;
}


template<> 
inline
BITBOARD Ugraph<sparse_bitarray>::number_of_edges	(bool lazy) {
/////////////
//Specialization for sparse graphs (is adjacent runs in O(log)) 
	if(lazy && Graph<sparse_bitarray>::m_edges !=0) 
				return Graph<sparse_bitarray>::m_edges;
	
	BITBOARD  edges=0;
	for(int i=0; i<Graph<sparse_bitarray>::m_size-1; i++){
		edges+=m_g[i].popcn64(i+1);		//O(log)
	}
	Graph<sparse_bitarray>::m_edges=edges;
	
return Graph<sparse_bitarray>::m_edges;
}

template<class T>
void Ugraph<T>::print_edges (std::ostream& o) const{
	for(int i=0; i<Graph<T>::m_size-1; i++){
		for(int j=i+1; j<Graph<T>::m_size; j++){
			if(Graph<T>::is_edge(i, j)){
				o<<"["<<i<<"]"<<"--"<<"["<<j<<"]"<<endl;
			}
		}
	}
}

template<class T>
void Ugraph<T>::remove_vertices (const BitBoardN& bbn){
//////////////////////////
// Experimental: inefficient implementation: allcating memory twice 
	Ugraph g;
	Graph<T>::remove_vertices(bbn,g);		//allocation 1
	(*this)=g;								//allocation 2	
}


template<class T>
inline
void Ugraph<T>::write_dimacs (ostream & o) {
/////////////////////////
// writes file in dimacs format with timestamp
		
	//timestamp 
	o<<"c File written by GRAPH:"<<PrecisionTimer::local_timestamp();

	//name
	if(!Graph<T>::m_name.empty())
		o<<"c "<<Graph<T>::m_name.c_str()<<endl;

	//tamaño del grafo
	o<<"p edge "<<Graph<T>::m_size<<" "<<number_of_edges()<<endl<<endl;
	
	//bidirectional edges
	for(int v=0; v<Graph<T>::m_size-1; v++){
		for(int w=v+1; w<Graph<T>::m_size; w++){
			if(Graph<T>::is_edge(v,w) )							//O(log) for sparse graphs: specialize
					o<<"e "<<v+1<<" "<<w+1<<endl;		//1 based vertex notation dimacs
		}
	}
}

template<>
inline
void Ugraph<sparse_bitarray>::write_dimacs (ostream& o) {
/////////////////////////
// writes file in dimacs format 
	
	//timestamp 
	o<<"c File written by GRAPH:"<<PrecisionTimer::local_timestamp();
	
	//name
	if(!Graph<sparse_bitarray>::m_name.empty())
		o<<"c "<<Graph<sparse_bitarray>::m_name.c_str()<<endl;

	//tamaño del grafo
	o<<"p edge "<<Graph<sparse_bitarray>::m_size<<" "<<number_of_edges()<<endl<<endl;
	
	//Escribir nodos
	for(int v=0; v<Graph<sparse_bitarray>::m_size-1; v++){
		//non destructive scan starting from the vertex onwards
		pair<bool, int> p=Graph<sparse_bitarray>::m_g[v].find_pos(WDIV(v));
		if(p.second==EMPTY_ELEM) continue;					//no more bitblocks
		Graph<sparse_bitarray>::m_g[v].m_scan.bbi=p.second; 
		(p.first)? Graph<sparse_bitarray>::m_g[v].m_scan.pos= WMOD(v) : Graph<sparse_bitarray>::m_g[v].m_scan.pos=MASK_LIM;		//if bitblock contains v, start from that position onwards
		while(1){
			int w=Graph<sparse_bitarray>::m_g[v].next_bit();
			if(w==EMPTY_ELEM)
					 break;
			o<<"e "<<v+1<<" "<<w+1<<endl;	
		}	
	
	}
}

template<class T>
inline
void  Ugraph<T>::write_EDGES(ostream& o){
/////////////////////////
// writes simple unweighted grafs  in edge list format 
// note: loops are not allowed
		
	//timestamp 
	o<<"% File written by GRAPH:"<<PrecisionTimer::local_timestamp();

	//name
	if(!Graph<T>::m_name.empty())
		o<<"% "<<Graph<T>::m_name.c_str()<<endl;
	
	//write edges
	for(int v=0; v<Graph<T>::m_size-1; v++){
		for(int w=v+1; w<Graph<T>::m_size; w++){
			if(Graph<T>::is_edge(v,w) )							//O(log) for sparse graphs: specialize
					o<<v+1<<" "<<w+1<<endl;						//1 based vertex notation dimacs
		}
	}
}


template<>
inline
void Ugraph<sparse_bitarray>::write_EDGES(ostream& o) {
/////////////////////////
// writes simple unweighted grafs  in edge list format 
// note: loops are not allowed
	
	//timestamp
	o<<"% File written by GRAPH:"<<PrecisionTimer::local_timestamp();
	
	//name
	if(!Graph<sparse_bitarray>::m_name.empty())
		o<<"% "<<Graph<sparse_bitarray>::m_name.c_str()<<endl;

	//writes edges
	for(int v=0; v<Graph<sparse_bitarray>::m_size-1; v++){
		//non destructive scan starting from the vertex onwards
		pair<bool, int> p=Graph<sparse_bitarray>::m_g[v].find_pos(WDIV(v));
		if(p.second==EMPTY_ELEM) continue;										//no more bitblocks
		Graph<sparse_bitarray>::m_g[v].m_scan.bbi=p.second; 
		(p.first)? Graph<sparse_bitarray>::m_g[v].m_scan.pos= WMOD(v) : Graph<sparse_bitarray>::m_g[v].m_scan.pos=MASK_LIM;			//if bitblock contains v, start from that position onwards
		while(1){
			int w=Graph<sparse_bitarray>::m_g[v].next_bit();
			if(w==EMPTY_ELEM)
					 break;
			o<<v+1<<" "<<w+1<<endl;	
		}	
	}
}

template<class T>
inline
void Ugraph<T>::write_mtx(ostream & o){
/////////////////////////
// writes simple unweighted grafs  in edge list format 
// note: loops are not allowed
		
	//header
	o<<"%%MatrixMarket matrix coordinate pattern symmetric"<<endl;
	
	//timestamp
	o<<"% File written by GRAPH:"<<PrecisionTimer::local_timestamp();
		
	//name
	if(!Graph<T>::m_name.empty())
		o<<"% "<<Graph<T>::m_name.c_str()<<endl;

	//size and edges
	Graph<T>::m_edges=0;																	//eliminates lazy evaluation of edge count 
	o<<Graph<T>::m_size<<" "<<Graph<T>::m_size<<" "<<number_of_edges()<<endl;
	
	//writes edges
	for(int v=0; v<Graph<T>::m_size-1; v++){
		for(int w=v+1; w<Graph<T>::m_size; w++){
			if(Graph<T>::is_edge(v,w) )							//O(log) for sparse graphs: specialize
					o<<v+1<<" "<<w+1<<endl;						//1 based vertex notation dimacs
		}
	}
}
	
template<>
inline
void Ugraph<sparse_bitarray>::write_mtx(ostream & o){
/////////////////////////
// writes simple unweighted grafs  in edge list format 
// note: loops are not allowed
		
	//header
	o<<"%%MatrixMarket matrix coordinate pattern symmetric"<<endl;
	
	//timestamp
	o<<"% File written by GRAPH:"<<PrecisionTimer::local_timestamp();
		
	//name
	if(!Graph<sparse_bitarray>::m_name.empty())
		o<<"% "<<Graph<sparse_bitarray>::m_name.c_str()<<endl;

	//size and edges
	m_edges=0;																	//eliminates lazy evaluation of edge count 
	o<<Graph<sparse_bitarray>::m_size<<" "<<Graph<sparse_bitarray>::m_size<<" "<<number_of_edges()<<endl;
	
	//writes edges
	for(int v=0; v<Graph<sparse_bitarray>::m_size-1; v++){
		//non destructive scan starting from the vertex onwards
		pair<bool, int> p=Graph<sparse_bitarray>::m_g[v].find_pos(WDIV(v));
		if(p.second==EMPTY_ELEM) continue;										//no more bitblocks
		Graph<sparse_bitarray>::m_g[v].m_scan.bbi=p.second; 
		(p.first)? Graph<sparse_bitarray>::m_g[v].m_scan.pos= WMOD(v) : Graph<sparse_bitarray>::m_g[v].m_scan.pos=MASK_LIM;			//if bitblock contains v, start from that position onwards
		while(1){
			int w=Graph<sparse_bitarray>::m_g[v].next_bit();
			if(w==EMPTY_ELEM)
					 break;
			o<<v+1<<" "<<w+1<<endl;	
		}	
	}	
}

template<class T>
void Ugraph<T>::print_data(bool lazy, std::ostream& o) {
	o<<Graph<T>::number_of_vertices()<<" "<<fixed<<number_of_edges(lazy)<<" "<<std::setprecision(6)<<density()<<" "<<Graph<T>::m_name.c_str()<<endl;
}


template<class T>
inline
int Ugraph<T>::degree (int v) const{
	return Graph<T>::m_g[v].popcn64();
}

template<>
inline
int Ugraph<sparse_bitarray>::degree (int v) const{
	if(Graph<sparse_bitarray>::m_g[v].is_empty()) return 0;

return Graph<sparse_bitarray>::m_g[v].popcn64();
}

template<class T>
inline
int Ugraph<T>::degree (int v, const BitBoardN& bbn) const	{
////////////////////
// degree of v considering only adjacent vertices in subgraph passed

	int ndeg=0;
	for(int i=0; i<Graph<T>::m_BB;i++)
		ndeg+=bitblock::popc64(Graph<T>::m_g[v].get_bitboard(i)& bbn.get_bitboard(i));

return ndeg;
}

template<>
inline
int Ugraph<sparse_bitarray>::degree (int v, const BitBoardN& bbn) const	{
////////////////////
// degree of v considering only adjacent vertices in subgraph passed
	
	int ndeg=0;
	for(sparse_bitarray::velem_cit it= m_g[v].begin(); it!=m_g[v].end(); ++it){
		ndeg+=bitblock::popc64(it->bb & bbn.get_bitboard(it->index));
	}

return ndeg;
}

template<>
inline
int Ugraph<sparse_bitarray>::degree (int v, const BitBoardS & bbs) const	{
////////////////////
// degree of v considering only adjacent vertices in subgraph passed

	int ndeg=0;
	sparse_bitarray::velem_cit itv = m_g[v].begin();
	sparse_bitarray::velem_cit itbb = bbs.begin();
	
	while(itv!=m_g[v].end() && itbb!=bbs.end()){

		if(itv->index<itbb->index){ 
			itv++;
		}else if(itv->index>itbb->index){
			itbb++;
		}else{ //same index
			ndeg+=bitblock::popc64(itv->bb & itbb->bb);
			itv++; itbb++;
		}
	}
	return ndeg;
}


template<class T>
int Ugraph<T>::create_complement (Ugraph& ug)	{
/////////////////////////
// computes complement graph in ug (includes memory allocation memory)
//
// RETURNS: complement graph in ug, -1 if ERROR, O if OK
	
	//memory allocation of new complement graph
	if( ug.init(Graph<T>::m_size)==-1 ) return -1;

	for(int i=0; i<Graph<T>::m_size; i++){
		for(int j=0; j<Graph<T>::m_size; j++){
			if(!Graph<T>::m_g[i].is_bit(j)){
				ug.add_edge(i,j);
			}
		}
	}
	return 0;
}
#endif






