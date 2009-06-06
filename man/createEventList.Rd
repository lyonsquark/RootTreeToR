\name{createEventList}
\alias{createEventList}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Create an Event List from entry numbers }
\description{
  This function allows you to create an event list from entry numbers,
  so you can use complex selections in \R.
}
\usage{
createEventList(name, title, entryNums)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{name}{ Short name for the Event List. }
  \item{title}{ A longer descriptive title for the Event List. }
  \item{entryNums}{ A vector of Root Tree entry numbers to select for
    the Event List. }
}
\details{
  Here's a use case: You have a Root Chain with two main branches: one
  branch with general information about HEP events (like run number,
  event number, luminosity information, and event quality information);
  the other branch has electron data (where an entry can have more than
  one electron). You want to look at various aspects of the electron
  data (branch 2), but only for the events that pass quality
  requirements (branch 1). You could give a Root selection to the
  \code{\link{toR}} function with the quality requirements, but you
  don't want to have to remember this for every call to
  \code{\link{toR}}. Also, perhaps some of your requirements are too
  complicated for the Root selection.

  Event Lists are a solution - once applied to a Root Chain, they
  restrict subsequent access to that Chain to the selected entries. With
  this function, you can specify a vector of entry numbers for the Event
  List. So for the use case here, you would call \code{\link{toR}} on
  the event quality information (branch 1). Do the requirements in \R to
  select the data frame rows you want. From those rows, you can get a
  vector of the corresponding entry numbers, and use them to set the Event
  List. Apply it to the Root Chain. Then when you use \code{\link{toR}}
  to get the electron data (branch 2), the data will automatically only
  come from the entries in the Event List. This method makes \code{\link{toR}}
  faster and less prone to error.

  You can also make Event Lists with Root Selections. See
  \code{\link{makeEventList}}.

  Note that your Event List is not active until you apply it to the Root
  Chain with \code{\link{narrowWithEventList}}. 
}
\value{
  This function returns an \code{\link{EventList}} object. You can use
  this object in functions like \code{\link{narrowWithEventList}},
  \code{\link{isInEventList}}, \code{\link{addEventLists}}, etc.  
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon }

\seealso{ \code{\link{makeEventList}},
  \code{\link{narrowWithEventList}}, \code{\link{isInEventList}},
  \code{\link{addEventLists}}, \code{\link{toR}} }

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

## Does not work on the Mac (??)
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

