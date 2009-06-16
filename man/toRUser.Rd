\name{toRUser}
\alias{toRUser}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Load data from a Root Chain into \R with an external function }
\description{
  Select data from a Root Chain and load data into an \R data frame, but
  with an external function.
}
\usage{
toRUser(rootChain, userFunction, nEntries = 100, firstEntry = 0, initialSize = 1000, growthFactor = 1.7, args = NULL)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{rootChain}{ The \code{\link{RootChain}} object obtained with
    \code{\link{openRootChain}}.}
  \item{userFunction}{ A string with the name of the function to
    call. See details section below.}
  \item{nEntries}{ The number of Root Entries to load (defaults to 100). If zero or less, do 99,999,999 entries.}
  \item{firstEntry}{ The first entry to load (defaults to 0, whcih means
    to start at the beginning).}
  \item{initialSize}{ The initial guess for the number of rows in the
    resulting \R data frame.}
  \item{growthFactor}{ If the guess for the number of rows in the
    resulting \R data frmae is too low, expand by this factor.}
  \item{args}{ A \R variable to pass to your \code{userFunction},
    or \code{NULL} for no arguments (the default).}
}
\details{
  This \code{toRUser} function is like \code{\link{toR}}, but much more
  complicated. See \code{\link{toR}} for basic information for
  retreiving data from a Root chain into \R. \code{\link{toR}} can
  handle any Root variable or expression than can go in a
  \code{TTree::Draw} command. There are some instances where you need to
  do complicated manipulation of data, beyond what \code{TTree::Draw}
  can handle. In those cases, you write a C++ class to directly
  manipulate the Root data and fill an \R data frame. You then use this
  \code{\link{toRUser}} function to run the C++ code to bring in the
  data. You should look at \code{\link{toR}} to see if it can fulfill
  your requirements before writing C++ code to use this function. While
  the code is not complicated, \code{\link{toR}} is much easier. With
  that said, below are details of how to write the C++ code and use this
  function.

  There is sample code that you can look at. You can determine the
  appropriate directory with,
  
  \code{system.file("examples/toRUser", package="RootTreeToR")}

  Within that directory, look at \code{met.h} and \code{met.cpp}, which
  are heavily documented (much more so than the other files). They serve
  as templates for the code you need to write. The \code{Makefile} is
  also important. The other examples in that directory do more
  complicated Root manipulations. All of these examples use a particular
  Root Tree called D0 CAF (for common analysis format used by the D0
  experiment). Extra libaries are required to be loaded into Root before
  they will work (see \code{\link{rootCommand}} for how to load
  them). You probably do not have access to the libraries, but the files
  here should still serve as useful examples for doing what you want.

  The \code{RDataFrame} and associated column classes make loading an \R
  data frame from C++ quite simple. There is automatic checking that the
  correct type was loaded into each column and that you did not forget a
  column before committing the data to the data frame. See the met
  examples mentioned above for how to use this class.

  To run your code and read in data, you need to build the code with the
  Makefile. That will make a shared object file. Load that shared object
  into \R with...

  \code{dyn.load("/path/to/the/sharedObject.so")}

  You can use a relative path from the current \R working directory
  (\code{\link{getwd}} tells you the current working directory).

  Then call \code{toRUser} where the second argument is the name of the
  C driver function (the one in wrapped in extern "C" in the code). See
  the example section below.

  See \code{\link{toR}} for explanations of the other arguments.

  Let me know if you need more information on this function and writing
  the C++ code and I can expand this documentation.
}
\value{
  See \code{\link{toR}} for a description of the returned data frame.
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon lyon@fnal.gov }

\section{DISCLAIMER}{Although the author is unaware of mistakes in
  importing Root information into \R, YOU are responsible for checking
  your results with Root. There will be minor differences due to round
  off errors. There may be major differences due to bugs. You should
  repeat parts of your analysis in Root and compare to \R. If you find
  problems, please inform the author, who assumes absolutely no
  responsibility or liability as to the correctness of your results when
  you use this package.}

\seealso{ \code{\link{openRootChain}}, \code{\link{toR}} }

\examples{
\dontrun{
## This code will not run, but use it as an example
##
## Load your Root Tree
rt = openRootChain("MyTree", "myTreeFile.root")

## Load the shared object with your code that you built previously
dyn.load("mySpecialCode.so")

## Run toRUser
df = toRUser(rt, "myDriverFunction", 999999)
}

}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }
