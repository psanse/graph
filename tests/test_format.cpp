
#include <iostream>
#include "google/gtest/gtest.h"
#include "../graph.h"

using namespace std;

TEST(Graph_test, read_mmx){
///////////////////
// Undirected graphs read by directed graph class (all edges are non symmetrical)
	
	ugraph g("bio-yeast.mtx");
	EXPECT_EQ(1948,g.number_of_edges());
	EXPECT_EQ(1458,g.number_of_vertices());
	

	sparse_ugraph sg("bio-yeast.mtx");
	EXPECT_EQ(1948,sg.number_of_edges());
	EXPECT_EQ(1458,sg.number_of_vertices());
}



TEST(Graph_test, read_all){
///////////////////
// Undirected graphs read by directed graph class (all edges are non symmetrical)
	
	//DIMACS
	ugraph g1("brock200_1.clq");
	cout<<"read DIMACS:"; g1.print_data(); 
	EXPECT_EQ(200, g1.number_of_vertices());
		
	//MTX
	ugraph g2("bio-yeast.mtx");
	cout<<"read MTX:"; g2.print_data(); 
	EXPECT_EQ(1458, g2.number_of_vertices());
	EXPECT_EQ(1948, g2.number_of_edges());

}

TEST(Graph_test, read_edgelist){
	//EDGES
	string filename("bio-yeast-protein-inter.edges");
	cout<<"READING: "<<filename<<"-----------------------"<<endl;
	ugraph g1(filename);
	g1.print_data();

	EXPECT_EQ(2114, g1.number_of_vertices());
	EXPECT_EQ(4406, g1.number_of_edges());
	EXPECT_TRUE(g1.is_edge(2111, 2110));
	EXPECT_TRUE(g1.is_edge(2110, 2111));
	EXPECT_TRUE(g1.is_edge(1, 35));

	cout<<"FINISHED READING"<<"-----------------------"<<endl;

	filename="ia-southernwomen.edges";
	cout<<"READING: "<<filename<<"-----------------------"<<endl;
	ugraph g2(filename);
	g2.print_data();

	EXPECT_EQ(18, g2.number_of_vertices());
	EXPECT_EQ(75, g2.number_of_edges());
	EXPECT_TRUE(g2.is_edge(0, 1));
	EXPECT_TRUE(g2.is_edge(17, 13));
	

	cout<<"FINISHED READING"<<"-----------------------"<<endl;


	filename="bio-MUTAG_g1.edges";
	cout<<"READING: "<<filename<<"-----------------------"<<endl;
	ugraph g3(filename);
	g3.print_data();

	EXPECT_EQ(23, g3.number_of_vertices());
	EXPECT_EQ(54, g3.number_of_edges());
	EXPECT_TRUE(g3.is_edge(0, 1));
	EXPECT_TRUE(g3.is_edge(20, 22));
	

	cout<<"FINISHED READING"<<"-----------------------"<<endl;
}


TEST(Graph_test, write_edgelist){
//////////////////
// reads as undirected graph a file with edges repeated
// writes it correctly 

	string filename="bio-MUTAG_g1.edges";					
	cout<<"READING: "<<filename<<"-----------------------"<<endl;
	ugraph g1(filename);
	g1.print_data();
	EXPECT_EQ(54,g1.number_of_edges());

	ofstream f("bio-MUTAG_g1_u.edges");
	g1.write_EDGES(f);
	f.close();

	g1.set_graph("bio-MUTAG_g1_u.edges");
	g1.print_data();
	EXPECT_EQ(27,g1.number_of_edges());


	////////////////////////
	filename="ia-southernwomen.edges";	
	cout<<"READING: "<<filename<<"-----------------------"<<endl;
	g1.set_graph(filename);
	g1.print_data();
	EXPECT_EQ(75,g1.number_of_edges());

	f.open("ia-southernwomen_u.edges");
	g1.write_EDGES(f);
	f.close();

	g1.set_graph("ia-southernwomen_u.edges");
	g1.print_data();
	EXPECT_EQ(64,g1.number_of_edges());
}

//
//
//TEST(Graph_test, write_edgelist_I){
////////////////////
//// reads as undirected graph a file with edges repeated
//// writes it correctly 
//
//	string filename="aff-digg.mtx";					
//	cout<<"READING: "<<filename<<"-----------------------"<<endl;
//	ugraph g1(filename);
//	g1.print_data();
//	//EXPECT_EQ(54,g1.number_of_edges());
//
//	ofstream f("aff-digg_u.edges");
//	g1.write_EDGES(f);
//	f.close();
//
//	g1.set_graph("aff-digg_u.edge");
//	g1.print_data();
//	EXPECT_EQ(27,g1.number_of_edges());
//
//}








