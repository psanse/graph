//filter_graph_sort_type.h : filter types for GraphSort<T> metaclass 
//Graphs which are not undirected cannot be sorted

template <class Graph_t>
struct filterGraphSort;		//error at compile time

//valid specializations
template <>
struct filterGraphSort<ugraph>{};
template <>
struct filterGraph<sparse_ugraph>{};
