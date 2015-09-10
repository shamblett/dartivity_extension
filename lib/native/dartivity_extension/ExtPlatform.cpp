/* 
 * File:   ExtAll.cpp
 * Package: dartivity_extension
 * Author: S. Hamblett <steve.hamblett@linux.com>
 * Copyright S. Hamblett 2015
 *
 * Created on 8 September 2015, 08:34
 */

#include "ExtPlatform.h"

static std::string dbFile;

static FILE* client_open(const char* /*path*/, const char *mode) {
    return fopen(dbFile.c_str(), mode);
}

void wrappedPlatformCfg(Dart_Port dest_port_id,
        Dart_CObject* message) {
    Dart_CObject* param8 = message->value.as_array.values[8];
    Dart_Port reply_port_id  = param8->value.as_send_port.id;
    if (message->type == Dart_CObject_kArray &&
            9 == message->value.as_array.length) {
        // Use .as_array and .as_int64 to access the data in the Dart_CObject.
        Dart_CObject* param0 = message->value.as_array.values[0];
        Dart_CObject* param1 = message->value.as_array.values[1];
        Dart_CObject* param2 = message->value.as_array.values[2];
        Dart_CObject* param5 = message->value.as_array.values[5];
        Dart_CObject* param6 = message->value.as_array.values[6];
        Dart_CObject* param7 = message->value.as_array.values[7];
       

        if (/*param0->type == Dart_CObject_kInt64 &&
               param1->type == Dart_CObject_kInt64 &&
                param2->type == Dart_CObject_kInt64 &&
                param5->type == Dart_CObject_kString &&
                param6->type == Dart_CObject_kInt64 &&
                param7->type == Dart_CObject_kString &&*/
                param8->type == Dart_CObject_kSendPort) {
            int service = param0->value.as_int64;
            int mode = param1->value.as_int64;
            int qos = param2->value.as_int64;
            std::string ip = std::string(param5->value.as_string);
            int port = param6->value.as_int64;
            dbFile = std::string(param7->value.as_string);
           

            OCPersistentStorage ps{client_open, fread, fwrite, fclose, unlink};

            PlatformConfig cfg{
                (OC::ServiceType)service,
                (OC::ModeType)mode,
                ip,
                (uint16_t) port,
                (OC::QualityOfService)qos,
                &ps
            };
            OCPlatform::Configure(cfg);
            
            Dart_CObject result;
            result.type = Dart_CObject_kBool;
            result.value.as_bool = true;
            Dart_PostCObject(reply_port_id, &result);

            // It is OK that result is destroyed when function exits.
            // Dart_PostCObject has copied its data.
            return;
        }
    }

    Dart_CObject result;
    result.type = Dart_CObject_kNull;
    Dart_PostCObject(reply_port_id, &result);
}

void platformServicePort(Dart_NativeArguments arguments) {
    Dart_EnterScope();
    Dart_SetReturnValue(arguments, Dart_Null());
    Dart_Port service_port =
            Dart_NewNativePort("PlatformCfg", wrappedPlatformCfg, true);
    if (service_port != ILLEGAL_PORT) {
        Dart_Handle send_port = HandleError(Dart_NewSendPort(service_port));
        Dart_SetReturnValue(arguments, send_port);
    }
    Dart_ExitScope();
}
