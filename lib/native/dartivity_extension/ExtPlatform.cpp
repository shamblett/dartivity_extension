/* 
 * File:   ExtPLatform.cpp
 * Package: dartivity_extension
 * Author: S. Hamblett <steve.hamblett@linux.com>
 * Copyright S. Hamblett 2015
 *
 * Created on 8 September 2015, 08:34
 */

#include "ExtPlatform.h"

// Resource mutex
std::mutex resourceMutex;
std::mutex resourceCallbackMutex;

// Find Resource callback class

class resourceFindCallback {
public:

    static void foundResource(std::shared_ptr<OCResource> resource);

    static Dart_CObject* m_message;
    static bool m_callbackInvoked;
    static Dart_CObject** m_resList;
    static int m_resListCount;
};
Dart_CObject* resourceFindCallback::m_message;
bool resourceFindCallback::m_callbackInvoked;
Dart_CObject** resourceFindCallback::m_resList;
int resourceFindCallback::m_resListCount;

void resourceFindCallback::foundResource(std::shared_ptr<OCResource> resource) {

    resourceCallbackMutex.lock();
    Dart_CObject* result = new Dart_CObject;

    // Indicate invocation
    m_callbackInvoked = true;

    try {
        // Check for a null resource
        if (resource == NULL) {
            // Return a boolean here to indicate no resources found
            Dart_CObject* servicePortObject = m_message->value.as_array.values[EXT_SERVICE_PORT];
            Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
            result->type = Dart_CObject_kBool;
            result->value.as_bool = false;
            Dart_PostCObject(reply_port_id, result);
            resourceCallbackMutex.unlock();
#ifdef DEBUG
            std::cout << "<<< foundResource - returned invalid result" << std::endl;
#endif
        } else {
#ifdef DEBUG
            //std::cout << "<<< foundResource - resource id is " << resource->uniqueIdentifier() << std::endl;
#endif
            // Build and return the result for a found resource
            
            // Host
            Dart_CObject* retHost = new Dart_CObject;
            std::string host = resource->host();
            retHost->type = Dart_CObject_kString;
            retHost->value.as_string = const_cast<char*> (host.c_str());


            // Uri
            Dart_CObject* retUri = new Dart_CObject;
            std::string uri = resource->uri();
            retUri->type = Dart_CObject_kString;
            retUri->value.as_string = const_cast<char*> (uri.c_str());

            // Unique id
            Dart_CObject* retUid = new Dart_CObject;
            retUid->type = Dart_CObject_kString;
            retUid->value.as_string = const_cast<char*> (uri.c_str());
            
            // We don't know how many resource and interface types a resource has so
            // use dynamic memory allocation, not Dart as we have no zone in this callback.

            // Resource types
            Dart_CObject* retResourceTypes = new Dart_CObject;
            Dart_CObject* resTemp;
            std::vector<std::string> resourceTypes = resource->getResourceTypes();
            long unsigned int resourceTypesLen = resourceTypes.size();
            Dart_CObject** resourceTemp = new Dart_CObject*[resourceTypesLen];
            int i = 0;
            for (std::vector<std::string>::iterator it = resourceTypes.begin(); it != resourceTypes.end(); ++it) {
                resTemp = new Dart_CObject;
                resTemp->type = Dart_CObject_kString;
                resTemp->value.as_string = const_cast<char*> ((*it).c_str());
                resourceTemp[i] = resTemp;
                i++;
            }
            retResourceTypes->type = Dart_CObject_kArray;
            retResourceTypes->value.as_array.values = resourceTemp;
            retResourceTypes->value.as_array.length = resourceTypesLen;

            // Interface types
            Dart_CObject* retInterfaceTypes = new Dart_CObject;
            Dart_CObject* intTemp;
            std::vector<std::string> intTypes = resource->getResourceInterfaces();
            long unsigned intTypesLen = intTypes.size();
            Dart_CObject** interfaceTemp = new Dart_CObject*[intTypesLen];
            i = 0;
            for (std::vector<std::string>::iterator it = intTypes.begin(); it != intTypes.end(); ++it) {
                intTemp = new Dart_CObject;
                intTemp->type = Dart_CObject_kString;
                intTemp->value.as_string = const_cast<char*> ((*it).c_str());
                interfaceTemp[i] = intTemp;
                i++;
            }
            retInterfaceTypes->type = Dart_CObject_kArray;
            retInterfaceTypes->value.as_array.values = interfaceTemp;
            retInterfaceTypes->value.as_array.length = intTypesLen;

            // Observable
            Dart_CObject* retObservable = new Dart_CObject;
            bool observable = resource->isObservable();
            retObservable->type = Dart_CObject_kBool;
            retObservable->value.as_bool = observable;

            // Create a proxy object from the resource so we can see this outside
            // of this handler. 
            OCResource::Ptr resourcePtr = OCPlatform::constructResourceObject(resource->host(),
                    resource->uri(),
                    resource->connectivityType(), resource->isObservable(),
                    resource->getResourceTypes(),
                    resource->getResourceInterfaces());


            // The proxy object is only supports put/get/observe functionality so get as 
            // much resource data as we can here and return it to create a resource class.

            // The pointer
            Dart_CObject* retPtr = new Dart_CObject;
            retPtr->type = Dart_CObject_kInt64;
            retPtr->value.as_int64 = reinterpret_cast<int64_t> (resourcePtr.get());
            
            // Return it all
            Dart_CObject** temp = new Dart_CObject*[PLATFORM_FIND_RESOURCES_RET_PARAMS];
            temp[0] = retPtr;
            temp[1] = retUid;
            temp[2] = retUri;
            temp[3] = retHost;
            temp[4] = retResourceTypes;
            temp[5] = retInterfaceTypes;
            temp[6] = retObservable;

            result->type = Dart_CObject_kArray;
            result->value.as_array.values = temp;
            result->value.as_array.length = PLATFORM_FIND_RESOURCES_RET_PARAMS;
            if (m_resListCount <= (MAX_DISCOVERABLE_RESOURCES - 1)) {
                m_resList[m_resListCount] = result;
                ++m_resListCount;
            }
#ifdef DEBUG
            std::cout << "<<< foundResource - returned valid result id is " << resource->uniqueIdentifier() << std::endl;
            std::cout << "<<< foundResource - returned valid result uri is " << resource->uri() << std::endl;
#endif
        }
        resourceCallbackMutex.unlock();
    } catch (std::exception& e) {
        std::cout << "Exception in foundResource: " << e.what() << std::endl;
    }

}

void platformFindResource(Dart_Port dest_port_id,
        Dart_CObject* message) {


    // Arg check and parameter extraction
    if (message->type == Dart_CObject_kArray &&
            PLATFORM_FIND_RESOURCES_PARAMS == message->value.as_array.length) {

        Dart_CObject* param2 = message->value.as_array.values[2];
        Dart_CObject* param3 = message->value.as_array.values[3];
        Dart_CObject* param4 = message->value.as_array.values[4];

        // Parameter check
        if (param2->type == Dart_CObject_kString &&
                param3->type == Dart_CObject_kString &&
                param4->type == Dart_CObject_kInt32) {
            std::string host = std::string(param2->value.as_string);
            std::string resourceName = std::string(param3->value.as_string);
            int connectivity = param4->value.as_int32;

#ifdef DEBUG
            std::cout << "<<< platformFindResource - Param Block >>>" << std::endl;
            std::cout << "Host - " << host << std::endl;
            std::cout << "Resource Name - " << resourceName << std::endl;
            std::cout << "Connectivity - " << connectivity << std::endl;
            std::cout << "<<< Param Block >>>" << std::endl;
#endif         
            // Call find resource, mutexed
            resourceMutex.lock();
            resourceFindCallback::m_message = message;
            resourceFindCallback::m_callbackInvoked = false;
            resourceFindCallback::m_resListCount = 0;
            resourceFindCallback::m_resList = new Dart_CObject*[MAX_DISCOVERABLE_RESOURCES];
#ifdef DEBUG
            std::cout << "<<< platformFindResource - in mutex calling findResource" << std::endl;
#endif
            OCStackResult res = OCPlatform::findResource(host, resourceName,
                    CT_DEFAULT, resourceFindCallback::foundResource);
            if (res != OC_STACK_OK) {
                std::cout << "<<< platformFindResource call failed, result is " << res << std::endl;
            }
#ifdef DEBUG
            std::cout << "<<< platformFindResource - in mutex returned from findResource call" << std::endl;
#endif
            // Wait until we either find the resource or give up, in which case we
            // return failed to find resource. Note, to allow the iotivity client to work better
            // we do lots of little sleeps rather than a few big ones.
            int waitCount = 0;
            while (true) {
                ++waitCount;
                if (waitCount == PLATFORM_FIND_WAIT) {
                    Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
                    Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
                    Dart_CObject result;
                    if (!resourceFindCallback::m_callbackInvoked) {
                        // Return a boolean here to indicate no resources found                       
                        result.type = Dart_CObject_kBool;
                        result.value.as_bool = false;
#ifdef DEBUG
                        std::cout << "<<< platformFindResource - in mutex returned not found result" << std::endl;
#endif
                    } else {
                        // Return the found resource list                     
                        result.type = Dart_CObject_kArray;
                        result.value.as_array.values = resourceFindCallback::m_resList;
                        result.value.as_array.length = resourceFindCallback::m_resListCount;
#ifdef DEBUG
                        std::cout << "<<< platformFindResource - in mutex returned found result, count is " << resourceFindCallback::m_resListCount << std::endl;
#endif
                    }
                    Dart_PostCObject(reply_port_id, &result);
                    delete[] resourceFindCallback::m_resList;
                    break;
                } else {
                    usleep(PLATFORM_FIND_SLICE);
                }
            }
        }
        // Either we have found the resource or we have timed out, either way release the mutex
        resourceMutex.unlock();

    } else {

        // Failure - return a null result objects   
        Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
        Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
        Dart_CObject result;
        result.type = Dart_CObject_kNull;
        Dart_PostCObject(reply_port_id, &result);
    }

}

void platformCfg(Dart_Port dest_port_id,
        Dart_CObject * message) {

    // Get the service port
    Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
    Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;

    // Arg check and parameter extraction
    if (message->type == Dart_CObject_kArray &&
            PLATFORM_CFG_PARAMS == message->value.as_array.length) {

        Dart_CObject* param2 = message->value.as_array.values[2];
        Dart_CObject* param3 = message->value.as_array.values[3];
        Dart_CObject* param4 = message->value.as_array.values[4];
        Dart_CObject* param5 = message->value.as_array.values[5];
        Dart_CObject* param6 = message->value.as_array.values[6];
        Dart_CObject* param7 = message->value.as_array.values[7];

        // Parameter check
        if (param2->type == Dart_CObject_kInt32 &&
                param3->type == Dart_CObject_kInt32 &&
                param4->type == Dart_CObject_kInt32 &&
                param5->type == Dart_CObject_kInt32 &&
                param6->type == Dart_CObject_kString &&
                param7->type == Dart_CObject_kInt32) {
            int service = param2->value.as_int32;
            int mode = param3->value.as_int32;
            int qos = param4->value.as_int32;
            int clientConn = param5->value.as_int32;
            std::string ip = std::string(param6->value.as_string);
            int port = param7->value.as_int32;

            // Setup and call the Iotivity function

#ifdef DEBUG
            std::cout << "<<< platformCfg - Param Block >>>" << std::endl;
            std::cout << "Service Type - " << service << std::endl;
            std::cout << "Mode - " << mode << std::endl;
            std::cout << "IP - " << ip << std::endl;
            std::cout << "Port - " << port << std::endl;
            std::cout << "Qos - " << qos << std::endl;
            std::cout << "Client Connectivity - " << clientConn << std::endl;
            std::cout << "<<< Param Block >>>" << std::endl;
#endif            
            PlatformConfig cfg{
                (OC::ServiceType)service,
                (OC::ModeType)mode,
                ip,
                (uint16_t) port,
                (OC::QualityOfService)qos,
                NULL
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

void wrappedPlatformService(Dart_Port dest_port_id,
        Dart_CObject * message) {
    // Get the reply port, we have to assume this is OK or
    // we can't reply with null to the caller.
    Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
    Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;

    // Switch on the incoming command
    Dart_CObject* commandObject = message->value.as_array.values[EXT_COMMAND];
    if (commandObject->type == Dart_CObject_kInt32) {
        int command = commandObject->value.as_int32;
        switch (command) {

            case PLATFORM_CFG:
                platformCfg(dest_port_id, message);
                return;

            case PLATFORM_FIND_RESOURCE:
                platformFindResource(dest_port_id, message);
                return;

            default:
#ifdef DEBUG
                std::cout << "wrappedPlatformService::Oops invalid command - value is " << command << " " << std::endl;
#endif
                break;
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
