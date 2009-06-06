\name{narrowWithEventList}
\alias{narrowWithEventList}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Narrow with an Event List}
\description{
  Restrict (or Narrow) a Root Chain with an Event List.
}
\usage{
narrowWithEventList(rootChain, eventList)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{rootChain}{ The \code{\link{RootChain}} to narrow, obtained by
    \code{\link{openRootChain}}. }
  \item{eventList}{ The \code{\link{EventList}} to use, obtained by
    \code{\link{makeEventList}} or \code{\link{createEventList}}. }
}
\details{
  This function actually applies an \code{\link{EventList}} to a
  \code{\link{RootChain}}. Subsequent operations on the Root Chain will
  only process the events in the Event List. Use
  \code{\link{clearEventList}} to remove the narrowing and get access to
  the full Root Chain.
}
\value{
  Nothing is returned.
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon }

\seealso{ \code{\link{createEventList}}, \code{\link{clearEventList}},
\code{\link{makeEventList}} }

\examples{
## Let's load the sample Root file that comes with this package
exFile = system.file("examples/exampleEs.root", package="RootTreeToR")
rt = openRootChain("exampleEs", exFile)

## Let's get all of the data
df = toR(rt, c("px", "py"), "", 999999)

## Calculate Pt
df$pt = with(df, sqrt(px*px+py*py))

head(df)

## Get a vector of entry numbers for entires with Pt > 100 GeV/c
goodEntries = df$globalEntry[ df$pt > 100 ]

## Create an event list
el = createEventList("highPt", "Pt > 100 GeV/c", goodEntries)
el

## Apply it to the Root chain
narrowWithEventList(rt, el)
rt

## DOES NOT WORK!
getEventListName(rt)

## Now toR will only look at those passing events!
df2 = toR( rt, c("eCh", "px", "py"), "", 9999999)
df2$pt = with(df2, sqrt(px*px+py*py))
head(df2)

## Remove the restriction
clearEventList(rt)

}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }
