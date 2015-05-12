//kcore.h: header for the Kcore class which computes graph degeneracy linearly in the numbe
//author:pss
//date of creation: 6/6/14

#ifndef __KCORE_H__
#define __KCORE_H__


#include <vector>
#include "graph.h"
#include "pablodev/utils/logger.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>

using namespace std;
typedef map<int,int>			map_t;
typedef map<int,int>::iterator	map_it;


template<class T>
class KCore{
	friend ostream& operator<< (ostream& o, const KCore& kc){ kc.print_kcore(o); return o;}
private:
	typedef vector<int> vint;
	typedef vector<int>::iterator vint_it;
	typedef vector<int>::const_iterator vint_cit;
	enum data_t{DEG, BIN, VER, POS};

public:
/////////////
// constructor, setters and getters
	KCore(T& g, typename T::bb_type* bbset=NULL);
	
	int get_kcore_number				();								//size of largest kcore		
	vint get_kcore_numbers				(int k)					const;							
const vint& get_kcore_numbers			()						const {return m_deg;}
	const vint& get_kcore_ordering		()						const {return m_ver;}
	void set_subgraph					(typename T::bb_type*);				

//////////////
// interface
	void kcore							();												
	vint find_heur_clique				(int num_iter=EMPTY_ELEM);
inline	vint find_heur_clique_opt		(int num_iter=EMPTY_ELEM);				//only available for sparse graphs
	vint find_heur_clique_sparse		(int num_iter=EMPTY_ELEM);
	
	
	int make_kcore_filter				(map_t& filter, bool reverse=true);		//***experimental, probably remove
	
	//I/O
	void print_kcore(ostream& o=cout)	const;
				
private:
	
	void print(data_t=VER, ostream& o=cout);	
	
////////////
// k-core init steps
	void init_kcore();									
	void bin_sort();


///////////
// state
	T& m_g;								
	typename T::bb_type* m_subg;						//kcore(subgraph) or kcore(full graph) if NULL
	const int m_NV;										//order of graph
	
	vint m_deg;			
	vint m_bin;
	vint m_ver;
	vint m_pos;
};

template<class T>
KCore<T>::KCore(T& g, typename T::bb_type* bbset): m_g(g), m_NV(g.number_of_vertices()), m_deg(m_NV), m_pos(m_NV){
	//*** check empty bbset
	//*** check id bbset may hold m_NV/64 bitblocks
	set_subgraph(bbset);
}

template<class T>
void KCore<T>::set_subgraph	(typename T::bb_type* new_subg){
	m_subg=new_subg;
	if(m_subg) 	
		m_ver.assign(m_subg->popcn64(), EMPTY_ELEM);
	else  m_ver.assign(m_NV,EMPTY_ELEM);
}


template<class T>
void KCore<T>::kcore(){
////////////
// 
// INPUT: Simple undirected graph
// RETURNS: 
// A) min width ordering m_pos[OLD_INDEX]=NEW_INDEX (lowest index, lowest degree) or m_ver[NEW_INDEX]=OLD_INDEX
// B) The resulting (k-core) degree for each vertex 
// C) The k-core upper bound for the graph is the degree of the last vertex

	//inits data structures
	init_kcore();
	bin_sort();
	int u,v;

	if(m_subg==NULL){
		//kcore of the full graph
		for(int i=0; i<m_ver.size(); i++){
			v=m_ver[i];
			//iteracion sobre vecinos
			if(m_g.get_neighbors(v).init_scan(bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM){
				while(1){
					u=m_g.get_neighbors(v).next_bit();
					if(u==EMPTY_ELEM) break;

					int dv=m_deg[v];
					if(m_deg[u]>m_deg[v]){
						int du=m_deg[u];
						int pu=m_pos[u];
						int pw=m_bin[du];
						int w= m_ver[pw];
						if(u!=w){			//else there is no need to move the vertex
							m_pos[u]=pw;
							m_pos[w]=pu;	//swap (u->w) with first vertex with same degree
							m_ver[pu]=w;
							m_ver[pw]=u;
						}
						//decrease degree
						m_deg[u]--;
						//add one to bin so that w has now one less degree
						m_bin[du]++;
					}
				}
			}//endif
		}//vertex iteration
	}else{
		//kcore of the subgraph
		typename T::bb_type neigh(m_NV);

		//sorts by degree and computes degeneracy
		for(int i=0; i<m_ver.size(); i++){
			v=m_ver[i];
			//iteracion sobre vecinos
			AND(m_g.get_neighbors(v), *m_subg, neigh);
			if(neigh.init_scan(bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM){
				while(true){
					u=neigh.next_bit();
					if(u==EMPTY_ELEM) break;

					int dv=m_deg[v];
					if(m_deg[u]>m_deg[v]){
						int du=m_deg[u];
						int pu=m_pos[u];
						int pw=m_bin[du];
						int w= m_ver[pw];
						if(u!=w){			//else there is no need to move the vertex
							m_pos[u]=pw;
							m_pos[w]=pu;	//swap (u->w) with first vertex with same degree
							m_ver[pu]=w;
							m_ver[pw]=u;
						}
						//decrease degree
						m_deg[u]--;
						//add one to bin so that w has now one less degree
						m_bin[du]++;
					}
				}
			}//endif
		}//vertex iteration
	}
}	


template<class T>
void KCore<T>::init_kcore(){

	int max_gdeg=0, v=EMPTY_ELEM;

	if(m_subg==NULL){
		//kcore of the whole graph
		for(int v=0; v<m_NV; v++){
			m_deg[v]=m_g.degree(v);
			if(max_gdeg<m_deg[v])
					max_gdeg=m_deg[v];
		}

		m_bin.assign(max_gdeg+1, 0);
		for(int v=0; v<m_NV; v++){
			m_bin[m_deg[v]]+=1;					//note that isolani should not be in the list
		}

	}else{
		//kcore of bbset of vertices
		m_subg->init_scan(bbo::NON_DESTRUCTIVE);	//bbset cannot be empty, no empty check condition
		while(true){
			v=m_subg->next_bit();
			if(v==EMPTY_ELEM) break;

			//compute degree in the set
			m_deg[v]=m_g.degree(v, *m_subg);
			if(max_gdeg<m_deg[v])
				max_gdeg=m_deg[v];
		}

		m_bin.assign(max_gdeg+1, 0);
		m_subg->init_scan(bbo::NON_DESTRUCTIVE);
		v=EMPTY_ELEM;
		while(true){
			v=m_subg->next_bit();
			if(v==EMPTY_ELEM) break;

			m_bin[m_deg[v]]+=1;	
		}
	}
}

template<class T>
void KCore<T>::bin_sort(){
////////////////
// sorts vertices in bbset by non decreasing degree in linear time
//
// REMARKS: init_k_core has to be called first

	//init m_bin to point at the correct position in vertex array according to size
	int start=0, num=0;
	for(int d=0; d<m_bin.size(); d++){		
		num=m_bin[d];					
		m_bin[d]=start;
		start+=num;
	}

	if(m_subg==NULL){
		//bin_sort of all the graph
		for(int v=0; v<m_ver.size(); v++){
			m_pos[v]=m_bin[m_deg[v]];			//stores the new position of v (m_pos is not strictly needed for bin_sort, but it is for kcore)
			m_ver[m_pos[v]]=v;		
			m_bin[m_deg[v]]++;
		}
	}else{
		//bin_sort of subgraph
		m_subg->init_scan(bbo::NON_DESTRUCTIVE);
		int v=EMPTY_ELEM;
		while(true){
			v=m_subg->next_bit();
			if(v==EMPTY_ELEM) break;

			m_pos[v]=m_bin[m_deg[v]];			
			m_ver[m_pos[v]]=v;		
			m_bin[m_deg[v]]++;
		}
	}

	//corrects bin indexes after ordering (after update will point to the first element of their type)
	//after correction: bin[0]=0 always, bin[1]=0 if there are no isolani etc.
	for(int d=m_bin.size()-1; d>=1; d--){
		m_bin[d]=m_bin[d-1];
	}
	m_bin[0]=0;
}


template<class T>
int KCore<T>::get_kcore_number(){
//must be called afer k_core()
	return m_deg[m_ver.back()];
}

template<class T>
typename KCore<T>::vint KCore<T>::get_kcore_numbers(int k)	const {
/////////////////////////
//INPUT PARAM: k>=0 
//
// RETURN VALUES 
// k>=0 : set of vertices with kcore number greater than k (k==0 is V)

	vint res;
	
	for(int v=0; v<m_deg.size(); ++v){
		if(m_deg[v]>=k)
			 res.push_back(v);
	}
		
return res;
}

template<class T>
int KCore<T>::make_kcore_filter (map_t& filter, bool reverse) {
///////////////////////////////
// maps kcore number to the starting vertex of the next kcore partition (filter[kc(v)]->first vertex w,  kc(w)=kc(v)+1)
// Example [v,kc(v)], reverse=TRUE, kcore dec:[1,1][2,1][3,2][4,1] then filter[2]=1 the single element in filter
// Example [v,kc(v)], reverse=FALSE, kcore dec:[1,1][2,1][3,2][4,1] then filter[1]=4 the single element in filter
//
// PARAMS:
// 1-filter: the mapping
// 2-reverse: if TRUE assumes kcore partitions are in increasing order from first to last
//            if FALSE assumes kcore partitions are in decreasing order from first to last
//
// RETURNS: size of filter
// REMARKS: kcore-decomposition must exist (prior call to k-core())
	
	//***assert existence of kcore decomposition
	
	filter.clear();
	int next_kcore_pos=0, current_k=EMPTY_ELEM;

	if(reverse){
		current_k=m_deg[m_ver.back()];
		for(int i=m_ver.size()-2; i>=0; --i){
			++next_kcore_pos;
			if(m_deg[m_ver[i]]!=current_k){
				//adds to map kcore values and intermediate values
				int diff=current_k-m_deg[m_ver[i]];
				int ck=current_k;
				
				do{
					filter.insert(pair<int, int>(ck--,next_kcore_pos));
					diff --;
				}while(diff>=1);

				//filter.insert(pair<int, int>(current_k,next_kcore_pos));
				current_k=m_deg[m_ver[i]];
			}
		}
	}else{
		
		current_k=m_deg[m_ver.front()];
		for(int i=1; i<m_ver.size(); ++i){
			++next_kcore_pos;
			if(m_deg[m_ver[i]]!=current_k){	
				//adds to map kcore values and intermediate values
				int diff=m_deg[m_ver[i]]-current_k;
				int ck=current_k;
				
				do{
					filter.insert(pair<int, int>(ck--,next_kcore_pos));
					diff --;
				}while(diff>=1);

				//filter.insert(pair<int, int>(current_k,next_kcore_pos));
				current_k=m_deg[m_ver[i]];
			}
		}
	}

return filter.size();
}

template<class T>
typename KCore<T>::vint KCore<T>::find_heur_clique(int num_iter){
///////////////////////
// A more efficient and clean implementation of the greedy clique heuristic based on Kcore
//
// COMMENTS: If num_iter==EMPTY_ELEM (default value) it will complete all possible iterations
	
	int max_size=1, iter=1; 
	vint curr_clique, largest_clique, neighbors;
	//main loop
	for(int i= m_ver.size()-1; i>=0; i--){
		
		//CUT at root level
		int v=m_ver[i];
		if(m_deg[v]<max_size){
			break;					
		}

		//determines neighbor set in degeneracy order
		neighbors.clear();
		for(int j=i-1; j>=0; j--){
			if(m_deg[m_ver[j]]>=max_size && m_g.get_neighbors(v).is_bit(m_ver[j])){
				neighbors.push_back(m_ver[j]);									//vertices are placed in neighbor in degeneracy order (I)
			}
		}

		//determines largest greedy clique in neighbor set
		curr_clique.clear();
		for(int n=0; n<neighbors.size(); n++){	//vertices selected in degeneracy order (I)							
			bool good_vertex=true;
			for(int l=0; l<curr_clique.size(); l++){
				if( !m_g.get_neighbors(curr_clique[l]).is_bit(neighbors[n]) ){
					good_vertex=false;
					break;
				}
			}
			if(good_vertex)
					curr_clique.push_back(neighbors[n]);
		}

		//adds initial vertex
		curr_clique.push_back(v);

		//update size
		if(max_size<curr_clique.size()){
			 largest_clique=curr_clique;
			 max_size=curr_clique.size();

			 //I/O
			 LOG_DEBUG(iter<<"."<<"lb:"<<max_size<<" seed:"<<m_ver[i]);
		}


		//evaluation of number of iterations (exit condition received parameter)
		if(++iter>=num_iter && num_iter!=EMPTY_ELEM )	break;
	}

	if(num_iter!=EMPTY_ELEM)
		LOG_INFO("kc["<<"it:"<<iter<<","<<" lb:"<<max_size<<"]");
	else LOG_INFO("kc[lb:"<<max_size<<"]");
	return largest_clique;
}

template<>
inline
KCore<sparse_ugraph>::vint KCore<sparse_ugraph>::find_heur_clique_opt(int num_iter){
///////////////////////
// Optimized version of find_clique for speed
// date:30/12/2014
//
// COMMENTS: At the moment, only available for sparse_graph type
	
	int max_size=1, iter=1, from=EMPTY_ELEM; 
	vint curr_clique, largest_clique;
	sparse_bitarray bbneigh(m_NV);
	
	//main loop
	for(int i=m_ver.size()-1; i>0; i--){
		
		//CUT at root level
		if(m_deg[m_ver[i]]<max_size){
			break;					
		}

		//determines neighbor set
		curr_clique.clear();
		bbneigh=m_g.get_neighbors(m_ver[i]);
		
		//iterates over all vertices to pick them in degeneracy ordering
		for(int j=i-1; j>=0; j--){
			if(m_deg[m_ver[j]]<max_size) break;
			if(bbneigh.is_bit(m_ver[j])){				//adjacent
				curr_clique.push_back(m_ver[j]);	
							
				bbneigh&=m_g.get_neighbors(m_ver[j]);
			}
		}

		//adds initial vertex
		curr_clique.push_back(m_ver[i]);

		//update size
		if(max_size<curr_clique.size()){
			 largest_clique=curr_clique;
			 max_size=largest_clique.size();

			 //I/O
			 LOG_DEBUG(iter<<"."<<"lb:"<<max_size<<" seed: ["<<i<<":"<<m_ver[i]<<"]");
		}

		//evaluation of number of iterations 
		if( ++iter>=num_iter && num_iter!=EMPTY_ELEM)	break;
	}

	if(num_iter!=EMPTY_ELEM)
		LOG_INFO("kc["<<"it:"<<iter<<","<<" lb:"<<max_size<<"]");
	else LOG_INFO("kc[lb:"<<max_size<<"]");

	return largest_clique;
}

struct less_kcore{
	less_kcore(vector<int>& pos, vector<int>& ver,  vector<int>& deg ):pos(pos), ver(ver), deg(deg){}
	bool operator()(int lhs, int rhs) const{
		return(deg[ver[pos[lhs]]]>deg[ver[pos[lhs]]]);
	}
	vector<int>& pos;
	vector<int>& ver;
	vector<int>& deg;
};

struct remove_kcore{
	remove_kcore(vector<int>& pos, vector<int>& ver, vector<int>& deg, int max_clique):pos(pos), ver(ver), deg(deg), max_kcore(max_clique){}
	bool operator()(int data) const{
		return(deg[ver[pos[data]]]<max_kcore);
	}
	vector<int>& pos;
	vector<int>& ver;
	vector<int>& deg;
	int max_kcore;
};

template<>
inline
KCore<sparse_ugraph>::vint KCore<sparse_ugraph>::find_heur_clique_sparse(int num_iter){
///////////////////////
// Optimized version of find_clique for speed in large saprse graphs
// date:30/12/2014
//
// COMMENTS: (only available for sparse_graph type)
	
	int max_size=1, iter=1, from=EMPTY_ELEM; 
	vint curr_clique, largest_clique, candidates;
	sparse_bitarray bbneigh(m_NV);
	
	//main loop
	for(int i=m_ver.size()-1; i>0; i--){
		
		//CUT at root level
		if(m_deg[m_ver[i]]<max_size){
			break;					
		}

		//determines neighbor set
		curr_clique.clear();
		bbneigh=m_g.get_neighbors(m_ver[i]);
		bbneigh.to_vector(candidates);
		sort(candidates.begin(), candidates.end(), less_kcore(m_pos, m_ver, m_deg));		//sort degeneracy
		candidates.erase(remove_if(candidates.begin(), candidates.end(),remove_kcore(m_pos, m_ver,m_deg, max_size)), candidates.end());
		
		
		//iterates over all vertices to pick them in degeneracy ordering
		for(int n=0; n<candidates.size(); n++){
			if(bbneigh.is_bit(candidates[n])){				//adjacent
				curr_clique.push_back(candidates[n]);	
							
				bbneigh&=m_g.get_neighbors(candidates[n]);
			}
		}

		//adds initial vertex
		curr_clique.push_back(m_ver[i]);

		//update size
		if(max_size<curr_clique.size()){
			 largest_clique=curr_clique;
			 max_size=largest_clique.size();

			 //I/O
			 LOG_DEBUG(iter<<"."<<"lb:"<<max_size<<" seed: ["<<i<<":"<<m_ver[i]<<"]");
		}

		//evaluation of number of iterations 
		if( ++iter>=num_iter && num_iter!=EMPTY_ELEM)	break;
	}

	if(num_iter!=EMPTY_ELEM)
		LOG_INFO("kc["<<"it:"<<iter<<","<<" lb:"<<max_size<<"]");
	else LOG_INFO("kc[lb:"<<max_size<<"]");
	return largest_clique;
}

///////////////////////////
//
// INPUT/OUTPUT
//
///////////////////////


template<class T>
void KCore<T>::print(data_t type, ostream& o){
	switch(type){
	case DEG:
		for(int i=0; i<m_deg.size();i++)
			o<<m_deg[i]<<" ";
		o<<endl;
		break;
	case BIN:
		for(int i=0; i<m_bin.size();i++)
			o<<m_bin[i]<<" ";
	    o<<endl;
		break;
	case VER:
		for(int i=0; i<m_ver.size();i++)
			o<<m_ver[i]<<" ";
		o<<endl;
		break;
	case POS:
		for(int i=0; i<m_pos.size();i++)
			o<<m_pos[i]<<" ";
		o<<endl;
		break;
	default:
		;
	}
}

template<class T>
void KCore<T>::print_kcore(ostream& o)	const{
	if(m_subg==NULL){	//whole graph
		for(vint_cit it=m_deg.begin(); it!=m_deg.end(); ++it){
			o<<"["<<it-m_deg.begin()<<","<<*it<<"] ";
		}
	}else{				//subgraph
		int v=EMPTY_ELEM;
		if(m_subg->init_scan(bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM){
			while(true){
				v=m_subg->next_bit();
				if(v==EMPTY_ELEM) break;
				o<<"["<<v<<","<<m_deg[v]<<"] ";
			}
		}
	}
}

#endif