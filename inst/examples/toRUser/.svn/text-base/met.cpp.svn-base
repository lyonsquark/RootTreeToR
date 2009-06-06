/*
 *
 * met.cpp
 *
 * Get missing ET information
 *
 */

// You must have the three lines below to load the
// TChain header only if necessary.
#ifndef ROOT_TTree
#include "TChain.h"
#endif

// You must have these two includes to obtain the Root chain to 
// process
#include "rootChainManager.h"
#include "rootChainManagerWrapper.h"

// The following is the header for this implementation
#include "met.h"

// The following includes are specific to D0 CAF trees (the Root tree
//  type we're processing here)
#include "cafe/Event.hpp"
#include "met_util/BMetStruct.hpp"

// You declared this function in the extern "C" block of the header.
// R will call this function to drive the processing of the Tree/Chain.
// Except for the name of your class (in this case met), your 
// function should look identical to the below... 
SEXP metToR(SEXP manager, SEXP nEntriesR, SEXP firstEntryR,
	    SEXP initialSizeR, SEXP growthFactorR, SEXP argsR)
{

  try {
    // Pull out the arguments
    Long64_t nEntries = INTEGER(nEntriesR)[0];
    Long64_t firstEntry = INTEGER(firstEntryR)[0];
    int initialSize = INTEGER(initialSizeR)[0];
    double growthFactor = REAL(growthFactorR)[0];

    // Get the Root chain manager
    checkForRootChainManagerPtr(manager);
    RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);
    
    bool verbose = rcm->getVerbose();
    
    // Get the chain we wish to deal with
    TChain* tree = rcm->tree();
    
    // Create the TSelector (replace Met with your class name)
    Met stor(initialSize, growthFactor, verbose, tree->GetTreeOffset() );
                             
    // Process it!
    tree->Process(&stor, "", nEntries, firstEntry);
      
    // Return the data frame
    return stor.rDataFrame();
  }

  catch ( RDataFrameException e ) {
    error(e.what().c_str());
    return 0;
  }
}
   
// This member function is the implementation of your constructor. It should
// look just like the below (except replace Met with your class name). 
Met::Met(unsigned int initialSize, float growthFactor, bool verbose,
	 Long64_t* treeOffsets) :
  m_tree(0),
  m_df(initialSize, growthFactor, verbose),
  m_treeOffsets(treeOffsets)
{}

// This member function stores a pointer to the curent tree an also
// sets up the output R data frame. Adapt the data frame code 
// (e.g. m_yourVarR = m_df.addRealColumn("columnName") ) to what you
// have in your header.
void Met::Begin(TTree* tree)
{
  // Set the tree
  m_tree = tree;

  // Create the columns in the data frame
  m_mexR = m_df.addRealColumn("mex");
  m_meyR = m_df.addRealColumn("mey");
  m_metR = m_df.addRealColumn("met");
  m_zVertex = m_df.addRealColumn("zVertex");

  // You must have the following three columns
  m_globalEntryR = m_df.addIntegerColumn("globalEntry");
  m_localEntryR = m_df.addIntegerColumn("localEntry");
  m_treeNumR = m_df.addIntegerColumn("treeNum");

  // You may need an idx column too (see the header)
  //m_idxR = m_df.addIntegerColumn("idx");
}

// You need this member function verbatim. It sets up Root when you activate 
// a new Tree in the Chain.
Bool_t Met::Notify()
{
  m_treeNumber = m_tree->GetTreeNumber();

  // This line below is specific to D0 CAF Trees
  m_event.setBranchAddresses(m_tree);

  return kTRUE;
}

// This function does the real work of storing the data. Adapt it to your
// requirements.
void Met::ProcessFill(Long64_t localEntry)
{
  // Get the global entry number (you must do this)
  Long64_t globalEntry = localEntry + m_treeOffsets[m_treeNumber];
  
  // This is where you do your work
  // In this case, we get missing ET data
  const TMBMet* theMet = m_event.getMet();
  const metid::BMetQualInfo* metqual = theMet->getMetQualInfo("corrJCCB");
  metid::BMetStruct metstr = metqual->getMETcorrCALO();
  m_mexR->set( metstr.getmex() );
  m_meyR->set( metstr.getmey() );
  m_metR->set( metstr.getmet() );
  m_zVertex->set( theMet->getZvertex() );

  // You must fill these columns
  m_globalEntryR->set( globalEntry );
  m_localEntryR->set( localEntry );
  m_treeNumR->set( m_treeNumber );
  // Maybe you need to fill m_idx too?

  // Commit the row to the data frame - you must do this step
  m_df.commitRow();
}

  
