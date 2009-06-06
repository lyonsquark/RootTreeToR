\name{openFileWithHists}
\alias{openFileWithHists}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Open a Root file containing Histograms }
\description{
  Open a Root file containing Histograms so you can read them into \R.
}
\usage{
openFileWithHists(fileName)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{fileName}{ The name of the Root file containing Histograms. }
}
\details{
  It it sometimes useful to read a Histogram into R. You start this
  process by opening the Root file that contains the Histograms you
  want. At this moment, only \code{TH1F} Histograms may be read in.
  Once you open the Root Histogram file, you may want to find names of
  directories within the file with \code{\link{listDirs}}, find names of
  Histograms with \code{\link{listHists}}, and load the histograms into \R
  with \code{\link{getHists}}.
}
\value{
  Returns a FileForHists object.
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon }
\seealso{ \code{\link{listDirs}}, \code{\link{listHists}},
\code{\link{getHists}} }
\examples{
## Let's load the sample Root file that comes with this package
exFile = system.file("examples/histos.root", package="RootTreeToR")
hf = openFileWithHists(exFile)
hf

\dontrun{
## If your file is in the working directory, just do
hf = openFileWithHists("myfile.root")
}
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }
