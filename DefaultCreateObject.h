#pragma once

#include"ObjectManager.h"

template<typename T,typename ...ARGS> T *DefaultCreateObject(const char *source_file,const char *source_function,const size_t source_line,ARGS...args)
{
    const size_t hc=GetHashCode<T>();

    ObjectManager *om=GetObjectManager(hc);

    if(!om)
        return nullptr;

    T *obj=static_cast<T *>(om->CreateObject(source_file,source_function,source_line));

    obj->Initailize(args...);

    return obj;
}

#define DEFAULT_CREATE_OBJECT(class_name,...) DefaultCreateObject<class_name>(__FILE__,__FUNCTION__,__LINE__ __VA_OPT__(,) __VA_ARGS__)

#define DEFINE_DEFAULT_CREATE_OBJECT(class_name,type_name,...) class_name *type_name=DefaultCreateObject<class_name>(__FILE__,__FUNCTION__,__LINE__ __VA_OPT__(,) __VA_ARGS__)
