\name{clearEventList}
\alias{clearEventList}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Clear an event list }
\description{
  Remove the event list narrowing on a Root Chain (all events will be read).
}
\usage{
clearEventList(rootChain)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{rootChain}{  The \code{\link{RootChain}} object obtained with
    \code{\link{openRootChain}}. }
}
\details{
  This function removes the narrowing from an Event List on a Root
  Chain. 
}
 \value{
  Nothing is returned.
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon }

\seealso{ \code{\link{openRootChain}} }

% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }
