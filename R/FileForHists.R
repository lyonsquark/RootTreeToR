###############
# FileForHists.R
#
#  S4 Class for a FileForHists
#

setClass("FileForHists",
         representation(ptr="externalptr", fileName="character")
         )

#######
setMethod("initialize",
          "FileForHists",
          function(.Object, fileName) {
            .Object@fileName = fileName

            .Object@ptr = .Call(openFileForHists, fileName)

            if ( is.null(.Object@ptr) ) stop ("Could not create FileForHists@ptr")

            .Object
          }
          )

#######
## Show
setMethod("show", "FileForHists",
          function(object) {
            cat("File For Histograms file name", object@fileName, "\n")
          }
          )

####################
# Open a file
openFileWithHists <- function(fileName) {
  new("FileForHists", fileName)
}

########################################
# listDirs - List the directories in a file
listDirs <- function(fileForHists, dir=NULL) {
  listNamesMatchingClassInFile(fileForHists, "TDirectory",
                               dir)
}

########################################
# listHists - list histograms
listHists <- function(fileForHists,
                     className=c("TH1F","TH1D"), dir=NULL, cycles=F) {

  className=match.arg(className)
  
  listNamesMatchingClassInFile(fileForHists, className, dir, cycles)
}
  

########################################
# listNamesMatchingClassInFile - names of classes in a file
listNamesMatchingClassInFile <- function(fileForHists, className="TH1F",
                                        dir=NULL, cycles=F ) {
  
  .assertClass(fileForHists, "FileForHists")

  # Get the list of matching names
  names = .Call(namesMatchingClass, fileForHists@ptr, dir, className)

  # Remove the cycles if necessary
  if ( ! cycles ) {
    allNames = sapply( strsplit(names, ";", fixed="T") ,
                       function(x) x[1])

    # Make a warning if there are duplicated names
    if ( any(duplicated(allNames)) ) {
      warning("Duplicated names were removed - use cycles to access older versions")
    }

    names = unique(allNames)
  }

  # We are done
  names
}
    
##################################################
# getHists - Get Histograms
getHists <- function(fileForHists, histNames, dir=NULL) {
  .assertClass(fileForHists, "FileForHists")

  if ( ! is.character(histNames) ) stop("Names must be a character vector")
  
  ans = .Call(getHistsR, fileForHists@ptr, histNames, dir)

  # Create the density (same as counts) and equidist and apply class
  ans = lapply(ans,
          function(x) { x$density=x$counts ; x$intensities=x$counts ;
                        d = diff(x$breaks) ; x$equidist = all(d==d[1]) ;
                        class(x) <- c(x$type, "histogram") ; x } )

  # If there is just one, then just return it straight
  if ( length(ans) == 1 ) return( ans[[1]] )

  # Otherwise, return the whole thing as a list
  ans
}
