// Code to process a Root command
#include <TROOT.h>
#include <TRint.h>

#include <R.h>
#include <Rdefines.h>

#include <string>

static TRint* rootApp;
static int argc;
char **argv;

extern "C" {
  SEXP initRoot();
  SEXP rootCommand(SEXP comm);
}

SEXP initRoot()
{
  // Initialize Root
  argc = 0;
  rootApp = new TRint("RootTreeToR", &argc, argv, 0, 0, 0);
	
  return R_NilValue;
}

SEXP rootCommand(SEXP comm)
{
	std::string command = CHAR(STRING_ELT(comm, 0));
  int err;
	
  gROOT->ProcessLine(command.c_str(), &err);
	
  SEXP r = NEW_INTEGER(1);
  INTEGER(r)[0] = err;
	
  return r;
}
