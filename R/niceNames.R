#### niceNames.R
niceNames = function(df) {
  # Replace data frame names with nice names
  # Remove parentheses and underscores
  # Only take the last part of the name
  #  e.g. MCpart._particle -> particle
  #       MCpart.getPMCvtx().nparents -> nparents
  #
  # Usage: names(df) = niceNames(df)

  # Get the current names of the data frame columns
  n = names(df)
  
  # Remove parentheses and underscores
  n = gsub("()", "", n, fixed=T)
  n = gsub("_", "", n, fixed=T)

  # For multipart names, take the last part
  parts = strsplit(n, ".", fixed=T)

  lastNames = sapply(parts, function(x) x[[length(x)]])

  lastNames
}
