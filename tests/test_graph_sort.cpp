
#include <iostream>
#include "google/gtest/gtest.h"
#include "../graph.h"
#include "../algorithms/graph_sort.h"
#include "pablodev/utils/common.h"

using namespace std;
TEST(Graph_sort, basic_min_width){
	
	cout<<"-----------------------------------"<<endl;
	const int SIZE=10;
	ugraph ug(SIZE);
	ug.add_edge(0,1);
	ug.add_edge(1,2);
	ug.add_edge(2,3);
	ug.add_edge(0,3);

	GraphSort<ugraph> gs(ug);
	ug.print_data(); 
	int edges=ug.number_of_edges();
	vector<int> new_order=gs.new_order(GraphSort<ugraph>::MIN_DEG_DEGEN,GraphSort<ugraph>::PLACE_FL);
	gs.reorder(new_order, &cout);

	//solution 
	vector<int> sol(10);
	sol[0]=6; sol[1]=7;  sol[2]=8;  sol[3]=9; 
	int i=4;
	int index=0;
	while(i<10){
		sol[i]=index; 
		i++; index++;
	}

	cout<<endl; ug.print_data(); cout<<endl;
	EXPECT_EQ(SIZE, ug.number_of_vertices());
	EXPECT_EQ(edges, ug.number_of_edges(false));
	EXPECT_EQ(sol, new_order);

	cout<<"-----------------------------------"<<endl;
}

TEST(Graph_sort_sparse, basic_min_width){
	
	cout<<"-----------------------------------"<<endl;
	const int SIZE=10;
	sparse_ugraph ug(SIZE);
	ug.add_edge(0,1);
	ug.add_edge(1,2);
	ug.add_edge(2,3);
	ug.add_edge(0,3);

	GraphSort<sparse_ugraph> gs(ug);
	ug.print_data(); 
	int edges=ug.number_of_edges();
	vector<int> new_order=gs.new_order(GraphSort<sparse_ugraph>::MIN_DEG_DEGEN,GraphSort<sparse_ugraph>::PLACE_FL);
	gs.reorder(new_order, &cout);

	//solution 
	vector<int> sol(10);
	sol[0]=6; sol[1]=7;  sol[2]=8;  sol[3]=9; 
	int i=4;
	int index=0;
	while(i<10){
		sol[i]=index; 
		i++; index++;
	}

	cout<<endl; ug.print_data(); cout<<endl;
	EXPECT_EQ(SIZE, ug.number_of_vertices());
	EXPECT_EQ(edges, ug.number_of_edges(false));
	EXPECT_EQ(sol, new_order);

	cout<<"-----------------------------------"<<endl;
}

TEST(Graph_sort, basic_max_width){
	
	cout<<"-----------------------------------"<<endl;
	const int SIZE=10;
	ugraph ug(SIZE);
	ug.add_edge(0,1);
	ug.add_edge(1,2);
	ug.add_edge(2,3);
	ug.add_edge(0,3);

	GraphSort<ugraph> gs(ug);
	ug.print_data(); 
	int edges=ug.number_of_edges();
	vector<int> new_order=gs.new_order(GraphSort<ugraph>::MAX_DEG_DEGEN, GraphSort<ugraph>::PLACE_FL);
	gs.reorder(new_order, &cout);

	//solution 
	vector<int> sol(10);
	for(int i=0; i<10; i++){
		sol[i]=i;
	}
	sol[1]=2; sol[2]=1;		//consequence of degenerate sorting
	
	cout<<endl; ug.print_data(); cout<<endl;
	EXPECT_EQ(SIZE, ug.number_of_vertices());
	EXPECT_EQ(edges, ug.number_of_edges(false));
	EXPECT_EQ(sol, new_order);

	cout<<"-----------------------------------"<<endl;
}

TEST(Graph_sort, basic_subgraph){
	
	cout<<"-----------------------------------"<<endl;
	const int SIZE=10;
	ugraph ug(SIZE);
	ug.add_edge(0,1);
	ug.add_edge(1,2);
	ug.add_edge(2,3);
	ug.add_edge(0,3);
	bitarray subgraph(10);
	subgraph.set_bit(1,5);

	GraphSort<ugraph> gs(ug);
	ug.print_data(); 
	vector<int> new_order=gs.new_subg_order(GraphSort<ugraph>::MIN_DEG_DEGEN, subgraph, GraphSort<ugraph>::PLACE_FL);
	
	//solution 
	vector<int> sol(subgraph.popcn64());
	sol[0]=4; sol[1]=5; //isolani first
	sol[2]=1; sol[3]=2; sol[4]=3;
	EXPECT_EQ(sol, new_order);

	cout<<"-----------------------------------"<<endl;
}


TEST(Graph_sort, basic_kcore){
	cout<<"-----------------------------------"<<endl;
	const int SIZE=10;
	ugraph ug(SIZE);
	ug.add_edge(0,1);
	ug.add_edge(1,2);
	ug.add_edge(2,3);
	ug.add_edge(0,3);

	GraphSort<ugraph> gs(ug);
	ug.print_data(); 
	
	//non decreasing kcore ordering
	vector<int> new_order=gs.new_order(GraphSort<ugraph>::KCORE,GraphSort<ugraph>::PLACE_FL);
	com::stl::print_collection<vector<int>>(new_order); cout<<endl;

	vector<int> sol(10);
	sol[0]=6; sol[1]=7;  sol[2]=8;  sol[3]=9; 
	sol[4]=0; sol[5]=1;  sol[6]=2;  sol[7]=3; sol[8]=4; sol[9]=5;

	EXPECT_EQ(sol, new_order);

	//non increasing kcore ordering (standard for clique)
	new_order=gs.new_order(GraphSort<ugraph>::KCORE,GraphSort<ugraph>::PLACE_LF);
	com::stl::print_collection<vector<int>>(new_order);

	sol[0]=3; sol[1]=2;  sol[2]=1;  sol[3]=0; 
	sol[4]=9; sol[5]=8;  sol[6]=7;  sol[7]=6; sol[8]=5; sol[9]=4;

	EXPECT_EQ(sol, new_order);

	cout<<"-----------------------------------"<<endl;
}


//TEST(Graph_sort, graph_from_file){
/////////////////////
//// Undirected graphs read by directed graph class (all edges are non symmetrical)
//	
//	graph g1("brock200_1.clq");
//	//*** TODO **
//}



