#include <iostream>
#include <fstream>
#include <string>
#include "google/gtest/gtest.h"
#include "../graph_gen.h"
#include "../kcore.h"

using namespace std;

template <class Collection>
void printCol(const Collection& c, ostream& o=cout ){
	o<<endl;
	copy(c.begin(), c.end(), ostream_iterator<typename Collection::value_type>(o," " ));
	o<<endl;
}

TEST(KCore, kcore_example){
	ugraph ug("brock200_1.clq");
			
	KCore<ugraph> kc(ug);
	kc.kcore();
	EXPECT_EQ(134, kc.get_kcore_number());
}

TEST(KCore, kcore_example_I){
	ugraph ug("ia-southernwomen.edges");
			
	KCore<ugraph> kc(ug);
	kc.kcore();
	EXPECT_EQ(6, kc.get_kcore_number());
}

TEST(KCore, kcore_clq){
	ugraph ug("brock200_1.clq");
		
	KCore<ugraph> kc(ug);
	kc.kcore();
	printCol(kc.find_heur_clique());
	printCol(kc.find_heur_clique(10));
}


TEST(KCore, kcore_example_sparse){
	sparse_ugraph ug("brock200_1.clq");

	KCore<sparse_ugraph> kc(ug);
	kc.kcore();
	EXPECT_EQ(134, kc.get_kcore_number());

	sparse_bitarray bbs(ug.number_of_vertices());
	bbs.set_bit(1,2);
	EXPECT_TRUE(ug.is_edge(1,2));
	KCore<sparse_ugraph> kc1(ug, &bbs);
	kc1.kcore();
	EXPECT_EQ(1, kc1.get_kcore_number());
	kc1.print_kcore();
	cout<<"-----------------------------------------"<<endl;
}

TEST(KCore, change_subgraph){
	sparse_ugraph ug("brock200_1.clq");

	KCore<sparse_ugraph> kc(ug);
	kc.kcore();
	EXPECT_EQ(134, kc.get_kcore_number());

	//change subgraph to analyse
	sparse_bitarray bbs(ug.number_of_vertices());
	bbs.set_bit(1,2);
	EXPECT_TRUE(ug.is_edge(1,2));
	kc.set_subgraph(&bbs);
	kc.kcore();
	EXPECT_EQ(1, kc.get_kcore_number());
	kc.print_kcore();

	//change to full graph again
	bbs.set_bit(0, ug.number_of_vertices()-1);
	kc.set_subgraph(&bbs);
	kc.kcore();
	EXPECT_EQ(134, kc.get_kcore_number());
	
	cout<<"-----------------------------------------"<<endl;
}

TEST(KCore, kcore_decomposition){
	ugraph ug("star.clq");			//0, 1, 6 form a 3-clique
	
	KCore<ugraph> kc(ug);
	kc.kcore();
	vector<int> v= kc.get_kcore_numbers(2);
	
	EXPECT_EQ(1, count(v.begin(), v.end(), 0));
	EXPECT_EQ(1, count(v.begin(), v.end(), 1));
	EXPECT_EQ(1, count(v.begin(), v.end(), 6));
	EXPECT_EQ(0, count(v.begin(), v.end(), 7));

	////////////
	v.clear();
	ugraph ug1(100);							//an empty graph
	ug1.add_edge(0,1);
	ug1.add_edge(0,2);
	ug1.add_edge(0,3);
	ug1.add_edge(0,4);
	ug1.add_edge(0,5);

	KCore<ugraph> kc1(ug1);
	kc1.kcore();
	
	v= kc1.get_kcore_numbers(1);
	for(int i=0; i<6; i++)
		EXPECT_EQ(i, v[i]);
}

TEST(KCore, filter_simple){
	ugraph ug("star.clq");			//0, 1, 6 form a 3-clique
	map<int,int> filter;
	
	KCore<ugraph> kc(ug);
	kc.kcore();
	
	//standard use for max clique (vertices will be ordered in this way)
	kc.make_kcore_filter(filter);	
	EXPECT_EQ(3,filter[2]);
	EXPECT_EQ(1,filter.size());

	//direct order: assume vertices are order by increasing kcore number
	kc.make_kcore_filter(filter,false);
	EXPECT_EQ(8,filter[1]);
	EXPECT_EQ(1,filter.size());
			
	//I/O
	/*cout<<kc<<endl;
	for(map_it it=filter.begin(); it!=filter.end(); ++it){
		cout<<"["<<it->first<<","<<it->second<<"]"<<endl;
	}*/
	
}

TEST(KCore, filter_non_kcore_vakues){
	ugraph ug("auxfilerandomTEST.txt");			//kcores are (v:k(v)) 4:0, 0:1, rest have k(v)=4
	map<int,int> filter;
	
	KCore<ugraph> kc(ug);
	kc.kcore();
	
	//standard use for max clique (vertices will be ordered in this way)
	kc.make_kcore_filter(filter, true);	
	EXPECT_EQ(9,filter[2]);
	EXPECT_EQ(8,filter[3]);				//3 is not a core order
	EXPECT_EQ(8,filter[4]);
}


TEST(KCore, kcore_decomposition_sparse){
	sparse_ugraph ug("star.clq");
	
	KCore<sparse_ugraph> kc(ug);
	kc.kcore();
	vector<int> v= kc.get_kcore_numbers(2);
	EXPECT_EQ(1, count(v.begin(), v.end(), 0));
	EXPECT_EQ(1, count(v.begin(), v.end(), 1));
	EXPECT_EQ(1, count(v.begin(), v.end(), 6));
	EXPECT_EQ(0, count(v.begin(), v.end(), 7));
	
	////////////
	v.clear();
	sparse_ugraph ug1(100);							//an empty graph
	ug1.add_edge(0,1);
	ug1.add_edge(0,2);
	ug1.add_edge(0,3);
	ug1.add_edge(0,4);
	ug1.add_edge(0,5);

	KCore<sparse_ugraph> kc1(ug1);
	kc1.kcore();
	
	v= kc1.get_kcore_numbers(1);
	for(int i=0; i<6; i++)
		EXPECT_EQ(i, v[i]);
	
}

TEST(Ugraph_sparse, find_clique){
	
	const int NV_INF=100, NV_SUP=1000, INC_SIZE=100, REP_MAX=10;
    const double DEN_INF=.1,DEN_SUP=.9, INC_DENSITY=.1;

    for(int N=NV_INF; N<NV_SUP; N+=INC_SIZE)  {
        for(double P=DEN_INF; P<DEN_SUP;P+=INC_DENSITY){
            for(int rep=0;rep<REP_MAX;rep++){
				 cout<<"--------------------------------------------------------"<<endl;
				sparse_ugraph ug;
				SparseRandomGen<>().create_ugraph(ug,N,P);
				KCore<sparse_ugraph> kc(ug);
				kc.kcore();
				
				vector<int> v1=kc.find_heur_clique();
				vector<int> v2=kc.find_heur_clique_opt();
				vector<int> v3=kc.find_heur_clique_sparse();
				printCol(v1);
				printCol(v2);
				printCol(v3);

				//determine clique for opt
				for(int i=0; i<v2.size()-1; i++){
					for(int j=i+1; j<v2.size(); j++){
						if(!ug.is_edge(v2[i],v2[j])){
							cerr<<"incorrect clique"<<endl;
						}
						ASSERT_TRUE(ug.is_edge(v2[i],v2[j]));
					}
				}

				//determine clique for opt
				for(int i=0; i<v3.size()-1; i++){
					for(int j=i+1; j<v3.size(); j++){
						if(!ug.is_edge(v3[i],v3[j])){
							cerr<<"incorrect clique"<<endl;
						}
						ASSERT_TRUE(ug.is_edge(v3[i],v3[j]));
					}
				}
																
				cout<<"[N:"<<N<<" p:"<<P<<" r:"<<rep<<"]"<<endl;
				ASSERT_EQ(v1.size(),v2.size());
				//ASSERT_EQ(v1.size(),v3.size());
			}
		}
	}
}

//
//TEST(KCore, find_clique_I){
//	sparse_ugraph ug("brock200_1.clq");
//		
//	KCore<sparse_ugraph> kc(ug);
//	kc.kcore();
//	vector<int> v1=kc.find_clique_lb();
//	vector<int> v2=kc.find_clique_lb_opt();
//	vector<int> v3=kc.find_clique_lb_opt_B();
//	EXPECT_EQ(v1.size(), v2.size());
//	EXPECT_EQ(v1.size(), v3.size());
//	printCol(v1);
//	printCol(v2);
//	printCol(v3);
//}
//
//TEST(KCore, find_clique_II){
//	sparse_ugraph ug("failed_lb.clq");
//		
//	KCore<sparse_ugraph> kc(ug);
//	kc.kcore();
//	vector<int> v1=kc.find_clique_lb();
//	vector<int> v2=kc.find_clique_lb_opt();
//	vector<int> v3=kc.find_clique_lb_opt_B();
//
//	//determine clique for opt
//	for(int i=0; i<v2.size()-1; i++){
//		for(int j=i+1; j<v2.size(); j++){
//			ASSERT_TRUE(ug.is_edge(v2[i],v2[j]));
//		}
//	}
//
//	//determine clique for opt_B
//	for(int i=0; i<v3.size()-1; i++){
//		for(int j=i+1; j<v3.size(); j++){
//			ASSERT_TRUE(ug.is_edge(v3[i],v3[j]));
//		}
//	}
//
//
//}










