#pragma once

#include<iostream>

template<typename T> constexpr const size_t GetHashCode() noexcept
{
    return typeid(T).hash_code();
}

class ObjectManager;

struct ObjectBaseInfo
{
    size_t          hash_code;          ///<对象数据类型的hash值
    ObjectManager * object_manager;     ///<对象管理器
    size_t          serial_number;      ///<对象的序列号

    const char *    source_file;        ///<对象创建的源文件
    const char *    source_function;    ///<对象创建的源函数
    size_t          source_line;        ///<对象创建的源代码行
};

inline void PrintObjectBaseInfo(const ObjectBaseInfo &obi)
{
    std::cout<<"Object(hash_code="<<obi.hash_code
             <<", serial_number="<<obi.serial_number
             <<", source_file="<<obi.source_file
             <<", source_function="<<obi.source_function
             <<", source_line="<<obi.source_line
             <<")"<<std::endl;
}
