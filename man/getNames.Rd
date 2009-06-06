\name{getNames}
\alias{getNames}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Get names of data in a Root Tree}
\description{
  Given a \code{\link{RootChain}} object, return a list of names in the
  Root Tree (branches and leaves).
}
\usage{
getNames(rootChain, raw = FALSE)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{rootChain}{ \code{\link{RootChain}} object to process obtained
    by \code{\link{openRootChain}}. }
  \item{raw}{ If FALSE (default), then return a named list of branches. Each
    branch is then a character vector of leaf names. If TRUE, then the
    character vector of leaf names is replaced by the Root Tree branch
    specification string.}
}
\details{
  Return the names of data in a Root Tree or Chain. Obtain the
  \code{\link{RootChain}} object with the \code{\link{openRootChain}}
  function. Note that this function works best on simple Root Trees
  based on simple types. You may not get much information from special
  Trees, like the D0 CAF Tree.
}
\value{
  A named list of branches. Within each branch is either a character
  vector of leaf names (if \code{raw} is FALSE, the default) or a string
  with the Root Branch specification (if raw is TRUE).
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{Addm Lyon \email{lyon@fnal.gov} }

\seealso{ \code{\link{openRootChain}}, \code{\link{RootChain}} }
\examples{
## Let's load the sample Root file that comes with this package
exFile = system.file("examples/exampleEs.root", package="RootTreeToR")
rt = openRootChain("exampleEs", exFile)

getNames(rt)

getNames(rt, raw=T)
}

% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }
