#pragma once

struct SourceCodeLocation
{
    const char *    file;               ///<源文件
    const char *    func;               ///<函数
    size_t          line;               ///<行号
};

#define SOURCE_CODE_LOCATION    __FILE__,__FUNCTION__,__LINE__
