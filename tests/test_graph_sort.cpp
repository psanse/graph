
#include <iostream>
#include "google/gtest/gtest.h"
#include "../graph.h"
#include "../graph_sort.h"

using namespace std;
TEST(Graph_sort, basic){


	ugraph ug(10);
	ug.add_edge(0,1);
	ug.add_edge(1,2);
	ug.add_edge(2,3);
	ug.add_edge(0,3);

	GraphSort(ug);


	

	//vertex 2 is the only neighbor of 1 and has degree 2
	/*EXPECT_EQ(1,vec.size());
	EXPECT_EQ(1,count(vec.begin(), vec.end(), 2));*/
}

//TEST(Graph_sort, graph_from_file){
/////////////////////
//// Undirected graphs read by directed graph class (all edges are non symmetrical)
//	
//	graph g1("brock200_1.clq");
//	graph g2("brock200_2.clq");
//	graph g3("brock200_3.clq");
//	graph g4("brock200_4.clq");
//
//	//Number of (directed) edges
//	EXPECT_EQ(14834,g1.number_of_edges());
//	EXPECT_EQ(9876,g2.number_of_edges());
//	EXPECT_EQ(12048,g3.number_of_edges());
//	EXPECT_EQ(13089,g4.number_of_edges());
//
//	//is directed
//	EXPECT_NE(g1.is_edge(3,2),g1.is_edge(2,3));
//	EXPECT_NE(g1.is_edge(4,2),g1.is_edge(2,4));
//
//	//Number of vertices
//	EXPECT_EQ(200,g1.number_of_vertices());
//	EXPECT_EQ(200,g2.number_of_vertices());
//	EXPECT_EQ(200,g3.number_of_vertices());
//	EXPECT_EQ(200,g4.number_of_vertices());
//
//	g1.make_bidirected();
//	g2.make_bidirected();
//	g3.make_bidirected();
//	g4.make_bidirected();
//
//	//Number of (bidirected) edges	
//	EXPECT_EQ(2*14834,g1.number_of_edges());
//	EXPECT_EQ(2*9876,g2.number_of_edges());
//	EXPECT_EQ(2*12048,g3.number_of_edges());
//	EXPECT_EQ(2*13089,g4.number_of_edges());
//
//	//Name
//	EXPECT_STREQ("brock200_1.clq",g1.get_name().c_str());
//	EXPECT_STREQ("brock200_2.clq",g2.get_name().c_str());
//	EXPECT_STREQ("brock200_3.clq",g3.get_name().c_str());
//	EXPECT_STREQ("brock200_4.clq",g4.get_name().c_str());
//}



