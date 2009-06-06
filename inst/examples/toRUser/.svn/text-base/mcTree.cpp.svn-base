/*
 *  mcTree.cpp
 *
 *  Created by Adam Lyon on 9/21/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

/* Write information about the Monte Carlo tree
   to R */


#ifndef ROOT_TTree
#include "TChain.h"
#endif

#include "mcTree.h"

#include "cafe/Event.hpp"

#include "rootChainManager.h"
#include "rootChainManagerWrapper.h"

#include <map>


SEXP mcTreeToR(SEXP manager, SEXP nEntriesR, SEXP firstEntryR,
	       SEXP initialSizeR, SEXP growthFactorR, 
	       SEXP argsR )
{
   
  try {
    // Pull out the arguments
    Long64_t nEntries = INTEGER(nEntriesR)[0];
    Long64_t firstEntry = INTEGER(firstEntryR)[0];
    int initialSize = INTEGER(initialSizeR)[0];
    double growthFactor = REAL(growthFactorR)[0];
    
    REprintf("A\n");
    int selectPdgid = 0;
    int selectParentPdgid = 0;
    REprintf("B\n");
    if ( ! isNull( argsR ) ) {
      selectPdgid = INTEGER(argsR)[0];
      REprintf("C\n");
      
      if ( GET_LENGTH(argsR) > 1 ) selectParentPdgid = INTEGER(argsR)[1];
      REprintf("D\n");
      
    }
    
    // Get the Root chain manager
    checkForRootChainManagerPtr(manager);
    RootChainManager* rcm = (RootChainManager*) R_ExternalPtrAddr(manager);
    
    bool verbose = rcm->getVerbose();
    
    // Get the chain we wish to deal with
    TChain* tree = rcm->tree();
    
    // Create the TSelector
    McTree stor(initialSize, growthFactor, verbose, tree->GetTreeOffset(),
                             selectPdgid, selectParentPdgid);
    
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

McTree::McTree(unsigned int initialSize,
               float growthFactor, bool verbose, 
               Long64_t* treeOffsets,
               int selectPdgid, int selectParentPdgid) :
  m_tree(0),
  m_df(initialSize, growthFactor, verbose),
  m_treeOffsets(treeOffsets),
  m_selectPdgid(selectPdgid),
  m_selectParentPdgid(selectParentPdgid)
{}

void McTree::Begin(TTree* tree)
{
	// Set the tree
	m_tree = tree;
	
	// Create the columns
  m_pnumR = m_df.addIntegerColumn("pnum");  // Particle number [1..n]
  m_pdgidR = m_df.addIntegerColumn("pdgid");  // PDG Particle ID
  m_statusR = m_df.addIntegerColumn("status");  // Status from MCPart
  m_nParentsR = m_df.addIntegerColumn("nParents");  // Number of parents
  m_parentXR = m_df.addRealColumn("parentX");  // X of production vertex
  m_parentYR = m_df.addRealColumn("parentY");  // Y of production vertex
  m_parentZR = m_df.addRealColumn("parentZ");  // Z of production vertex
  m_parentCtR = m_df.addRealColumn("parentCt");  // ct of production vertex
  m_parentPnumR = m_df.addIntegerColumn("parentPnum");  // Particle number of parent 0
  m_parentPdgidR = m_df.addIntegerColumn("parentPdgid");  // PDG Particle ID of parent 0
  m_nDaughtersR = m_df.addIntegerColumn("nDaughters");  // Number of daughters
  m_pxR = m_df.addRealColumn("px");  // px
  m_pyR = m_df.addRealColumn("py");  // py
  m_pzR = m_df.addRealColumn("pz");  // pz
  m_ptR = m_df.addRealColumn("pt");  // pt
  m_eR = m_df.addRealColumn("e");  // E
  m_mR = m_df.addRealColumn("m");  // Mass
  m_etaR = m_df.addRealColumn("eta");  // Eta
  m_phiR = m_df.addRealColumn("phi");  // Phi
  m_globalEntryR = m_df.addIntegerColumn("globalEntry");  
  m_localEntryR = m_df.addIntegerColumn("localEntry");  
  m_treeNumR = m_df.addIntegerColumn("treeNum");  
}	

Bool_t McTree::Notify()
{
	m_treeNumber = m_tree->GetTreeNumber();  
	m_event.setBranchAddresses(m_tree);
	return kTRUE;
}

void McTree::ProcessFill(Long64_t localEntry)
{
  // Get the global entry number
  Long64_t globalEntry = localEntry + m_treeOffsets[m_treeNumber];
  
  // Get mc particle data
  cafe::Collection<TMBMCpart> particles = m_event.getMCParticles();
  
  // Get the vertex data to be sure we've loaded the branch
  cafe::Collection<TMBMCvtx> vertices = m_event.getMCVertices();
  
  // First, we need to create a map to produce particle numbers (start at 1)
  int nextNum = 0;
  std::map<TMBMCpart*, int> particlePtrToNum;
  for ( cafe::Collection<TMBMCpart>::iterator pIter = particles.begin() ;
	pIter != particles.end() ;
	++pIter )
    {			 
      nextNum++;
      
      // Set the map
      particlePtrToNum[ &(*pIter) ] = nextNum;
    }
  
  // Loop over the particles again and fill information
  nextNum = 0;
  for ( cafe::Collection<TMBMCpart>::iterator pIter = particles.begin() ;
	pIter != particles.end() ;
	++pIter )
    {	
      
      // Do we want this pdgid?
      if ( m_selectPdgid != 0 && (*pIter).abspdgid() 
	   != m_selectPdgid ) continue;
      
      // Get parent information
      int nParents = 0;
      float parentX = 0.0;
      float parentY = 0.0;
      float parentZ = 0.0;
      float parentCt = 0.0;
      int parentPnum = 0;
      int parentPdgid = 0;
      int parentAbsPdgid = 0;
      
      const TMBMCvtx* parentVtx = (*pIter).getPMCvtx();
      if ( parentVtx ) {
	nParents = (*parentVtx).nparents();
	parentX = (*parentVtx).x();
	parentY = (*parentVtx).y();
	parentZ = (*parentVtx).z();
	parentCt = (*parentVtx).ct();
	
	if ( nParents > 0 ) {
	  const TMBMCpart* parentParticle = (*parentVtx).getParent(0);
	  if ( parentParticle ) {
	    parentPnum = particlePtrToNum[ 
				   const_cast<TMBMCpart*>(parentParticle) ];
	    parentPdgid = (*parentParticle).pdgid();
	    parentAbsPdgid = (*parentParticle).abspdgid();
	  }
      }
    }
    
    if ( m_selectParentPdgid != 0 && m_selectParentPdgid 
	 != parentAbsPdgid ) continue;
    
    nextNum++;
    
    m_pnumR->set( nextNum );
    m_pdgidR->set( (*pIter).pdgid() );
    m_statusR->set( (*pIter).status() );
        
    m_nParentsR->set( nParents );
    m_parentXR->set( parentX );
    m_parentYR->set( parentY);
    m_parentZR->set( parentZ );
    m_parentCtR->set( parentCt );
    m_parentPnumR->set( parentPnum );
    m_parentPdgidR->set( parentPdgid );
    
    int nDaughters = 0;
    const TMBMCvtx* decayVtx = (*pIter).getDMCvtx();
    if ( decayVtx ) nDaughters = (*decayVtx).ndaughters();
    
    m_nDaughtersR->set( nDaughters );
    m_pxR->set( (*pIter).Px() );
    m_pyR->set( (*pIter).Py() );
    m_pzR->set( (*pIter).Pz() );
    m_ptR->set( (*pIter).Pt() );
    m_eR->set( (*pIter).E() );
    m_mR->set( (*pIter).M() );
    m_etaR->set( (*pIter).Eta() );
    m_phiR->set( (*pIter).Phi() );    
    m_globalEntryR->set( globalEntry );
    m_localEntryR->set( localEntry );
    m_treeNumR->set( m_treeNumber );

    // Commit the row to the data frame
    m_df.commitRow();
    
  } // Loop over MC particles
} 
