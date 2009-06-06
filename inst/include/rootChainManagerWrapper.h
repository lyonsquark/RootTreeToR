// Header for rootChainManagerWrapper

#ifndef __ROOTCHAINMANAGERWRAPPER_H__
#define __ROOTCHAINMANAGERWRAPPER_H__

extern "C"
{
	
  SEXP initRootChainManagerWrapper();
	
  SEXP newRootChainManager(SEXP treeName, SEXP fileList, SEXP doVerbose, SEXP doTrace);
	
  SEXP deleteRootChainManager(SEXP manager);
	
  SEXP nEntries(SEXP manager);
	
  SEXP names(SEXP manager);
	
  SEXP toR(SEXP manager, SEXP columns, SEXP selection, SEXP nEntries, SEXP firstEntry,
	   SEXP initialSize, SEXP growthFactor);
	
  SEXP makeEventList(SEXP manager, SEXP name, SEXP selection, SEXP nEntries, SEXP firstEntry);
  SEXP applyEventList(SEXP manager, SEXP eventList);
  SEXP getEventListName(SEXP manager);
  SEXP clearEventList(SEXP manager);
	
  SEXP setVerbose(SEXP manager, SEXP verbose);
  SEXP getVerbose(SEXP manager);

  SEXP setTrace(SEXP manager, SEXP trace);
  SEXP getTrace(SEXP manager);
}

void checkForRootChainManagerPtr(SEXP ptr);

#endif
