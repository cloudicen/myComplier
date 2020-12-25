#ifndef TOCKEN_H
#define TOCKEN_H

#include "dfamatcher.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "QVariant"

/**
 * @brief The tockenType enum 抽象tocken类型定义
 */
enum tockenType{
    CONST_ID,// 常量符号
    SYMBOL,// 变量符号
    FUNC,//  函数符号
    KEYWORD,// 其他保留符号
    OP,// 运算符
    MARK,// 标记
    END,// 语句结束符
    NUMBER, //数值
    EMPTY //空符号，作用对应空字符
};

/**
 * @brief The tocken class 定义了一个文法符号
 */
class tocken:public QEnableSharedFromThis<tocken>
{
private:
    QSharedPointer<QString> regExpr;// 一类文法符号有相同的正则表达式指针
    QSharedPointer<dfaMatcher> matcher;// 用于匹配该符号的自动机指针
    QSharedPointer<std::function<QVariant(QSharedPointer<tocken>,std::initializer_list<QVariant>)>> func;// 该符号的副作用函数指针
    void castType();//将type属性转为json的字段
public:
    tockenType type;//tocken 类型
    QJsonObject property;//tocken 属性，由json存储，可存储任意格式。所有tocken都必须具有的属性有两个：type，string；前者表示该符号的类型，后者表示当前tocken代表的字符串

    /**
     * @brief tocken 默认构造一个EMPTY类型的tocken
     */
    tocken():regExpr(nullptr),matcher(nullptr),func(nullptr),type(EMPTY)
    {
        property["string"] = "";
        property["discription"] = "null tocken";
        castType();
    };

    /**
     * @brief tocken
     * @param _regExpr
     * @param _type
     * @param _func
     * @param _property
     * @param str
     * @param info
     */
    tocken(const QString &_regExpr,tockenType _type,std::function<QVariant(QSharedPointer<tocken>,std::initializer_list<QVariant>)> _func,QJsonObject _property=QJsonObject(),const QString & str="",const QString & info=""):
        regExpr(QSharedPointer<QString>::create(_regExpr)),
        matcher(QSharedPointer<dfaMatcher>::create(*regExpr)),
        func(QSharedPointer<std::function<QVariant(QSharedPointer<tocken>,std::initializer_list<QVariant>)>>::create(_func)),
        type(_type),
        property(_property){
        property["string"] = str;
        property["discription"] = info;
        castType();
    };

    /**
     * @brief tocken
     * @param _regExpr
     * @param _type
     * @param _property
     * @param str
     * @param info
     */
    tocken(const QString &_regExpr,tockenType _type,QJsonObject _property=QJsonObject(),const QString & str="",const QString & info=""):
        regExpr(QSharedPointer<QString>::create(_regExpr)),
        matcher(QSharedPointer<dfaMatcher>::create(*regExpr)),
        func(QSharedPointer<std::function<QVariant(QSharedPointer<tocken>,std::initializer_list<QVariant>)>>::create(nullptr)),
        type(_type),
        property(_property){
        property["string"] = str;
        property["discription"] = info;
        castType();
    };

    /**
     * @brief tocken 复制构造
     * @param other
     */
    tocken(const tocken& other)
    {
        regExpr = other.regExpr;
        matcher = other.matcher;
        property = other.property;
        type = other.type;
        func = other.func;
    }

    /**
     * @brief tocken 重载的复制构造函数，复制的时候复制所有属性，但把string变更为新的
     * @param other
     * @param str
     */
    tocken(const tocken& other,const QString& str)
    {
        regExpr = other.regExpr;
        matcher = other.matcher;
        property = other.property;
        type = other.type;
        func = other.func;
        property["string"] = str;
    }

    /**
     * @brief tocken 移动构造函数
     * @param other
     */
    tocken(tocken&& other)
    {
        regExpr = std::move(other.regExpr);
        matcher = other.matcher;
        func = other.func;
        property = std::move(other.property);
        type = other.type;

        other.matcher.clear();
        other.func.clear();
    }

    /**
     * @brief ~tocken 析构函数，这是一个虚函数，为了将tocken作为基类，必须有一个虚函数
     */
    virtual ~tocken(){};

    tocken operator=(const tocken& other)
    {
        regExpr = other.regExpr;
        matcher = other.matcher;
        property = other.property;
        type = other.type;
        func = other.func;
        return *this;
    }

    /**
     * @brief operator ==判断两个tocken是否相同
     * @param other
     * @return
     */
    bool operator==(const tocken& other)
    {
        //如果是以下三类tocken，则只需判断类型是否相同即可
        if(type == SYMBOL || type == NUMBER || type == EMPTY)
        {
            return this->type == other.type;
        }
        else
        {
            //判断类型和string属性是否都相同
            return isSame(other);
        }
    }

    /**
     * @brief isSame 判断两个tocken是否完全相同
     * @param other
     * @return
     */
    bool isSame(const tocken& other)
    {
        return this->type == other.type && this->property["string"]==other.property["string"];
    }

    /**
     * @brief act 执行tocken的副作用函数
     * @param args
     * @return
     */
    QVariant act(std::initializer_list<QVariant> args)
    {
        QVariant ret;
        if(!func.isNull() && (*func))
        {
            ret = (*func)(sharedFromThis(),args);
        }
        return ret;
    }

    /**
     * @brief match 执行tocken的匹配
     * @param str
     * @return
     */
    int match(const QString & str);

    /**
     * @brief toPrintable 返回可显示的字符串
     * @return
     */
    QString toPrintable()
    {
        QString str="properties:\n";
        return str.append(QJsonDocument(property).toJson());
    }
};

#endif // TOCKEN_H
