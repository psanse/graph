//graph_sort.h: header for GraphSort class which sorts graphs by different criteria
//date: 20/11/15
//authors: pss

#ifndef __GRAPH_SORT_H__
#define __GRAPH_SORT_H__

#include "../graph.h"
#include "../kcore.h"
#include "filter_graph_sort_type.h"			//Template Graph_t reduced to undirected types
#include "pablodev/utils/logger.h"
#include "pablodev/utils/common.h"
#include "decode.h"
#include <iostream>
#include <algorithm>


using namespace std;

//vertex neighborhood info
struct deg_t{
	friend ostream & operator<<(ostream& o, const deg_t& d){ o<<d.index<<":("<<d.deg<<","<<d.deg_of_n<<")"; return o;}
	deg_t():index(EMPTY_ELEM), deg(0), deg_of_n(0){}
	int index;
	int deg;
	int deg_of_n;
};

typedef std::vector<int>				vint;	

///////////////////////////
//
// GraphSort class
// (Graph_t restricted to ugraph and sparse_ugraph)
//
////////////////////////////

template <class Graph_t>
class GraphSort: public filterGraphSortType<Graph_t>{
	static const int GRAPH_SORT_INFINITE=0x1FFFFFFF;
protected:
	typedef vector< deg_t >					vdeg;							
	typedef vdeg::iterator					vdeg_it;
		
	//sorting criteria
	struct degreeLess: 
		public binary_function<deg_t, deg_t, bool>{
			bool operator() (deg_t i,deg_t j) const { 
				return (i.deg<j.deg);
			} 
	};

	struct degreeWithTieBreakLess: 
		public binary_function<deg_t, deg_t, bool>{
			bool operator() (deg_t i,deg_t j) const {
				if(i.deg<j.deg) return true;
				else if(i.deg==j.deg){
					if(i.deg_of_n<j.deg_of_n) return true;
				}
				return false;
			} 
	};

public:
	enum sort_t						{MIN_DEG_DEGEN=0, MAX_DEG_DEGEN, MIN_DEG_DEGEN_TIE_STATIC, KCORE, NONE};
	enum place_t					{PLACE_FL=0, PLACE_LF};
	enum pick_t						{PICK_MINFL=0, PICK_MINLF, PICK_MAXFL, PICK_MAXLF, PICK_FL, PICK_LF, PICK_MINABSFL,PICK_MAXABSFL, PICK_MINABSLF, PICK_MAXABSLF};

typedef vector< pair<sort_t, place_t> >				vpair;
typedef typename vpair::iterator					vpair_it;

	static void print				(const vint& order, bool revert=false, ostream& o=std::cout);			
	GraphSort						(Graph_t& gout):g(gout){}

//reorders graph (it is CHANGED)
	int reorder						(const vint& new_order, ostream* o = NULL);							   
	int reorder_in_place			(const vint& new_order, ostream* o=NULL);
	int reorder						(const vint& new_order, Decode& d,  ostream* o = NULL);
	int reorder_in_place			(const vint& new_order, Decode& d, ostream* o = NULL);

//computes a reordering [OLD_INDEX]=NEW_INDEX
	vint new_order					(sort_t alg, place_t=PLACE_LF);																						//use by default

	//iterative variants (use get_v)
	vint new_order					(pick_t, place_t=PLACE_LF);	
	vint new_order					(const typename Graph_t::bb_type& sg, pick_t, place_t=PLACE_LF);													//reorders induced subgrapg sg
	vint new_order					(const typename Graph_t::bb_type& sgfrom, const typename Graph_t::bb_type& sgref, pick_t, place_t, bool is_degen);	
private:
	vint new_order_kcore			(place_t=PLACE_LF);

public:
//computes a reordering of the subgraph not accesible by vertex index
	vint new_subg_order				(sort_t, typename Graph_t::bb_type&,  place_t=PLACE_LF);					//cannot be used as input to REORDER functions
	
///////////////
//composite orderings
	int reorder_composite			(vpair&, Decode& d,  ostream* o = NULL);
	
///////////////
// degree computation 
	int sum_of_neighbor_deg			(int v);																	//computes support(sum of degree of neighbors)
	int sum_of_neighbor_deg			(int v, const typename Graph_t::bb_type& subgraph);

////////////////
// vertex selection primitives
	int get_v						(pick_t=PICK_MINFL);	
	int	get_v						(typename Graph_t::bb_type& sg, pick_t=PICK_MINFL);	
	int get_v						(typename Graph_t::bb_type& sgfrom, const typename Graph_t::bb_type& sgref, pick_t=PICK_MINFL);

protected:
////////////////
// data members
	
	Graph_t& g;																									
};

template<class Graph_t>
int GraphSort<Graph_t>::sum_of_neighbor_deg(int v){
/////////////////////////
//Sum of degrees of neighbors to v in the current graph considered

	int ndeg=0,vadj=EMPTY_ELEM;
	if(g.get_neighbors(v).init_scan(BBObject::NON_DESTRUCTIVE)!=EMPTY_ELEM){
		while(true){
			vadj=((g.get_neighbors(v)).next_bit());
			if(vadj==EMPTY_ELEM) break;
			ndeg+=g.degree(vadj);
		}
	}
return ndeg;
}

template<class Graph_t>
int GraphSort<Graph_t>::sum_of_neighbor_deg(int v, const typename Graph_t::bb_type& sg){
/////////////////////////
//Sum of degrees of neighbors to v in the current graph considered,circumscribed to sg

	int ndeg=0,vadj=EMPTY_ELEM;
	typename Graph_t::bb_type nset(g.number_of_vertices());
	AND(sg, g.get_neighbors(v), nset);
	if(nset.init_scan(BBObject::NON_DESTRUCTIVE)!=EMPTY_ELEM){
		while(true){
			vadj=nset.next_bit();
			if(vadj==EMPTY_ELEM) break;
			ndeg+=g.degree(vadj, sg);
		}
	}
return ndeg;
}

template<class Graph_t>
void GraphSort<Graph_t>::print(const std::vector<int>& new_order, bool revert, ostream& o){
	o<<"new order: ";
	if(revert){
		copy(new_order.rbegin(), new_order.rend(), ostream_iterator<int>(o, " "));
	}else{
		copy(new_order.begin(), new_order.end(), ostream_iterator<int>(o, " "));
	}
o<<endl;
}

template<class Graph_t>
inline
int GraphSort<Graph_t>::reorder(const vint& new_order, ostream* o){
/////////////////////
// reordering in place
// new order logs to "o"
//
// REMARKS: 
// 1-Experimental (uses auxiliary graph: should be done truly in place)
// 2-only for undirected graphs

	//control
	if(new_order.size()!=g.number_of_vertices()){
		LOG_ERROR("reorder: cannot reorder graph");
		return -1;
	}

	int size=g.number_of_vertices();
	Graph_t gn(size);
	gn.set_name(g.get_name());
	
	//only for undirected graphs
	for(int i=0; i<size-1; i++){
		for(int j=i+1; j<size; j++){
			if(g.is_edge(i,j)){								//in O(log) for sparse graphs, should be specialized for that case
				//switch edges according to new numbering
				gn.add_edge(new_order[i], new_order[j]);
			}
		}
	}
	g=gn;
			
	//new order to stream if available
	if(o!=NULL)
		copy(new_order.begin(), new_order.end(), ostream_iterator<int>(*o, " "));
	
return 0;		
}

template<class Graph_t>
inline
int GraphSort<Graph_t>::reorder (const vint& new_order, Decode& d,  ostream* o){
/////////////////////
// reordering in place and stores a way of decoding the new vertex indexes 
// new order logs to "o"
//
// REMARKS: 
// 1-Experimental (uses auxiliary graph: should be done truly in place)
// 2-only for undirected graphs
// 3-Assumes Decode state is properly updated before the call


	//control
	if(new_order.size()!=g.number_of_vertices()){
		LOG_ERROR("reorder: cannot reorder graph");
		return -1;
	}

	int size=g.number_of_vertices();
	Graph_t gn(size);
	gn.set_name(g.get_name());
	
	//only for undirected graphs
	for(int i=0; i<size-1; i++){
		for(int j=i+1; j<size; j++){
			if(g.is_edge(i,j)){								//in O(log) for sparse graphs, should be specialized for that case
				//switch edges according to new numbering
				gn.add_edge(new_order[i], new_order[j]);
			}
		}
	}
	g=gn;
	
	//decode
	vint aux(new_order);
	Decode::reverse_in_place(aux);				//changes to [NEW_INDEX]=OLD_INDEX
	d.insert_ordering(aux);


	//new order to stream if available
	if(o!=NULL)
		copy(new_order.begin(), new_order.end(), ostream_iterator<int>(*o, " "));
	
return 0;		
}

template<>
inline
int GraphSort<sparse_ugraph>::reorder(const vint& new_order, ostream* o){
/////////////////////
// specialization for sparse graphs
//
// REMARKS: 
// 1-Experimental (uses auxiliary graph: should be done truly in place)
// 2-only for undirected graphs



	//control
	if(new_order.size()!=g.number_of_vertices()){
		LOG_ERROR("reorder: cannot reorder sparse undirected graph");
		return -1;
	}

	int size=g.number_of_vertices();
	sparse_ugraph gn(size);
	gn.set_name(g.get_name());
	
	//only for undirected graphs
	int j=EMPTY_ELEM;
	for(int i=0; i<size-1; i++){
		sparse_bitarray neigh=g.get_neighbors(i);
		if(neigh.init_scan_from(i, bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM){
			while(true){
				j=neigh.next_bit();
				if(j==EMPTY_ELEM)
						break;
				gn.add_edge(new_order[i], new_order[j]);
			}
		}
	}
	g=gn;
			
	//new order to stream if available
	if(o!=NULL)
		copy(new_order.begin(), new_order.end(), ostream_iterator<int>(*o, " "));
	
return 0;		
}

template<>
inline
int GraphSort<sparse_ugraph>::reorder(const vint& new_order, Decode& d, ostream* o){
/////////////////////
// specialization for sparse graphs with decoding
//
// REMARKS: 
// 1-Experimental (uses auxiliary graph: should be done truly in place)
// 2-only for undirected graphs


	//control
	if(new_order.size()!=g.number_of_vertices()){
		LOG_ERROR("reorder: cannot reorder sparse undirected graph");
		return -1;
	}

	int size=g.number_of_vertices();
	sparse_ugraph gn(size);
	gn.set_name(g.get_name());
	
	//only for undirected graphs
	int j=EMPTY_ELEM;
	for(int i=0; i<size-1; i++){
		sparse_bitarray neigh=g.get_neighbors(i);
		if(neigh.init_scan_from(i, bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM){
			while(true){
				j=neigh.next_bit();
				if(j==EMPTY_ELEM)
						break;
				gn.add_edge(new_order[i], new_order[j]);
			}
		}
	}
	g=gn;

	
	//decode
	vint aux(new_order);
	Decode::reverse_in_place(aux);				//changes to [NEW_INDEX]=OLD_INDEX
	d.insert_ordering(aux);

			
	//new order to stream if available
	if(o!=NULL)
		copy(new_order.begin(), new_order.end(), ostream_iterator<int>(*o, " "));
	
return 0;		
}

template<class Graph_t>
inline
int GraphSort<Graph_t>::reorder_composite (vpair& lord, Decode& d,  ostream* o = NULL){
//////////////////
// iterates over the list and reorders the graph accordingly
// stores in decoder each change in format [NEW_INDEX]=OLD_INDEX
// for a later decoding
//
// RETURNS -1 if ERROR, 0 if OK

	d.clear();
	
	for(vpair_it it=lord.begin(); it!=lord.end(); it++){
		pair<sort_t, place_t> ord=*it;

		//sort
		switch(ord.first){
		case MIN_DEG_DEGEN:
			reorder(new_order(MIN_DEG_DEGEN,ord.second),d,o);
			break;
		case MAX_DEG_DEGEN:
			reorder(new_order(MAX_DEG_DEGEN,ord.second),d,o);
			break;
		case MIN_DEG_DEGEN_TIE_STATIC:
			reorder(new_order(MIN_DEG_DEGEN_TIE_STATIC,ord.second),d,o);
			break;
		case NONE:
			reorder(new_order(NONE,ord.second),d,o);
			break;
		//others
		default:
			LOG_ERROR("GraphSort::reorder_composite: unknown algorithm");
			return -1;
		}
	}

	return 0;	//OK
}



template<>
inline
int GraphSort<ugraph>::reorder_in_place(const vint& new_order, ostream* o){
	struct this_type_is_not_available_for_GraphSort;
	return 0;
}

template<>
inline
int GraphSort<ugraph>::reorder_in_place(const vint& new_order,  Decode& d,  ostream* o){
	struct this_type_is_not_available_for_GraphSort;
	return 0;
}

template<>
inline
int GraphSort<sparse_ugraph>::reorder_in_place(const vint& new_order, ostream* o){
/////////////////////
// Reorders graph in place (only for large sparse UNDIRECTED graphs)
// date: 22/12/14
// author: Alvaro Lopez
//
// COMMENTS: Opimmize for space requirements

	//control
	int N=g.number_of_vertices();
	if(new_order.size()!=N){
        LOG_ERROR("reorder_in_place: cannot reorder sparse undirected graph");
        return -1;
    }
	   	
	//Deletes lower triangle of adjacency matrix
	LOG_DEBUG("deleting low triangle--------------------");
    for(int i=0; i<N; i++){
        g.get_neighbors(i).clear_bit(0,i);
		g.get_neighbors(i).shrink_to_fit();
	}
		
	LOG_DEBUG("new order upper to lower triangle--------------");		
	sparse_bitarray neigh;
    int j=EMPTY_ELEM;
    for(int i=0; i<N; i++){
        neigh=g.get_neighbors(i);
		//reorders using upper triangle information
        if(neigh.init_scan_from(i, bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM){
		    while(true){
                j=neigh.next_bit();
                if(j==EMPTY_ELEM)
                        break;
			
				//writes new edge in lower triangle
                if(new_order[i]>new_order[j]){
					g.get_neighbors(new_order[i]).set_bit(new_order[j]);
				} else{
                    g.get_neighbors(new_order[j]).set_bit(new_order[i]);
				}
            }
        }
		//Deletes each neighborhood once read
		g.get_neighbors(i).clear_bit(i, N-1);
		g.get_neighbors(i).shrink_to_fit();
    }

    //Makes the graph bidirected: copies in54.-51kl,.5formation from lower to upper triangle
	LOG_DEBUG("making graph bidirected--------------------");	
    for(int i=0; i<N; i++){
        neigh=g.get_neighbors(i);
        if(neigh.init_scan(bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM){ 
            while(true){
                j=neigh.next_bit();
                if((j==EMPTY_ELEM) || (j>i))
                        break;
				g.get_neighbors(j).set_bit(i);
	        }
        }
    }
	   
    //new order to stream if available
    if(o!=NULL)
        copy(new_order.begin(), new_order.end(), ostream_iterator<int>(*o, " "));
  return 0;        
}

template<>
inline
int GraphSort<sparse_ugraph>::reorder_in_place(const vint& new_order, Decode& d, ostream* o){
/////////////////////
// Reorders graph in place and stores a way of decoding the vertices (only for large sparse UNDIRECTED graphs)
// date: 22/12/14
// author: Alvaro Lopez
//
// COMMENTS: Opimmize for space requirements


    //control
	int N=g.number_of_vertices();
	if(new_order.size()!=N){
        LOG_ERROR("reorder_in_place: cannot reorder sparse undirected graph");
        return -1;
    }
	   	
	//Deletes lower triangle of adjacency matrix
	LOG_DEBUG("deleting low triangle--------------------");
    for(int i=0; i<N; i++){
        g.get_neighbors(i).clear_bit(0,i);
		g.get_neighbors(i).shrink_to_fit();
	}
		
	LOG_DEBUG("new order upper to lower triangle--------------");		
	sparse_bitarray neigh;
    int j=EMPTY_ELEM;
    for(int i=0; i<N; i++){
        neigh=g.get_neighbors(i);
		//reorders using upper triangle information
        if(neigh.init_scan_from(i, bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM){
		    while(true){
                j=neigh.next_bit();
                if(j==EMPTY_ELEM)
                        break;
			
				//writes new edge in lower triangle
                if(new_order[i]>new_order[j]){
					g.get_neighbors(new_order[i]).set_bit(new_order[j]);
				} else{
                    g.get_neighbors(new_order[j]).set_bit(new_order[i]);
				}
            }
        }
		//Deletes each neighborhood once read
		g.get_neighbors(i).clear_bit(i, N-1);
		g.get_neighbors(i).shrink_to_fit();
    }

    //Makes the graph bidirected: copies in54.-51kl,.5formation from lower to upper triangle
	LOG_DEBUG("making graph bidirected--------------------");	
    for(int i=0; i<N; i++){
        neigh=g.get_neighbors(i);
        if(neigh.init_scan(bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM){ 
            while(true){
                j=neigh.next_bit();
                if((j==EMPTY_ELEM) || (j>i))
                        break;
				g.get_neighbors(j).set_bit(i);
	        }
        }
    }

	//decode info
	vint aux(new_order);
	Decode::reverse_in_place(aux);				//changes to [NEW_INDEX]=OLD_INDEX
	d.insert_ordering(aux);
	   
    //new order to stream if available
    if(o!=NULL)
        copy(new_order.begin(), new_order.end(), ostream_iterator<int>(*o, " "));
  return 0;    
}

template<typename Graph_t>
vint GraphSort<Graph_t>::new_order (sort_t alg, place_t place)
{
/////////////////////////////
// Sorts vertices by different strategies always picking them by non-decreasing index
// PARAMS: LF (last to first) TRUE places each vertex taken at the end of the new order; if  FALSE at the beginning
//         LF=TRUE is the degeneracy ordering (reverse min-width ordering)
//
// As usual new order[OLD_INDEX]=NEW_INDEX
// RETURNS: Empty vertex set if ERROR, else new ordering
//
// REMARKS
// 1.Had to make tie-breaks more efficient (28/8/14)
// 2.There was a lot to do! Basically degrees with respect to the vertex removed and support can be recomputed over the updated degrees.
	
	
	//KCORE ordering special case: does not requiere degree computation
	if(alg==KCORE){
		return new_order_kcore(place);
	}
			
	//remaining cases: all require explicit degree computation
	const int NV=g.number_of_vertices();
	vint new_order(NV);
	vdeg degs;	
	int k; 
	(place==PLACE_LF)? k=g.number_of_vertices()-1 : k=0;
	
	//computes degree of vertices
	for(int i=0; i<NV; i++){
		deg_t vt;
		vt.index=i;
		vt.deg=g.degree(i);
		if(alg==MIN_DEG_DEGEN_TIE_STATIC)
				vt.deg_of_n=sum_of_neighbor_deg(vt.index);
		degs.push_back(vt);		
	}

		
	//computes order
	BitBoardN bbn(NV);
	bbn.set_bit(0,NV-1);
	switch(alg){
	case MIN_DEG_DEGEN:
		while(!degs.empty()){
			vdeg_it it1=min_element(degs.begin(), degs.end(), degreeLess());
			new_order[it1->index]=k;
			(place==PLACE_LF)? k-- : k++;
			bbn.erase_bit(it1->index);
			degs.erase(it1);

			//recompute degrees
			for(int i=0; i<degs.size(); i++){
				degs[i].deg=g.degree(degs[i].index, bbn);
			}
		}
		break;
	case MAX_DEG_DEGEN:
		while(!degs.empty()){
			vdeg_it it1=max_element(degs.begin(), degs.end(), degreeLess());
			new_order[it1->index]=k;
			(place==PLACE_LF)? k-- : k++;
			bbn.erase_bit(it1->index);
			degs.erase(it1);

			//recompute degrees
			for(int i=0; i<degs.size(); i++){
				degs[i].deg=g.degree(degs[i].index, bbn);
			}
		}
		break;
	case MIN_DEG_DEGEN_TIE_STATIC:
		while(!degs.empty()){
			vdeg_it it_sel=min_element(degs.begin(), degs.end(), degreeWithTieBreakLess());
			int v_sel=it_sel->index;
			new_order[v_sel]=k;
			(place==PLACE_LF)? k-- : k++;
			bbn.erase_bit(v_sel);
			degs.erase(it_sel);
							
			//recompute degrees 
			for(int i=0; i<degs.size(); i++){
				degs[i].deg=g.degree(degs[i].index,bbn);
			}
		}
		break;
	case NONE:							//to implement reverse ordering
		//warning for petition in which vertex order remains as is but is computed nevertheless
		if(place==PLACE_FL){
			LOG_WARNING("GraphSort<Graph_t>::new_order: NONE + PLACE_FL->order unchanged but will be processed");
		}

		for(int i=0; i<new_order.size(); i++){
			new_order[i]=k;
			(place==PLACE_LF)? k-- : k++;
		}
		break;
	default:
		LOG_ERROR("GraphSort<Graph_t>::new_order: unknown ordering strategy");
		vint().swap(new_order);
	}
return new_order;
}

template<typename Graph_t>
vint GraphSort<Graph_t>::new_order (pick_t pick, place_t place){
//////////////////////
// Determines new order in format [OLD_VERTEX]= NEW_VERTEX incrementally (vertex by vertex)
// using the primitives for vertex selection

	int v_sel=EMPTY_ELEM; int k=0;
	int NV=g.number_of_vertices();
	vint res(NV);
	typename Graph_t::bb_type sg(NV);
	sg.set_bit(0, NV-1);
	
	(place==PLACE_LF)?  k=NV-1 : k=0;
	while(true){
		v_sel=get_v(sg, pick);
		if(v_sel==EMPTY_ELEM) break;
		res[v_sel]=k;
		(place==PLACE_LF)?  k-- : k++;
	sg.erase_bit(v_sel);
	}
	
return res;
}

template<typename Graph_t>
vint GraphSort<Graph_t>::new_order	(const typename Graph_t::bb_type& sg_in, pick_t pick, place_t place){
////////////////////////
// Reorders INDUCED SUBGRAPH by sg_in (returns a full ordering which may be used in a reorder operation)

	int v_sel=EMPTY_ELEM; int k=0;
	int NV=g.number_of_vertices();
	vint res;
	typename Graph_t::bb_type sg(sg_in);
	
	//inits vector with current order
	for(int i=0; i<NV; i++){
		res.push_back(i);
	}

	//get vertex list to reorder
	vint vsg;
	sg.to_vector(vsg);

	(place==PLACE_LF)?  k=vsg.size()-1 : k=0;
	while(1){
		v_sel=get_v(sg, pick);
		if(v_sel==EMPTY_ELEM) break;
		(place==PLACE_LF)?  res[v_sel]=vsg[k--] : res[v_sel]=vsg[k++];			//swaps related vertices
	sg.erase_bit(v_sel);
	}

return res;
}

template<typename Graph_t>
vint GraphSort<Graph_t>::new_order (const typename Graph_t::bb_type& sgfrom_in, const typename Graph_t::bb_type& sgref_in, pick_t pick, place_t place, bool is_degen){
///////////////////////
// Reorders, by the usual criteria, vertices in sg in connection to subgraph INDUCED by sgref
// Typically sgref should include sg but this is not strictly neccessary
// 
// PARAMS
//  BOOL is_degen: TRUE removes vertex selected in sg from sgref after being picked 
//				    User case for non degenerate (is_degen=FALSE) is a max or min degree ordering which takes 
//					into account all vertices in each choice.
//
// User case: reorder a subset of vertices using global structure criteria

	int v_sel=EMPTY_ELEM; int k=0;
	int NV=g.number_of_vertices();
	vint res;
	typename Graph_t::bb_type sgfrom(sgfrom_in);
	typename Graph_t::bb_type sgref(sgref_in);
	
	//inits vector with current order
	for(int i=0; i<NV; i++){
		res.push_back(i);
	}

	//get vertex list to reorder
	vint vsg;
	sgfrom.to_vector(vsg);

	(place==PLACE_LF)?  k=vsg.size()-1 : k=0;
	while(true){
		v_sel=get_v(sgfrom,sgref,pick);  
		if(v_sel==EMPTY_ELEM) break;
		(place==PLACE_LF)?  res[v_sel]=vsg[k--] : res[v_sel]=vsg[k++];			//swaps related vertices
		sgfrom.erase_bit(v_sel);
		if(is_degen) sgref.erase_bit(v_sel);									//implements degeneracy by modifying the reference induced subgraph
	}

	return res;
}

template<typename Graph_t>
vint GraphSort<Graph_t>::new_order_kcore (place_t place){
/////////////////////
// Ret
	int nV=g.number_of_vertices();
	vint new_order(nV);

	KCore<Graph_t> kc(g);
	kc.kcore();
	const vint& kco=kc.get_kcore_ordering();		//ordered by non decreasing
	
////////////////
//translates to [OLD_INDEX]=NEW_INDEX
	int l=0;
	if(place==PLACE_LF){				//the standard use in clique
		for(vint::const_reverse_iterator it=kco.rbegin(); it!=kco.rend(); ++it){
			new_order[*it]=l++;
		}
	}else{
		//PLACE_FL
		LOG_WARNING("new_order_kcore: non typical ordering by increasing kcore");
		for(vint::const_iterator it=kco.begin(); it!=kco.end(); ++it){
			new_order[*it]=l++;
		}
	}

	return new_order;
}

template<typename Graph_t>
vint GraphSort<Graph_t>::new_subg_order (sort_t alg, typename Graph_t::bb_type& sg,  place_t place){
/////////////////
// Returns a list of vertices in a subgraph (as a bitstring of vertices) ordered by non decreasing param-alg criteria
// CANNOT be used in REORDER functions
//
// date of creation: 9/3/15
// date of refactoring: 21/11/15
//
// On ERROR: The empty graph
//
// COMMENTS: **EXPERIMENTAL***
// 1-All sorting criteria are referred solely to the subgraph passed
// 2-NOT OPTIMIZED (sparse_graph specialization, erase elements in vectors, recompute degree at each iteration)
// 3-The returned list CANNOT be accessed by vertex index

// TODO: Other possible orderings (absolute and degenerate)

	deg_t vt;
	vdeg degs;
	
	//consistency check
	if(sg.is_empty()){
		LOG_WARNING("new_subg_order: empty subgraph");
		return vint();
	}
	
	//computes initial relevant degree information of vertices
	sg.init_scan(bbo::NON_DESTRUCTIVE);
	while(true){
		vt.index=sg.next_bit();
		if(vt.index==EMPTY_ELEM) break;

		vt.deg=g.degree(vt.index, sg);
		if(alg==MIN_DEG_DEGEN_TIE_STATIC){
			vt.deg_of_n=sum_of_neighbor_deg(vt.index, sg);
		}
		degs.push_back(vt);	
	}
	
	//computes direct order
	vint new_order;
	typename Graph_t::bb_type bbn(sg); 
	switch(alg){
	case MIN_DEG_DEGEN:
		while(!degs.empty()){
			vdeg_it it1=min_element(degs.begin(), degs.end(), degreeLess());
			new_order.push_back(it1->index);
			bbn.erase_bit(it1->index);
			degs.erase(it1);

			//recompute degrees
			for(int i=0; i<degs.size(); i++){
				degs[i].deg=g.degree(degs[i].index, bbn);
			}
		}
		break;
	case MAX_DEG_DEGEN:
		while(!degs.empty()){
			vdeg_it it1=max_element(degs.begin(), degs.end(), degreeLess());
			new_order.push_back(it1->index);
			bbn.erase_bit(it1->index);
			degs.erase(it1);

			//recompute degrees
			for(int i=0; i<degs.size(); i++){
				degs[i].deg=g.degree(degs[i].index, bbn);
			}
		}
		break;
	case MIN_DEG_DEGEN_TIE_STATIC:
		//degrees of supporters fixed at the beginning of the search
		while(!degs.empty()){
			vdeg_it it1=min_element(degs.begin(), degs.end(), degreeWithTieBreakLess());
			new_order.push_back(it1->index);
			bbn.erase_bit(it1->index);
			degs.erase(it1);
			
			//recompute degrees
			for(int i=0; i<degs.size(); i++){
				degs[i].deg=g.degree(degs[i].index, bbn);
			}
		}
		break;
	case NONE:
		//warning for petition in which vertex order of subgraph remains as is but is computed nevertheless
		if(place==PLACE_FL){
			LOG_WARNING("GraphSort<Graph_t>::new_subg_order: NONE + PLACE_FL->order unchanged but will be processed");
		}
		
		sg.init_scan(bbo::NON_DESTRUCTIVE);
		while(true){
			vt.index=sg.next_bit();
			if(vt.index==EMPTY_ELEM) break;
			new_order.push_back(vt.index);
		}
		break;
	default:
		LOG_ERROR("new_subg_order: unknown ordering strategy");
		return vint();
	}

	//reverse for place parameter consistency
	if(place==PLACE_LF)
		reverse(new_order.begin(), new_order.end());
		
return new_order;
}


///////////////
// 
// vertex selection primitives
//
////////////////
template<typename Graph_t>
int GraphSort<Graph_t>::get_v(pick_t pick){
///////////////////
// Picks vertex from the graph according to pick strategy (ties lexicographical)
// Breaks ties first found

	int opt_val;
	int v_sel=EMPTY_ELEM; 
	int NV=g.number_of_vertices();
	
	switch(pick){
	case PICK_MINFL:	
		opt_val=GRAPH_SORT_INFINITE;
		for(int v=0; v<NV; v++){
			int deg=g.degree(v);
			if(deg<opt_val){
				opt_val=deg;
				v_sel=v;
			}
		}
		break;
	case PICK_MINLF:	
		opt_val=GRAPH_SORT_INFINITE;
		for(int v=NV-1; v>=0; v--){
			int deg=g.degree(v);
			if(deg<opt_val){
				opt_val=deg;
				v_sel=v;
			}
		}
		break;
	case PICK_MAXFL:	
		opt_val=-1;
		for(int v=0; v<NV; v++){
			int deg=g.degree(v);
			if(deg>opt_val){
				opt_val=deg;
				v_sel=v;
			}
		}
		break;
	case PICK_MAXLF:	
		opt_val=-1;
		for(int v=NV-1; v>=0; v--){
			int deg=g.degree(v);
			if(deg>opt_val){
				opt_val=deg;
				v_sel=v;
			}
		}
		break;
	default:
		LOG_ERROR("GraphSort<Graph_t>::sel_v_by_deg: unknown vertex selection criteria");
	}
		
	return v_sel;	
}

template<typename Graph_t>
int GraphSort<Graph_t>::get_v(typename Graph_t::bb_type& sg, pick_t pick){	
// ////////////////////
// Picks vertex from induced subgraph according to pick degree criteria (ties lexicographical)
//
// OBSERVATIONS: PROPERTIES ARE ALWAYS LOCAL TO THE INDUCED SUBGRAPH (sg)

	int opt_val;
	int v_sel=EMPTY_ELEM; 
	int NV=g.number_of_vertices();

	switch(pick){
	case PICK_MINFL:	
		opt_val=GRAPH_SORT_INFINITE;
		sg.init_scan(bbo::NON_DESTRUCTIVE);
		while(true){
			int v=sg.next_bit();
			if(v==EMPTY_ELEM) break;
			int deg=g.degree(v, sg);
			if(deg<opt_val){
				opt_val=deg;
				v_sel=v;
			}
		}
		break;
	case PICK_MINLF:	
		opt_val=GRAPH_SORT_INFINITE;
		sg.init_scan(bbo::NON_DESTRUCTIVE_REVERSE);
		while(true){
			int v=sg.previous_bit();
			if(v==EMPTY_ELEM) break;
			int deg=g.degree(v, sg);
			if(deg<opt_val){
				opt_val=deg;
				v_sel=v;
			}
		}
		break;
	case PICK_MAXFL:	
		opt_val=-1;
		sg.init_scan(bbo::NON_DESTRUCTIVE);
		while(true){
			int v=sg.next_bit();
			if(v==EMPTY_ELEM) break;
			int deg=g.degree(v, sg);
			if(deg>opt_val){
				opt_val=deg;
				v_sel=v;
			}
		}
		break;
	case PICK_MAXLF:	
		opt_val=-1;
		sg.init_scan(bbo::NON_DESTRUCTIVE_REVERSE);
		while(true){
			int v=sg.previous_bit();
			if(v==EMPTY_ELEM) break;
			int deg=g.degree(v, sg);
			if(deg>opt_val){
				opt_val=deg;
				v_sel=v;
			}
		}
		break;
	default:
		LOG_ERROR("GraphSort<Graph_t>::sel_v_by_deg: unknown vertex selection criteria");
	}

	return v_sel;
}

template<typename Graph_t>
int GraphSort<Graph_t>::get_v (typename Graph_t::bb_type& sgfrom, const typename Graph_t::bb_type& sgref, pick_t pick){
////////////////////////////
// Picks vertex from induced subgraph sgfrom according to pick degree criteria (ties lexicographical)
// related to induced graph sgref (sgfrom and sgref are not necessarily disjoint) 
	
	int opt_val;
	int v_sel=EMPTY_ELEM; 
	int NV=g.number_of_vertices();
	typename Graph_t::bb_type neigh(NV);


	switch(pick){
	case PICK_MINFL:	
		opt_val=GRAPH_SORT_INFINITE;
		sgfrom.init_scan(bbo::NON_DESTRUCTIVE);
		while(true){
			int v=sgfrom.next_bit();
			if(v==EMPTY_ELEM) break;
			AND(g.get_neighbors(v), sgref, neigh);
			int deg=neigh.popcn64();
			if(deg<opt_val){
				opt_val=deg;
				v_sel=v;
			}
		}
		break;
	case PICK_MINLF:	
		opt_val=GRAPH_SORT_INFINITE;
		sgfrom.init_scan(bbo::NON_DESTRUCTIVE_REVERSE);
		while(true){
			int v=sgfrom.previous_bit();
			if(v==EMPTY_ELEM) break;
			AND(g.get_neighbors(v), sgref, neigh);
			int deg=neigh.popcn64();
			if(deg<opt_val){
				opt_val=deg;
				v_sel=v;
			}
		}
		break;
	case PICK_MAXFL:	
		opt_val=-1;
		sgfrom.init_scan(bbo::NON_DESTRUCTIVE);
		while(true){
			int v=sgfrom.next_bit();
			if(v==EMPTY_ELEM) break;
			AND(g.get_neighbors(v),sgref, neigh);
			int deg=neigh.popcn64();
			if(deg>opt_val){
				opt_val=deg;
				v_sel=v;
			}
		}
		break;
	case PICK_MAXLF:	
		opt_val=-1;
		sgfrom.init_scan(bbo::NON_DESTRUCTIVE_REVERSE);
		while(true){
			int v=sgfrom.previous_bit();
			if(v==EMPTY_ELEM) break;
			AND(g.get_neighbors(v),sgref, neigh);
			int deg=neigh.popcn64();
			if(deg>opt_val){
				opt_val=deg;
				v_sel=v;
			}
		}
		break;
	default:
		LOG_ERROR("GraphSort<Graph_t>::sel_v_by_deg: unknown vertex selection criteria");
	}
	

	return v_sel;
}
	


#endif