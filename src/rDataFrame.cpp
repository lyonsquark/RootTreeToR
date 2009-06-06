/*
 *  rDataFrame.cpp
 *  RootTreeToR
 *
 *  Created by Adam Lyon on 9/25/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.    
 */

#include "rDataFrame.h"

//////////////////////////////////////////////////////////
// Implementation of RDataFrameColumn
void RDataFrameColumn::resizeColumn(unsigned int newSize)
{
  // Resize the column
  REPROTECT( SET_LENGTH( m_column, newSize ), m_protectIndex );
}

RDataFrameColumn::~RDataFrameColumn()
{
  if ( beVerbose() ) REprintf("RDataFrame - Deleting column %s\n", 
			      name().c_str() );
	
  // Unprotect our vector
  UNPROTECT(1);
}
//////////////////////////////////////////////////////////
// Implementation of RDataFrameIntegerColumn
void RDataFrameIntegerColumn::makeColumn(unsigned int initialSize)
{	
  PROTECT_WITH_INDEX( m_column = allocVector( INTSXP, initialSize ), 
		      &m_protectIndex );

  if ( beVerbose() ) REprintf("RDataFrame - Creating integer column %s\n", 
			      name().c_str());

}

template<class T> 
void RDataFrameIntegerColumn::set(T value)
{
  throw RDataFrameColumn_WrongTypeException( name() + " must be integer type" );
}

template<>
void RDataFrameIntegerColumn::set(int value)
{
  m_value = value;
  m_isSet = true;
}

template<>
void RDataFrameIntegerColumn::set(unsigned int value)
{ set( static_cast<int>(value) ); }

template<>
void RDataFrameIntegerColumn::set(long value)
{ set( static_cast<int>(value) ); }

template<>
void RDataFrameIntegerColumn::set(long long value)
{ set( static_cast<int>(value) ); }

template<>
void RDataFrameIntegerColumn::set(unsigned long value)
{ set( static_cast<int>(value) ); }

void RDataFrameIntegerColumn::setNA()
{
  m_value = NA_INTEGER;
  m_isSet = true;
}

void RDataFrameIntegerColumn::commitRow(unsigned int row)
{
  if ( ! m_isSet ) throw RDataFrameColumn_NotSetException( name() );
	
  INTEGER( m_column )[row] = m_value;
	
  m_isSet = false;
}

//////////////////////////////////////////////////////////
// Implementation of RDataFrameRealColumn
void RDataFrameRealColumn::makeColumn(unsigned int initialSize)
{
  PROTECT_WITH_INDEX( m_column = allocVector( REALSXP, initialSize ), 
		      &m_protectIndex );

  if ( beVerbose() ) REprintf("RDataFrame - Creating real column %s\n", 
			      name().c_str());		
}

template<class T> 
void RDataFrameRealColumn::set(T value)
{
  throw RDataFrameColumn_WrongTypeException( name() + " must be real type" );
}

template<>
void RDataFrameRealColumn::set(double value)
{
  m_value = value;
  m_isSet = true;
}

template<>
void RDataFrameRealColumn::set(float value)
{ set( static_cast<double>(value) ); }

void RDataFrameRealColumn::setNA()
{
  m_value = NA_REAL;
  m_isSet = true;
}

void RDataFrameRealColumn::commitRow(unsigned int row)
{
  if ( ! m_isSet ) throw RDataFrameColumn_NotSetException( name() );
	
  REAL( m_column )[row] = m_value;
	
  m_isSet = false;
}

//////////////////////////////////////////////////////////
// Implementation of RDataFrameStringColumn
void RDataFrameStringColumn::makeColumn(unsigned int initialSize)
{
  if ( beVerbose() ) REprintf("RDataFrame - Creating string column %s\n", 
			      name().c_str());
	
  PROTECT_WITH_INDEX( m_column = allocVector( STRSXP, initialSize ), 
		      &m_protectIndex );
}

template<class T> 
void RDataFrameStringColumn::set(T value)
{
  throw RDataFrameColumn_WrongTypeException( name() + " must be string type" );
}

template<>
void RDataFrameStringColumn::set(std::string value)
{
  m_value = value;
  m_isSet = true;
}

template<>
void RDataFrameStringColumn::set(const char* value)
{ set( std::string(value) ); }

void RDataFrameStringColumn::setNA()
{
  m_value = std::string("NA");
  m_isSet = true;
}

void RDataFrameStringColumn::commitRow(unsigned int row)
{
  if ( ! m_isSet ) throw RDataFrameColumn_NotSetException( name() );

  if ( m_value != std::string("NA") ) {
    SET_ELEMENT( m_column, row, mkChar(m_value.c_str()) );
  }
  else {
    SET_ELEMENT( m_column, row, NA_STRING );
  }
	
  m_isSet = false;
}

//////////////////////////////////////////////////////////
// Implementation of RDataFrameLogicalColumn
void RDataFrameLogicalColumn::makeColumn(unsigned int initialSize)
{
  if ( beVerbose() ) REprintf("RDataFrame - Creating logical column %s\n", 
			      name().c_str());
		
  PROTECT_WITH_INDEX( m_column = allocVector( LGLSXP, initialSize ), 
		      &m_protectIndex );
}

template<class T> 
void RDataFrameLogicalColumn::set(T value)
{
  throw RDataFrameColumn_WrongTypeException( name() + " must be bool type" );
}

template<>
void RDataFrameLogicalColumn::set(bool value)
{
  m_value = value;
  m_isSet = true;
}

void RDataFrameLogicalColumn::commitRow(unsigned int row)
{
  if ( ! m_isSet ) throw RDataFrameColumn_NotSetException( name() );
	
  LOGICAL( m_column )[row] = m_value;
	
  m_isSet = false;
}


////////////////////////////////////////////////
////////////////////////////////////////////////
// RDataFrame implementation
RDataFrame::RDataFrame(unsigned int initialSize, float growthFactor,
		       bool verbose) :
  m_reservedSize(initialSize),
  m_currentSize(0),
  m_growthFactor(growthFactor),
  m_verbose(verbose),
  m_nCols(0),
  m_columns()
{
  if ( initialSize == 0 ) throw RDataFrameException("intialSize must be > 0");
  if ( growthFactor <= 1.5 ) throw RDataFrameException("growthFactor must be > 1.5");
	
  if ( m_verbose ) {
    REprintf("RDataFrame::ctor - Creating RDataFrame with size %d, growth %f\n", 
	     m_reservedSize, m_growthFactor);
  }
};

template <class T>
T* RDataFrame::addColumn(std::string name)
{
  // If we've already committed some data, can't do this
  if ( m_currentSize > 0 ) throw RDataFrameException("Cannot add more columns after a commit");
	
  // Make the column
  T* column = new T(name, m_nCols, m_reservedSize, m_verbose);
	
  // Increase the order number
  m_nCols++;
	
  // Save the column away
  m_columns.push_back(column);
	
  // Return the new column
  return column;
}

RDataFrameIntegerColumn* RDataFrame::addIntegerColumn(std::string name) 
{
  return addColumn<RDataFrameIntegerColumn>(name);
}

RDataFrameRealColumn* RDataFrame::addRealColumn(std::string name) 
{
  return addColumn<RDataFrameRealColumn>(name);
}

RDataFrameStringColumn* RDataFrame::addStringColumn(std::string name) 
{
  return addColumn<RDataFrameStringColumn>(name);
}

RDataFrameLogicalColumn* RDataFrame::addLogicalColumn(std::string name) 
{
  return addColumn<RDataFrameLogicalColumn>(name);
}


void RDataFrame::resizeVectors()
{
  // Deterine new size
  m_reservedSize = (int)(m_reservedSize * m_growthFactor);

  if ( beVerbose() ) {
    REprintf("RDataFrame::resizeVectors - Reached size of %d, new size %d\n",
	     m_currentSize, m_reservedSize);
  }
	
  // Grow the vectors
  for_each( m_columns.begin(), m_columns.end(), resizeOnColumn(m_reservedSize) );
}

void RDataFrame::commitRow()
{
  // Error if there are no columns defined
  if ( m_nCols == 0 ) throw RDataFrameException("There are no columns defined to commit");
		
  // If our size has exceeded reserved size, then grow the vectors
  if ( m_currentSize >= m_reservedSize) resizeVectors();
		
  // All is well, commit the row
  for_each( m_columns.begin(), m_columns.end(), commitRowOnColumn(m_currentSize) );
	
  // Grow the size
  m_currentSize++;
}

SEXP RDataFrame::dataFrameInAnsForm() 
{
  if ( m_nCols == 0 ) throw RDataFrameException("There are no columns defined");
		
  // Make the data frame structure
  // Create the "answer" list
  SEXP ans;
  PROTECT( ans = NEW_LIST(3) );
	
  // Set the elements
  SET_ELEMENT(ans, 0, NEW_LIST(m_nCols)); // The data
  SET_ELEMENT(ans, 1, NEW_STRING(m_nCols)); // The column names
  SET_ELEMENT(ans, 2, NEW_INTEGER(1)); // The true number of rows
	 
  // Set the data and name columns
  SEXP data = VECTOR_ELT(ans, 0);
  SEXP data_names = VECTOR_ELT(ans, 1);
	
  // Fill these lists
  for ( unsigned int i=0; i < m_nCols; ++i )
  {
    SET_ELEMENT( data, i, m_columns[i]->column() );
    SET_ELEMENT( data_names, i, mkChar( m_columns[i]->name().c_str() ) );
  }
  
  SET_NAMES(data, data_names);
	
  // Set the number of rows
  INTEGER( VECTOR_ELT(ans, 2) )[0] = m_currentSize;
	
  if ( beVerbose() )
  { 
    REprintf("RDataFrame - Saved %d rows\n", m_currentSize);
  }
  
  // No longer need protection from the garbage collector
  UNPROTECT(1);
 	
  return ans;
}

// Destructor
RDataFrame::~RDataFrame()
{
  // Remove the columns vector and its contents -- for some reason,
  //   vector.clear doesn't work
  for ( unsigned int i = 0; i < m_columns.size(); ++i )
    delete m_columns[i];
}
