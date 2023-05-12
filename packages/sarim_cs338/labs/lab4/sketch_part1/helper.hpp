#ifdef __cplusplus
extern "C" {
#endif

#include "concurrency.h"

//////////// * create, destroy and search functions

psm* psmCreate(void);
void psmDestroy(psm** managerPtr);
process_t* psmFind(psm* manager, unsigned int sp);

//////////// * add and remove functions

unsigned int psmPop(psm* manager);
int psmPushToBack(psm* manager, unsigned int sp);
int psmPushToFront(psm* manager, unsigned int sp);

//////////// * print functions

void SerialPrintWrapper(char* msg);
void pPrint(process_t* process);
void psmPrint(psm* manager);

//////////// * interrupt functions
void disableInterruptsWrapper();
void enableInterruptsWrapper();

#ifdef __cplusplus
}
#endif
