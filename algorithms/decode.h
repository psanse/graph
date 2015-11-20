///////////////////////////////
// interface for the Decode class, which decodes orderings in graphs
// author: pss
// date: 29/11/2013

#ifndef __DECODE_H__
#define __DECODE_H__

#include <vector>
#include <algorithm>

using namespace std;

class Decode{
public:
	
	static void reverse_in_place(std::vector<int>& o);			//changes [index]-->[value] in place

	void clear(){orderings.clear();}
	void insert_ordering(const std::vector<int>& o){ orderings.push_back(o);}
	vector<int> decode_list(const vector<int>& list);
private:
	struct DecodeFunction{
	public:
		DecodeFunction(vector< vector<int> >& o): orderings(o){}
		int operator() (int v) const {  
			vector< vector<int> >::reverse_iterator it;
			it=orderings.rbegin();
			int new_v=v;
			while(it!=orderings.rend()){
				new_v=(*it)[new_v];
				it++;
			}
			return new_v;
		}

		vector< vector<int> >& orderings;
	};
//////////////////////
// data members
	vector< vector<int> > orderings;
	
};

inline
void Decode::reverse_in_place(vector<int>& o){
///////////////////////////////
//changes [index]-->[value] in place

	vector<int> vaux(o.size());
	vector<int>::iterator it;
	int i=0;
	for(it=o.begin(); it!=o.end(); it++){
		vaux.at(*it)=i++;
	}
	o.assign(vaux.begin(), vaux.end());
}

inline
vector<int> Decode::decode_list(const std::vector<int>& l){
////////////////////////////////
// For a given vertex list decodes the vertices according to the orderings
// 
// OBSERVATIONS: Assumes that all orderings are in the form [NEW_INDEX]= OLD_INDEX

	
	vector<int> res(l.size());

	if(orderings.empty()){					//no reordering, return a copy
		res.assign(l.begin(), l.end());
		return res;
	}
	
	DecodeFunction df(orderings);
	transform(l.cbegin(), l.cend(), res.begin(), df );
return res;
}

#endif