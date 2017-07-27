# naive_cpp_file

"jarray.h"is a naive data struct, which is implemented by shared_ptr and overload operator[].

Declare a jarray by 

**jarray t(/*size of this jarray*/);**

Or 

**jarray t{/*element of this jarray, like {1,2,3,4}*/};**

Declare a jarray2 only by //two-demention array

**jarray2 t(/*size of row,size of column*/);**

**Use jarray by*t[index]; and jarray2 by t[index_row][index_column];**

I forgot to initialize the array in its constructor or provide a overload construct to initialize its.

This struct is vary naive, so it only provide to mark my study.
