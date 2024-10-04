#pragma once

#include<iostream>

template<typename T> inline constexpr const size_t GetTypeHash()noexcept{return typeid(T).hash_code();} ///<取得一个类型的哈希值
template<typename T> inline constexpr const char * GetTypeName()noexcept{return typeid(T).name();}      ///<取得一个类型的名称

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
