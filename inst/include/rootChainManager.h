/*
 *  rootChainManager.h
 *  RootTreeToR
 *
 *  Created by Adam Lyon on 1/7/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ROOT_CHAIN_MANAGER_H
#define ROOT_CHAIN_MANAGER_H

class TChain;

#include <R.h>
#include <Rdefines.h>


class RootChainManager {
	
 public:
	
  // C'tor
  RootChainManager(SEXP treeName, SEXP fileList, bool verbose, bool trace);
	
  // D'tor
  virtual ~RootChainManager();
	
  // Return the number of entries
  unsigned int nEntries() const;
	
  // Get the names in the branch
  SEXP names() const;
	
  // Convert the chain to R
  SEXP toR(SEXP columns, SEXP selection, SEXP nEntries, SEXP firstEntry,
	   SEXP initialSize, SEXP growthFactor);
	
  // Make an EventList
  SEXP makeEventList(SEXP name, SEXP selection, SEXP nEntries, SEXP firstEntry);
	
  // Apply an EventList
  SEXP applyEventList(TEventList* el);
	
  // Clear an EventList
  SEXP clearEventList();
	
  // Get the tree
  TChain* tree() { return m_chain; }
	
  // Set the verbose flag
  void setVerbose(bool verbose) { 
    m_verbose = verbose; 
  }
	
  // Set the trace flag
  void setTrace(bool trace) { 
    m_trace = trace; 
  }

  // Check the verbose flag
  bool getVerbose() { return m_verbose; }

  // Check the trace flag
  bool getTrace() { return m_trace; }
	
  // Get the Eventlist
  SEXP getEventListName();
	
 private:
  bool m_verbose;
  bool m_trace;
  TChain* m_chain;
};
		
#endif
