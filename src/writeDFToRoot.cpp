/////////////
// writeDFToRoot - write a dataset to a root file
//
//   A bit ugly, but it works!
//

#include "TTree.h"
#include "TFile.h"

#include <R.h>
#include <Rdefines.h>

#include <string>

extern "C" {
  SEXP writeDFToRoot(SEXP nameR, SEXP titleR, SEXP df, SEXP nrowsR,
		     SEXP colNames, 
		     SEXP rt, SEXP branchStrings, SEXP rootFileNameR, 
		     SEXP modeR);
}

SEXP writeDFToRoot(SEXP nameR, SEXP titleR, SEXP df, SEXP nrowsR,
		   SEXP colNames,
		   SEXP rt, SEXP branchStrings, SEXP rootFileNameR,
		   SEXP modeR)
{
  // Extract stuff
	std::string name = CHAR(STRING_ELT(nameR,0));
	std::string title = CHAR(STRING_ELT(titleR,0));
  int nrows = INTEGER(nrowsR)[0];
	std::string rootFileName = CHAR(STRING_ELT(rootFileNameR,0));
	std::string mode = CHAR(STRING_ELT(modeR,0));

  int ncols = LENGTH(df);

  // Open the output file
  TFile rootFile(rootFileName.c_str(), mode.c_str());
  if ( ! rootFile.IsOpen() ) error("File cannot be opened");

  // Create the tree
  TTree* tree = new TTree(name.c_str(), title.c_str());
  
  // Loop over branches
  for ( unsigned col = 0; col < ncols; ++col ) {

    // Create the leaf holder
    void* brAddress;

    switch ( CHAR(STRING_ELT(rt, col))[0] ) {
    case 'I':
    case 'L':
      brAddress = new int;
      break;
     
    case 'D':
      brAddress = new double;
      break;

    case 'C':
      brAddress = new char[255];
      break;

    default:
      // Unknown type
      error("Data type not supported");
      break;
    }

    // Create the branch
    TBranch* branch = tree->Branch( CHAR(STRING_ELT(colNames,col)),
				    brAddress, 
				    CHAR(STRING_ELT(branchStrings,col)) );
    
    // Fill this branch
    for ( unsigned int row = 0; row < nrows; ++row ) 
    {
      switch ( CHAR(STRING_ELT(rt,col))[0] ) {
      case 'I':
	*((int*)brAddress) = INTEGER( VECTOR_ELT(df, col) )[row];
	break;

      case 'L':
	if ( LOGICAL( VECTOR_ELT(df, col) )[row] ) {
	  *((int*)brAddress) = 1;
	}
	else {
	  *((int*)brAddress) = 0;
	}
	break;
	
      case 'D':
	*((double*)brAddress) = REAL( VECTOR_ELT(df, col) )[row];
	break;
	
      case 'C':
	strncpy( (char*)brAddress,
		 CHAR( STRING_ELT(VECTOR_ELT(df, col), row)  ), 254 );
	break;
	
      default:
	// Unknown type
	error("Data type not supported");
	break;
      }

      branch->Fill();
    } // for over rows

    // Clean up
    switch ( CHAR(STRING_ELT(rt,col))[0] ) {
    case 'I':
    case 'L':
      delete (int*)brAddress;
      break;
     
    case 'D':
      delete (double*)brAddress;
      break;

    case 'C':
      delete (char*)brAddress;
      break;

    default:
      // Unknown type
      error("Data type not supported");
      break;
    }

  } // for over columns

  // Set the size of the tree
  tree->SetEntries(nrows);
  
  // Write it out
  tree->Write();

  return rootFileNameR;
  
}
  
  
   
    
