#pragma once
#include"Object.h"
#include<tsl/robin_set.h>

class Object;
class ObjectManager
{
    size_t      object_hash_code;
    const char *object_type_name;
    size_t      object_count;

protected:

    const size_t AcquireSerialNumber(){return ++object_count;}

    virtual Object *_CreateObject(const ObjectBaseInfo &obi)=0;
    virtual void _ReleaseObject(Object *obj)=0;

public:

    const size_t GetHashCode()const{return object_hash_code;}
    const char * GetTypename()const{return object_type_name;}
    const size_t GetCount()const{return object_count;}

public:

    ObjectManager(const size_t &hc,const char *tn)
    {
        object_hash_code=hc;
        object_type_name=tn;
        object_count=0;
    }

    virtual ~ObjectManager()=default;
    
    virtual Object *CreateObject(const char *source_file,const char *source_function,const size_t source_line);
    virtual bool ReleaseObject(Object *obj);
};//class ObjectManager

bool RegistryObjectManager(ObjectManager *om);
void UnregistryObjectManager(ObjectManager *om);

template<typename T> class DefaultObjectManager:public ObjectManager
{
    tsl::robin_set<T *> object_set;

protected:

    virtual Object *_CreateObject(const ObjectBaseInfo &obi)
    {
        T *obj=new T(obi);

        object_set.insert(obj);

        return obj;
    }

    virtual void _ReleaseObject(Object *obj)
    {
        object_set.erase(static_cast<T *>(obj));

        obj->Deinitailize();
    }

public:

    DefaultObjectManager():ObjectManager(typeid(T).hash_code(),typeid(T).name())
    {
        RegistryObjectManager(this);
    }

    virtual ~DefaultObjectManager()
    {
        if(object_set.size())
        {
            std::cerr<<"DefaultObjectManager::~DefaultObjectManager("<<GetTypename()<<") "<<object_set.size()<<" objects havn't been release."<<std::endl;

            for(auto &it:object_set)
            {
                PrintObjectBaseInfo(it->GetObjectBaseInfo());
            }
        }

        UnregistryObjectManager(this);
    }
};//class DefaultObjectManager

#define DEFINE_DEFAULT_OBJECT_MANAGER(T) namespace{static DefaultObjectManager<T> T##ObjectManager;}

ObjectManager *GetObjectManager(const size_t &hash_code);

template<typename T> inline ObjectManager *GetObjectManager()
{
    return GetObjectManager(typeid(T).hash_code());
}
