// Wrapper for a TEventList object -- called by R
//

#ifndef __EVENTLISTWRAPPER_H__
#define __EVENTLISTWRAPPER_H__

#include <string>

class TEventList;

extern "C"
{
  SEXP initEventListWrapper();
	
  SEXP deleteEventList(SEXP eventList);
  SEXP getEventListWithName(SEXP name, SEXP manageItsMemory);
  SEXP newEventList(SEXP name, SEXP title, SEXP entryNums);
	
  SEXP getName(SEXP eventList);
  SEXP getTitle(SEXP eventList);
  SEXP nEntriesEventList(SEXP eventList);
  SEXP getEntries(SEXP eventList);
	
  SEXP clear(SEXP eventList);
	
  SEXP isInEventList(SEXP eventList, SEXP entryNums);
  SEXP putIntoEventList(SEXP eventList, SEXP entryNums);
	
  SEXP addEventLists(SEXP eventList1, SEXP eventList2);
  SEXP subtractEventLists(SEXP eventList1, SEXP eventList2);
  //SEXP intersectEventLists(SEXP eventList1, SEXP eventList2);

}

TEventList* checkForEventListWrapper(SEXP ptr);

SEXP getEventListWithNameC(std::string name, bool manageItsMemory);

SEXP wrapTEventListPointer(TEventList* ptr, bool manageItsMemory);

#endif
