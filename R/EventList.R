############
## EventList.R
##
##  S4 Class for an EventList#

## nEntries becomes a generic function
nEntries <- function(object) stop("Called without a registered object")
setGeneric("nEntries")

setClass("EventList",
         representation(ptr="externalptr")
         )

############
## Show
setMethod("show", "EventList",
          function(object) {
            cat("EventList", getName(object), getDetails(object), 
                "with", nEntries(object), "Entries\n")
          }
          )

##########
## getEventList -- Get the event list from Root
getEventList <- function(name, manageItsMemory=F) {
  
  if ( !is.character(name) ) stop ("name must be a character")
  if ( !is.logical(manageItsMemory) ) stop("manageItsMemory must be logical")
  
  ptr = .Call("getEventListWithName", name, manageItsMemory, 
    PACKAGE="RootTreeToR")
  
  new("EventList", ptr=ptr)
}

############
## createEventList -- make a new event list
createEventList <- function(name, title, entryNums) {
  
  if ( !is.character(name) ) stop ("name must be a character")
  if ( !is.character(title) ) stop ("title must be a character")
  if ( !is.numeric(entryNums) ) stop ("entryNums must be an integer vector")
  
  ptr = .Call("newEventList", name, title, as.integer(entryNums), 
    PACKAGE="RootTreeToR")
  
  new("EventList", ptr=ptr)
}

################
## getName - Get the name of this eventList
getName <- function(eventList) {
  .assertClass(eventList, "EventList")
  
  .Call("getName", eventList@ptr,
        PACKAGE="RootTreeToR")
}

################
## getDetails - Get the title of the eventList
getDetails <- function(eventList) {
  .assertClass(eventList, "EventList")
  
  .Call("getTitle", eventList@ptr,
        PACKAGE="RootTreeToR")
}

################
## nEntries - Get the number of entries in the event list
setMethod("nEntries", "EventList",
          function(object) {
            
            .Call("nEntriesEventList", object@ptr, 
                  PACKAGE="RootTreeToR")
          } 
          )

################
## getEntries - Return a vector of the passed entry numbers
getEntries <- function(eventList) {
  .assertClass(eventList, "EventList")
  
  .Call("getEntries", eventList@ptr,
        PACKAGE="RootTreeToR")
}

################
## clear - Clear an event list
clear <- function(eventList) {
  .assertClass(eventList, "EventList")
  
  .Call("clear", eventList@ptr,
        PACKAGE="RootTreeToR")
  
  invisible()
}

################
## isInEventList -- return a logical vector
isInEventList <- function(eventList, entryNums) {
  .assertClass(eventList, "EventList")
  
  if ( !is.numeric(entryNums) ) stop ("entryNums must be an integer vector")
  
  .Call("isInEventList", eventList@ptr, as.integer(entryNums),
        PACKAGE="RootTreeToR")
}

################
## putIntoEventList -- put entries into the event list
putIntoEventList <- function(eventList, entryNums) {
  .assertClass(eventList, "EventList")
  
  if ( !is.numeric(entryNums) ) stop ("entryNums must be an integer vector")
  
  .Call("putIntoEventList", eventList@ptr, as.integer(entryNums),
        PACKAGE="RootTreeToR")
  
  invisible()
}

#################
## addEventLists -- add eventList2 to eventList1
addEventLists <- function(eventList1, eventList2) {
  .assertClass(eventList1, "EventList")
  .assertClass(eventList2, "EventList")
  
  .Call("addEventLists", eventList1@ptr, eventList2@ptr, 
        PACKAGE="RootTreeToR")
  
  invisible()
}

#################
## subtractEventLists -- subtract eventList2 from eventList1
subtractEventLists <- function(eventList1, eventList2) {
  .assertClass(eventList1, "EventList")
  .assertClass(eventList2, "EventList")
  
  .Call("subtractEventLists", eventList1@ptr, eventList2@ptr, 
        PACKAGE="RootTreeToR")
  
  invisible()
}

#################
# intersectEventLists -- intersect eventList2 with eventList1
#intersectEventLists <- function(eventList1, eventList2) {
#	.assertClass(eventList1, "EventList")
#	.assertClass(eventList2, "EventList")
#	
#	.Call("intersectEventLists", eventList1@ptr, eventList2@ptr, 
#		    PACKAGE="RootTreeToR")
#
#  invisible()
#}
