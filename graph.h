/*  
 * graph.h file from the GRAPH library, a C++ library for bit encoded 
 * simple unweighted graphs. GRAPH stores the adjacency matrix un full 
 * but each row is encoded as a bitsrting. GRAPH is at the core of  BBMC, a 
 * state of the art leading exact maximum clique algorithm. 
 * (see license file (legal.txt) for references)
 *
 * Copyright (C)
 * Author: Pablo San Segundo
 * Intelligent Control Research Group CAR(UPM-CSIC) 
 *
 * Permission to use, modify and distribute this software is
 * granted provided that this copyright notice appears in all 
 * copies, in source code or in binaries. For precise terms 
 * see the accompanying LICENSE file.
 *
 * This software is provided "AS IS" with no warranty of any 
 * kind, express or implied, and with no claim as to its
 * suitability for any purpose.
 *
 *
 * The one and only header file for simple graphs (encoded as BITSCAN bitstrings)
 */

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "simple_ugraph.h"

//the only possible graphs
typedef Graph<bitarray> graph;						//simple graph
typedef Ugraph<bitarray> ugraph;					//simple undirected graph
typedef Graph<sparse_bitarray> sparse_graph;		//simple sparse graph
typedef Ugraph<sparse_bitarray> sparse_ugraph;		//simple sparse undirected graph

#endif