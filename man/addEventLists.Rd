\name{addEventLists}
\alias{addEventLists}
\alias{subtractEventLists}
\alias{clear}
\alias{getDetails}
\alias{getEntries}
\alias{getName}
\alias{isInEventList}
\alias{putIntoEventList}

%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Event List Manipulations }
\description{
  Use the functions listed here to manipulate Event Lists.
}
\usage{
addEventLists(eventList, eventList2)
clear(eventList)
getDetails(eventList)
getEntries(eventList)
getName(eventList)
isInEventList(eventList, entryNums)
putIntoEventList(eventList, entryNums)
subtractEventLists(eventList1, eventList2)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{eventList}{ An Event List - will hold result of the function action. }
  \item{eventList2}{ Another Event List. }
  \item{entryNums}{ A vector of Root Global Entry Numbers. }
}
\details{
  \code{addEventLists} allows you to add events that are in
  \code{eventList2} to \code{eventList}. The Event List in the
  \code{eventList} argument is altered.

  \code{subtractEventLists} removes entry numbers in \code{eventList2}
     from \code{eventList}. The Event List in \code{eventList} argument
     is altered.

  \code{clear} removes all of the entry numbers from the Event List.

  \code{getDetails} returns the title of the Event List (the title
  typically has details about how the Event List was produced).

  \code{getEntries} returns a vector of entry numbers that are in the
  Event List.

  \code{getName} returns the name of the Event List.

  \code{isInEventList} checks if entry numbers in the \code{entryNums}
  vector are in the Event List. Returns a logical vector of trues and
  falses accordingly.

  \code{putIntoEventList} adds entry numbers in the \code{entryNums}
  vector to the Event List. You cannot create an Event List with this
  function; use \code{\link{createEventList}} instead.
  
}
\value{
  See details above.
} 
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon }

\seealso{ \code{\link{createEventList}}, \code{\link{makeEventList}},
  \code{\link{narrowWithEventList}} }
\examples{
## The functions should be self-explanatory
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }
