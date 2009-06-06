\name{RootTreeToR-package}
\alias{RootTreeToR-package}
\alias{RootTreeToR}
\docType{package}
\title{
Root Trees, Chains, and Histograms To R
}
\description{
 With RootTreeToR you can pull data from Root Trees, Chains, and
  Histograms into R. You can also write an R data frame into a Root Tree
  file. 
}
\details{
  The specifics of this package are described in other help
  pages. Select the help pages according to the table below (from the
  command line, execute the help command)...

  \tabular{lll}{
    TASK       \tab HELP PAGE   \tab HELP COMMAND \cr
    Read data from a Root tree \tab \code{\link{openRootChain}} \tab
    ?openRootChain \cr
    Read data from a Root chain \tab \code{\link{openRootChain}} \tab
    ?openRootChain \cr
    Read data from a histogram \tab \code{\link{openFileWithHists}} \tab ?openFileWithHists \cr
    Restrict data in a Root chain/tree \tab \code{\link{createEventList}} \tab
    ?createEventList \cr
    Run an arbitrary Root command \tab \code{\link{rootCommand}} \tab
    ?rootCommand \cr
    Write R data to Root \tab \code{\link{writeDFToRoot}} \tab ?writeDFToRoot \cr
  }

  For a list of all commands within this package, do
  \code{library(help="RootTreeToR")}.
}
\author{
Adam Lyon <lyon@fnal.gov> Fermi National Accelerator
Laboratory, Batavia, IL, 60510 USA
}

\keyword{ package }
