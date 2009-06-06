#ifndef __MCTREE__
#define __MCTREE__

#include "TSelector.h"

#include "cafe/Event.hpp"

#include "rDataFrame.h"


extern "C" {
  SEXP mcTreeToR(SEXP manager, SEXP nEntriesR, SEXP firstEntryR,
		 SEXP initialSizeR, SEXP growthFactorR, SEXP argsR );
  };

class McTree : public TSelector
{
 public:
	McTree(unsigned int initialSize, float growthFactor, bool verbose, 
         Long64_t* treeOffsets,
         int selectPdgid, int selectParentPdgid);
	
	virtual void Begin(TTree *tree);
	virtual Bool_t Notify();
	virtual void ProcessFill(Long64_t localEntry);
	
	SEXP rDataFrame() { return m_df.dataFrameInAnsForm(); }
									
private:
	TTree* m_tree;  // The tree (or chain)
  Long64_t* m_treeOffsets;
	
	RDataFrame m_df; // The data frame
  
  int m_selectPdgid;
  int m_selectParentPdgid;
	
  // The columns we are going to fill
	RDataFrameIntegerColumn* m_pnumR;  // Particle number [1..n]
	RDataFrameIntegerColumn* m_pdgidR; // PDG Particle ID   
	RDataFrameIntegerColumn* m_statusR; // Status from MCPart
	RDataFrameIntegerColumn* m_nParentsR; // Number of parents      
	RDataFrameRealColumn* m_parentXR; // X of production vertex      
	RDataFrameRealColumn* m_parentYR; // Y of production vertex
	RDataFrameRealColumn* m_parentZR; // Z of production vertex        
  RDataFrameRealColumn* m_parentCtR; // ct of production vertex        
	RDataFrameIntegerColumn* m_parentPnumR; // Particle number of parent 0
	RDataFrameIntegerColumn* m_parentPdgidR; // PDG Particle ID of parent 0
	RDataFrameIntegerColumn* m_nDaughtersR; // Number of daughters
	RDataFrameRealColumn* m_pxR; // px
	RDataFrameRealColumn* m_pyR; // py
	RDataFrameRealColumn* m_pzR; // pz        
  RDataFrameRealColumn* m_ptR; // pt        
  RDataFrameRealColumn* m_eR; // E        
  RDataFrameRealColumn* m_mR; // Mass 
  RDataFrameRealColumn* m_etaR; // Eta
  RDataFrameRealColumn* m_phiR; // Phi 
  RDataFrameIntegerColumn* m_globalEntryR;    
  RDataFrameIntegerColumn* m_localEntryR;    
	RDataFrameIntegerColumn* m_treeNumR;  
	
	unsigned int m_treeNumber;
	
	cafe::Event m_event;

};
	
#endif
