\name{listDirs}
\alias{listDirs}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ List directories in a Root Histogram file }
\description{
  Return a character vector of 
}
\usage{
listDirs(fileForHists, dir = NULL)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{fileForHists}{ The histogram file object obtained by
    \code{\link{openFileWithHists}}. }
  \item{dir}{ A directory to go into to look for subdirectories (default
  is NULL, which means the top level directory).}
}
\details{
  This function lists directories in a Root file opened by
  \code{\link{openFileWithHists}}. If you need to
  look for directories inside directories, use the \code{dirs} argument.
  You can then use the directory names in \code{\link{getHists}} and
  \code{\link{listHists}}. 

}
\value{
  A charactor vector of directory names.
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon }
\seealso{ \code{\link{openFileWithHists}}, \code{\link{getHists}},
  \code{\link{listHists}} }
\examples{
## Let's load the sample Root file that comes with this package
exFile = system.file("examples/histos.root", package="RootTreeToR")
hf = openFileWithHists(exFile)
hf

## This file has one directory under the top level
listDirs(hf)

## And no directories within that directory
listDirs(hf, dir="components")
}

\keyword{ manip }

