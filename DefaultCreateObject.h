#pragma once

#include"ObjectManager.h"

template<typename T,typename ...ARGS> SafePtr<T> DefaultCreateObject(const char *source_file,const char *source_function,const size_t source_line,ARGS...args)
{
    const size_t hc=GetTypeHashCode<T>();

    ObjectManager *om=GetObjectManager(hc);

    if(!om)
        return SafePtr<T>();

    T *obj=static_cast<T *>(om->CreateObject(source_file,source_function,source_line));

    obj->Initailize(args...);

    return SafePtr<T>(obj);
}

#define NEW_OBJECT(class_name,...) DefaultCreateObject<class_name>(__FILE__,__FUNCTION__,__LINE__ __VA_OPT__(,) __VA_ARGS__)

#define DEFAULT_NEW_OBJECT(class_name,type_name,...) SafePtr<class_name> type_name=NEW_OBJECT(class_name,__VA_OPT__(,) __VA_ARGS__)
