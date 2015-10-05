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
				
	//degree
	int degree_sort				(vint::iterator first, vint::iterator last);	
	int degree_sort				(bitarray& bb, vint::iterator from);	
	int degree					(vint::iterator first, vint::iterator last);	
	int degree					(bitarray& bb);	

	
    void reset_deg					();				
	
//functor for absolute sorting in decreasing order of degree
	bool operator()(int v, int w){
		 return (m_vdeg[v]<m_vdeg[w]);
	}

////////////
//the graph
	ugraph& m_g;				
	vint m_vdeg;				//init to 0 on construction
};

inline
void DegUg::reset_deg(){
///////////
// selectively initializes degree vector to correct size and 0 values
	int gsize=m_g.number_of_vertices();
	if(gsize!=m_vdeg.size()){
		m_vdeg.assign(m_g.number_of_vertices(),0);
	}
}
inline
int DegUg::degree_sort(vint::iterator first, vint::iterator last){

	degree(first, last);					//degrees are computed from scratch
	sort(first, last, *this);
	
return 0;
}
	
inline
int DegUg::degree_sort(bitarray& bb, vint::iterator from){
//////////////
// computes degree sort in subgraph bb and copies the new order 
// starting in from
// RETURNS -1 if error, 0 if ok	
//
// Not optimized
	
//compute degrees from scratch
	degree(bb);

	vint lv;
	bb.to_vector(lv);
	std::sort(lv.begin(), lv.end(),*this);
	copy(lv.begin(), lv.end(), from);

return 0;
}

inline 
int DegUg::degree(bitarray& sg){

	//updates size of degree vector if necessary
	reset_deg();	
	
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
int DegUg::degree(vint::iterator first, vint::iterator last){
//compute degrees from scratch
//returns -1 if ERROR, 0 if OK

	m_vdeg.assign(m_g.number_of_vertices(), 0);		//obligatory

	//loops over all elements 
	for(vint::iterator begin=first; begin!=last; begin++){
		for(vint::iterator begin_2=first; begin_2!=last; begin_2++){
			if(m_g.get_neighbors(*begin).is_bit(*begin_2)){
				m_vdeg[*begin]+=1;
			}
		}
	}
	
return 0;
}

#endif






