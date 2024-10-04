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
