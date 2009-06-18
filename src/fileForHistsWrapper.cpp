// Wrapper for TFile for histograms -- called by R

// Right now only does TH1F histograms

#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TKey.h"
#include <string.h>
#include <string>

#include <R.h>
#include <Rdefines.h>

#include "fileForHistsWrapper.h"

#define BUFSIZE 255

// Some definition of True is interfering
#undef TRUE

#include <vector>

static SEXP fh_type_tag;

//////////////////////////
// Check that the external pointer is the right thing
TFile* checkForFileForHistsWrapper(SEXP ptr)
{

  if ( TYPEOF(ptr) != EXTPTRSXP || 
       R_ExternalPtrTag(ptr) != fh_type_tag ||
       ! R_ExternalPtrAddr(ptr) )
    error("Bad Pointer to FileForHistsWrapper");
  
  return (TFile*) R_ExternalPtrAddr(ptr);
}

/////////////////////////////////////////
// initFileForHistsWrapper -- initialize
SEXP initFileForHistsWrapper()
{
  fh_type_tag = install("FileForHists_TAG");
  return R_NilValue;
}


//////////////////////////////
// Wrap up a TEventList pointer and perhaps register the finalizer
SEXP wrapTFileForHistsPointer(TFile* f)
{
  SEXP val = R_MakeExternalPtr(f, fh_type_tag, R_NilValue);
  
  R_RegisterCFinalizerEx(val, 
			 (R_CFinalizer_t)deleteFileForHists, TRUE);
  
  return val;
}

///////////////////////////////////
// Delete the event list
SEXP deleteFileForHists(SEXP fileForHists)
{
  TFile* f = checkForFileForHistsWrapper(fileForHists);
  
  REprintf("Deleting TFile at ptr %p\n", f);
  
  delete f;
  
  R_ClearExternalPtr(fileForHists);
  
  return R_NilValue;
}

////////////////////////////////////////
// Open a Root file with histograms
SEXP openFileForHists(SEXP rootFileNameR)
{
  // Get the file name
	std::string fileName = CHAR(STRING_ELT(rootFileNameR, 0));
  
  TFile* f = new TFile(fileName.c_str());

  if ( ! f ) error("Cannot open file");
  if ( ! f->IsOpen() ) {
    delete f;
    error("Cannot open file");
  }

  return wrapTFileForHistsPointer(f);
}

const char* setFileDirectory(TFile* f, SEXP directoryR)
{

  // Hold the old directory
  const char* oldDirectory = gDirectory->GetPath();

  // Change directory to the file
  f->cd();

  // Change to a sub directory directory if necessary
  if ( ! isNull(directoryR) ) {
    
		std::string dir = CHAR( STRING_ELT(directoryR, 0 ) );
    if ( ! f->cd(dir.c_str()) ) {
      error("namesMatchingClass: cd failed");
    }
  }

  return oldDirectory;
}


///////////////////////////////
// Return matching key names
SEXP namesMatchingClass(SEXP fileForHists, SEXP directoryR, SEXP classTypeR)
{
  TFile* f = checkForFileForHistsWrapper(fileForHists);

  const char* oldDirectory = setFileDirectory(f, directoryR);

  // Get the class type
	std::string classType = CHAR( STRING_ELT(classTypeR, 0) );
  
  // Keep track of the ones we want
  std::vector<const char*> names;
  std::vector<int> cycles;
  
  // Loop over keys in this directory -- pull out names and cycles for classes
  // that match the ones we want.
  TList* l = gDirectory->GetListOfKeys();
  for ( unsigned int i = 0; i < l->GetEntries(); ++i ) {
    TKey* k = (TKey*) l->At(i);
    if ( strcmp( k->GetClassName(), classType.c_str() ) == 0 ) {
      names.push_back( k->GetName() );
      cycles.push_back( k->GetCycle() );
    }
  }

  // Now we loop over the vectors to get the list of names
  SEXP rNames;
  PROTECT( rNames = NEW_CHARACTER( names.size() ) );

  for ( unsigned int i = 0; i < names.size(); ++i ) {
    // Form the string
    char buf[BUFSIZE];
    snprintf(buf, BUFSIZE, "%s;%d", names[i], cycles[i]);
    
    // Save this away
    SET_STRING_ELT( rNames, i, mkChar( buf ) );
  }

  UNPROTECT(1);

  // Restore the old directory
  if ( ! f->cd(oldDirectory) ) {
    error("namesMatchingClass: cd to old directory failed");
  }
  
  return rNames;
}
   
////////////////////////////////////////
// getHists - Get Histograms	   
SEXP getHistsR(SEXP fileForHists, SEXP histNames, SEXP directoryR)
{
  TFile* f = checkForFileForHistsWrapper(fileForHists);

  const char* oldDirectory = setFileDirectory(f, directoryR);

  // Make the return list
  SEXP ans, ansNames;
  PROTECT( ans = NEW_LIST( GET_LENGTH(histNames) ) );
  PROTECT( ansNames = NEW_STRING( GET_LENGTH(histNames) ) );

  // Loop over the names
  for ( unsigned int i = 0; i < GET_LENGTH(histNames); ++i ) {

    // Get the name of the object
		std::string name = CHAR( STRING_ELT(histNames, i) );
    
    // Set the list name
    SET_STRING_ELT( ansNames, i, mkChar(name.c_str()) );

    // What is this thing?
    TKey* key = gDirectory->FindKey(name.c_str());

    // If not found, then skip
    if ( ! key ) continue;

    // The object is real! - Get the class name
    const char* className = key->GetClassName();

    // Do certain things depending on the class name

    // For right now, we'll just do TH1F's and TH1D's. Maybe do more later.
    if ( strcmp(className, "TH1F") == 0 || strcmp(className, "TH1D") == 0 ) {

      // Get the histogram
      TH1* hist;
      gDirectory->GetObject(name.c_str(), hist);
      
      // For TH1F, there are 6 elements to the list (name, type, title,
      //    breaks, counts, 
      //    uncert, 
      //    mids, xname, underOverFlows, mean, rms, yname)
      SET_ELEMENT( ans, i, NEW_LIST(12) );
      SEXP data = VECTOR_ELT(ans, i);
      SEXP dataNames;
      PROTECT( dataNames = NEW_STRING(12) );

      // Add the type
      unsigned int j = 0;
      SEXP type = addCharVector(data, dataNames, j++, 1, "type");
      SET_STRING_ELT(type, 0, mkChar(className));
      
      // Add the name
      SEXP theName = addCharVector(data, dataNames, j++, 1, "name");
      SET_STRING_ELT(theName, 0, mkChar(name.c_str()));

      // Add basic histogram information
      j = addHistInfo(data, dataNames, j, hist);
      
      // Add the x-axis information
      j = addXAxis(data, dataNames, j, hist);

      // Add the x-contents information
      j = addXContents(data, dataNames, j, hist);

      // Add Y-axis name
      SEXP yname = addCharVector(data, dataNames, j++, 1, "yname");
      SET_STRING_ELT(yname, 0, mkChar( hist->GetYaxis()->GetTitle() ) );
      
      // Done
      SET_NAMES(data, dataNames);
      UNPROTECT(1);  // unprotect dataNames
    }

    else {
      REprintf("!! Do not know how to handle %s of class %s !!\n", 
	       name.c_str(), className);
      continue;
    }

  } // For over names
	
  // Restore the root old directory
  if ( ! f->cd(oldDirectory) ) {
    error("namesMatchingClass: cd to old directory failed");
  }

  // return the results
  SET_NAMES(ans, ansNames);
  UNPROTECT(2);
  
  return ans;
}      

//////////////////////////////////////////
// addNumericVector
SEXP addNumericVector(SEXP data, SEXP dataNames, 
		      unsigned int j, int n, const char* name)
{
  SET_ELEMENT(data, j, NEW_NUMERIC(n));
  SET_STRING_ELT(dataNames, j, mkChar(name));
  return VECTOR_ELT(data, j);
}
  
//////////////////////////////////////////
// addCharVector
SEXP addCharVector(SEXP data, SEXP dataNames, 
		      unsigned int j, int n, const char* name)
{
  SET_ELEMENT(data, j, NEW_STRING(n));
  SET_STRING_ELT(dataNames, j, mkChar(name));
  return VECTOR_ELT(data, j);
}

////////////////////////////////////////
// addHistInfo -- add histogram information
unsigned int addHistInfo(SEXP data, SEXP dataNames, unsigned int j, TH1* hist)
{
  SEXP title = addCharVector(data, dataNames, j++, 1, "title");
  
  SET_STRING_ELT( title, 0, mkChar(hist->GetTitle()) );
  
  return j;
}

//////////////////////////////////////////////////
// addXAxis - add X axis information
unsigned int addXAxis(SEXP data, SEXP dataNames, unsigned int j, TH1* hist)
{
  int n = hist->GetNbinsX();
  TAxis* axis = hist->GetXaxis();
  
  // Determine breaks--
  // Add to list
  SEXP breaks = addNumericVector(data, dataNames, j++, n+1, "breaks");

  // Get information
  for ( unsigned int i=0; i<n; ++i ) {
    NUMERIC_POINTER(breaks)[i] = axis->GetBinLowEdge(i+1);
  }

  // Add the high edge
  NUMERIC_POINTER(breaks)[n] = axis->GetBinUpEdge(n);

  // Determine mids--
  SEXP mids = addNumericVector(data, dataNames, j++, n, "mids");

  // Get information
  for ( unsigned int i=0; i<n; ++i ) {
    NUMERIC_POINTER(mids)[i] = axis->GetBinCenter(i+1);
  }

  // Get name of axis
  SEXP xname = addCharVector(data, dataNames, j++, 1, "xname");
  SET_STRING_ELT( xname, 0, mkChar( axis->GetTitle() ) );
  
  // Done
  return j;
}
		 
  
//////////////////////////////////////////////////
// addXContents
unsigned int addXContents(SEXP data, SEXP dataNames, unsigned int j, TH1* hist)
{
  int n = hist->GetNbinsX();
  
  // Determine counts and uncert--
  // Add to list
  SEXP counts =  addNumericVector(data, dataNames, j++, n, "counts");
  SEXP uncert =  addNumericVector(data, dataNames, j++, n, "uncert");
  
  for ( unsigned int i=0; i<n; ++i ) {
    NUMERIC_POINTER(counts)[i] = hist->GetBinContent(i+1);
    NUMERIC_POINTER(uncert)[i] = hist->GetBinError(i+1);
  }

  // Set under and over flows
  SEXP uof = addNumericVector(data, dataNames, j++, 2, "underOverFlows");
  NUMERIC_POINTER(uof)[0] = hist->GetBinContent(0);
  NUMERIC_POINTER(uof)[1] = hist->GetBinContent(n+1);

  // Get mean
  SEXP mean = addNumericVector(data, dataNames, j++, 2, "mean");
  NUMERIC_POINTER(mean)[0] = hist->GetMean(1);
  NUMERIC_POINTER(mean)[1] = hist->GetMean(11);

  // Get rms
  SEXP rms = addNumericVector(data, dataNames, j++, 2, "rms");
  NUMERIC_POINTER(rms)[0] = hist->GetRMS(1);
  NUMERIC_POINTER(rms)[1] = hist->GetRMS(11);
  
  return j;
}
