// ============================================================================
// Name: ThinkADot FlipFlop class declarations               File: flipflop.hpp
// Author:  Sarim Abbas                                 Copyright: October 2018
// Modified October 2018
// ============================================================================
#ifndef FLIPFLOP_HPP
#define FLIPFLOP_HPP

#include "tools.hpp"

class FlipFlop {
    private:
        int state = NONE;
        enum FlipFlopState {LEFT, RIGHT, NONE};

        inline char privateFlip( void );

    public:

        char flip( void );

        char getState( void ) const;

        void setState( char input );

        ostream& print( ostream& out ) const;

};

/**
 * @brief : extension of << to print FlipFlop objects to stream
 *
 * @param out : stream to print to
 * @param ff : FlipFlop object
 * @return ostream& : modified stream
 */
inline ostream& operator<<(ostream& out, const FlipFlop& ff) {
    return ff.print(out);
}

#endif
