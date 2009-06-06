#ifndef __MET__
#define __MET__

#include "TSelector.h"
#include "cafe/Event.hpp"
#include "rDataFrame.h"

extern "C" {
  SEXP trksInConeToR(SEXP manager, SEXP nEntriesR, SEXP firstEntryR,
		     SEXP initialSizeR, SEXP growthFactorR, SEXP argsR);
};

class TrksInCone : public TSelector
{
 public:
  
  TrksInCone(unsigned int initializeSize, float growthFactor, bool verbose,
      Long64_t* treeOffsets);

  virtual void Begin(TTree *tree);
  virtual Bool_t Notify();
  virtual void ProcessFill(Long64_t localEntry);
  
  SEXP rDataFrame() { return m_df.dataFrameInAnsForm(); }

 private:
  TTree* m_tree; // Tree or chain
  Long64_t* m_treeOffsets;
  RDataFrame m_df;

  // Columns in the data frame
  RDataFrameRealColumn* m_sumPtR;
  RDataFrameRealColumn* m_pzR;
  RDataFrameIntegerColumn* m_globalEntryR;
  RDataFrameIntegerColumn* m_localEntryR;
  RDataFrameIntegerColumn* m_idx;
  RDataFrameIntegerColumn* m_treeNumR;

  unsigned int m_treeNumber;

  cafe::Event m_event;
};

#endif
