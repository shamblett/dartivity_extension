/* 
 * File:   ExtAll.cpp
 * Package: dartivity_extension
 * Author: S. Hamblett <steve.hamblett@linux.com>
 * Copyright S. Hamblett 2015
 *
 * Created on 8 September 2015, 08:34
 */

#include "ExtPlatform.h"

void wrappedPlatformClass(Dart_Port dest_port_id,
                        Dart_CObject* message) {
  Dart_Port reply_port_id = ILLEGAL_PORT;
  if (message->type == Dart_CObject_kArray &&
      3 == message->value.as_array.length) {
    // Use .as_array and .as_int32 to access the data in the Dart_CObject.
    Dart_CObject* param0 = message->value.as_array.values[0];
    Dart_CObject* param1 = message->value.as_array.values[1];
    Dart_CObject* param2 = message->value.as_array.values[2];
    if (param0->type == Dart_CObject_kInt32 &&
        param1->type == Dart_CObject_kInt32 &&
        param2->type == Dart_CObject_kSendPort) {
      int seed = param0->value.as_int32;
      int length = param1->value.as_int32;
      reply_port_id = param2->value.as_send_port.id;
      uint8_t* values = randomArray(seed, length);

      if (values != NULL) {
        Dart_CObject result;
        result.type = Dart_CObject_kTypedData;
        result.value.as_typed_data.type = Dart_TypedData_kUint8;
        result.value.as_typed_data.values = values;
        result.value.as_typed_data.length = length;
        Dart_PostCObject(reply_port_id, &result);
        free(values);
        // It is OK that result is destroyed when function exits.
        // Dart_PostCObject has copied its data.
        return;
      }
    }
  }
  Dart_CObject result;
  result.type = Dart_CObject_kNull;
  Dart_PostCObject(reply_port_id, &result);
}

void platformServicePort(Dart_NativeArguments arguments) {
  Dart_EnterScope();
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_Port service_port;// =
     Dart_NewNativePort("PlatformClass", wrappedPlatformClass, true);
 if (service_port != ILLEGAL_PORT) {
    Dart_Handle send_port = HandleError(Dart_NewSendPort(service_port));
    Dart_SetReturnValue(arguments, send_port);
  }
  Dart_ExitScope();
}
