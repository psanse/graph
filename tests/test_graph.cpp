
#include <iostream>
#include "google/gtest/gtest.h"
#include "../graph.h"

using namespace std;

TEST(Graph_test, graph_from_file){
///////////////////
// Undirected graphs read by directed graph class (all edges are non symmetrical)
	
	graph g1("brock200_1.clq");
	graph g2("brock200_2.clq");
	graph g3("brock200_3.clq");
	graph g4("brock200_4.clq");

	//Number of (directed) edges
	EXPECT_EQ(14834,g1.number_of_edges());
	EXPECT_EQ(9876,g2.number_of_edges());
	EXPECT_EQ(12048,g3.number_of_edges());
	EXPECT_EQ(13089,g4.number_of_edges());

	//is directed
	EXPECT_NE(g1.is_edge(3,2),g1.is_edge(2,3));
	EXPECT_NE(g1.is_edge(4,2),g1.is_edge(2,4));

	//Number of vertices
	EXPECT_EQ(200,g1.number_of_vertices());
	EXPECT_EQ(200,g2.number_of_vertices());
	EXPECT_EQ(200,g3.number_of_vertices());
	EXPECT_EQ(200,g4.number_of_vertices());

	g1.make_bidirected();
	g2.make_bidirected();
	g3.make_bidirected();
	g4.make_bidirected();

	//Number of (bidirected) edges	
	EXPECT_EQ(2*14834,g1.number_of_edges());
	EXPECT_EQ(2*9876,g2.number_of_edges());
	EXPECT_EQ(2*12048,g3.number_of_edges());
	EXPECT_EQ(2*13089,g4.number_of_edges());

	//Name
	EXPECT_STREQ("brock200_1.clq",g1.get_name().c_str());
	EXPECT_STREQ("brock200_2.clq",g2.get_name().c_str());
	EXPECT_STREQ("brock200_3.clq",g3.get_name().c_str());
	EXPECT_STREQ("brock200_4.clq",g4.get_name().c_str());
}

TEST(Graph_test, neighbors){
	graph g(10);
	g.add_edge(0,1);
	g.add_edge(1,2);
	g.add_edge(2,3);
	g.add_edge(0,3);
	
	int v=1;
	BBIntrin& aux=g.get_neighbors(v);
		
	//computes vertex support
	vector<int> vec;
	aux.init_scan(bbo::NON_DESTRUCTIVE);
	while(true){
		int vadj=aux.next_bit();
		if(vadj==EMPTY_ELEM) break;
		vec.push_back(vadj);
	}

	//vertex 2 is the only neighbor of 1 and has degree 2
	EXPECT_EQ(1,vec.size());
	EXPECT_EQ(1,count(vec.begin(), vec.end(), 2));
}

TEST(Graph_test,graph_copy){

	graph g("brock200_2.clq");
	graph g1(g);

	EXPECT_EQ(g.number_of_edges(),g1.number_of_edges());
	EXPECT_EQ(g.number_of_vertices(),g1.number_of_vertices());
	EXPECT_EQ(g.get_name(),g1.get_name());
	EXPECT_EQ("brock200_2.clq",g1.get_name());
	EXPECT_TRUE(g==g1);

	//make bidirected
	g1.make_bidirected();
	EXPECT_FALSE(g==g1);
}

TEST(Graph_test,is_edge){
	
	graph g("sample.clq");
	
	EXPECT_TRUE(g.is_edge(1,0));
	EXPECT_TRUE(g.is_edge(2,0));
	EXPECT_TRUE(g.is_edge(2,1));
	EXPECT_TRUE(g.is_edge(3,0));
	EXPECT_TRUE(g.is_edge(3,1));
	EXPECT_TRUE(g.is_edge(3,2));
	EXPECT_TRUE(g.is_edge(4,3));
	EXPECT_TRUE(g.is_edge(5,3));
	EXPECT_TRUE(g.is_edge(5,4));
	EXPECT_TRUE(g.is_edge(6,4));
	EXPECT_TRUE(g.is_edge(6,5));

	EXPECT_FALSE(g.is_edge(0,1));
	EXPECT_FALSE(g.is_edge(0,2));
	EXPECT_FALSE(g.is_edge(1,2));
	EXPECT_FALSE(g.is_edge(0,3));
	EXPECT_FALSE(g.is_edge(1,3));
	EXPECT_FALSE(g.is_edge(2,3));
	EXPECT_FALSE(g.is_edge(3,4));
	EXPECT_FALSE(g.is_edge(3,5));
	EXPECT_FALSE(g.is_edge(4,5));
	EXPECT_FALSE(g.is_edge(4,6));
	EXPECT_FALSE(g.is_edge(5,6));


	g.remove_edge(2, 0);
	EXPECT_FALSE(g.is_edge(2,0));
	
	//makes bidirected and removes the first 4 vertices
	g.make_bidirected();
	BitBoardN bbn(4);
	bbn.set_bit(0, 3);
	g.remove_vertices(bbn);
	EXPECT_EQ(3, g.number_of_vertices());		//7-4=3 vertices which form a clique (but directed)
	EXPECT_EQ(3, g.number_of_edges());
}

TEST(Graph_test, degree_sample){
	
	ugraph g("sample.clq");
	
	/*EXPECT_TRUE(g.is_edge(1,0));
	EXPECT_TRUE(g.is_edge(2,0));
	EXPECT_TRUE(g.is_edge(2,1));
	EXPECT_TRUE(g.is_edge(3,0));
	EXPECT_TRUE(g.is_edge(3,1));
	EXPECT_TRUE(g.is_edge(3,2));
	EXPECT_TRUE(g.is_edge(4,3));
	EXPECT_TRUE(g.is_edge(5,3));
	EXPECT_TRUE(g.is_edge(5,4));
	EXPECT_TRUE(g.is_edge(6,4));
	EXPECT_TRUE(g.is_edge(6,5));*/
	
	EXPECT_EQ (3,g.degree(1));
	
	bitarray bba(g.number_of_vertices());
	bba.set_bit(0); bba.set_bit(1); bba.set_bit(6);

	EXPECT_EQ (1,g.degree(1,bba));

	bba.erase_bit();
	bba.set_bit(0); bba.set_bit(1); 
	EXPECT_EQ (1,g.degree(1,bba));
}

TEST(Graph_test, degree_brock){
	
	ugraph g("brock200_1.clq");
	

	EXPECT_EQ (200,  g.number_of_vertices());
	EXPECT_EQ (14834,  g.number_of_edges());
	
	bitarray bba(g.number_of_vertices());
	EXPECT_FALSE(g.is_edge(0,1));		//(1)
	
	bba.set_bit(0); bba.set_bit(1); 
	EXPECT_EQ (0,g.degree(1,bba));		//expected because of (1)

	bba.set_bit(0, g.number_of_vertices()-1);
	EXPECT_EQ (g.get_neighbors(1).popcn64(),g.degree(1,bba));

}

TEST(Graph_test, max_degree_subgraph){
	cout<<"Graph_test: max deg subgraph ------------------------"<<endl;
	ugraph g(100);
	g.add_edge(0,1);
	g.add_edge(1,2);
	g.add_edge(1,3);
	g.add_edge(2,3);
	g.add_edge(78,79);
	g.add_edge(79,80);
	
	bitarray sg(g.number_of_vertices());
	sg.init_bit(0, 3);
	EXPECT_EQ(3,g.max_degree_of_subgraph(sg));	//1(3)

	sg.init_bit(0, 2);
	EXPECT_EQ(2,g.max_degree_of_subgraph(sg));  //1(2)

	sg.init_bit(78, 79);						//78(1)
	EXPECT_EQ(1,g.max_degree_of_subgraph(sg));

	cout<<"------------------------------"<<endl;
}

TEST(Graph_test, max_degree_subgraph_sparse){
	cout<<"Graph_test: max deg subgraph sparse------------------------"<<endl;
	sparse_ugraph g(100);
	g.add_edge(0,1);
	g.add_edge(1,2);
	g.add_edge(1,3);
	g.add_edge(2,3);
	g.add_edge(78,79);
	g.add_edge(79,80);
	
	sparse_bitarray sg(g.number_of_vertices());
	sg.init_bit(0, 3);
	EXPECT_EQ(3,g.max_degree_of_subgraph(sg));	//1(3)

	sg.init_bit(0, 2);
	EXPECT_EQ(2,g.max_degree_of_subgraph(sg));  //1(2)

	sg.init_bit(78, 79);						//78(1)
	EXPECT_EQ(1,g.max_degree_of_subgraph(sg));

	cout<<"------------------------------"<<endl;
}

TEST(UGraph, complement_graph){

	cout<<"Graph_test: complement graph ------------------------"<<endl;
	ugraph g(4);
	g.add_edge(0,1);
	g.add_edge(1,2);
	g.add_edge(0,2);
	

	ugraph cg;
	g.create_complement(cg);

	EXPECT_TRUE(cg.is_edge(0,3));
	EXPECT_TRUE(cg.is_edge(1,3));
	EXPECT_TRUE(cg.is_edge(2,3));
	EXPECT_TRUE(g.number_of_vertices(),cg.number_of_vertices());
	//cg.print_edges();
}

TEST(Graph, remove_edges){
	cout<<"Graph_test: remove edges ------------------------"<<endl;
	graph g(4);
	g.add_edge(0,1);
	g.add_edge(1,2);
	g.add_edge(1,3);
	g.add_edge(0,2);

	g.remove_edges(1);
	g.print_edges();
	g.print_data(false);
	EXPECT_TRUE(g.is_edge(0,2));
	
	cout<<"------------------------------"<<endl;
}

TEST(Ugraph, remove_edges){
	cout<<"Ugraph_test: remove edges ------------------------"<<endl;
	ugraph g(4);
	g.add_edge(0,1);
	g.add_edge(1,2);
	g.add_edge(1,3);
	g.add_edge(0,2);

	g.remove_edges(1);
	g.print_edges();
	g.print_data(false);
	EXPECT_TRUE(g.is_edge(0,2));
	
	cout<<"------------------------------"<<endl;
}


//
//
//TEST(Graph_test,average_block_density){
//	const int NUM_BB=4;
//	graph g(64*NUM_BB);
//	g.add_edge(0,1);
//	g.add_edge(1,2);
//	g.add_edge(0,2);
//	
//	int nV=g.number_of_vertices();
//	EXPECT_DOUBLE_EQ(2.0/(nV*NUM_BB) ,g.block_density());		//around 1/500 of useful bitblocks
//	
//	sparse_graph g1(64*NUM_BB);
//	g1.add_edge(0,1);
//	g1.add_edge(1,2);
//	g1.add_edge(0,2);
//
//	EXPECT_DOUBLE_EQ(1.0, g1.block_density());					//all bitblocks are supposed to be non empty
//	
//	//almost full density
//	ugraph g2(4);
//	g2.add_edge(0,1);
//	g2.add_edge(1,2);
//	g2.add_edge(0,2);
//
//	EXPECT_DOUBLE_EQ(3.0/g2.number_of_vertices(), g2.block_density());		
//
//	//spare case
//	sparse_ugraph g3(4);
//	g3.add_edge(0,1);
//	g3.add_edge(1,2);
//	g3.add_edge(0,2);
//
//	EXPECT_DOUBLE_EQ(1.0, g3.block_density());	
//}
//
//TEST(Graph_test, resize){
//	graph g(100);
//	g.add_edge(0,1);
//	g.add_edge(1,2);
//	g.add_edge(2,3);
//	g.add_edge(0,3);
//	g.add_edge(54,55);
//	g.add_edge(88,89);
//	g.add_edge(88,90);
//	g.add_edge(89,90);
//
//	graph g1;
//	g.create_subgraph(50, g1);
//	EXPECT_EQ(50, g1.number_of_vertices());
//	EXPECT_EQ(4, g1.number_of_edges());			//0->1, 1->2, 2->3, 0->3
//	cout<<"----------------------------"<<endl;
//
//	g.create_subgraph(3, g1);
//	EXPECT_EQ(3, g1.number_of_vertices());
//	EXPECT_EQ(2, g1.number_of_edges());			//0->1, 1->2, 2->3
//	cout<<"----------------------------"<<endl;
//}
//
//TEST(Graph_test, shrink_graph){
//	graph g(100);
//	g.add_edge(0,1);
//	g.add_edge(1,2);
//	g.add_edge(2,3);
//	g.add_edge(0,3);
//	g.add_edge(54,55);
//	g.add_edge(88,89);
//	g.add_edge(88,90);
//	g.add_edge(89,90);
//
//	//shrinking not possible in non sparse graphs
//	int r=g.shrink_to_fit(50);
//	EXPECT_EQ(-1,r);
//
//
//	ugraph g1(100);
//	g1.add_edge(0,1);
//
//	//shrinking not possible in non sparse graphs
//	int r1=g1.shrink_to_fit(50);
//	EXPECT_EQ(-1,r);
//}
//
//
//TEST(Ugraph_test,ugraph_from_file){
/////////////////////
//// correct use of ugraph object to read undirected DIMACS graph
//
//	ugraph g1("brock200_1.clq");
//	ugraph g2("brock200_2.clq");
//	ugraph g3("brock200_3.clq");
//	ugraph g4("brock200_4.clq");
//
//	//Number of edges
//	EXPECT_EQ(14834,g1.number_of_edges());
//	EXPECT_EQ(9876,g2.number_of_edges());
//	EXPECT_EQ(12048,g3.number_of_edges());
//	EXPECT_EQ(13089,g4.number_of_edges());
//
//	//is undirected
//	EXPECT_EQ(g1.is_edge(3,2),g1.is_edge(2,3));
//	EXPECT_EQ(g1.is_edge(4,2),g1.is_edge(2,4));
//
//	//Number of vertices
//	EXPECT_EQ(200,g1.number_of_vertices());
//	EXPECT_EQ(200,g2.number_of_vertices());
//	EXPECT_EQ(200,g3.number_of_vertices());
//	EXPECT_EQ(200,g4.number_of_vertices());
//
//	//Name
//	EXPECT_STREQ("brock200_1.clq",g1.get_name().c_str());
//	EXPECT_STREQ("brock200_2.clq",g2.get_name().c_str());
//	EXPECT_STREQ("brock200_3.clq",g3.get_name().c_str());
//	EXPECT_STREQ("brock200_4.clq",g4.get_name().c_str());
//}
//
//
//TEST(Ugraph_test,removing_vertices){
//	ugraph g("sample.clq");
//		
//	BitBoardN bbn(4);
//	bbn.set_bit(0, 3);
//	g.remove_vertices(bbn);
//
//	EXPECT_EQ(3, g.number_of_vertices());		//7-4=3 vertices which form a clique (proper)
//	EXPECT_EQ(3, g.number_of_edges());
//}
//
//TEST(Ugraph_test,numberof_edges){
//	ugraph g("brock200_1.clq");
//		
//	EXPECT_EQ(14834, g.number_of_edges());		//7-4=3 vertices which form a clique (proper)
//}
//
//TEST(Graph_test, block_density_index){
//////////////////////
//// block density: number of blocks / total possible number of blocks 
//	sparse_ugraph sg(100);
//	sg.add_edge(0,1);
//	sg.add_edge(1,2);
//	
//	EXPECT_EQ(3/ceil(10000/64.0),sg.block_density_index());		
//}
//
