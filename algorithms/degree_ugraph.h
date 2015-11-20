/*  
 * degreeh_ugraph.h file from the GRAPH library, a C++ library for bit encoded 
 * simple unweighted graphs. Computes degree related operations for UNDIRECTED
 * NON-SPARSE graphs, specifically SORTING
 *
 * Copyright (C)
 * Author: Pablo San Segundo
 * date of creation: 5/10/2015
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
 * EXPERIMENTAL-WILL CHANGE IN THE FUTURE
 */

#ifndef __DEGREE_H__
#define __DEGREE_H__

#include "../graph.h"
#include "../simple_ugraph.h"
#include "pablodev/utils/common.h"

#include <vector>
#include <algorithm>	

using namespace std; 

typedef vector<int> vint;

class DegUg{
public:
	
	DegUg						(ugraph& ug):m_g(ug), m_vdeg(m_g.number_of_vertices(),0){}
	~DegUg						(){};
				
	int degree_sort				(int* first, int* end);	
	int degree_sort				(bitarray& bb, int* to);	
	int degree_stable_sort		(int* first, int* end);
	int degree_stable_sort		(bitarray& bb, int* to);

	int degree_sort				(int* first, int* end, int* from);					//set of vertices to order [first, end[; subgrah relevant for degrees [from, end[ 		
	

	//functor for sorting criteria (change for specific sorting)
	bool operator()(int v, int w) const{
		 return (m_vdeg[v]>m_vdeg[w]);
	}

private:
	int degree					(int* first, int* end);		
	int degree					(int* first, int* end, int* from);	
	int degree					(bitarray& bb);							

////////////
//the graph
	ugraph& m_g;				
	vint m_vdeg;				//initialized to 0 upon construction
};


inline
int DegUg::degree_sort(int* first, int* end){
	degree(first, end);					//degrees are computed from scratch
	sort(first, end, *this);
return 0;
}

inline
int DegUg::degree_sort	(int* first, int* end, int* from){
//////////////////
// degrees are based on subgraph [from, end[
// vertex set sorted [first, end[
	degree(first, end, from);					//degrees are computed from scratch
	sort(first, end, *this);
return 0;
}
	
inline
int DegUg::degree_sort(bitarray& bb, int* to){
//////////////
// computes degree sort in subgraph bb and copies the new order 
// in "to"
// RETURNS -1 if error, 0 if ok	
//
// TODO: experimental, needs optimization
	
//compute degrees from scratch
	degree(bb);

	vint lv;
	bb.to_vector(lv);
	sort(lv.begin(), lv.end(),*this);
	copy(lv.begin(), lv.end(), to);

return 0;
}

int DegUg::degree_stable_sort	(int* first, int* end){
	degree(first, end);											//degrees are computed from scratch
	stable_sort(first, end, *this);
return 0;
}
int DegUg::degree_stable_sort	(bitarray& bb, int* to){
//////////////
// computes degree sort in subgraph bb and copies the new order
// in "to"
// RETURNS -1 if error, 0 if ok
//
// TODO: experimental, needs optimization

//compute degrees from scratch
	degree(bb);

	vint lv;
	bb.to_vector(lv);
	stable_sort(lv.begin(), lv.end(),*this);
	copy(lv.begin(), lv.end(), to);

return 0;
}

inline 
int DegUg::degree(bitarray& sg){
//////////////
// computes degree sort in subgraph bb and copies the new order 
// in "to"
// RETURNS -1 if error, 0 if ok	
//
// TODO: experimental, needs optimization

	//iterates over all vertices
	int v=EMPTY_ELEM;
	if(sg.init_scan(bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM){
		while(true){
			v=sg.next_bit();
			if(v==EMPTY_ELEM) break;

			//compute max_degree circumscribed to subgraph
			m_vdeg[v]=m_g.degree(v, sg);
		}
	}
return 0;
}

inline 
int DegUg::degree(int* first, int* end){
//compute degrees from scratch in O(n*n) 
//returns -1 if ERROR, 0 if OK

	fill(m_vdeg.begin(), m_vdeg.end(), 0);	

	//loops over all elements 
	for(int* f1=first; f1!=end; f1++){
		for(int* f2=first; f2!=end; f2++){
			if(m_g.get_neighbors(*f1).is_bit(*f2)){
				m_vdeg[*f1]+=1;
			}
		}
	}
return 0;
}

inline
int DegUg::degree (int* first, int* end, int* from){
	
	fill(m_vdeg.begin(), m_vdeg.end(), 0);	

	//loops over all elements 
	for(int* f1=first; f1!=end; f1++){
		for(int* f2=from; f2!=end; f2++){
			if(m_g.get_neighbors(*f1).is_bit(*f2)){
				m_vdeg[*f1]+=1;
			}
		}
	}
return 0;
}

#endif






