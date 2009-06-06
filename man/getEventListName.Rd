\name{getEventListName}
\alias{getEventListName}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Event List name for a Root Chain}
\description{
  Get the Event List name associated with the Root Chain.
}
\usage{
getEventListName(rootChain)
}

\arguments{
  \item{rootChain}{ Root Chain object obtained with
    \code{\link{openRootChain}}}
}
\details{
  Return the name of the Event List associated with the Root Chain object.
}
\value{ Name of the Event List.
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon }
\seealso{ \code{\link{getEventList}}, \code{\link{openRootChain}}, \code{\link{createEventList}},
  \code{\link{makeEventList}} }
\examples{
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }

