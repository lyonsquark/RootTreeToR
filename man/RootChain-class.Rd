\name{RootChain-class}
\docType{class}
\alias{RootChain-class}
\alias{RootChain}

\title{Class "RootChain" }
\description{ A \code{\link{RootChain}} object represents a chain of
  Root files with Trees. The documentation here is mostly for internal
  use only. Look at the See Also section for a list of commands that
  work on RootChains.}

\section{Objects from the Class}{
Objects can be created by calls of the form \code{new("RootChain", tree,
  files, verbose, trace)}
!! USERS SHOULD NEVER CALL THIS! Use
  \code{\link{openRootChain}} instead !!

}
\section{Slots}{
	 \describe{
    \item{\code{tree}:}{Object of class \code{"character"}; the name of
      the Root Tree in the chain. }
    \item{\code{files}:}{Object of class \code{"character"}; the list of
      files making up the chain. }
    \item{\code{manager}:}{Object of class \code{"externalptr"}; the
      pointer to the C++ RootChainManager object. }
  }
}

\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon lyon@fnal.gov}

\seealso{
  \code{\link{openRootChain}}, \code{\link{getNames}}, \code{\link{nEntries}}, \code{\link{toR}}, \code{\link{toRUser}}, \code{\link{makeEventList}},
  \code{\link{narrowWithEventList}}, \code{\link{getEventListName}}, \code{\link{clearEventList}}, \code{\link{getVerbose}},
  \code{\link{setVerbose}}, \code{\link{getTrace}}, \code{\link{setTrace}}
}
\examples{
}
\keyword{classes}
