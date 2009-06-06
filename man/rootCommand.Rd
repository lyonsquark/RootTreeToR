\name{rootCommand}
\alias{rootCommand}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Run an arbitrary Root command }
\description{
  Run an arbitrary Root command from \R. 
}
\usage{
rootCommand(comm)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{comm}{ The Root command to perform (string). }
}
\details{
  This function allows you to issue an arbitary Root command. An example
  use case is to load libraries necessary for opening a particular Root
  Tree file. You are actually controlling the Root session - so
  variables you set will persist from one call to the next. If you mess
  things up, you will probably have to quit \R and start over. 

  Not all commands will work. Graphics commands seem to fail. Issuing
  ".q" will make you very sad. Use caution, since crashing the Root
  session will most certainly crash \R too.

  Anything quoted in the Root command should use double quotes. Thus,
  you should use the \R single quotes for the string itself. See the
  examples.

  This command is not meant for heavy-duty Root stuff, but rather to get
  information and to set up libraries and such. 
}
\value{
  Nothing is returned. Output from the Root command should appear
  in the console. You cannot easily capture the output into \R.
}

\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{Addm Lyon \email{lyon@fnal.gov} }
\examples{
## Let's determine the name of the tree in the sample Root file.
## Use system.file to get the path to the sample Root file that comes
## with RootTreeToR.
##  Note the use of single and double quotes. Strings within the Root
##  command must be double-quoted.
exFile = system.file("examples/exampleEs.root", package="RootTreeToR")
rootCommand( paste('TFile a("', exFile, '")', sep="") )

\dontrun{
## You would probably do something like...
rootCommand( 'TFile a("myFile.root")' )
}

## Get the tree name and close it
rootCommand('a->ls() ; a->Close()')

## It's called exampleEs!

\dontrun{
## If you are trying to read a D0 CAF Tree (a very fancy Tree used by
## the Fermilab D0 experiment), you would need to issue something
## similar to the following BEFORE you opened any Root files...
rootCommand(".L lib/libmet_util.so")
rootCommand(".L lib/libtmb_tree.so")
rootCommand(".L lib/libcafe.so")
rootCommand(".L lib/libcafe_sam.so")
## The above commands load the necessary libraries into Root.
}
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ manip }
