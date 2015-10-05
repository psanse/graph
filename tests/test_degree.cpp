//test_degree.cpp: tests for algorithms related to degree in the DegUg class

#include <iostream>
#include "google/gtest/gtest.h"
#include "../graph.h"
#include "../algorithms/degree_ugraph.h"
#include "pablodev\utils\common.h"

using namespace std;

TEST(Graph_test, graph_from_file){
///////////////////
// Undirected graphs read by directed graph class (all edges are non symmetrical)
	
	ugraph ug("brock200_1.clq");
	DegUg deg(ug);
	//vector<int> v(200, 0);
	//for(int i=0; i<200; i++){
	//	v[i]=i;
	//}
	//deg.degree_sort(v.begin(),v.begin()+10);
	////com::stl::print_collection(deg.m_vdeg);
	//com::stl::print_collection(v);
	//cout<<endl;
	//for(vint::iterator it=v.begin(); it!=v.begin()+10; it++){
	//	cout<<(deg.m_vdeg[*it]); cout<<" ";	
	//}

	bitarray bb(200);
	//bb.set_bit(0,10);
	bb.set_bit(0); bb.set_bit(10); bb.set_bit(20);
	vector<int> v2(200,0);
	deg.degree_sort(bb,v2.begin());
	cout<<endl;
	com::stl::print_collection(v2);
	for(vint::iterator it=v2.begin(); it!=v2.end(); it++){
		cout<<(deg.m_vdeg[*it]); cout<<" ";	
	}


	
	

	////Number of (directed) edges
	//EXPECT_EQ(14834,g1.number_of_edges());
	//EXPECT_EQ(9876,g2.number_of_edges());
	//EXPECT_EQ(12048,g3.number_of_edges());
	//EXPECT_EQ(13089,g4.number_of_edges());

	////is directed
	//EXPECT_NE(g1.is_edge(3,2),g1.is_edge(2,3));
	//EXPECT_NE(g1.is_edge(4,2),g1.is_edge(2,4));

	////Number of vertices
	//EXPECT_EQ(200,g1.number_of_vertices());
	//EXPECT_EQ(200,g2.number_of_vertices());
	//EXPECT_EQ(200,g3.number_of_vertices());
	//EXPECT_EQ(200,g4.number_of_vertices());

	//g1.make_bidirected();
	//g2.make_bidirected();
	//g3.make_bidirected();
	//g4.make_bidirected();

	////Number of (bidirected) edges	
	//EXPECT_EQ(2*14834,g1.number_of_edges());
	//EXPECT_EQ(2*9876,g2.number_of_edges());
	//EXPECT_EQ(2*12048,g3.number_of_edges());
	//EXPECT_EQ(2*13089,g4.number_of_edges());

	////Name
	//EXPECT_STREQ("brock200_1.clq",g1.get_name().c_str());
	//EXPECT_STREQ("brock200_2.clq",g2.get_name().c_str());
	//EXPECT_STREQ("brock200_3.clq",g3.get_name().c_str());
	//EXPECT_STREQ("brock200_4.clq",g4.get_name().c_str());
}

//TEST(Graph_test, neighbors){
//	graph g(10);
//	g.add_edge(0,1);
//	g.add_edge(1,2);
//	g.add_edge(2,3);
//	g.add_edge(0,3);
//	
//	int v=1;
//	BBIntrin& aux=g.get_neighbors(v);
//		
//	//computes vertex support
//	vector<int> vec;
//	aux.init_scan(bbo::NON_DESTRUCTIVE);
//	while(true){
//		int vadj=aux.next_bit();
//		if(vadj==EMPTY_ELEM) break;
//		vec.push_back(vadj);
//	}
//
//	//vertex 2 is the only neighbor of 1 and has degree 2
//	EXPECT_EQ(1,vec.size());
//	EXPECT_EQ(1,count(vec.begin(), vec.end(), 2));
//}
//
//TEST(Graph_test,graph_copy){
//
//	graph g("brock200_2.clq");
//	graph g1(g);
//
//	EXPECT_EQ(g.number_of_edges(),g1.number_of_edges());
//	EXPECT_EQ(g.number_of_vertices(),g1.number_of_vertices());
//	EXPECT_EQ(g.get_name(),g1.get_name());
//	EXPECT_EQ("brock200_2.clq",g1.get_name());
//	EXPECT_TRUE(g==g1);
//
//	//make bidirected
//	g1.make_bidirected();
//	EXPECT_FALSE(g==g1);
//}
//
//TEST(Graph_test,is_edge){
//	
//	graph g("sample.clq");
//	
//	EXPECT_TRUE(g.is_edge(1,0));
//	EXPECT_TRUE(g.is_edge(2,0));
//	EXPECT_TRUE(g.is_edge(2,1));
//	EXPECT_TRUE(g.is_edge(3,0));
//	EXPECT_TRUE(g.is_edge(3,1));
//	EXPECT_TRUE(g.is_edge(3,2));
//	EXPECT_TRUE(g.is_edge(4,3));
//	EXPECT_TRUE(g.is_edge(5,3));
//	EXPECT_TRUE(g.is_edge(5,4));
//	EXPECT_TRUE(g.is_edge(6,4));
//	EXPECT_TRUE(g.is_edge(6,5));
//
//	EXPECT_FALSE(g.is_edge(0,1));
//	EXPECT_FALSE(g.is_edge(0,2));
//	EXPECT_FALSE(g.is_edge(1,2));
//	EXPECT_FALSE(g.is_edge(0,3));
//	EXPECT_FALSE(g.is_edge(1,3));
//	EXPECT_FALSE(g.is_edge(2,3));
//	EXPECT_FALSE(g.is_edge(3,4));
//	EXPECT_FALSE(g.is_edge(3,5));
//	EXPECT_FALSE(g.is_edge(4,5));
//	EXPECT_FALSE(g.is_edge(4,6));
//	EXPECT_FALSE(g.is_edge(5,6));
//
//
//	g.remove_edge(2, 0);
//	EXPECT_FALSE(g.is_edge(2,0));
//	
//	//makes bidirected and removes the first 4 vertices
//	g.make_bidirected();
//	BitBoardN bbn(4);
//	bbn.set_bit(0, 3);
//	g.remove_vertices(bbn);
//	EXPECT_EQ(3, g.number_of_vertices());		//7-4=3 vertices which form a clique (but directed)
//	EXPECT_EQ(3, g.number_of_edges());
//}
//
//TEST(Graph_test, degree_sample){
//	
//	ugraph g("sample.clq");
//	
//	/*EXPECT_TRUE(g.is_edge(1,0));
//	EXPECT_TRUE(g.is_edge(2,0));
//	EXPECT_TRUE(g.is_edge(2,1));
//	EXPECT_TRUE(g.is_edge(3,0));
//	EXPECT_TRUE(g.is_edge(3,1));
//	EXPECT_TRUE(g.is_edge(3,2));
//	EXPECT_TRUE(g.is_edge(4,3));
//	EXPECT_TRUE(g.is_edge(5,3));
//	EXPECT_TRUE(g.is_edge(5,4));
//	EXPECT_TRUE(g.is_edge(6,4));
//	EXPECT_TRUE(g.is_edge(6,5));*/
//	
//	EXPECT_EQ (3,g.degree(1));
//	
//	bitarray bba(g.number_of_vertices());
//	bba.set_bit(0); bba.set_bit(1); bba.set_bit(6);
//
//	EXPECT_EQ (1,g.degree(1,bba));
//
//	bba.erase_bit();
//	bba.set_bit(0); bba.set_bit(1); 
//	EXPECT_EQ (1,g.degree(1,bba));
//}
//
//TEST(Graph_test, degree_brock){
//	
//	ugraph g("brock200_1.clq");
//	
//
//	EXPECT_EQ (200,  g.number_of_vertices());
//	EXPECT_EQ (14834,  g.number_of_edges());
//	
//	bitarray bba(g.number_of_vertices());
//	EXPECT_FALSE(g.is_edge(0,1));		//(1)
//	
//	bba.set_bit(0); bba.set_bit(1); 
//	EXPECT_EQ (0,g.degree(1,bba));		//expected because of (1)
//
//	bba.set_bit(0, g.number_of_vertices()-1);
//	EXPECT_EQ (g.get_neighbors(1).popcn64(),g.degree(1,bba));
//
//}

