#pragma once
#include"ObjectBaseInfo.h"

class Object
{
    ObjectBaseInfo object_base_info;

public:

    const ObjectBaseInfo &  GetObjectBaseInfo   () const noexcept { return object_base_info; }                          ///<获取对象基本信息

    const size_t            GetHashCode         () const noexcept { return object_base_info.hash_code; }                ///<获取对象数据类型的hash值
    const size_t            GetSerialNumber     () const noexcept { return object_base_info.serial_number; }            ///<获取对象的序列号

public:

    Object(const ObjectBaseInfo &obi) noexcept;

protected:

    virtual ~Object();

public:
    
    virtual void Deinitailize()=0;
    virtual void Destory();
};//class Object

template<typename T> class DefaultObjectManager;

#define OBJECT_CLASS_BODY(class_name) \
private:    \
\
    friend class DefaultObjectManager<class_name>;\
\
    class_name(const ObjectBaseInfo &obi):Object(obi)   \
    {   \
        std::cout<<#class_name " Construct("<<GetSerialNumber()<<")"<<std::endl;   \
    }   \
\
    virtual ~class_name() override  \
    {   \
        std::cout<<#class_name " Destruct("<<GetSerialNumber()<<")"<<std::endl;    \
    }   \
\
public: \
\
    static const size_t StaticHashCode()\
    {   \
        return GetTypeHashCode<class_name>(); \
    }
