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

    template<typename T> friend class SafePtr;

    virtual ~Object();
    virtual void Destory();

public:
    
    virtual void Deinitailize()=0;
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

template<typename T> struct SafePtrData
{
    T *ptr;
};

/**
 * 安全访问指针<Br>
 * 其本质类似于的WeakPtr，但是不同的是:
 * <ul>
 *      <li>SafePtr不使用atom计数器，所以它不是线程安全的</li>
 *      <li>SafePtr不会自动释放指针，而是在访问时检查指针是否有效，如果无效则返回nullptr</li>
 * </ul>
 */
template<typename T> class SafePtr
{
    SafePtrData<T> data;

public:

    SafePtr()
    {
        data.ptr=nullptr;
    }
    SafePtr(T *ptr)
    {
        data.ptr=ptr;
    }
    SafePtr(SafePtrData<T> &spd):data(spd){}
    virtual ~SafePtr()=default;

            T *Get()        {return data.ptr;}
    const   T *Get() const  {return data.ptr;}

    T *operator->() { return Get(); }
    T &operator*() { return *Get(); }

    const bool operator==(const T *ptr) const noexcept { return Get()==ptr; }
    const bool operator!=(const T *ptr) const noexcept { return Get()!=ptr; }

    const bool operator==(const SafePtr<T> &sp) const { return Get()==sp.Get(); }
    const bool operator!=(const SafePtr<T> &sp) const { return Get()!=sp.Get(); }

    const bool IsValid() const noexcept
    {
        return data.ptr;
    }

    SafePtr<T> &operator=(SafePtrData<T> &spd)
    {
        data=spd;

        return *this;
    }

    template<typename OT>
    SafePtr<T> &operator=(SafePtrData<OT> &spd)
    {
        if(T::StaticHashCode()!=OT.StaticHashCode())
        {
            data.ptr=nullptr;
            return *this;
        }

        data.ptr=(T *)(spd.ptr);
        return *this;
    }

    SafePtr<T> &operator=(Object *obj)
    {
        if(!obj)
        {
            data.ptr=nullptr;
            return *this;
        }

        if(T::StaticHashCode()!=obj->GetHashCode())
        {
            data.ptr=nullptr;
            return *this;
        }

        data.ptr=obj;
        return *this;
    }

    void Destory()
    {
        if(data.ptr)
        {
            data.ptr->Destory();
            data.ptr=nullptr;
        }
    }
};//template<typename T> class SafePtr
