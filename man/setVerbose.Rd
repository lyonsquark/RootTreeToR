\name{setVerbose}
\alias{setVerbose}
\alias{getVerbose}
\alias{setTrace}
\alias{getTrace}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Get and set verbose and trace options }
\description{
  Determine and possibly change the verbosity and trace settings on a
  \code{RootChain} object.
}
\usage{
setVerbose(rootChain, verbose = TRUE)
getVerbose(rootChain)
setTrace(rootChain, trace = TRUE)
getTrace(rootChain)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{rootChain}{ The \code{\link{RootChain}} object obtained from
    \code{\link{openRootChain}}. }
  \item{verbose}{ If TRUE, turn on verbosity. }
  \item{trace}{ If TRUE, turn on debug tracing. }
}
\details{
  The verbosity and trace settings are initially specified in the
  \code{\link{openRootChain}} function. You can change them with the
  functions given here.
}
\value{
  The \code{get} functions return a logical value. 
}
\author{ Adam L. Lyon }
\seealso{ \code{\link{openRootChain}}, \code{\link{RootChain}} }
\examples{
## Let's load the sample Root file that comes with this package
exFile = system.file("examples/exampleEs.root", package="RootTreeToR")
rt = openRootChain("exampleEs", exFile)
rt

## Turn off verbosity
getVerbose(rt)
setVerbose(rt, F)
rt

## Turn on debugging
setTrace(rt, T)
rt
}

% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }
