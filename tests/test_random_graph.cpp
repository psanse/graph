
#include <iostream>
#include <iomanip>
#include "google/gtest/gtest.h"
#include "../graph.h"
#include "../graph_gen.h"

using namespace std;

//*** TODO remove log files created

TEST(Random_Graph, basic_struct){
	random_attr_t r1(100, 150, .3, .5, 1, 50, .1);
	EXPECT_EQ(50, r1.incN);
	EXPECT_EQ(.1, r1.incP);
	
	random_attr_t r2(r1);
	r2=r1;
	EXPECT_EQ(r2.incN, r1.incN);
	cout<<r1;
	cout<<r2;
}

TEST(Random_Graph, ugraph){
///////////////////
// Undirected graphs read by directed graph class (all edges are non symmetrical)
	
	ugraph ug;
	RandomGen<ugraph>::create_ugraph(ug, 100, .30);
	ug.print_data();

	//check if inside the range
	EXPECT_GE(.33, ug.density());
	EXPECT_LE(.27, ug.density());
	
	//will crate a benchmark in default (project) directory
	random_attr_t r(100, 150, .3, .5, 1, 50, .1); 
	RandomGen<ugraph>::create_graph_benchmark(".", r);
	RandomGen<ugraph>::create_ugraph_benchmark(".", r);
}

TEST(Random_Graph, sparse_ugraph){
///////////////////
// Undirected graphs read by directed graph class (all edges are non symmetrical)
	
	sparse_ugraph ug;
	RandomGen<sparse_ugraph>::create_ugraph(ug, 100, .30);
	ug.print_data();

	//check if inside the range
	EXPECT_GE(.33, ug.density());
	EXPECT_LE(.27, ug.density());
	
	//will crate a benchmark in default (project) directory
	random_attr_t r(100, 150, .3, .5, 1, 50, .1); 
	RandomGen<sparse_ugraph>::create_graph_benchmark(".", r);
	RandomGen<sparse_ugraph>::create_ugraph_benchmark(".", r);
}
//
//TEST(Sparse_Random_Graph, write_to_file){
//	sparse_ugraph ug;
//	SparseRandomGen<> spgen;
//	spgen.create_ugraph(ug,10000,.02);
//	ug.print_data();
//	
//	EXPECT_GE(10000, ug.number_of_vertices());
//	EXPECT_GE(.025, ug.density());
//
//	//will create a log file
//	ofstream f("log.txt");
//	ug.write_dimacs(f);
//	f.close();
//	ugraph ugnsp("log.txt");
//	
//	EXPECT_EQ(ug.number_of_vertices(), ugnsp.number_of_vertices());
//	EXPECT_EQ(ug.number_of_edges(), ugnsp.number_of_edges());
//	
//	//*** remove log file
//
//}
//
//TEST(Sparse_Random_Graph, ugraph){
//	sparse_ugraph ug;
//	SparseRandomGen<> spgen;
//	spgen.create_ugraph(ug,100000,.002);
//	ug.print_data();
//	
//	EXPECT_GE(100000, ug.number_of_vertices());
//	EXPECT_GE(.025, ug.density());
//}

//1.000.000 graph generation test
//TEST(Sparse_Random_Graph, very_large_ugraph){
//	sparse_ugraph ug;
//	SparseRandomGen<> spgen;
//	spgen.create_ugraph(ug,1000000,.0002);
//	ug.print_data();
//	
//	EXPECT_GE(1000000, ug.number_of_vertices());
//	EXPECT_GE(.00025, ug.density());
//}





