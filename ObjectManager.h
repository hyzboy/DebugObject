#pragma once
#include"Object.h"
#include<tsl/robin_set.h>
#include<tsl/robin_map.h>

class Object;
class ObjectManager
{
    size_t      object_hash_code;
    const char *object_type_name;
    size_t      object_count;

protected:

    const size_t AcquireSerialNumber(){return ++object_count;}

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
};//class ObjectManager

bool RegistryObjectManager(ObjectManager *om);
void UnregistryObjectManager(ObjectManager *om);

struct ObjectLifetime
{
    virtual void Join(ObjectManager *){};
    virtual void Unjoin(ObjectManager *){};

    virtual Object *Create(const ObjectBaseInfo &obi)=0;
    virtual void Release(Object *)=0;
};

template<typename T> struct DefaultObjectLifetime:public ObjectLifetime
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

template<typename T> class DefaultObjectManager:public ObjectManager
{
    tsl::robin_set<T *> object_set;
    tsl::robin_map<size_t,SafeObjectData<T> *> object_map;

    ObjectLifetime *object_lifetime;

public:

    DefaultObjectManager(ObjectLifetime *ol):ObjectManager(typeid(T).hash_code(),typeid(T).name())
    {
        object_lifetime=ol;

        object_lifetime->Join(this);
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

        object_lifetime->Unjoin(this);
        UnregistryObjectManager(this);
    }

protected:

    template<typename T,typename ...ARGS> friend SafeObject<T> DefaultCreateObject(const char *source_file,const char *source_function,const size_t source_line,ARGS...args);

    template<typename ...ARGS>
    SafeObjectData<T> *CreateObject(const SourceCodeLocation &scl,ARGS...args)
    {
        ObjectBaseInfo obi
        {
            .hash_code      =GetHashCode(),
            .object_manager =this,
            .serial_number  =AcquireSerialNumber(),
            .source_code_location=scl
        };

        Object *obj=object_lifetime->Create(obi);

        SafeObjectData<T> *spd=new SafeObjectData<T>((T *)obj);

        object_set.insert({(T *)obj});
        object_map.insert({obj->GetSerialNumber(),spd});

        spd->ptr->Initailize(args...);

        return spd;
    }

    template<typename T> friend class SafeObject;

    void ReleaseObject(SafeObjectData<T> *spd)
    {
        if(!spd)
            return;

        object_map.erase(spd->ptr->GetSerialNumber());
        object_set.erase(spd->ptr);

        if(spd->ptr)
        {
            spd->ptr->Deinitailize();
            object_lifetime->Release(spd->ptr);
            spd->ptr=nullptr;
        }

        if(spd->count>1)
        {
            --spd->count;
        }
        else
        {
            delete spd;
        }
    }

    template<typename T> friend SafeObject<T> GetObjectBySerial(const size_t &serial);

    SafeObjectData<T> *GetObjectBySerial(const size_t &serial)
    {
        return object_map.at(serial);
    }
};//class DefaultObjectManager

#define DEFINE_DEFAULT_OBJECT_MANAGER(T) namespace \
{   \
    static DefaultObjectLifetime<T> T##ObjectLifetime; \
    static DefaultObjectManager<T> T##ObjectManager(&T##ObjectLifetime); \
}

ObjectManager *GetObjectManager(const size_t &hash_code);

template<typename T> inline ObjectManager *GetObjectManager()
{
    return GetObjectManager(typeid(T).hash_code());
}

template<typename T> inline SafeObject<T> GetObjectBySerial(const size_t &serial)
{
    ObjectManager *om=GetObjectManager<T>();

    if(!om)
        return SafeObject<T>();

    DefaultObjectManager<T> *dom=static_cast<DefaultObjectManager<T> *>(om);

    return SafeObject<T>(dom->GetObjectBySerial(serial));
}
