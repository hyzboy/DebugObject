// DebugObject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include"Object.h"
#include"DefaultCreateObject.h"

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


class DebugObject:public Object
{
    OBJECT_CLASS_BODY(DebugObject)

public:

    void Initailize()
    {
        std::cout<<"DebugObject::Initailize("<<GetSerialNumber()<<")"<<std::endl;
    }

    void Deinitailize() override
    {
        std::cout<<"DebugObject::Deinitailize("<<GetSerialNumber()<<")"<<std::endl;
    }
};

DEFINE_DEFAULT_OBJECT_MANAGER(DebugObject);

int main()
{
    //DebugObject *obj1=DefaultCreateObject<DebugObject>();
    //DebugObject *obj2=DefaultCreateObject<DebugObject>();

    DebugObject *obj1=DEFAULT_CREATE_OBJECT(DebugObject);

    DEFINE_DEFAULT_CREATE_OBJECT(DebugObject,obj2);        //等于上一行

//    obj2->Destory();
    obj1->Destory();
}