#ifndef __TreeToR__
#define __TreeToR__

/////////////////////////
// TreeToR
//

#include "TSelector.h"
#include "TTreeFormula.h"
#include "TTreeFormulaManager.h"
#include "TChain.h"

#include "rDataFrame.h"

#include <vector>
#include <string>

class TreeToR : public TSelector {
		
 public:
  TreeToR(SEXP desiredVariables, const char *selection, 
	  unsigned int initialSize, float growthFactor, 
          bool verbose, bool trace);
    
  virtual void Begin(TTree* tree);
  virtual Bool_t Notify();
  virtual void ProcessFill(Long64_t localEntry);
	
  SEXP rDataFrame() { return m_df.dataFrameInAnsForm(); }
	
  virtual ~TreeToR();

 private:
		
  SEXP m_desiredVariables;
  std::string m_selection;
	
  RDataFrame m_df; // The data frame
  TTree* m_tree;
	
  std::vector<RDataFrameIntegerColumn*> m_integerColumns;
  std::vector<RDataFrameRealColumn*> m_realColumns;
  std::vector<RDataFrameStringColumn*> m_stringColumns;
		
  bool m_verbose;
  bool m_trace;
		
  unsigned int m_nColumns;
		
  TList m_formulaList;
  TTreeFormulaManager* m_manager;
  TTreeFormula** m_variable; // The TTree desired variables
  TTreeFormula* m_select;
  bool m_isArray;
		
  RDataFrameIntegerColumn* m_globalEntryColumn;
  RDataFrameIntegerColumn* m_localEntryColumn;
  RDataFrameIntegerColumn* m_treeColumn;
  RDataFrameIntegerColumn* m_idxColumn;	 
		
  unsigned int m_treeNumber;
  Long64_t m_globalEntry;
		
  bool m_pastBegin; // Needed because Notify will be called before we are
  // done with the begin routine.
			 
};

#endif
