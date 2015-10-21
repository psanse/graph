//test_color.cpp: tests for algorithms related to coloring in Color class

#include <iostream>
#include "google/gtest/gtest.h"
#include "../graph.h"
#include "../color/color.h"
#include "pablodev\utils\common.h"

using namespace std;

TEST(Color, basic){
///////////////////
// Undirected graphs read by directed graph class (all edges are non symmetrical)
	
	const INT SIZE=5;
	ugraph ug(SIZE);
	ug.add_edge(0,1);
	ug.add_edge(1,2);
	ug.add_edge(0,2);			//complete graph

	Color mycolor(ug);

	//....
	EXPECT_EQ(5, SIZE);			//test tonto



	cout<<endl<<"-------------------"<<endl;
}


	

