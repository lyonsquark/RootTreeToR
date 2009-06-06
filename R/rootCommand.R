#######
## rootCommand

rootCommand <- function(comm) {
  if ( ! is.character(comm) ) stop("comm must be a character")
  
  .Call("rootCommand", comm, PACKAGE="RootTreeToR")
}
