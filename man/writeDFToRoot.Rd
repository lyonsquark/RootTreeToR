\name{writeDFToRoot}
\alias{writeDFToRoot}
\title{ Write a data frame to a Root Tree file }
\description{
  Writes out an \R data frame as a Root Tree to a file.
}
\usage{
writeDFToRoot(df, name = deparse(substitute(df)),
              rootFileName = paste(name, ".root", sep = ""),
              mode = "RECREATE", title = "A Tree")
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{df}{ data frame to write out. }
  \item{name}{ Name of the Root Tree. Default is the data frame name
               provided in the \code{df} argument. }
  \item{rootFileName}{ Name of the Root File to write. Default is
                       \code{name} with .root added to the end. }
  \item{mode}{ Write mode. RECREATE (default) overwrites the file if it
               already exists. NEW will cause the write to fail if the file
               already exists. }
  \item{title}{ The title of the tree. }
}
\details{
  This function allows you to write out an \R data frame as a Root
  Tree file so that others using Root can look at your data. The Root
  Tree is simple, using only base types (ints, doubles, etc.). Note that
  every datum will be on its own branch.
}
\value{
  Nothing is returned. The structure of the data frame is printed to the
  screen.
}
\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{Addm Lyon \email{lyon@fnal.gov} }

\examples{
## Let's make a data frame of, say, electron data from events. We want
## store the event number (an event can have more than one electron),
## the electron charge, and px, py, pz.

## Step 1: How many electrons per event? Let's say we can have a maximum
## of 5 per event. Let's have 50 events. 
ePerEvent = sample(1:5, 50, replace=TRUE)

## Step 2: Build the event numbers. If ePerEvent is 1, 4, 2, ...
## then the event numbers should be 1, 2, 2, 2, 2, 3, 3, ...
## Note that we force this to be an integer
evNums = as.integer( rep.int(1:length(ePerEvent), times=ePerEvent) )

## Step 3: Build the charge. Again, note that we force this to be
## an integer
eCh = as.integer( sample( c(-1,1), length(evNums), replace=T) )

## Step 4: Build px, py, pz (with somewhat arbitrary distributions)
px = rnorm(length(evNums), mean=50, sd=30)
py = rnorm(length(evNums), mean=50, sd=30)
pz = rnorm(length(evNums), mean=90, sd=50)

## Step 5: Make the data frame
exampleEs = data.frame(evNums, eCh, px, py, pz)

## Step 6: Write this out as a Root file
## Since we use the defaults, the tree name will be exampleEs and
## the file name will be exampleEs.root
writeDFToRoot(exampleEs, title="Example electron data")

## This example is used to write the sample root file for this
## RootTreeToR package.
}

\keyword{ manip }

