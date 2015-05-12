[![Build Status](https://webapi.biicode.com/v1/badges/pablodev/pablodev/graph/master)](https://www.biicode.com/pablodev/graph) 

GRAPH library for bit-encoded graphs
===================

GRAPH is a currenty a  small  C++ library which manages basic functionality of **bit-encoded** simple **unweighted** graphs. At the heart of GRAPH is BITSCAN, an efficient library to manage bit strings. GRAPH's main class is Graph, a template for generic simple graphs. Ugraph is a specialization of the Graph template for undirected graphs only. Graph can also manage undirected graphs, but Ugraph implements some operations  more efficiently (such as counting edges or equality checks).

Also recently available in GRAPH is a graph analyser which gives useful information for unweighted graphs (currently **minimum and maximum graph degree**, **k-core analysis** and a reasonably tight **lower bound for the clique number** based on cores). The analyser is wrapped inside the `KCore` class template. 

GETTING STARTED
-------------------------------

To use GRAPH's basic functionality for simple graphs in your projects add the following header and execute the command *bii find*; this will download the library in the *deps* folder. 

	#include "pablodev/graph/graph.h"

To add k-core graph analysis (see GRAPH ANALYSIS section below) the header is:

	#include "pablodev/graph/kcore.h"

BUILDING GRAPHS
-------------------------------
GRAPH currently distinguishes 4 different types of graph objects:

- `graph`: for simple graphs
- `ugraph`: specialization of `graph` for undirected graphs
- `sparse_graph`: for sparse simple graphs
- `sparse_ugraph`: specialization of `sparse_graph` for sparse undirected graphs
 
Internally these names are specialized typedefs which hide a Graph class template instantiation of the different types of encodings.

To build a graph from scratch an empty graph of the correct order has to be declared first. Functions *add\-edge* and *delete\_edge* allow to customize edges at will. Here is an example:

    #include "pablodev/graph/graph.h"
    //...
    
    graph g(3);						//creates an empty simple graph with 100 vertices
    g.add_edge(0,1);				//configures the edges
	g.add_edge(0,2);
	g.add_edge(1,2);


Notice that Graph always adds directed edges, so this is not actually a 3-clique. If a 3-clique is what you need you should call the *make\_bidirected* function.
   
    g.make_bidirected()				//creates a 3-clique

A better option would have been to use the class Ugraph from the beginning to build the clique:
   
    ugraph g(3);					//creates an empty simple graph with 100 vertices
	g.add_edge(0,1);				//configures edges (symmetrical)
	g.add_edge(0,2);
	g.add_edge(1,2);

EDITING GRAPHS
-------------------------------

Edges may be added or deleted in constant time (*remove\_edge* member function) for simple graphs (in the case of sparse graphs complexity increases because it takes logarithmic time to find an edge). Removing vertices is possible as well (member function *remove\_vertices*) but requires memory allocation. Currently GRAPH does not support adding new vertices (the graph order should be passed to the constructor). Here is a trivial example:
   
    graph g(3);						//creates an empty graph with 100 vertices
	g.add_edge(0,1);				//configures the edges
	g.add_edge(0,2);
	g.add_edge(1,2);
	g.remove_edge(0,1);
	g.remove_edge(0,2);
	g.remove_edge(1,2);
    if(g.is_empty()) cout<<"empty graph"<<endl;			//will print the message on the screen

GRAPH ANALYSIS
-------------------------------
By Graph Analysis we refer to any type of precomputation which gives useful information about the graph, such as invariants, decompositions etc. The current release includes as major functionality [k-core decomposition analysis](http://en.wikipedia.org/wiki/Degeneracy_(graph_theory)) but also basic info such as density or maximum graph degree. Clique number is implemented in the [pablodev/copt](https://www.biicode.com/pablodev/copt "repo for compbinatorial optimization") block in the Biicode repository.

K-core analysis is wrapped inside the `KCore` class.  It computes core decomposition for the graph as well as for any subgraph. We note that the graph type should be undirected (type `ugraph` or `sparse_ugraph`). 

Here is an example:
    
    #include "pablodev/graph/graph.h" 
    #include "pablodev/graph/kcore.h" 
   				
    void main(){
	  sparse_ugraph ug("brock200_1.clq");
	  KCore<sparse_ugraph> kc(ug);				//configures KCore class
      kc.kcore();								//computes k-core analysis for the full graph
	  kc.print_kcore();	

	 sparse_bitarray bbs(ug.number_of_vertices());
	 bbs.init_bit(0,29);
	 kc.set_subgraph(&bbs);
	 kc.kcore();					//computes k-core for the subgraph induced by the first 30 vertices
    
	 kc.print_kcore();							
    }

k-core analysis has been [optimized](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&cad=rja&uact=8&ved=0CCgQFjAB&url=http%3A%2F%2Fvlado.fmf.uni-lj.si%2Fpub%2Fnetworks%2Fdoc%2Fcores%2Fcores.pdf&ei=Pe8FVJfZD6PIyAO0-IKIAQ&usg=AFQjCNFNFQZTbvdmsjXTqTSH1BFYf1ACKg&sig2=_leTrcnaQKbfFHpSwiZuKQ&bvm=bv.74115972,d.bGQ) and is extremely fast (i.e. runs in O(|E|). This makes it ideal for very large sparse graphs. Tests on real graphs with millions of vertices have been solved in but a few seconds.

    
EXTERNAL SOURCES
-------------------------------

Currently version GRAPH reads simple undirected unweighted graphs in ASCII DIMACS, Matrix Market Exchange and Edge list formats. A few examples of undirected graphs in different formats may be found in tha *data* folder of the block. To load a graph from file simply pass the appropiate filename to the constructor:

    
    graph g("foo-filename");				//loads "brock200_2.clq" file in DIMACS/MTX/EDGES in g
    
OTHERS
-------------------------------

Other basic functionality includes computing degree, density, comparing graphs etc. Examples can be found in the *tests* folder which uses google *gtest* framework imported from the Biicode Repository.


COMPATIBILITY
-------------------------------

GRAPH has been tested in Windows 64-bit OS and Linux OS. Please note that a 64-bit OS is mandatory becase of the BITSCAN dependency.

Acknowledgements
-------------------------------

This research has been partially funded by the Spanish Ministry of Economy and Competitiveness (MINECO), national grant DPI 2010-21247-C02-01.

