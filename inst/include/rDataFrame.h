#ifndef __RDataFrame__
#define __RDataFrame__

/*
 *  rDataFrame.h
 *  RootTreeToR
 *
 *  Created by Adam Lyon on 9/25/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

// Represent an R Data Frame for "idiot" proof declaring and filling

extern "C"
{
#include <R.h>
#include <Rdefines.h>
}

#include <string>
#include <vector>
#include <algorithm>
#include <functional>


/////////////////////////////////////////////////////////////
// Exceptions
struct RDataFrameException
{
RDataFrameException(std::string what) : m_what(what) {}
  std::string what() { return m_what; }
  std::string m_what;
};

struct RDataFrameColumn_WrongTypeException : public RDataFrameException
{
 RDataFrameColumn_WrongTypeException( std::string what ) :
  RDataFrameException(what) {}
};

struct RDataFrameColumn_NotSetException : public RDataFrameException
{
 RDataFrameColumn_NotSetException( std::string name ) :
  RDataFrameException(name + "was not set before commit") {}
};

	
/////////////////////////////////////////////////////////////
// RDataFrameColumn
// Represent columns in the data frame
// An ABSTRACT base class
class RDataFrameColumn
{
 public:

 RDataFrameColumn(const std::string name, unsigned int order, 
		  bool verbose) :
  m_name(name),
    m_order(order),
    m_isSet(false),
    m_verbose(verbose)
    {};
	
  unsigned int order() const { return m_order; } 
  const std::string name() const { return m_name; }
	
  virtual void makeColumn(unsigned int initialSize) = 0;
  void resizeColumn(unsigned int newSize);
  virtual void commitRow(unsigned int row) = 0;
	
  SEXP column() { return m_column; }
	
  bool beVerbose() const { return m_verbose; }
	
  virtual ~RDataFrameColumn();
	
  // The set function would be virtual, except that it's templated
	
 protected:
  SEXP m_column;
  PROTECT_INDEX m_protectIndex;
  bool m_isSet;
		
 private:
  const std::string m_name;   // Name of this column
  unsigned int m_order; // Order of this column in the data frame
  bool m_verbose;
};

/////////////////////////////////////////////////////////////
// RDataFrameIntegerColumn
class RDataFrameIntegerColumn : public RDataFrameColumn
{
 public:
	
 RDataFrameIntegerColumn(const std::string name, unsigned int order, 
			 unsigned int initialSize, bool verbose) :
  RDataFrameColumn(name, order, verbose)
    { makeColumn(initialSize); }
	
  template<class T>
    void set(T value);
	
  void setNA();
	
  void makeColumn(unsigned int initialSize);
  void commitRow(unsigned int row);

  virtual ~RDataFrameIntegerColumn() {};
	
 private:
  int m_value;
	
};	

/////////////////////////////////////////////////////////////
// RDataFrameRealColumn
class RDataFrameRealColumn : public RDataFrameColumn
{
 public:
	
 RDataFrameRealColumn(const std::string name, unsigned int order, 
		      unsigned int initialSize, bool verbose) :
  RDataFrameColumn(name, order, verbose)
    { makeColumn(initialSize); }
	
	
  template<class T>
    void set(T value);
	
  void setNA();
	
  void makeColumn(unsigned int initialSize);
  void commitRow(unsigned int row);

  virtual ~RDataFrameRealColumn() {};
	
 private:
  double m_value;
};	

/////////////////////////////////////////////////////////////
// RDataFrameStringColumn
class RDataFrameStringColumn : public RDataFrameColumn
{
 public:
	
 RDataFrameStringColumn(const std::string name, unsigned int order, 
			unsigned int initialSize, bool verbose) :
  RDataFrameColumn(name, order, verbose)
    { makeColumn(initialSize); }
	
	
  template<class T>
    void set(T value);
	
  void setNA();
	
  void makeColumn(unsigned int initialSize);
  void commitRow(unsigned int row);
	
  virtual ~RDataFrameStringColumn() {};
	
 private:
  std::string m_value;
};	

/////////////////////////////////////////////////////////////
// RDataFrameLogicalColumn
class RDataFrameLogicalColumn : public RDataFrameColumn
{
 public:
	
 RDataFrameLogicalColumn(const std::string name, unsigned int order, 
			 unsigned int initialSize, bool verbose) :
  RDataFrameColumn(name, order, verbose)
    { makeColumn(initialSize); }
	
	
  template<class T>
    void set(T value);
	
  void setNA();
	
  void makeColumn(unsigned int initialSize);
  void commitRow(unsigned int row);
	
  virtual ~RDataFrameLogicalColumn() {};
	
 private:
  bool m_value;
};	

/////////////////////////////////////////////////////////
// RDataFrame
class RDataFrame
{
	
 public:
  RDataFrame(unsigned int initialSize, float growthFactor, 
	     bool verbose=true); 	
	
  RDataFrameIntegerColumn* addIntegerColumn(std::string name);
  RDataFrameRealColumn* addRealColumn(std::string name);
  RDataFrameStringColumn* addStringColumn(std::string name);
  RDataFrameLogicalColumn* addLogicalColumn(std::string name);
	
  void commitRow(); // Commit a row
	
  SEXP dataFrameInAnsForm(); // Get back the data frame
	
  bool beVerbose() const { return m_verbose; }
	
  virtual ~RDataFrame();
	
 private:
		
  // An easy way to add columns
  template <class T>
    T* addColumn(std::string name);
	
  void createColumns(); // Make the data frame itself
  void resizeVectors(); // Resize the vectors
	
  unsigned int m_reservedSize;
  unsigned int m_currentSize;

  float m_growthFactor;
  bool m_verbose;
  unsigned int m_nCols;
	
  std::vector<RDataFrameColumn*> m_columns;	// RDataFrame owns this list and
  // contents

  struct commitRowOnColumn : public std::unary_function<RDataFrameColumn*, void>
  {
  commitRowOnColumn(unsigned int row) : m_row(row) {}
    void operator()(RDataFrameColumn* column) { column->commitRow(m_row); }
    unsigned int m_row;
  };
	
  struct resizeOnColumn : public std::unary_function<RDataFrameColumn*, void>
  {
  resizeOnColumn(unsigned int newSize) : m_newSize(newSize) {}
    void operator()(RDataFrameColumn* column) {
      column->resizeColumn(m_newSize); }
    unsigned int m_newSize;
  };
	
};

#endif
