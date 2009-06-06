\name{getEventList}
\alias{getEventList}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Event List name for a Root Chain}
\description{
  Get the Event List object associated with the Root Chain.
}
\usage{
getEventList(rootChain)
}

\arguments{
  \item{rootChain}{ Root Chain object obtained with
    \code{\link{openRootChain}}}
}
\details{
  Return the Event List object associated with the Root Chain object.
}
\value{ An Event List object.
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon }
\seealso{ \code{\link{getEventListName}}, \code{\link{openRootChain}}, \code{\link{createEventList}},
  \code{\link{makeEventList}} }
\examples{
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }

