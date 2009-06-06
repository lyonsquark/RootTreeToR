###########
# WriteDFToRoot
#
#   Write a data frame to Root
#

writeDFToRoot <- function(df, name=deparse(substitute(df)),
  rootFileName=paste(name, ".root", sep=""),
  mode="RECREATE", title="A Tree") {

  # Check that df is indeed a data frame
  if ( ! "data.frame" %in% class(df) ) stop("First argument must be a data frame")

  # Check the mode
  mode = toupper(mode)
  if (! mode %in% c("RECREATE", "NEW") ) stop("mode must be RECREATE or NEW")

  # Get types
  t = sapply(df, storage.mode)

  rootType <- function(type) {
    switch(type,
      integer = "I",
      logical = "L",
      character = "C",
      double="D")
  }

  # Get Root types
  rt = sapply(t, rootType)

  # Replace the L's with I's for root (root has no boolean type)
  #  L is a long type!
  nrt = rt
  nrt[ rt == "L" ] <- "I"

  # Build the branch strings
  branchTypes= paste(names(rt), nrt, sep="/")
  
  nrowsR = nrow(df)
  colNames = names(df)

  cat(branchTypes, '\n')
  
  # Write it
  .Call("writeDFToRoot", name, title, df, nrowsR, colNames, rt, branchTypes,
        rootFileName, mode,
        package="RootTreeToR")

  invisible()
}
