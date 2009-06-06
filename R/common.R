##### 
## R Routines common to RootChain.R and EventList.R


########
## .assertClass -- Make sure the class passed is what it should be
.assertClass <- function(ptr, class) {
  if ( class %in% is(ptr) )
    invisible(ptr)
  else
    stop(paste("ptr is not a member of class", class))
}
