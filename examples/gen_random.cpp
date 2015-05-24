//an example to generate a benchmark of simple undirected middle-size graphs  
//author:pablo san segundo
//inital date: 24/05/2015
// last update: 24/05/15

#include <iostream>
#include <string.h>
#include <string>
#include "../graph.h"
#include "../graph_gen.h"

using namespace std;

#define LOG_BENCHMARK "C:/Users/Pablo/Desktop/logrand/"

int main(){
	ugraph g;
	
	//create a single graph
	RandomGen<ugraph>::create_ugraph(g,100,.1);
	g.print_data();

	//create a benchmark
	random_attr_t rt(1000, 1001, .01, .31, 10, 50, .1);
	RandomGen<ugraph>::create_ugraph_benchmark(LOG_BENCHMARK, rt);

}

