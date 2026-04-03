#ifndef KEYWORDS_H

enum keywords {
  // primitive types
  INT, INT8, INT16, INT32, INT64, INT128,
  FLOAT, FLOAT16, FLOAT32, FLOAT64, FLOAT128,
  CHAR,
  STR,
  PTR,
  BOOL, TRUE, FALSE,

  // compound types
  ARR, TUP,

  // immutability modifiers
  CONST, VAR,

  // functions
  FN, RETURN,
  
  // control flow
  IF, ELSE,
    
  // loops
  WHILE, FOR,

  // add result, adt, generics, pattern matching, structures, imports and error handling later on
};

#define KEYWORDS_H
#endif
