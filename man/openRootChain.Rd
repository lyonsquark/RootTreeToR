\name{openRootChain}
\alias{openRootChain}
\title{Open a Root Chain}
\description{
  Open a Root Chain. A Root Chain is one or more Root files that contain a
  Tree. Opening the Root Chain makes the data available to R.
}
\usage{
openRootChain(tree, files, verbose = TRUE, trace = FALSE)
}

\arguments{
  \item{tree}{The tree name common to all of the files in the chain that you want
    to load.}
  \item{files}{If one file, then just the file name. If multiple files,
    then a character vector of file names.}
  \item{verbose}{TRUE if you want informative output while processing
    the chain. FALSE if you want silence.}
  \item{trace}{TRUE if you want debugging information. FALSE if you want
  silence.}
}
\details{
  In order to read data in a Root Tree, you need to open the Root Tree
  files. Tree files can be "chained" together to make a
  Root Chain, or a Chain can be made up of just one file. Either way
  everything works the same.

  Use \code{openRootChain} to open one or more files making up the
  Chain. You specify the list of files as well as the name of the Root
  Tree within (the Tree must have the same name and format in all of the
  files). The examples in \code{\link{rootCommand}} show what to do if
  you don't know the name of the Tree. A \code{\link{RootChain}} object is
  returned by this command. You can use that object to process the Tree
  within. See the Next Steps section below.
}

\value{
  A \code{RootChain} object is returned.
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon }
\note{You may need to load some libraries into Root before you can open
  the Root Tree files (this is the case for D0 CAF Trees). See the
  examples in \code{\link{rootCommand}} for information.

  When you open the Root Chain, you can set a verbosity and a trace
  flag. These flags are set as specified and retain their values for all
  processing of the returned \code{RootChain} object.

  You can turn on and off verbosity and trace with \code{\link{setVerbose}}
  and \code{\link{setTrace}} respectively. 
}

\section{Next Steps}{Once you have a \code{RootChain} object by running
  this \code{openRootChain} command, you can ...

  \describe{
    \item{Get names of contents within the Root Tree:}{Use
      \code{\link{getNames}}}.
    \item{Get the number of entries in the Root Chain:}{Use
      \code{\link{nEntries}}}.
    \item{Pull data from the Root Tree into R:}{Use \code{\link{toR}}}.
    \item{Restrict the entries of the RootChain:}{Use
    \code{\link{createEventList}} or less often \code{\link{makeEventList}}}.
  }
  
}
 
\seealso{ \code{\link{setVerbose}}, \code{\link{setTrace}}, \code{\link{rootCommand}},
  \code{\link{getNames}}, \code{\link{nEntries}}, \code{\link{toR}},
  \code{\link{makeEventList}}, \code{\link{RootChain}}
}
\examples{
## Let's load the sample Root file that comes with this package
exFile = system.file("examples/exampleEs.root", package="RootTreeToR")
rt = openRootChain("exampleEs", exFile)
rt

\dontrun{
## You would probably do something like
rt = openRootChain("Global", "myCafTree.root")

## If you had a chain...
rt = openRootChain("Global", c("myCafTree_1.root", "myCafTree_2.root"))
}
}
\keyword{ manip }
