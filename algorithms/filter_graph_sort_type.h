//filter_graph_sort_type.h : filter types for GraphSort<T> metaclass 
//Graphs which are not undirected cannot be sorted

template <class Graph_t>
struct filterGraphSortType;		//error at compile time

//valid specializations
template <>
struct filterGraphSortType<ugraph>{};
template <>
struct filterGraphSortType<sparse_ugraph>{};
