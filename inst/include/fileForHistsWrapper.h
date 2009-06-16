// Wrapper for a TFile object to get histograms -- called by R
//

#ifndef __FILEFORHISTSWRAPPER_H__
#define __FILEFORHISTSWRAPPER_H__

class TFile;
class TH1;
class TH1F;
class TH1D;

extern "C"
{
  SEXP initFileForHistsWrapper();
  SEXP deleteFileForHists(SEXP fileForHists);
  SEXP openFileForHists(SEXP rootFileNameR);
  SEXP namesMatchingClass(SEXP fileForHists, SEXP directoryR, SEXP classTypeR);
  SEXP getHistsR(SEXP fileForHists, SEXP histNames, SEXP directoryR);
};

TFile* checkForFileForHistsWrapper(SEXP ptr);

const char* setFileDirectory(TFile* f, SEXP directoryR);

SEXP wrapTFileForHistsPointer(TFile* f);

SEXP addCharVector(SEXP data, SEXP dataNames, unsigned int j, 
		   int n, const char* name);

SEXP addNumericVector(SEXP data, SEXP dataNames, unsigned int j, 
		      int n, const char* name);

unsigned int addHistInfo(SEXP data, SEXP dataNames, unsigned int j, TH1* hist);

unsigned int addXAxis(SEXP data, SEXP dataNames, unsigned int j, TH1* hist);

unsigned int addXContents(SEXP data, SEXP dataNames,
			  unsigned int j, TH1* hist);

#endif

