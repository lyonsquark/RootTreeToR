#
# Test writeDFToR
#

library(RootTreeToR)

makeRootFile = function() {

  testDF = data.frame(
                       adam = as.integer( c(1,2,3,4,5) ),
                       caryn = c(F,T,T,F,T),
                       pauline = c("New Jersey", "Illinois",
                                              "Florida",
                                              "Michigan", "New York"),
                       tiipaq=c(34.5, 91.4, 10.44, -34.1, 10034.4452)
                     )
  testDF$paulineC = as.character(testDF$pauline)

  writeDFToRoot(testDF)

}


makeRootFile()
    
