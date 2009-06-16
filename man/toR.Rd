\name{toR}
\alias{toR}
\title{ Load data from a Root Chain into \R}
\description{
  Select data from a Root Chain and load into an \R data frame.
}
\usage{
toR(rootChain, columns, selection = "", nEntries = 100,
    firstEntry = 0, initialSize = 1000, growthFactor = 1.7, prefix = "")
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{rootChain}{ The \code{\link{RootChain}} object obtained with
    \code{\link{openRootChain}}.}
  \item{columns}{ A Root Tree column specification, either as a string
                 or a character vector of Root variable names. }
  \item{selection}{ A Root Tree selection specification to restrict the Tree. Default is no selection.}
  \item{nEntries}{ The number of Root Entries to load (defaults to 100) If zero or less, do 99,999,999 entries. }
  \item{firstEntry}{ The first entry to load (defaults to 0, which means
                     to start at the beginning).}
  \item{initialSize}{ The initial guess for the number of rows in the
                      resulting \R data frame.}
  \item{growthFactor}{ If the guess for the number of rows in the
                       resulting \R data frame is too low, expand
		       by this factor.}
  \item{prefix}{ A prefix to attach to each item in the \code{columns}
    argument. The "." is added automatically.}
}
\details{
  This \code{toR} function brings data from the Root Chain into \R.
  Remember that \R holds data in memory, so what you bring it cannot be
  too big. But for large Root Trees, you typically don't look at the
  whole thing at once. So bring in the columns you need, process the
  resulting data frame, remove that processed data, and then
  bring in more as needed.

  The \code{columns} argument gives the list of Root Tree variables you
  want to read into \R. Any valid Root variable that can go into
  \code{TTree::Draw} or \code{TTree:Scan} can be used. See the examples
  below for samples. You can
  list variables in a Root format (e.g. \code{"Em.pt:Em.py:Em.pz"}) or
  as an \R
  character vector, \code{c("Em.pt", "Em.px", "Em.py")}. If you've also loaded
  in a library (see \code{\link{rootCommand}}) that gives functions for
  accessing the Tree, you can use
  those functions too, \code{c("Global.evtno()", "Global.instlum()")}. Note
  that accessing information via functions is significantly slower for
  Root than accessing the raw data. Functions with complicated arguments
  may not work here. In that case you may need to write C++ code and use
  \code{\link{toRUser}}. If all of the variables come from the same branch,
  then instead of repeating the branch name, you can give it in the
  \code{prefix} argument and it will be automatically applied. For
  example, \code{columns=c("pt", "px", "py"), prefix="Em"}. Note that
  the "." is added for you.

  The \code{selection} argument can take any valid Root selection string
  for restricting the data to load. For example, "Em.pt>50".

  The arguments \code{nEntries} and \code{firstEntry} specify how many
  entries to load and where to start respectively. Note that the
  defaults are set low so you can try out the command with a small
  sample. Loading large amounts of data can take a significant amount of
  time, but note that almost all of that time is processing in Root. The
  overhead from \R is minimal. If you set the \code{verbose} option of
  \code{\link{openRootChain}} to TRUE, then you will see output at
  various stages of loading so you can see that things are working.

  The arguments \code{initialSize} and \code{growthFactor} involve the
  size of the resulting data frame. \R needs to know the size of the
  data frame (in number of rows) when the loading starts, but typically
  that information is not known. Instead of doing two passes over the
  data (one to determine the size and another to fill), \R sets the data
  frame size initially to \code{initialSize} rows. If \R needs more
  space, it will expand the data frame by the \code{growthFactor}. At
  the end of the load, the size of the data frame is adjusted to remove
  any unused rows. Setting \code{intialSize} too low means that you will
  need to resize often. Resizing is an expensive operation, but
  typically is fast compared to the speed of Root reading the data. So
  you can typically leave it as the defaults and you will be ok. If you
  know you will be returning a large data frame, it can't hurt to set
  \code{initialSize} to something larger.

  See the value section below for a description of the returned Data
  Frame.

  See the tips section too (below).

}

\value{ This function returns a data frame with the information
requested. You will get a column for each item in the \code{columns}
argument. You will also get three or four extra columns. The
\code{globalEntry} column is the Root entry number for that particular
row in the chain starting at 0 and going up to the total number of
entries in the chain. These values are unique. The \code{localEntry}
column specifies the entry number in a particular Tree, with the
\code{treeNum} column specifying the Tree. Both of these values start at
zero. The \code{treeNum} of zero is from the first file listed in the
\code{\link{openRootChain}} call; \code{treeNum} of 1 is the second file
listed, and so on.

See the Tips section below for an explanation of the \code{idx} column.

The data frame column names may be inconvenient to use. A function for
renaming them is provided in \code{\link{niceNames}}.

}

\section{Tips}{

  See
  \url{http://root.cern.ch/root/html/TTreePlayer.html#TTreePlayer:DrawSelect}
  for variables and formulas you can put in the \code{columns}
  argument. Note to ignore references to making histograms. 
  
  Root trees are arranged by "entries". Typically, an entry is an
  HEP event. Some quantities in the entry may have more than one
  value. For example, you may have a Root Tree of electron information,
  and so a Root entry may have more than one electron. Data Frames have
  a flat structure, so this hierarchy of entries is flattened. The
  columns \code{globalEntry}, \code{localEntry}, and \code{idx} help you
  to reconstruct the hierarchy of the events. For example, an event with
  four electrons will make a data frame where those four rows will have
  the same \code{globalEntry}/\code{localEntry} numbers and the
  \code{idx} value will go from 1 to 4. You should take care not to mix
  different multiple object types in the same \code{toR} call. For
  example, do not load electrons and muons (where each event can have a
  different number of electrons and muons). You'll get a result, but
  you'll be very confused. Instead, use separate \code{toR} calls, or
  write your own code and use \code{toRUser}.

  If verbosity is set, you will get a message when the Tree is expanded
  and when you cross into a new Tree file in the chain. If trace is set,
  you will see lots of debugging information. WARNING: The amount of
  debugging output can be large.  Use \code{\link{setVerbose}},
  \code{\link{setTrace}} to change these settings.

}

\section{DISCLAIMER}{Although the author is unaware of mistakes in
  importing Root information into \R, YOU are responsible for checking
  your results with Root. There will be minor differences due to round
  off errors. There may be major differences due to bugs. You should
  repeat parts of your analysis in Root and compare to \R. If you find
  problems, please inform the author, who assumes absolutely no
  responsibility or liability as to the correctness of your results when
  you use this package.}

\references{For more information on \emph{Root} see \url{http://root.cern.ch}.
Root is written by Rene Brun and Fons Rademakers. }
\author{ Adam L. Lyon }

\seealso{ \code{\link{openRootChain}}, \code{\link{toRUser}},
  \code{\link{niceNames}}, \code{\link{setVerbose}},
  \code{\link{setTrace}}, \code{\link{RootChain}} }

\examples{
## Let's load the sample Root file that comes with this package
exFile = system.file("examples/exampleEs.root", package="RootTreeToR")
rt = openRootChain("exampleEs", exFile)

## Note that for this case, each branch has only one leaf, so we can
## just use the branch names.
df = toR(rt, c("evNums", "eCh", "px", "py", "pz"))
df$pt=with(df, sqrt(px*px+py*py))
head(df)

## Note we can use any valid TTree variable
df1 = toR(rt, c("evNums", "eCh", "sqrt(px*px+py*py)"))
head(df1)

## We can do restrictions too, using TTree selection string
df2 = toR(rt, c("evNums", "eCh", "px", "py"), "sqrt(px*px+py*py)>100")
df2$pt = with(df2, sqrt(px*px+py*py))
head(df2)

## Though not needed here, we could give the branch names
df3 = toR(rt, c("evNums.evNums", "eCh.eCh"))
head(df3)
## This leads to ugly column names (like evNums.evNums)
names(df3)
## Use niceNames to clean them up
names(df3) = niceNames(df3)
names(df3)
}

% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{  manip }

