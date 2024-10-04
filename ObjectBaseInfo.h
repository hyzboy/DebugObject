#pragma once

#include<iostream>

template<typename T> inline constexpr const size_t GetTypeHash()noexcept{return typeid(T).hash_code();} ///<取得一个类型的哈希值
template<typename T> inline constexpr const char * GetTypeName()noexcept{return typeid(T).name();}      ///<取得一个类型的名称

class ObjectManager;

struct SourceCodeLocation
{
    const char *    file;               ///<源文件
    const char *    func;               ///<函数
    size_t          line;               ///<行号
};

#define SOURCE_CODE_LOCATION    __FILE__,__FUNCTION__,__LINE__

struct ObjectBaseInfo
{
    size_t          hash_code;          ///<对象数据类型的hash值
    ObjectManager * object_manager;     ///<对象管理器
    size_t          serial_number;      ///<对象的序列号

    SourceCodeLocation source_code_location;
};

inline void PrintObjectBaseInfo(const ObjectBaseInfo &obi)
{
    std::cout<<"Object(hash_code="<<obi.hash_code
             <<", serial_number="<<obi.serial_number
             <<", source_file="<<obi.source_code_location.file
             <<", source_function="<<obi.source_code_location.func
             <<", source_line="<<obi.source_code_location.line
             <<")"<<std::endl;
}
