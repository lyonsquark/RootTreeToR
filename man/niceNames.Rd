\name{niceNames}
\alias{niceNames}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{Shorten column names}
\description{
  Shorten long and ugly column names in a data frame.
}
\usage{
niceNames(df)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{df}{ Data frame with long and ugly column names. }
}
\details{
  The \code{\link{toR}} function will sometimes leave you with a data
  framew with very ugly column names. Such names are painful to
  type. This function adjusts the names to make them short. Note that
  this could lead to duplicated names, so you may have to adjust them
  afterwards.

  Parentheses, underscores, and dots are removed from the column names.

  Only the last part of the name is kept.

  For example, \code{MCpart._particle} goes to \code{particle}.
  \code{MCpart.getPMCvtx().nparents} goes to \code{nparents}.

  See the example for proper usage.
}
\value{
  A character vector of column names, suitable for assignment into
  \code{names()}. See the examples.
}
\author{ Adam L. Lyon lyon@fnal.gov}

\seealso{ \code{\link{toR}}, \code{\link{names}} }
\examples{
## Let's load the sample Root file that comes with this package
exFile <- system.file("examples/exampleEs.root", package="RootTreeToR")
rt <- openRootChain("exampleEs", exFile)

df3 <- toR(rt, c("evNums.evNums", "eCh.eCh"))
head(df3)

## df3 has ugly column names (like evNums.evNums)
names(df3)

## Use niceNames to clean them up
## !! Here is the important part !!
names(df3) <- niceNames(df3)
names(df3)

## Sometimes, you may get duplicated names.
dfbad <- data.frame(one.bla=c(1,2), two.bla=c(3,4))
names(dfbad)
names(dfbad) <- niceNames(dfbad)
names(dfbad)
## Uh oh - separate the names
names(dfbad)[2] <- "bla2"
names(dfbad)
}

% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }
