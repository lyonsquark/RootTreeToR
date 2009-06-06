#include "treeToR.h"

#include <algorithm>

#undef PrintValue // Or else R's will interfere with root's PrintValue

#define VECTORIFARRAYFACTOR 4

/////////////////////////
// Some special functors
// Special template functors for setting variables in the data frame
template<class T, class U> struct set : public std::unary_function<T, void>
{
    set(TTreeFormula** variable, unsigned arrayIdx) : 
	m_variable(variable),
	m_arrayIdx(arrayIdx)
	{}
	
    void operator()(T* column) { 
	unsigned i = column->order();
	if ( m_variable[i]->GetNdim() ) { // have something to store
	    column->set( static_cast<U>(m_variable[i]->EvalInstance(m_arrayIdx)) );
	}
	else {
	    column->setNA();
	}
    }
    
    unsigned int m_arrayIdx;
    TTreeFormula** m_variable;
};

// Specialize the string version because it is different
template<> struct set<RDataFrameStringColumn, std::string> : 
    public std::unary_function<RDataFrameStringColumn*, void>
{
    set(TTreeFormula** variable, unsigned arrayIdx) : 
	m_variable(variable),
	m_arrayIdx(arrayIdx)
	{}
    
    void operator()(RDataFrameStringColumn* column) { 
	unsigned i = column->order();
	if ( m_variable[i]->GetNdim() ) { // have something to store
	    column->set( std::string(m_variable[i]->PrintValue(0, m_arrayIdx) ) );
	}
	else {
	    column->setNA();
	}
    }
    
    unsigned int m_arrayIdx;
    TTreeFormula** m_variable;
};		

/////////////////
// C'tor
TreeToR::TreeToR(SEXP desiredVariables, const char *selection, 
		 unsigned int initialSize, float growthFactor, 
                 bool verbose, bool trace):
    m_desiredVariables(desiredVariables),
    m_selection(selection),
    m_df(initialSize, growthFactor, verbose),
    m_verbose(verbose),
    m_trace(trace),
    m_tree(0),
    m_formulaList(),
    m_variable(0),
    m_select(0),
    m_manager(0),
    m_isArray(false),
    m_integerColumns(),
    m_realColumns(),
    m_stringColumns(),
    m_globalEntryColumn(),
    m_localEntryColumn(),
    m_treeColumn(),
    m_idxColumn(),
    m_treeNumber(0),
    m_nColumns(0),
    m_globalEntry(0),
    m_pastBegin(false)
{}

/////////////////
// Begin
void TreeToR::Begin(TTree* tree)
{
  if ( m_trace) REprintf("TreeToR:Trace - In Begin\n");

  // Set the tree
  m_tree = tree;

  // Reset the global entry #
  m_globalEntry = 0;
  
  // Compile selection expression if there is one
  if (strlen(m_selection.c_str())) {
    
    // Make the tree formula
    m_select = new TTreeFormula("Selection", m_selection.c_str(), m_tree);
    if (!m_select) error("TreeToR::toR- Selecton Failed (select is null)");
    if (!m_select->GetNdim()) { 
      delete m_select;
      error("TreeToR::toR- Selection Failed (GetNdim is zero)"); 
    }
    
    // Add it to our formula list
    m_formulaList.Add(m_select);
  }
  
  // Make the formulas
  m_nColumns = length(m_desiredVariables);
  m_variable = new TTreeFormula* [m_nColumns];
  
  // -- Create TreeForumlas for each column
  for (unsigned int i = 0; i<m_nColumns; i++) {
    m_variable[i] = new TTreeFormula("Var1",
				     CHAR(STRING_ELT(m_desiredVariables,i)), 
				     m_tree);
    
    // Add to our formula list
    m_formulaList.Add(m_variable[i]);
  }
  
  // If there's selection criteria, then we need a formula manager to keep 
  //   everything in sync
  if (m_select) {
    
    // Is there an array involved?
    if (m_select->GetManager()->GetMultiplicity() > 0 ) {
      m_manager = new TTreeFormulaManager;
      
      // Add our variable forumlas to the manager
      for(unsigned int i=0; i<=m_formulaList.LastIndex(); i++) {
	m_manager->Add((TTreeFormula*)m_formulaList.At(i));
      }
      
      m_manager->Sync();
    }  // if ( seletion has an array )
    
  } // if ( there is a selection )
  
    // Figure out if there is an array involved
  m_isArray = false;
  for (unsigned int i=0; i<=m_formulaList.LastIndex(); i++) {
    TTreeFormula *form = ((TTreeFormula*)m_formulaList.At(i));
    if ( form->GetManager()->GetMultiplicity() != 0 ) {
      m_isArray = true;
      break;
    }
    
  } // ( for over formulas )
  
    // Create the R data frame columns
  for ( unsigned int j=0; j < m_nColumns; ++j ) {
    
    char* colName = m_variable[j]->PrintValue(-1); // -1 returns the name
    
    if      ( m_variable[j]->IsInteger() ) // An integer
      m_integerColumns.push_back( m_df.addIntegerColumn(colName) );
    
    else if ( m_variable[j]->IsString()  )  // A string
      m_stringColumns.push_back( m_df.addStringColumn(colName) );
    
    else  // Must be real
      m_realColumns.push_back( m_df.addRealColumn(colName) );
  } // For over desired columns
  
    //  Make an extra column for the entry number
  m_globalEntryColumn = m_df.addIntegerColumn("globalEntry");
  m_localEntryColumn  = m_df.addIntegerColumn("localEntry");
  
  // Make an extra column for the tree number
  m_treeColumn = m_df.addIntegerColumn("treeNum");
  
  // If necessary, add an index column for arrays
  m_idxColumn = 0;	 
  if (m_isArray) { // Add the idx column if is in an array
    m_idxColumn = m_df.addIntegerColumn("idx");
  }	 
  
  m_pastBegin = true;

  if ( m_trace) REprintf("TreeToR:Trace - Begin - Done\n");

} // Begin(...)

////////////////////////////////////////
Bool_t TreeToR::Notify()
{
  if ( m_trace) REprintf("TreeToR:Trace - In Notify\n");

    // Check if we are ready to call Notify
    if ( ! m_pastBegin ) {
      if ( m_trace ) REprintf("TreeToR - Notify called early to load tree - Done\n");
      return kTRUE;
    }
    
    // New tree!
    m_treeNumber = m_tree->GetTreeNumber();
    
    if ( m_verbose )
	REprintf("TreeToR - Now in tree %d\n", m_treeNumber);
    
    // Update the leaves for this tree
    if ( m_trace) REprintf("TreeToR:Trace - Notify - update formula leaves\n");
    if (m_manager) m_manager->UpdateFormulaLeaves(); // the select's manager
    else {
	// Don't have a select manager --  update the individual formulas
	for(unsigned int i=0; i<=m_formulaList.LastIndex(); i++) {
	    ((TTreeFormula*)m_formulaList.At(i))->UpdateFormulaLeaves();
	}
    }
    
    if ( m_trace) REprintf("TreeToR:Trace - Notify - Done\n");
    return kTRUE;
}

/////////////////////////////////////
void TreeToR::ProcessFill(Long64_t localEntry)
{
  if ( m_trace ) REprintf("TreeToR:Trace - In Process Fill local # %d\n", localEntry);
  if ( m_trace ) REprintf("TreeToR:Trace - In Process Fill global # %d\n", m_globalEntry);
      
    
  // Determine the number of elements in the variable arrays
  int arraySize = 1;
  if (m_isArray) {				
    
    // We have a variable array!
    if (m_manager) {
      arraySize = m_manager->GetNdata(kTRUE); // force refresh
    } 
    else {
      for (unsigned int i=0; i<m_nColumns; i++) {
	if (arraySize < m_variable[i]->GetNdata() ) {
	  arraySize = m_variable[i]->GetNdata();
	}
      }
      if (m_select && m_select->GetNdata()==0) arraySize = 0;
    }
  } // if (isArray) a variable array
  
  if ( m_trace) REprintf("TreeToR:Trace - Process Fill - A\n");
  
  bool loaded = false; // Have we loaded leaves?
  
  // Loop over array indices
  for( unsigned int arrayIndex=0; arrayIndex < arraySize; arrayIndex++ ) {
    
    // Check the selection
    if (m_select) {
      if (m_select->EvalInstance(arrayIndex) == 0) {
	continue; // This row fails the selection, don't include it
      }
    }
    
    if ( m_trace) REprintf("TreeToR:Trace - Process Fill - B %d\n", arrayIndex);
    
    // Read in the results of this forumla instance
    if (arrayIndex==0) loaded = true;
    
    else if (!loaded) { 
      // Ensure that proper branches are loaded if the first row fails
      // the selection					  
      for (unsigned int i=0; i<m_nColumns; i++) {
	m_variable[i]->EvalInstance(0);
      }
      loaded = true;
    }
    
    if ( m_trace) REprintf("TreeToR:Trace - Process Fill - C %d\n", arrayIndex);
    
    // Store the data
    // First integers
    std::for_each(m_integerColumns.begin(),
		  m_integerColumns.end(),
		  set<RDataFrameIntegerColumn, int>(m_variable, arrayIndex)
		  );
    
    if ( m_trace) REprintf("TreeToR:Trace - Process Fill - D %d\n", arrayIndex);
    
    // Then reals
    std::for_each(m_realColumns.begin(),
		  m_realColumns.end(),
		  set<RDataFrameRealColumn, double>(m_variable, arrayIndex)
		  );
    
    if ( m_trace) REprintf("TreeToR:Trace - Process Fill - E %d\n", arrayIndex);
    
    // And finally strings
    std::for_each(m_stringColumns.begin(),
		  m_stringColumns.end(),
		  set<RDataFrameStringColumn, std::string>(
							   m_variable, arrayIndex)
		  );
    
    // Write the entry and tree number number
    m_globalEntryColumn->set( m_globalEntry );
    m_localEntryColumn->set( localEntry );
    m_treeColumn->set( m_treeNumber );
    
    if ( m_trace) REprintf("TreeToR:Trace - Process Fill - F %d\n", arrayIndex);
    
    if ( m_isArray ) {
      m_idxColumn->set( arrayIndex );
    }
    
    // Commit
    m_df.commitRow();
    
    if ( m_trace) REprintf("TreeToR:Trace - Process Fill - G %d\n", arrayIndex);
    
  } // for (over array indices)
  
  // Increment the global entry
  ++m_globalEntry;

  if ( m_trace) REprintf("TreeToR:Trace - Process Fill # %d - DONE\n", localEntry);

  
} // ProcessFill(...)

/////////////////////////////
TreeToR::~TreeToR() {
    
    // Clean up
    delete [] m_variable;
}
