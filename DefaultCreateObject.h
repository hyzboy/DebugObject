#pragma once

#include"ObjectManager.h"

template<typename T,typename ...ARGS> SafePtr<T> DefaultCreateObject(const char *source_file,const char *source_function,const size_t source_line,ARGS...args)
{
    const size_t hc=GetTypeHash<T>();

    ObjectManager *om=GetObjectManager(hc);

    if(!om)
        return SafePtr<T>();
    
    DefaultObjectManager<T> *dom=static_cast<DefaultObjectManager<T> *>(om);

    SafePtrData<T> *spd=dom->CreateObject(source_file,source_function,source_line,args...);

    return SafePtr<T>(spd);
}

#define NEW_OBJECT(class_name,...) DefaultCreateObject<class_name>(__FILE__,__FUNCTION__,__LINE__ __VA_OPT__(,) __VA_ARGS__)

#define DEFINE_NEW_OBJECT(class_name,type_name,...) SafePtr<class_name> type_name=NEW_OBJECT(class_name __VA_OPT__(,) __VA_ARGS__)
