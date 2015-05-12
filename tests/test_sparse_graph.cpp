
#include <iostream>
#include "google/gtest/gtest.h"
#include "../graph_gen.h"
#include "../graph.h"

using namespace std;

TEST(Graph_sparse_test, graph_from_file){
///////////////////
// Undirected graphs read by directed graph class (all edges are non symmetrical)
	
	sparse_graph g1("brock200_1.clq");
	sparse_graph g2("brock200_2.clq");
	sparse_graph g3("brock200_3.clq");
	sparse_graph g4("brock200_4.clq");

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


TEST(Graph_sparse_test,graph_copy){

	sparse_graph g("brock200_2.clq");
	sparse_graph g1(g);

	EXPECT_EQ(g.number_of_edges(),g1.number_of_edges());
	EXPECT_EQ(g.number_of_vertices(),g1.number_of_vertices());
	EXPECT_EQ(g.get_name(),g1.get_name());
	EXPECT_EQ("brock200_2.clq",g1.get_name());
	EXPECT_TRUE(g==g1);

	//make bidirected
	g1.make_bidirected();
	EXPECT_FALSE(g==g1);
	
}

TEST(Graph_sparse_test,is_edge){
	
	sparse_graph g("sample.clq");
	
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

TEST(Graph_sparse_test, degree_sample){
	
	sparse_ugraph g("sample.clq");
	
	
	EXPECT_EQ (3,g.degree(1));
	
	sparse_bitarray bbs(g.number_of_vertices());
	bbs.set_bit(0); bbs.set_bit(1); bbs.set_bit(6);

	EXPECT_EQ (1,g.degree(1,bbs));

	bbs.erase_bit();
	bbs.set_bit(0); bbs.set_bit(1); 
	EXPECT_EQ (1,g.degree(1,bbs));
}

TEST(Graph_sparse_test, degree_brock){
	
	sparse_ugraph g("brock200_1.clq");
	
	sparse_bitarray bbs(g.number_of_vertices());
	EXPECT_FALSE(g.is_edge(0,1));		//(1)
	
	bbs.set_bit(0); bbs.set_bit(1); 
	EXPECT_EQ (0,g.degree(1,bbs));		//expected because of (1)

	bbs.set_bit(0, g.number_of_vertices()-1);
	EXPECT_EQ (g.get_neighbors(1).popcn64(),g.degree(1,bbs));		
}


TEST(Graph_sparse_test, write_to_file){
	sparse_graph sg(1000);
	sg.add_edge(1,3);
	sg.add_edge(5,7);
	sg.add_edge(100,999);
	sg.add_edge(0,10);
	ofstream f("log.txt");
	sg.write_dimacs(f);
	f.close();

}

TEST(Graph_sparse_test, create_subgraph){
	sparse_graph g(100);
	g.add_edge(0,1);
	g.add_edge(1,2);
	g.add_edge(2,3);
	g.add_edge(0,3);
	g.add_edge(54,55);
	g.add_edge(88,89);
	g.add_edge(88,90);
	g.add_edge(89,90);

	sparse_graph g1;
	g.create_subgraph(50, g1);
	EXPECT_EQ(50, g1.number_of_vertices());
	EXPECT_EQ(4, g1.number_of_edges());			//0->1, 1->2, 2->3, 0->3
	
	g.create_subgraph(3, g1);
	EXPECT_EQ(3, g1.number_of_vertices());
	EXPECT_EQ(2, g1.number_of_edges());			//0->1, 1->2, 2->3
	
	//creating a wrong size subgraph
	int nV=g.number_of_vertices();
	EXPECT_EQ(nV,(g.create_subgraph(0, g1)).number_of_vertices());

}

TEST(Graph_sparse_test, shrink_graph){
	sparse_graph g(100);
	g.add_edge(0,1);
	g.add_edge(1,2);
	g.add_edge(2,3);
	g.add_edge(0,3);
	g.add_edge(54,55);
	g.add_edge(88,89);
	g.add_edge(88,90);
	g.add_edge(89,90);

	g.shrink_to_fit(50);
	EXPECT_EQ(50, g.number_of_vertices());
	EXPECT_EQ(4, g.number_of_edges());			//0->1, 1->2, 2->3, 0->3
	g.print_edges();
	
	g.shrink_to_fit(3);
	EXPECT_EQ(3, g.number_of_vertices());
	EXPECT_EQ(2, g.number_of_edges());			//0->1, 1->2, 2->3
	EXPECT_TRUE(g.is_edge(1,2));
	EXPECT_FALSE(g.is_edge(2,3));
}

TEST(Ugraph_sparse_test,ugraph_from_file){
///////////////////
// correct use of ugraph object to read undirected DIMACS graph

	sparse_ugraph g1("brock200_1.clq");
	sparse_ugraph g2("brock200_2.clq");
	sparse_ugraph g3("brock200_3.clq");
	sparse_ugraph g4("brock200_4.clq");

	//Number of edges
	EXPECT_EQ(14834,g1.number_of_edges());
	EXPECT_EQ(9876,g2.number_of_edges());
	EXPECT_EQ(12048,g3.number_of_edges());
	EXPECT_EQ(13089,g4.number_of_edges());

	//is undirected
	EXPECT_EQ(g1.is_edge(3,2),g1.is_edge(2,3));
	EXPECT_EQ(g1.is_edge(4,2),g1.is_edge(2,4));

	//Number of vertices
	EXPECT_EQ(200,g1.number_of_vertices());
	EXPECT_EQ(200,g2.number_of_vertices());
	EXPECT_EQ(200,g3.number_of_vertices());
	EXPECT_EQ(200,g4.number_of_vertices());

	//Name
	EXPECT_STREQ("brock200_1.clq",g1.get_name().c_str());
	EXPECT_STREQ("brock200_2.clq",g2.get_name().c_str());
	EXPECT_STREQ("brock200_3.clq",g3.get_name().c_str());
	EXPECT_STREQ("brock200_4.clq",g4.get_name().c_str());
}

TEST(Ugraph_sparse_test, numberof_edges){
	sparse_ugraph g(140);
	g.add_edge(1,4);
	g.add_edge(0,4);
	g.add_edge(5,7);
	g.add_edge(0,5);
	g.add_edge(7,128);

	EXPECT_EQ(5,g.number_of_edges());
}

TEST(Ugraph_sparse_test, write_to_file){
	sparse_ugraph sug(1000);
	sug.add_edge(1,3);
	sug.add_edge(5,7);
	sug.add_edge(100,999);
	sug.add_edge(0,10);
	ofstream f("logsug.txt");
	sug.write_dimacs(f);
	f.close();

}

TEST(Ugraph_sparse, adjacency_matrix){
	
	const int NV_INF=100, NV_SUP=1000, INC_SIZE=100, REP_MAX=10;
    const double DEN_INF=.1,DEN_SUP=.9, INC_DENSITY=.1;

    for(int N=NV_INF; N<NV_SUP; N+=INC_SIZE)  {
        for(double P=DEN_INF; P<DEN_SUP;P+=INC_DENSITY){
            for(int rep=0;rep<REP_MAX;rep++){
				 cout<<"--------------------------------------------------------"<<endl;
				sparse_ugraph ug;
				SparseRandomGen<>().create_ugraph(ug,N,P);
				sparse_ugraph ug2(ug);
			

				//deletes higher triangle
				for(int i=0; i<N; i++){
					ug.get_neighbors(i).erase_bit(i, N-1);
				}

				//expected number of edges is 0 because it is only reading upper triangle
				ASSERT_EQ(0, ug.number_of_edges(false));
				
				//deletes lower triangle
				BITBOARD E=ug2.number_of_edges(false);
				for(int i=0; i<N; i++){
					ug2.get_neighbors(i).erase_bit(0, i);
				}

				//expected number of edges is the same as before (only reading upper triangle)
				ASSERT_EQ(E, ug2.number_of_edges(false));
												
				cout<<"[N:"<<N<<" p:"<<P<<" r:"<<rep<<"]"<<endl;
			}
		}
	}
}

