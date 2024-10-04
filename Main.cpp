#include"DebugObject.h"
#include"DefaultCreateObject.h"

int main()
{
    SafePtr<DebugObject> obj1=NEW_OBJECT(DebugObject);

    DEFINE_NEW_OBJECT(DebugObject,obj2);                    //等于上一行

    //DebugObject *obj3=new DebugObject();                  //编译不过(构造函数被定义为私有)

    //obj1->Destory();                                      //编译不过(不允许直接调用DebugObject自己的Destory)
    obj1.Destory();             //SafePtr的Destory

    //delete obj2;                                          //编译不过,SafePtr<>不能被delete

    if(obj1.IsValid())
    {
        std::cerr<<"obj1 IsValid() error!"<<std::endl;
    }
    else
    {
        std::cout<<"obj1 isn't valid!"<<std::endl;
    }

    return 0;
}
