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

void PlatformCfg(Dart_Port dest_port_id,
        Dart_CObject* message) {


}

void wrappedPlatformService(Dart_Port dest_port_id,
        Dart_CObject* message) {
    // Get the reply port, we have to assume this is OK or
    // we can't reply with null to the caller.
    Dart_CObject* param8 = message->value.as_array.values[8];
    Dart_Port reply_port_id = param8->value.as_send_port.id;

    // Arg check and parameter extraction
    if (message->type == Dart_CObject_kArray &&
            9 == message->value.as_array.length) {

        Dart_CObject* param0 = message->value.as_array.values[0];
        Dart_CObject* param1 = message->value.as_array.values[1];
        Dart_CObject* param2 = message->value.as_array.values[2];
        Dart_CObject* param5 = message->value.as_array.values[5];
        Dart_CObject* param6 = message->value.as_array.values[6];
        Dart_CObject* param7 = message->value.as_array.values[7];

        // Parameter check
        if (param0->type == Dart_CObject_kInt32 &&
                param1->type == Dart_CObject_kInt32 &&
                param2->type == Dart_CObject_kInt32 &&
                param5->type == Dart_CObject_kString &&
                param6->type == Dart_CObject_kInt32 &&
                param7->type == Dart_CObject_kString) {
            int service = param0->value.as_int32;
            int mode = param1->value.as_int32;
            int qos = param2->value.as_int32;
            std::string ip = std::string(param5->value.as_string);
            int port = param6->value.as_int32;
            dbFile = std::string(param7->value.as_string);

            // Setup and call the Iotivity function
            OCPersistentStorage ps{client_open, fread, fwrite, fclose, unlink};
#ifdef DEBUG
            std::cout << "<<< wrappedPlatformCfg - Param Block >>>" << std::endl;
            std::cout << "Service Type - " << service << std::endl;
            std::cout << "Mode - " << mode << std::endl;
            std::cout << "IP - " << ip << std::endl;
            std::cout << "Port - " << port << std::endl;
            std::cout << "Qos - " << qos << std::endl;
            std::cout << "Db File - " << dbFile << std::endl;
            std::cout << "<<< Param Block >>>" << std::endl;
#endif            
            PlatformConfig cfg{
                (OC::ServiceType)service,
                (OC::ModeType)mode,
                ip,
                (uint16_t) port,
                (OC::QualityOfService)qos,
                &ps
            };
            OCPlatform::Configure(cfg);

            // Build and return the result
            Dart_CObject result;
            result.type = Dart_CObject_kBool;
            result.value.as_bool = true;
            Dart_PostCObject(reply_port_id, &result);
            return;
        }
    }

    // Failure - return a null result object
    Dart_CObject result;
    result.type = Dart_CObject_kNull;
    Dart_PostCObject(reply_port_id, &result);
}

void platformServicePort(Dart_NativeArguments arguments) {
    Dart_EnterScope();
    Dart_SetReturnValue(arguments, Dart_Null());
    Dart_Port service_port =
            Dart_NewNativePort("PlatformService", wrappedPlatformService, true);
    if (service_port != ILLEGAL_PORT) {
        Dart_Handle send_port = HandleError(Dart_NewSendPort(service_port));
        Dart_SetReturnValue(arguments, send_port);
    }
    Dart_ExitScope();
}
