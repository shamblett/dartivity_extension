/* 
 * File:   ExtAll.h
 * Package: dartivity_extension
 * Author: Steve Hamblett
 * Copyright Steve Hamblett 2015
 *
 * Created on 9 September 2015, 08:06
 */

#ifndef EXTALL_H
#define	EXTALL_H

/* General includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include <condition_variable>
#include <ostream>

/* Iotivity includes */
#include "OCPlatform.h"
#include "OCApi.h"

using namespace OC;


/* Dart includes */
#include "include/dart_native_api.h"

/**
 * General library wide native functions
 */
Dart_NativeFunction ResolveName(Dart_Handle name,
        int argc,
        bool* auto_setup_scope);

/**
 * General library wide Dart handle checker
 */
Dart_Handle HandleError(Dart_Handle handle);


#endif	/* EXTALL_H */

