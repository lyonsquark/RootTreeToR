\name{listHists}
\alias{listHists}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ List histogram names in a Root Histogram file }
\description{
  List the names of Histograms in a Root file.
}
\usage{
listHists(fileForHists, className = c("TH1F", "TH1D"), dir = NULL, cycles = F)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{fileForHists}{ Root Histogram file object obtained with
    \code{\link{openFileWithHists}}. }
  \item{className}{ The Root objects to look for (default is \code{TH1F}).}
  \item{dir}{ The directory to look into (default is NULL for the top
    level directory). }
  \item{cycles}{ If TRUE, then show the cycle number with the histogram
    name. If FALSE (default), then suppress the cycle number.}
}
\details{
  This function displays the names of histogram objects within a Root
  file. Use \code{\link{getHists}} to load them. Use
  \code{\link{listDirs}} to see names of directories. 
}
\value{
  A character vector of histogram names. 
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon }

\seealso{ \code{\link{openFileWithHists}}, \code{\link{getHists}},
  \code{\link{listDirs}} }
\examples{
## Let's load the sample Root file that comes with this package
exFile = system.file("examples/histos.root", package="RootTreeToR")
hf = openFileWithHists(exFile)
hf

## This file should have one histogram in the top level directory
listHists(hf)

## But there should be some more histograms in the "components"
##   directory
listHists(hf, dir="components")

## And see the cycle numbers
listHists(hf, dir="components", cycle=T)

}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }

