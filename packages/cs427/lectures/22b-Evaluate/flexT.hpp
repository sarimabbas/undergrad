// ==========================================================================
// Template declaration for a flexible array of base type T.
// A. Fischer, May 14, 2001                                   file: flexT.hpp

#pragma once

#include "tools.hpp"
#define FLEX_START 4    // Default length for initial array.

template <class T>
class FlexArray {
  protected:// ------------------------------------------------------------
    int allocSize;          // Current allocation size.
    int n;            // Number of array slots that contain data.
    T* store;          // Pointer to dynamic array of T.

  private: // --------------------------------------------------------------
    void grow();      // Double the allocation length.

  public: // ---------------------------------------------------------------
    FlexArray( int ss = FLEX_START ) : allocSize(ss), n(0), store( new T[allocSize] ) {}
    ~FlexArray() { if (store != NULL) delete[] store; }

    int put( T data );
    T&  operator[]( int k );
    int flexlen() const { return n; }
    T*  extract() { T* tmp=store; store=NULL; allocSize = n = 0; return tmp; }
    ostream& print( ostream& out ) const {
        for (int k=0; k<n; ++k) out << store[k] <<" ";
        return out;
    }
};
template <class T> inline ostream&
operator<< ( ostream& out, FlexArray<T>& F){ return F.print(out); }

// -------------------------------------------- copy a T into the FlexArray.
template <class T> int
FlexArray<T>::put( T data ) {
    if ( n == allocSize ) grow();    // Create more space if necessary.
    store[n] = data;
    return n++;            // Return subscript at which item was stored.
}

//------------------------------------------- access the kth T in the array.
template <class T> T&
FlexArray<T>::operator[]( int k ) {
    if ( k >= n ) throw Fatal( "Flex_array bounds error." );
    return store[k];               // Return reference to desired array slot.
}

// ------------------------------------- double the allocation length.
template <class T> void
FlexArray<T>::grow() {
    T* temp = store;                     // hang onto old data array.
    allocSize>0 ? allocSize*=2 : allocSize = FLEX_START;
    store = new T[allocSize];                  // allocate a bigger one.
    memcpy(store, temp, n*sizeof(T));    // copy info into new array.
    delete temp;                        // recycle (free) old array.
            // but do not free the things that were contained in it.
}
