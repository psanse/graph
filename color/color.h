//color.h: header for Color class: computes different colorings for simple undirected graphs
//
// author: luna
// first update: 21/10/15


#include "../graph.h"
#include "pablodev\utils\common.h"

#ifndef _COLOR_H_
#define _COLOR_H_

class Color{
public:
	Color(ugraph& gout):g(gout){}

	//greedy sequential coloring
	int SEQ();						//colorea g
	int SEQ(bitarray& bb);			//colorea el subgrafo inducido por el conjunto de vertices en bb		
	


private:
///////////////////
// data members
	ugraph& g;
};



#endif