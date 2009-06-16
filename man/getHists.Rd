\name{getHists}
\alias{getHists}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Read Root Histogram data into \R}
\description{
  Read a Histogram's data into \R.
}
\usage{
getHists(fileForHists, histNames, dir = NULL)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{fileForHists}{ Root Histogram file object obtained from
    \code{\link{openFileWithHists}}. }
  \item{histNames}{ Name of the histogram to read in, or a character
    vector of histogram names if more than one. }
  \item{dir}{ The directory within the Root file to look in (default is
    top level directory). }
}
\details{
  This function brings Root Histogram data into \R, in the form of an \R
  \code{hist} object (see help on \code{\link{hist}} and the Value
  section of this help file). AT THIS TIME, ONLY TH1F and TH1D HISTOGRAMS MAY BE
  IMPORTED. 
}
\value{
  If just one histogram is requested, then an \R hist object is
  returned. If a list of histogram names is given, then a list of \R
  hist objects is returned. See \code{\link{hist}} for information
  about the object and below.

  The histogram object is a list of information (it's an S3 class of
  type histogram). You can access parts of the histogram in the standard
  way (e.g. myHisto$counts). Here is an explanation of what is in the
  object (the ones marked with * are not in the regular \R histogram object):

  \item{type}{TH1F or TH1D for 1D histograms, TH2F or TH2D for 2D.*}
  \item{name}{Root Histogram name.*}
  \item{title}{Root Histogram title.*}
  \item{breaks}{The bin edges.}
  \item{mids}{Bin centers.}
  \item{xname}{x argument name (may not be filled in).*}
  \item{counts}{Number of counts for the bin.}
  \item{uncert}{Uncertainty for the bin.*}
  \item{underOverFlows}{ c(underflows, overflows).*}
  \item{mean}{ c(meanAlongX), c(meanAlongY).* }
  \item{rms}{ c(rmsAlongX), c(rmsAlongY).* }
  \item{yname}{ y argument name (may not be filled in).*}
  \item{density}{ Same as counts. }
  \item{intensities}{ Same as counts. }
  \item{equidist}{ True if bins are equidistant (note, may say false
  even if bins are equidistant due to round off errors).
}}

\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon lyon@fnal.gov }

\section{DISCLAIMER}{Although the author is unaware of mistakes in
  importing Root information into \R, YOU are responsible for checking
  your results with Root. There will be minor differences due to round
  off errors. There may be major differences due to bugs. You should
  repeat parts of your analysis in Root and compare to \R. If you find
  problems, please inform the author, who assumes absolutely no
  responsibility as to the correctness of your results when you use this
  package.}

\seealso{ \code{\link{hist}}, \code{\link{openFileWithHists}},
  \code{\link{listHists}}, \code{\link{listDirs}}. }
\examples{
## Let's load the sample Root file that comes with this package
exFile = system.file("examples/histos.root", package="RootTreeToR")
hf = openFileWithHists(exFile)
hf

## This file should have one histogram in the top level directory
listHists(hf)

## Load in the pt histogram
pt = getHists(hf, "pt")

## Plot it
plot(pt)

## Get the mean 
weighted.mean(pt$mids, pt$counts)

## Get more than one
h = getHists(hf, c("px", "py"), dir="components")

## What did we get? -- a list of named histograms
names(h)

## Plot them
plot(h$px)
plot(h$py)

}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }
