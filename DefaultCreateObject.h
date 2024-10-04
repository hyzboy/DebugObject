#pragma once

#include"ObjectManager.h"

template<typename T,typename ...ARGS> SafeObject<T> DefaultCreateObject(const char *source_file,const char *source_function,const size_t source_line,ARGS...args)
{
    const size_t hc=GetTypeHash<T>();

    ObjectManager *om=GetObjectManager(hc);

    if(!om)
        return SafeObject<T>();
    
    DefaultObjectManager<T> *dom=static_cast<DefaultObjectManager<T> *>(om);

    const SourceCodeLocation scl
    {
        .file=source_file,
        .func=source_function,
        .line=source_line
    };

    SafeObjectData<T> *spd=dom->CreateObject(scl,args...);

    return SafeObject<T>(spd);
}

#define NEW_OBJECT(class_name,...) DefaultCreateObject<class_name>(__FILE__,__FUNCTION__,__LINE__ __VA_OPT__(,) __VA_ARGS__)

#define DEFINE_NEW_OBJECT(class_name,type_name,...) SafeObject<class_name> type_name=NEW_OBJECT(class_name __VA_OPT__(,) __VA_ARGS__)
