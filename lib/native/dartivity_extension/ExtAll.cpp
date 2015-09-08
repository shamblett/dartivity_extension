/* 
 * File:   ExtAll.cpp
 * Package: dartivity_extension
 * Author: S. Hamblett <steve.hamblett@linux.com>
 * Copyright S. Hamblett 2015
 *
 * Created on 8 September 2015, 08:34
 */

#include "ExtAll.h"

Dart_Handle HandleError(Dart_Handle handle) {
    
  if (Dart_IsError(handle)) Dart_PropagateError(handle);
  return handle;
  
}

