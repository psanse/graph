//filter.h : filter types for Graph<T> metaclass 
// A Graph cannot be constructed on watched_bitstrings

template <class T>
struct filterGraph;		//error at compile time

//valid specializations
template <>
struct filterGraph<bitarray>{};
template <>
struct filterGraph<sparse_bitarray>{};
