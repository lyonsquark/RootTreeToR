/*
 *
 * TrksInCone.cpp
 *
 * Get missing ET information
 *
 */

#ifndef ROOT_TTree
#include "TChain.h"
#endif

#include "trksInCone.h"
#include "cafe/Event.hpp"
#include "rootChainManager.h"
#include "rootChainManagerWrapper.h"

SEXP trksInConeToR(SEXP manager, SEXP nEntriesR, SEXP firstEntryR,
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
    
    // Create the TSelector
    TrksInCone stor(initialSize, growthFactor, 
		    verbose, tree->GetTreeOffset() );
                             
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
    
TrksInCone::TrksInCone(unsigned int initialSize, 
		       float growthFactor, bool verbose,
		       Long64_t* treeOffsets) :
  m_tree(0),
  m_df(initialSize, growthFactor, verbose),
  m_treeOffsets(treeOffsets)
{}

void TrksInCone::Begin(TTree* tree)
{
  // Set the tree
  m_tree = tree;

  // Create the columns in the data frame
  m_sumPtR = m_df.addRealColumn("sumPt");
  m_pzR = m_df.addRealColumn("pz");
  m_globalEntryR = m_df.addIntegerColumn("globalEntry");
  m_localEntryR = m_df.addIntegerColumn("localEntry");
  m_idx = m_df.addIntegerColumn("idx");
  m_treeNumR = m_df.addIntegerColumn("treeNum");
}

Bool_t TrksInCone::Notify()
{
  m_treeNumber = m_tree->GetTreeNumber();
  m_event.setBranchAddresses(m_tree);
  return kTRUE;
}

void TrksInCone::ProcessFill(Long64_t localEntry)
{
  // Get the global entry number
  Long64_t globalEntry = localEntry + m_treeOffsets[m_treeNumber];
  
  // Get the em clusters
  cafe::Collection<TMBEMCluster> ems = m_event.getEMscone();

  // Get all of the tracks
  cafe::Collection<TMBTrack> trks = m_event.getTracks();

  const float dRCut = 0.4;      //cone size
  const float dRInner = 0.05;
  //  const float dZCut = 1.0;      //cut in cm on dZ from vtx @ DCA
  const float minTrkPt = 0.4;  //in GeV
  
  // Loop over the em clusters
  unsigned int idx = 0;
  for ( cafe::Collection<TMBEMCluster>::iterator emi = ems.begin();
        emi != ems.end() ; ++emi )
  {
    // Loop over the tracks in this event and calculate the Pt in the cone
    float ptInCone = 0.0;
    float emPhi = (*emi).CalPhi();
    float emEta = (*emi).CalEta();
    
    for ( cafe::Collection<TMBTrack>::iterator ti = trks.begin() ;
          ti != trks.end() ; ++ ti )
    {
      const TMBTrack* trk = &(*ti);

      //      float dZ = fabs(trk->trpars(1) - vtxZ);
      float dEta = trk->Eta() - emEta;
      float dPhi = fabs(trk->Phi() - emPhi);
      dPhi = fabs(dPhi);
      if( dPhi > (kinem::TWOPI/2.0) )
      {
	dPhi = (kinem::TWOPI) - dPhi;
      }
      
      float dR2 = (dEta*dEta) + (dPhi*dPhi);
    
      if( (dR2 < dRCut*dRCut)
	  && (dR2 > dRInner*dRInner)
	  //	  && (dZ < dZCut)
	  && (trk->Pt() > minTrkPt)
	  )
      {
	ptInCone += trk->Pt();
	
      }
    } // Loop over tracks
    
    m_sumPtR->set( ptInCone );
    m_pzR->set( (*emi).Pz() );
    m_idx->set( idx );
    m_globalEntryR->set( globalEntry );
    m_localEntryR->set( localEntry );
    m_treeNumR->set( m_treeNumber );
    
    m_df.commitRow();
    
    idx++;
  }
}
  
