/* 
 * File:   ExtAll.cpp
 * Package: amqp_extension
 * Author: S. Hamblett <steve.hamblett@linux.com>
 * Copyright steve@OSCF 2014
 *
 * Created on 20 January 2014, 08:34
 */

#include "ExtAll.h"

Dart_Handle HandleError(Dart_Handle handle) {
    
  if (Dart_IsError(handle)) Dart_PropagateError(handle);
  return handle;
  
}

void throwDartivityException(const char* message) {
    
    /* Create an exception class */
    Dart_Handle className = Dart_NewStringFromCString("DartivityException");
    
    Dart_Handle type = Dart_GetType(Dart_RootLibrary(),
                                    className,
                                    0,
                                    NULL);
    
    Dart_Handle exception = HandleError(Dart_New(type,
                                        Dart_Null(),
                                        0,
                                        NULL));
    
    /* Set the message field */
    Dart_Handle value = Dart_NewStringFromCString(message);
    Dart_Handle name = Dart_NewStringFromCString("message");
    HandleError(Dart_SetField(exception,
                              name,
                              value));
    
    /* Throw the exception */
    Dart_Handle exceptionHandle = Dart_NewUnhandledExceptionError(exception);
    HandleError(Dart_ThrowException(exceptionHandle));
}

