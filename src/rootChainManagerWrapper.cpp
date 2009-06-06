// Wrapper for rootChainManager -- called by R

#include "TEventList.h"
#include "rootChainManager.h"

#include "rootChainManagerWrapper.h"
#include "eventListWrapper.h"

// Some definition of True is interfering
#undef TRUE

static SEXP rcm_type_tag;

//////////////////////////
// Check that the external pointer is the right thing
void checkForRootChainManagerPtr(SEXP ptr)
{
  if ( TYPEOF(ptr) != EXTPTRSXP || 
       R_ExternalPtrTag(ptr) != rcm_type_tag ||
       ! R_ExternalPtrAddr(ptr) )
    error("Bad Pointer to RootChainManager");
}

/////////////////////////////////////////
// initRootChainManagerWrapper -- initialize
SEXP initRootChainManagerWrapper()
{
  rcm_type_tag = install("RootChainManager_TAG");
  return R_NilValue;
}

/////////////////////////////////////////
// newRootChainManager -- create a new root chain manager
SEXP newRootChainManager(SEXP treeName, SEXP fileList, SEXP doVerbose, SEXP doTrace)
{
  // Check verbose
  if ( ! IS_LOGICAL(doVerbose) ) error("verbose must be a logical");
  bool verbose = *(LOGICAL(doVerbose)) == 1;
	
  // Check trace
  if ( ! IS_LOGICAL(doTrace) ) error("trace must be a logical");
  bool trace = *(LOGICAL(doTrace)) == 1;

  RootChainManager* rcm = new RootChainManager(treeName, fileList, verbose, trace);
	
  if (!rcm) {
    return R_NilValue;
  }
	
  SEXP val = R_MakeExternalPtr(rcm, rcm_type_tag, R_NilValue);
  R_RegisterCFinalizerEx(val, 
			 (R_CFinalizer_t)deleteRootChainManager, TRUE);
  return val;
}


/////////////////////////////////////////
// deleteRootChainManager -- Delete the root chain manager
SEXP deleteRootChainManager(SEXP manager)
{
  checkForRootChainManagerPtr(manager);
  RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);
	
  REprintf("Deleting RootChainManager at ptr %p\n", rcm);

  delete rcm;
	
  R_ClearExternalPtr(manager);

  return R_NilValue;
}

/////////////////////////////
// Set the verbose flag
SEXP setVerbose(SEXP manager, SEXP verbose)
{
  checkForRootChainManagerPtr(manager);
  RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);
	
  // Convert the verbose to a bool
  bool daVerbosity = LOGICAL(verbose)[0] == 1;
  rcm->setVerbose(daVerbosity);
	
  return R_NilValue;
}

/////////////////////////////
// Get the verbose flag
SEXP getVerbose(SEXP manager)
{
  checkForRootChainManagerPtr(manager);
  RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);
	
  SEXP l = NEW_LOGICAL(1);
  LOGICAL(l)[0] = rcm->getVerbose();

  return l;
}

/////////////////////////////
// Set the trace flag
SEXP setTrace(SEXP manager, SEXP trace)
{
  checkForRootChainManagerPtr(manager);
  RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);
	
  // Convert the trace to a bool
  bool daVerbosity = LOGICAL(trace)[0] == 1;
  rcm->setTrace(daVerbosity);
	
  return R_NilValue;
}

/////////////////////////////
// Get the trace flag
SEXP getTrace(SEXP manager)
{
  checkForRootChainManagerPtr(manager);
  RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);
	
  SEXP l = NEW_LOGICAL(1);
  LOGICAL(l)[0] = rcm->getTrace();

  return l;
}
////////////////////////
// Get the EventList
SEXP getEventListName(SEXP manager)
{
  checkForRootChainManagerPtr(manager);
  RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);
	
  return rcm->getEventListName();
}

/////////////////////////////////////////
// nEntries -- Get the number of entries
SEXP nEntries(SEXP manager)
{
  checkForRootChainManagerPtr(manager);
  RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);

  unsigned int e = rcm->nEntries();
	
  SEXP ret = NEW_INTEGER(1);
  INTEGER(ret)[0] = e;
	
  return ret;
}

/////////////////////////////////////////
// names -- Get the number of entries
SEXP names(SEXP manager)
{
  checkForRootChainManagerPtr(manager);
  RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);

  return rcm->names();
}

/////////////////////////////////////////
// toR -- convert Root to R
SEXP toR(SEXP manager, SEXP columns, SEXP selection, SEXP nEntries, 
	 SEXP firstEntry, SEXP initialSize, SEXP growthFactor)
{
  checkForRootChainManagerPtr(manager);
  RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);

  return rcm->toR(columns, selection, nEntries, firstEntry, 
		  initialSize, growthFactor);
}

///////////////////////////////////////////
// makeEventList -- Create an event list
SEXP makeEventList(SEXP manager, SEXP name, SEXP selection, SEXP nEntries, SEXP firstEntry)
{
  checkForRootChainManagerPtr(manager);
  RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);

  return rcm->makeEventList(name, selection, nEntries, firstEntry);
}

//////////////////////////////
// applyEventList -- Apply an event list to this chain
SEXP applyEventList(SEXP manager, SEXP eventList)
{
  checkForRootChainManagerPtr(manager);
  RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);
	
  TEventList* el = checkForEventListWrapper(eventList);
	
  return rcm->applyEventList(el);
}

//////////////////////
// Clear the applied event list
SEXP clearEventList(SEXP manager)
{
  checkForRootChainManagerPtr(manager);
  RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);

  return rcm->clearEventList();
}
	
