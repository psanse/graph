
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

TEST(Decode, basic) {
	vector<int> v, e;
	v.push_back(1);v.push_back(2);v.push_back(0);
	Decode::reverse_in_place(v);
	e.push_back(2);e.push_back(0);e.push_back(1);
	EXPECT_EQ(e, v);
}

TEST(Graph_sort, Decode){
////////////
// decoding a sequence of orderings

	cout<<"Order: Decode------------------------"<<endl;
	ugraph ug(106);    
    ug.add_edge(1, 2);
    ug.add_edge(1, 3);
    ug.add_edge(1, 4);
	ug.add_edge(78, 5);
	ug.print_data();
		
	ugraph ug1(ug);

//MIN WIDTH algorithm
	cout<<endl<<"init min width----------------------"<<endl;
	GraphSort<ugraph> o(ug1);
	vint vres=o.new_order(GraphSort<ugraph>::MIN_DEG_DEGEN, GraphSort<ugraph>::PLACE_LF); cout<<endl;
	copy(vres.begin(), vres.end(), ostream_iterator<int>(cout, " "));
	o.reorder(vres);

	Decode d;
	d.reverse_in_place(vres);
	d.insert_ordering(vres);

	vint vlist;
	vlist.push_back(105);
	vlist.push_back(104);
	vlist.push_back(103);
	vlist.push_back(102);
	vlist.push_back(101);
	vlist.push_back(100);
	
	vint dec=d.decode_list(vlist);
	cout<<endl;
	com::stl::print_collection(dec);
			
	cout<<"--------------------------------------"<<endl;
}

TEST(GraphSort, support){
	cout<<"--------------------------------------------------------"<<endl;

	//-------------------------------------------------------------------------
	//Ugraph
	ugraph ug(100);
	GraphSort<ugraph> o(ug);
	ug.add_edge(0, 1);
	ug.add_edge(1, 2);
	ug.add_edge(2, 80);
	EXPECT_EQ(3,o.sum_of_neighbor_deg(1));
	cout<<"Test support ugraph passed"<<endl;
	cout<<"--------------------------------------------------------"<<endl;
	
	
	//-------------------------------------------------------------------------
	//Sparse Ugraph
	sparse_ugraph usg(100);
	usg.add_edge(0, 1);
	usg.add_edge(1, 2);
	usg.add_edge(2, 80);
	GraphSort<sparse_ugraph> os(usg);
	EXPECT_EQ(3,os.sum_of_neighbor_deg(1));
	cout<<"Test support sparse ugraph passed"<<endl;
	cout<<"--------------------------------------------------------"<<endl;

}

TEST(GraphSort, normal_subgraph){		//*** TODO: change test to GRAPH block"

cout<<"Order: normal_subgraph-------------------------"<<endl;
	ugraph ug(106);    
    ug.add_edge(1, 2);
    ug.add_edge(1, 3);
    ug.add_edge(1, 4);
	ug.add_edge(78, 5);
		
	//subgraph
	bitarray subg(106);
	subg.set_bit(1, 5);

//MIN WIDTH algorithm
	ugraph ug1(ug);
	cout<<endl<<"init min width----------------------"<<endl;
	GraphSort<ugraph> o(ug1);
	vint vres=o.new_subg_order(GraphSort<ugraph>::MIN_DEG_DEGEN, subg, GraphSort<ugraph>::PLACE_FL); cout<<endl;
	com::stl::print_collection(vres);
	
	vint vsol(subg.popcn64());
	vsol[0]=5; vsol[1]=2; vsol[2]=3; vsol[3]=1; vsol[4]=4;					//MIN WIDTH first to last
	EXPECT_EQ(vsol, vres);
	EXPECT_EQ(subg.popcn64(),vres.size());
	
	//reverse order
	ug1=ug;
	vres=o.new_subg_order(GraphSort<ugraph>::MIN_DEG_DEGEN, subg, GraphSort<ugraph>::PLACE_LF); 
	reverse(vsol.begin(), vsol.end());
	EXPECT_EQ(vsol, vres);
	
//MIN WIDTH MIN TIE STATIC algorithm
	ug1=ug;
	ug1.add_edge(2, 3);														//penalizes these edges since they now have more support
	ug1.print_data();							
	cout<<endl<<"init min width min tie static----------------------"<<endl;
	GraphSort<ugraph> o1(ug1);
	subg.print();
	vres=o1.new_subg_order(GraphSort<ugraph>::MIN_DEG_DEGEN_TIE_STATIC, subg,  GraphSort<ugraph>::PLACE_FL); 
	vsol[0]=5; vsol[1]=4; vsol[2]=1; vsol[3]=2; vsol[4]=3;	
	EXPECT_EQ(vsol, vres);
	EXPECT_EQ(subg.popcn64(),vres.size());

	com::stl::print_collection(vres);
	
	cout<<"--------------------------------------"<<endl;
}

TEST(GraphSort, sparse_subgraph){
cout<<"Order: sparse_subgraph-------------------------"<<endl;
	sparse_ugraph ug(106);    
    ug.add_edge(1, 2);
    ug.add_edge(1, 3);
    ug.add_edge(1, 4);
	ug.add_edge(78, 5);
		
	//subgraph
	sparse_bitarray subg(106);
	subg.set_bit(1, 5);

//MIN WIDTH algorithm
	sparse_ugraph ug1(ug);
	cout<<endl<<"init min width----------------------"<<endl;
	GraphSort<sparse_ugraph> o(ug1);
	vint vres=o.new_subg_order(GraphSort<sparse_ugraph>::MIN_DEG_DEGEN, subg, GraphSort<sparse_ugraph>::PLACE_FL); cout<<endl;
	com::stl::print_collection(vres);
		
	vint vsol(subg.popcn64());
	vsol[0]=5; vsol[1]=2; vsol[2]=3; vsol[3]=1; vsol[4]=4;					//MIN WIDTH first to last
	EXPECT_EQ(vsol, vres);
	EXPECT_EQ(subg.popcn64(),vres.size());
	
	//reverse order
	vres=o.new_subg_order(GraphSort<sparse_ugraph>::MIN_DEG_DEGEN, subg); 
	reverse(vsol.begin(), vsol.end());
	EXPECT_EQ(vsol, vres);
	
//MIN WIDTH MIN TIE STATIC algorithm
	ug1=ug;
	ug1.add_edge(2, 3);	
	
	//penalizes these edges since they now have more support
	cout<<endl<<"init min width min tie static----------------------"<<endl;
	GraphSort<sparse_ugraph> o1(ug1);
	vres=o1.new_subg_order(GraphSort<sparse_ugraph>::MIN_DEG_DEGEN_TIE_STATIC, subg, GraphSort<sparse_ugraph>::PLACE_FL); 
	vsol[0]=5; vsol[1]=4; vsol[2]=1; vsol[3]=2; vsol[4]=3;	
	EXPECT_EQ(vsol, vres);
	EXPECT_EQ(subg.popcn64(),vres.size());

	com::stl::print_collection(vres);
	
	cout<<"--------------------------------------"<<endl;
}

TEST(GraphSort, basic_new_graph_sort){
	cout<<"Order: basic_new_graph_sort-------------------------"<<endl;

	ugraph ug(106);    
    ug.add_edge(1, 2);
    ug.add_edge(1, 3);
    ug.add_edge(1, 4);
	ug.add_edge(78, 5);

	GraphSort<ugraph> cs(ug);
	cs.new_order(GraphSort<ugraph>::MIN_DEG_DEGEN, GraphSort<ugraph>::PLACE_LF);

	cout<<"--------------------------------------"<<endl;
}

TEST(GraphSort_in_place, reorder_simple){
        
    int myints[] = {5,4,2,3,1,0};
    std::vector<int> sol1 (myints, myints + (sizeof(myints) / sizeof(int)) );
    int myints2[] = {5,3,1,2,4,0};
    std::vector<int> sol2 (myints2, myints2 + (sizeof(myints2) / sizeof(int)) );
    cout<<"--------------------------------------------------------"<<endl;
	
    //-------------------------------------------------------------------------
    //Ugraphs
    sparse_ugraph ug(106);    
    GraphSort<sparse_ugraph> o(ug);
    ug.add_edge(3, 1);
    ug.add_edge(3, 2);
    ug.add_edge(3, 4);
    ug.add_edge(3, 5);
    ug.add_edge(2, 5);
    ug.add_edge(4, 5);
    ug.add_edge(78, 5);

    sparse_ugraph ug2(106);
    GraphSort<sparse_ugraph> o2(ug2);
    ug2.add_edge(3, 1);
    ug2.add_edge(3, 2);
    ug2.add_edge(3, 4);
    ug2.add_edge(3, 5);
    ug2.add_edge(2, 5);
    ug2.add_edge(4, 5);
    ug2.add_edge(78, 5);
    cout<<"1:"<<endl;
    ug.print_edges();
    cout<<"2:"<<endl;
    ug2.print_edges();
    cout<<"--------------"<<endl;
   
	//checks reverse order
    GraphSort<sparse_ugraph>::print(o.new_order(GraphSort<sparse_ugraph>::NONE, GraphSort<sparse_ugraph>::PLACE_LF));		
    o.reorder(o.new_order(GraphSort<sparse_ugraph>::NONE));
    o2.reorder_in_place(o2.new_order(GraphSort<sparse_ugraph>::NONE));
    cout<<"1:"<<endl;
    ug.print_edges();
    cout<<"2:"<<endl;
    ug2.print_edges();
    cout<<"--------------"<<endl;
   
	//Comparison between both graphs edge by edge
	//(currently operator == between graphs is not working because equality between sparse_bitarrays does not check empty bitblocks)
    int j,j2;
    ASSERT_TRUE(ug.number_of_vertices()==ug2.number_of_vertices());
    for(int i=0;i<ug.number_of_vertices();i++){
        sparse_bitarray neigh=ug.get_neighbors(i);
		sparse_bitarray neigh2=ug2.get_neighbors(i);
        if((neigh.init_scan(bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM) && (neigh2.init_scan(bbo::NON_DESTRUCTIVE)!=EMPTY_ELEM)) {
            while(true){
                j=neigh.next_bit();
                j2=neigh2.next_bit();
                ASSERT_TRUE(j==j2);
                if(j==EMPTY_ELEM)
                        break;                
            }
        }
    }

    cout<<"--------------------------------------------------------"<<endl;
}







//TEST(Graph_sort, graph_from_file){
/////////////////////
//// Undirected graphs read by directed graph class (all edges are non symmetrical)
//	
//	graph g1("brock200_1.clq");
//	//*** TODO **
//}



