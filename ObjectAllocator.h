#pragma once

struct ObjectBaseInfo;
class Object;
class ObjectManager;

struct ObjectAllocator
{
    virtual void Join(ObjectManager *){};
    virtual void Unjoin(ObjectManager *){};

    virtual Object *Create(const ObjectBaseInfo &obi)=0;
    virtual void Release(Object *)=0;
};

template<typename T> struct DefaultObjectAllocator:public ObjectAllocator
{
    virtual Object *Create(const ObjectBaseInfo &obi)override
    {
        return new T(obi);
    }

    virtual void Release(Object *obj)override
    {
        delete obj;
    }
};

