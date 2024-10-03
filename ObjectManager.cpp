#include"ObjectManager.h"
#include"Object.h"
#include<iostream>
#include<tsl/robin_map.h>

namespace
{
    using ObjectManagerMap=tsl::robin_map<size_t,ObjectManager *>;
    static ObjectManagerMap *object_manager_map=nullptr;

    ObjectManagerMap *GetObjectManagerMap()
    {
        if(!object_manager_map)
            object_manager_map=new ObjectManagerMap;

        return(object_manager_map);
    }
}//namespace

Object *ObjectManager::CreateObject(const char *source_file,const char *source_function,const size_t source_line)
{
    ObjectBaseInfo obi
    {
        .hash_code      =GetHashCode(),
        .object_manager =this,
        .serial_number  =AcquireSerialNumber(),
        .source_file    =source_file,
        .source_function=source_function,
        .source_line    =source_line
    };

    return _CreateObject(obi);
}

bool ObjectManager::ReleaseObject(Object *obj)
{
    if(!obj)return(false);

    if(obj->GetHashCode()!=GetHashCode())
        return false;

    _ReleaseObject(obj);

    return true;
}

bool RegistryObjectManager(ObjectManager *om)
{
    if(!om)
        return(false);

    const size_t hc=om->GetHashCode();

    auto *omm=GetObjectManagerMap();

    if(omm->contains(hc))
        return(false);

    omm->insert({hc,om});

    std::cout<<"RegistryObjectManager("<<hc<<":"<<om->GetTypename()<<")"<<std::endl;

    return(true);
}

void UnregistryObjectManager(ObjectManager *om)
{
    if(!om)
        return;

    const size_t hc=om->GetHashCode();

    auto *omm=GetObjectManagerMap();

    auto omi=omm->find(hc);

    if(omi.key()!=hc)
        return;

    std::cout<<"UnregistryObjectManager("<<hc<<":"<<om->GetTypename()<<")"<<std::endl;

    omm->erase(hc);
}

ObjectManager *GetObjectManager(const size_t &hash_code)
{
    auto *omm=GetObjectManagerMap();

    auto omi=omm->find(hash_code);

    if(omi.key()==hash_code)
        return(omi.value());
    else
        return(nullptr);
}
