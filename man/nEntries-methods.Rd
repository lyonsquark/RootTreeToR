\name{nEntries-methods}
\docType{methods}
\alias{nEntries-methods}
\alias{nEntries}
\title{ Return number of Enties in a Root Chain}
\description{
  Returns the number of entries in a Root Chain.
}
\section{Methods}{
\describe{
  Given a \code{\link{RootChain}} object (obtained from
  \code{\link{openRootChain}}), return the total number of entries in the Root Chain.
}}

\value{
  The number of entries in the Root Chain.
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{Addm Lyon \email{lyon@fnal.gov} }

\seealso{ \code{\link{openRootChain}}, \code{\link{RootChain}} }
\examples{
## Let's load the sample Root file that comes with this package
exFile = system.file("examples/exampleEs.root", package="RootTreeToR")
rt = openRootChain("exampleEs", exFile)

nEntries(rt)
}
\keyword{methods}
