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
        return GetTypeHash<class_name>(); \
    }

/**
 * 安全访问指针<Br>
 * 其本质类似于的WeakPtr，但是不同的是:
 * <ul>
 *      <li>SafePtr不使用atom计数器，所以它不是线程安全的</li>
 *      <li>SafePtr不会自动释放指针，它是在访问时检查指针是否有效，如果无效则返回nullptr</li>
 * </ul>
 */
template<typename T> class SafePtr
{
    struct SafePtrData
    {
        T *ptr;
        int count;

    public:

        SafePtrData(T *p)
        {
            ptr=p;
            count=1;
        }
    };

    SafePtrData *data;

protected:

    void unlink()
    {
        if(!data)
            return;

        if(data->count>0)
            --data->count;

        if(data->count==0)
            delete data;

        data=nullptr;
    }

public:

    SafePtr()
    {
        data=nullptr;
    }

    SafePtr(T *ptr)
    {
        data=new SafePtrData(ptr);
    }

    SafePtr(SafePtr<T> &sp)
    {
        data=nullptr;
        operator=(sp);
    }

    virtual ~SafePtr()
    {
        unlink();
    }

            T *Get()        {return data?data->ptr:nullptr;}
    const   T *Get() const  {return data?data->ptr:nullptr;}

    T *operator->() { return Get(); }
    T &operator*() { return *Get(); }

    const T *operator->() const { return Get(); }

    const bool operator==(const T *ptr) const noexcept { return Get()==ptr; }
    const bool operator!=(const T *ptr) const noexcept { return Get()!=ptr; }

    const bool operator==(const SafePtr<T> &sp) const { return Get()==sp.Get(); }
    const bool operator!=(const SafePtr<T> &sp) const { return Get()!=sp.Get(); }

    const bool IsValid() const noexcept
    {
        return data&&data->ptr;
    }

    SafePtr<T> &operator=(SafePtr<T> &sp)
    {
        if(data)
        {
            if(data->ptr==sp.ptr)
                return *this;

            unlink();
        }

        if(sp.IsValid())
        {
            data=sp.data;
            ++data->count;
        }

        return *this;
    }

    template<typename OT>
    SafePtr<T> &operator=(SafePtr<OT> &spd)
    {
        if(T::StaticHashCode()!=OT.StaticHashCode())
        {
            unlink();
            return *this;
        }

        if(data!=spd.data)
        {
            unlink();
            data=spd.data;
            ++data->count;
        }

        return *this;
    }

    SafePtr<T> &operator=(Object *obj)
    {
        if(!obj)
        {
            unlink();
            return *this;
        }

        if(T::StaticHashCode()!=obj->GetHashCode())
        {
            unlink();
            return *this;
        }

        data=new SafePtrData(obj);
        return *this;
    }

    void Destory()
    {
        if(data&&data->ptr)
        {
            data->ptr->Destory();
            data->ptr=nullptr;

            unlink();
        }
    }
};//template<typename T> class SafePtr
