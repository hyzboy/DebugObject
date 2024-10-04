// DebugObject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include"Object.h"
#include"DefaultCreateObject.h"

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
    SafePtr<DebugObject> obj1=NEW_OBJECT(DebugObject);

    DEFINE_NEW_OBJECT(DebugObject,obj2);                    //等于上一行

    //DebugObject *obj3=new DebugObject();                  //编译不过(构造函数被定义为私有)

    //obj1->Destory();                                      //编译不过(不允许直接调用DebugObject自己的Destory)
    obj1.Destory();             //SafePtr的Destory

    //delete obj2;                                          //编译不过,SafePtr<>不能被delete

    if(obj1.IsValid())
    {
        std::cerr<<"obj1 IsValid() error!"<<std::endl;
    }
    else
    {
        std::cout<<"obj1 isn't valid!"<<std::endl;
    }

    return 0;
}
