//test_degree.cpp: tests for algorithms related to degree in the DegUg class

#include <iostream>
#include "google/gtest/gtest.h"
#include "../graph.h"
#include "../algorithms/degree_ugraph.h"
#include "pablodev\utils\common.h"

using namespace std;

TEST(brock, sort_degree){
///////////////////
// Undirected graphs read by directed graph class (all edges are non symmetrical)
	
	ugraph ug("brock200_1.clq");
	DegUg deg(ug);
	const int NUM_VERTEX=10;

	int lv1[NUM_VERTEX];				//set pf vertices to order in place
	for(int i=0; i<NUM_VERTEX; i++){
		lv1[i]=i;
	}
	
	deg.degree_sort(lv1, lv1+NUM_VERTEX);
	for(int i=0; i<NUM_VERTEX; i++){
		cout<<(lv1[i]); cout<<" ";	
	}


///////////////////////////
// interface 2

	int lv2[NUM_VERTEX];				//destination of new order
	bitarray bb(200);
	bb.set_bit(0,NUM_VERTEX-1);			//set of vertices to order
	deg.degree_sort(bb,lv2);
	cout<<endl;
	for(int i=0; i<NUM_VERTEX; i++){
		cout<<(lv2[i]); cout<<" ";	
	}
		
	//comparison: must be equal
	vector<int> v1(lv1, lv1+NUM_VERTEX);
	vector<int> v2(lv2, lv2+NUM_VERTEX);
	EXPECT_EQ(v1, v2);

	cout<<endl<<"-------------------"<<endl;
}


	

