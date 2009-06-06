/*
 *  rootChainManager.cpp
 *  RootTreeToR
 *
 *  Created by Adam Lyon on 1/7/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

// Root includes
#include "Riostream.h" // This is needed or else TLeaf.h won't compile!
#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include <TBranch.h>
#include <TEventList.h>

#include "rootChainManager.h"
#include "treeToR.h"

////////////////////////////////////////////////////////////
// C'tor
RootChainManager::RootChainManager(SEXP treeName, SEXP fileList, bool verbose, bool trace) :
  m_chain(0),
  m_verbose(verbose),
  m_trace(trace)
{
  // Check arguments
  if ( ! IS_CHARACTER(treeName) ) error("treeName must be a string");
  if ( GET_LENGTH(treeName) != 1) error("treeName must have length 1");
  if ( ! IS_CHARACTER(fileList) ) error("fileList must be a list of strings");
	 
  // Get the tree name
  char* treeNameC = CHAR(STRING_ELT(treeName, 0));
	 
  if (m_verbose) REprintf("Will read tree %s\n", treeNameC);
	 
  // Get the list of files to chain
  if (m_verbose) 
    REprintf("There are %d files to add to the chain\n", GET_LENGTH(fileList) );
	 
  // Form the chain from the file lists
  m_chain = new TChain(treeNameC);
	 
  // Add files
  for ( unsigned int i = 0; i < GET_LENGTH(fileList); ++i ) {    
    char* fileNameC = CHAR(STRING_ELT(fileList, i) );
    if (m_verbose) REprintf("Adding file %s to chain\n", fileNameC);
    m_chain->Add( fileNameC, 0 );
  }
}

////////////////////////////////////////////////////////////
// D'tor
RootChainManager::~RootChainManager()
{
  delete m_chain;
}


////////////////////////////////////////////////////////////
// nEntries -- Get the number of entries
unsigned int RootChainManager::nEntries() const {
  return static_cast<unsigned int>( m_chain->GetEntries() );
}

////////////////////////////////////////////////////////////
// names -- get the names of the branches and their details
SEXP RootChainManager::names() const {
	
  // Advance to the first entry
  m_chain->GetEntry(0);
	
  // Get the branches
  TObjArray* branches = m_chain->GetListOfBranches();
	
  // How many?
  unsigned int nBranches = branches->GetEntriesFast();
	
  // Make the R list for returning the list of branch and detail names
  SEXP rBranchList, rBranchListNames;
  PROTECT(rBranchList = NEW_LIST(nBranches));
  PROTECT(rBranchListNames = NEW_CHARACTER(nBranches));
	
  // Loop over the branches in the tree
  for ( unsigned int branchIter = 0; branchIter < nBranches; ++branchIter ) {
		
    // Get the branch
    TBranch* branch = (TBranch*) branches->UncheckedAt(branchIter);
		
    // Save away the name of this branch for use as an attribute
    SET_STRING_ELT(rBranchListNames, branchIter, mkChar(branch->GetName()));
		
    SEXP details = NEW_CHARACTER(1);
    SET_ELEMENT(rBranchList, branchIter, details);
    
    SET_STRING_ELT(details, 0, mkChar( branch->GetTitle() ) );

  } // for over branches
	
  // Set the names attribute
  SET_NAMES(rBranchList, rBranchListNames);
	
  // Release rBranchList and rBranchListNames
  UNPROTECT(2);
	
  return rBranchList;
}

////////////////////////////////////////////////////////////
// toR -- convert from root to R via a TSelector
SEXP RootChainManager::toR(SEXP columns, SEXP selection, 
			   SEXP nEntries, SEXP firstEntry,
			   SEXP initialSize, SEXP growthFactor)
{
  // Run over tree with TreeToR
  TreeToR treeToR(columns, CHAR(STRING_ELT(selection, 0)), 
		  INTEGER(initialSize)[0], 
		  REAL(growthFactor)[0], 
		  m_verbose, m_trace);
	
  m_chain->Process( &treeToR, "", INTEGER(nEntries)[0], 
		    INTEGER(firstEntry)[0]);
	
  return treeToR.rDataFrame();
}

//////////////////////////////
// Make an event list
SEXP RootChainManager::makeEventList(SEXP name, SEXP selection, SEXP nEntries, SEXP firstEntry)
{
  if (m_verbose) REprintf("Running TDraw with |%s| |%s| %d %d\n", 
			  CHAR(STRING_ELT(name, 0)),
			  CHAR(STRING_ELT(selection, 0)),
			  INTEGER(nEntries)[0], INTEGER(firstEntry)[0] );
			  

  m_chain->Draw( CHAR(STRING_ELT(name, 0)), 
		 CHAR(STRING_ELT(selection, 0)), "",
		 INTEGER(nEntries)[0], INTEGER(firstEntry)[0] );
	
  return R_NilValue;
}

/////////////////////////////
// Apply event list
SEXP RootChainManager::applyEventList(TEventList* el)
{
  m_chain->SetEventList(el);
	
  if (m_verbose) REprintf("Narrow chain to event list %s\n", el->GetName() );
	
  return R_NilValue;
}

////////////////
// Get the event list
SEXP RootChainManager::getEventListName()
{
  TEventList* ev = m_chain->GetEventList();
	
  if (! ev ) return R_NilValue;
	
  SEXP r = NEW_CHARACTER(1);
  SET_STRING_ELT(r, 0, mkChar(ev->GetName()) );
	
  return r;
}

//////////////////////
// Clear an event list
SEXP RootChainManager::clearEventList()
{
  m_chain->SetEventList(0);
	
  if (m_verbose) REprintf("Removing event list restriction on chain\n");
	
  return R_NilValue;
}
