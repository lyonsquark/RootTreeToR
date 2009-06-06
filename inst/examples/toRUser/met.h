#ifndef __MET__
#define __MET__

// This sample, heavily documented, code is callable from toRUser.
// It is written to read Missing ET data from D0 CAF Root Trees. 
// Retrieving this data requires some complicated manipulation. Note
// that D0 CAF Trees need an external library with classes that wrap
// the Root data.

// This file and the corresponding implementation met.cpp file serve
// as a useful template for writing the code necessary for toRUser.

// You must include the following two includes
#include "TSelector.h"  
#include "rDataFrame.h"

// This include is specific to the Root tree we are 
// using (D0 CAF trees, in this case).
#include "cafe/Event.hpp"

// You need a C function that is called from R. You can choose the
// name of this function, but the arguments must be the same as shown
// here.
//   Note that SEXP's are "S expressions" (S is the foerunner of R).
// Don't forget to wrap in extern "C"
extern "C" {
  SEXP metToR(SEXP manager, SEXP nEntriesR, SEXP firstEntryR,
	      SEXP initialSizeR, SEXP growthFactorR, SEXP argsR);
};

// Your class must inherit from TSelector. 
class Met : public TSelector
{
 public:
  
  // The constructor must have the argumetns shown here
  Met(unsigned int initializeSize, float growthFactor, bool verbose,
      Long64_t* treeOffsets);

  // Yu must have these four methods with the arguments shown here.
  virtual void Begin(TTree *tree);
  virtual Bool_t Notify();
  virtual void ProcessFill(Long64_t localEntry);
  SEXP rDataFrame() { return m_df.dataFrameInAnsForm(); }

 private:

  // You must have these four private member data
  TTree* m_tree; // Tree or chain
  Long64_t* m_treeOffsets;
  RDataFrame m_df;
  unsigned int m_treeNumber;

  // The following are the columns in the output data frame.
  // There are four column types:
  //    RDataFrameIntegerColumn
  //    RDataFrameRealColumn
  //    RDataFrameLogicalColumn
  //    RDataFrameStringColumn
  // 
  // Note the style here- the pointer name ends with R for these 
  // data frame columns.
  RDataFrameRealColumn* m_mexR;
  RDataFrameRealColumn* m_meyR;
  RDataFrameRealColumn* m_metR;
  RDataFrameRealColumn* m_zVertex;

  // You must have the following three columns
  RDataFrameIntegerColumn* m_globalEntryR;
  RDataFrameIntegerColumn* m_localEntryR;
  RDataFrameIntegerColumn* m_treeNumR;
  // You may wan to add an idx column if there is 
  // more than one object per event
  // RDataFrameIntegerColumn* m_idxR;

  // This member datum is specifc to D0 CAF Trees.
  cafe::Event m_event;
};

#endif
