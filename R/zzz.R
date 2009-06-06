## zzz.R
## Load libraries

.onLoad <- function(lib.loc, package) {

  ## Load the methods library
  require(methods)
  
  ## Load the RootTreeToR shared library
  print("Loading ROOT")
  
  ## Note that library is loaded with useDynLib(RootTreeToR) in the NAMESPACE file
  ##library.dynam("RootTreeToR", package, lib.loc)
  
  ## Initialize Root (needed for some root commands)
  .Call(initRoot)
  
  ## Call the initialize routine for RootChainManagerWrapper
  .Call(initRootChainManagerWrapper)
  
  ## Call the initialize routine for EventListWrapper
  .Call(initEventListWrapper)

  ## Call the initialize routine for FileForHistsWrapper
  .Call(initFileForHistsWrapper)
}

.onUnload <- function(libpath) {
  
  ## Unload the RootTreeToR library
  library.dynam.unload("RootTreeToR", lib.loc)
}
