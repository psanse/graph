//filter.h : filter encoding bitstring types for Graph<T> metaclass 
// A Graph cannot be constructed on watched_bitstrings

template <class T>
struct filterGraphEncodingType;		//error at compile time

//valid specializations
template <>
struct filterGraphEncodingType<bitarray>{};
template <>
struct filterGraphEncodingType<sparse_bitarray>{};
