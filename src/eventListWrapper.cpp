// Wrapper for TEventList -- called by R

#include "TEventList.h"
#include "TFile.h"

extern "C" {
#include <R.h>
#include <Rdefines.h>
}

#include "eventListWrapper.h"

// Some definition of True is interfering
#undef TRUE

static SEXP el_type_tag;

//////////////////////////
// Check that the external pointer is the right thing
TEventList* checkForEventListWrapper(SEXP ptr)
{
  if ( TYPEOF(ptr) != EXTPTRSXP || 
       R_ExternalPtrTag(ptr) != el_type_tag ||
       ! R_ExternalPtrAddr(ptr) )
    error("Bad Pointer to EventListWrapper");
  
  return (TEventList*) R_ExternalPtrAddr(ptr);
}

/////////////////////////////////////////
// initEventListWrapper -- initialize
SEXP initEventListWrapper()
{
  el_type_tag = install("EventList_TAG");
  return R_NilValue;
}


//////////////////////////
// Get an event list given a name
//
//   If R is calling it, then we don't manage the memory!
SEXP getEventListWithName(SEXP name, SEXP mangeItsMemory)
{
  char* daName = CHAR(STRING_ELT(name, 0));
  bool doManage = LOGICAL(mangeItsMemory)[0] == 1;
  
  return getEventListWithNameC(daName, doManage);
}

//////////////////////////
// Get an event list given a name
SEXP getEventListWithNameC(char* name, bool manageItsMemory)
{
  
  TEventList* theList = (TEventList*) gDirectory->Get( name );
  
  if ( ! theList ) {
    REprintf("!! Could not find EventList with name %s\n", name);
    error("Abort!");
  }
  
  // Got an event list, wrap it in an R external pointer
  return wrapTEventListPointer(theList, manageItsMemory);
}

//////////////////////////////
// Wrap up a TEventList pointer and perhaps register the finalizer
SEXP wrapTEventListPointer(TEventList* ptr, bool manageItsMemory)
{
  SEXP val = R_MakeExternalPtr(ptr, el_type_tag, R_NilValue);
  
  if ( manageItsMemory ) {
    R_RegisterCFinalizerEx(val, 
			   (R_CFinalizer_t)deleteEventList, TRUE);
  }
  
  return val;
}

///////////////////////////////////
// Make a new event list given a name, title, and a set of events to put in it
SEXP newEventList(SEXP name, SEXP title, SEXP entryNums)
{
  char* daName = CHAR( STRING_ELT(name, 0) );
  char* daTitle = CHAR( STRING_ELT(title, 0) );
  
  int n = GET_LENGTH(entryNums);
  
  TEventList* el = new TEventList(daName, daTitle, n, 100);
  
  // Wrap it -- and I made it, so I manage it
  SEXP wrapPtr = wrapTEventListPointer(el, true);
  
  // Now fill it
  putIntoEventList(wrapPtr, entryNums);
  
  return wrapPtr;
}


///////////////////////////////////
// Delete the event list
SEXP deleteEventList(SEXP eventList)
{
  TEventList* el = checkForEventListWrapper(eventList);
  
  REprintf("Deleting TEventList at ptr %p\n", el);
  
  delete el;
  
  R_ClearExternalPtr(eventList);
  
  return R_NilValue;
}

///////////////////////
// Get the name of the event list
SEXP getName(SEXP eventList)
{
  TEventList* el = checkForEventListWrapper(eventList);
	
  SEXP name = NEW_CHARACTER(1);
  SET_STRING_ELT( name, 0, mkChar(el->GetName()) );
	
  return name;
}
	
///////////////////////
// Get the title of the event list
SEXP getTitle(SEXP eventList)
{
  TEventList* el = checkForEventListWrapper(eventList);
	
  SEXP name = NEW_CHARACTER(1);
  SET_STRING_ELT( name, 0, mkChar(el->GetTitle()) );
	
  return name;
}

///////////////////////
// Get the number of entries in the event list
SEXP nEntriesEventList(SEXP eventList)
{
  TEventList* el = checkForEventListWrapper(eventList);
	
  SEXP n = NEW_INTEGER(1);
  INTEGER(n)[0] = el->GetN();
	
  return n;
}

//////////////////////////
// Return all of the events as a big vector
SEXP getEntries(SEXP eventList)
{
  TEventList* el = checkForEventListWrapper(eventList);

  unsigned int l = el->GetN();
	
  SEXP e;
  PROTECT( e = NEW_INTEGER(l) );
	
  for ( unsigned int i = 0; i < l; ++i) {
    INTEGER(e)[i] = el->GetEntry(i);
  }
	
  UNPROTECT(1);
	
  return e;
}

////////////////////////
// Clear the event list
SEXP clear(SEXP eventList)
{
  TEventList* el = checkForEventListWrapper(eventList);
	
  el->Clear();
	
  return R_NilValue;
}

/////////////////////////
// Return a logical vector if entries are in the event list
SEXP isInEventList(SEXP eventList, SEXP entryNums)
{

  TEventList* el = checkForEventListWrapper(eventList);

  SEXP l;
  PROTECT(l = NEW_LOGICAL( GET_LENGTH(entryNums) ) );
	
  for ( unsigned int i = 0; i < GET_LENGTH(entryNums); ++i ) {
    LOGICAL(l)[i] = el->Contains( INTEGER(entryNums)[i] ) == 1;
  }
	
  UNPROTECT(1);
	
  return l;
}

/////////////////////////
// Put entries into the event list
SEXP putIntoEventList(SEXP eventList, SEXP entryNums)
{
	
  TEventList* el = checkForEventListWrapper(eventList);
	
  for ( unsigned int i = 0; i < GET_LENGTH(entryNums); ++i ) {
    el->Enter( INTEGER(entryNums)[i] );
  }

  return R_NilValue;
}

///////////////////////////////
// Add eventList2 to eventList1 -- this modifies eventList1
SEXP addEventLists(SEXP eventList1, SEXP eventList2)
{
  TEventList* el1 = checkForEventListWrapper(eventList1);
  TEventList* el2 = checkForEventListWrapper(eventList2);
	
  el1->Add(el2);
	
  return R_NilValue;
}

///////////////////////////////
// Subtract eventList2 from eventList1 -- this modifies eventList1
SEXP subtractEventLists(SEXP eventList1, SEXP eventList2)
{
  TEventList* el1 = checkForEventListWrapper(eventList1);
  TEventList* el2 = checkForEventListWrapper(eventList2);
	
  el1->Subtract(el2);
	
  return R_NilValue;
}

///////////////////////////////
// Intersect eventList1 with eventList2 -- this modifies eventList1
//  Entries not in eventList2 are removed from eventList1
//SEXP intersectEventLists(SEXP eventList1, SEXP eventList2)
//{
//	TEventList* el1 = checkForEventListWrapper(eventList1);
//	TEventList* el2 = checkForEventListWrapper(eventList2);
//	
//	el1->Intersect(el2);
//	
//	return R_NilValue;
//}
