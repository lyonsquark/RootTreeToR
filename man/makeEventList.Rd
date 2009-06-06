\name{makeEventList}
\alias{makeEventList}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Make a Root Event List}
\description{ Make a Root Event List to restrict the data in a Root
  Chain. 
}
\usage{
makeEventList(rootChain, name, selection, append = F, nEntries = 100, firstEntry = 0)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{rootChain}{ The \code{\link{RootChain}} obtained by
    \code{\link{openRootChain}}.}
  \item{name}{ Very short name for the Root Event List. }
  \item{selection}{ A Root Selection string defining the Event List. }
  \item{append}{ Logical; If TRUE, append passing events to an already
    existing Event List specified by the \code{name} argument. If FALSE,
  make a new Event List.}
  \item{nEntries}{ Number of entries in the chain to consider for the
    selection. WARNING: Default is low! }
  \item{firstEntry}{ The first entry in the chain to consider for the
    selection. Default is zero (the first entry).  }
}
\details{
  An Event List restricts a Root Chain to a particular set of events; so
  any actions on that Chain only process the passing events. You an make
  an Event List with Root syntax with this function, or from \R directly with
  \code{\link{createEventList}}.

  The \code{selection} argument is a Root Selection string that
  specifies the restriction you want. For example, "Em.Pt>50" means that
  you only want to see entries in the chain that statisfy that
  particular requirement. Subsequent operations on that chain, such as
  \code{\link{toR}}, will only process those entries.

  NOTE: Making or creating an Event List does not apply it to the
  chain. Once need to call \code{\link{narrowWithEventList}} to actually
  apply the Event List to the Root Chain.

  NOTE: Calling this function is somewhat unusual since you can do
  nearly the same thing with the \code{selection} argument in
  \code{\link{toR}}. What \emph{is} special is that you can create Event
  Lists from \R itself with \code{\link{createEventList}}, allowing for
  application of very complex requirements. 
}
\value{
  This function returns an \code{\link{EventList}} object. You can use
  this object in functions like \code{\link{narrowWithEventList}},
  \code{\link{isInEventList}}, \code{\link{addEventLists}}, etc.  }

\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon }

\seealso{ \code{\link{createEventList}}, \code{\link{narrowWithEventList}}, \code{\link{isInEventList}},
  \code{\link{addEventLists}}, \code{\link{toR}} }
\examples{
## Let's load the sample Root file that comes with this package
exFile = system.file("examples/exampleEs.root", package="RootTreeToR")
rt = openRootChain("exampleEs", exFile)

## Say we only care about events with Pt > 100 GeV
el = makeEventList(rt, "highPt", "sqrt(px*px+py*py)>100", nEntries=999999)
el

## At this point the Root Chain does NOT have the Event List
## applied. See narroWithEventList to do that.
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }

