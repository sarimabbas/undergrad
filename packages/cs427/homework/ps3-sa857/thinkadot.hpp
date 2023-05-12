// ============================================================================
// Name: ThinkADot device class                             File: thinkadot.hpp
// Author:  Sarim Abbas                                 Copyright: October 2018
// Modified October 2018
// ============================================================================
#ifndef THINKADOT_HPP
#define THINKADOT_HPP
#define FLIPFLOP_COUNT 8

#include "tools.hpp"
#include "flipflop.hpp"

class ThinkADot {
    private:
        FlipFlop flipFlops[FLIPFLOP_COUNT];

        char recurse( int position );

    public:

        ThinkADot();

        void reset( char key );
        
        ostream& print(ostream& out) const;

        char play( char in_hole );
};

/**
 * @brief : extension of << to print ThinkADot objects to stream
 *
 * @param out : stream to print to
 * @param tad : ThinkADot object
 * @return ostream& : modified stream
 */
inline ostream& operator<<(ostream& out, const ThinkADot& tad) {
    return tad.print(out);
}

#endif
