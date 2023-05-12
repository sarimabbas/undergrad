#include "driver.hpp"

int main(int argc, char** argv) {

    // mimic the structure of Figure 1

    // create genesis block
    Block* genesis_block = new Block();

    // create the first sptr to manage it
    SPtr genesis_sptr = SPtr( genesis_block );

    // create 4 blockchains/agents:
    // 0    1   2   3
    // ChA  ChB ChC ChD
    Blockchain bc[4];
    for(int i = 0; i < 4; i++) {
        bc[i] = Blockchain( genesis_sptr );
    }

    // extend ChC
    bc[2] = bc[2].extend();

    // make ChA point to same block
    bc[0] = bc[2];

    // extend ChC
    bc[2] = bc[2].extend();

    // make ChB point to same block
    bc[1] = bc[2];

    // extend ChD
    bc[3] = bc[3].extend();

    // print all the blockchains
    cout << bc[0] << endl;
    cout << bc[1] << endl;
    cout << bc[2] << endl;
    cout << bc[3] << endl;

    // make ChD point elsewhere (should result in delete)
    bc[3] = bc[2];

    // print all the blockchains
    cout << bc[0] << endl;
    cout << bc[1] << endl;
    cout << bc[2] << endl;
    cout << bc[3] << endl;

    // exit
    return 0;
}