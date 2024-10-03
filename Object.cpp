#include"Object.h"
#include"ObjectManager.h"

Object::Object(const ObjectBaseInfo &obi) noexcept
{
    object_base_info=obi;

    std::cout<<"Object Construct("<<GetSerialNumber()<<")"<<std::endl;
}

Object::~Object()
{
    std::cout<<"Object Destruct("<<GetSerialNumber()<<")."<<std::endl;
}

void Object::Destory()
{
    if(object_base_info.object_manager)
        object_base_info.object_manager->ReleaseObject(this);

    delete this;
}
