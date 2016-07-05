#ifndef __encryp_data__
#define __encryp_data__
#include <algorithm>

//基本类型数据加密
template<typename Type,int nLength = sizeof(Type)>
class CEncrypData
{
public:
    CEncrypData(){}
    CEncrypData(const CEncrypData &obj)
    {
        m_data = obj.m_data;
    }
    
    Type get()const
    {
        Type t;
        read(t);
        return t;
    }
    
    void set(Type t)
    {
        write(t);
    }
    
    template<typename T> CEncrypData(const T &v)
    {
        Type t = v;
        write(t);
    }
    
    template<typename T>
    CEncrypData & operator = (const T &v)
    {
        Type t = v;
        write(t);
        return *this;
    }
    
    template<typename T>
    CEncrypData & operator = (T &v)
    {
        Type t = v;
        write(t);
        return *this;
    }
    
    template<typename T>
    operator T()
    {
        Type t;
        read(t);
        return t;
    }
    
    //关系运算符
    //>
    template<typename T>
    bool operator > (const T &v)
    {
        Type t;
        read(t);
        return t>v;
    }
    
    //<
    template<typename T>
    bool operator < (const T &v)
    {
        Type t;
        read(t);
        return t<v;
    }
    
    //>=
    template<typename T>
    bool operator >= (const T &v)
    {
        Type t;
        read(t);
        return t>=v;
    }
    
    //<=
    template<typename T>
    bool operator <= (const T &v)
    {
        Type t;
        read(t);
        return t<=v;
    }
    
    //==
    template<typename T>
    bool operator == (const T &v)
    {
        Type t;
        read(t);
        return t==v;
    }
    
    //!=
    template<typename T>
    bool operator != (const T &v)
    {
        Type t;
        read(t);
        return t!=v;
    }
    
    //>
    template<typename T>
    friend bool operator > (const T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v>t;
    }
    
    //<
    template<typename T>
    friend bool operator < (const T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v<t;
    }
    
    //>=
    template<typename T>
    friend bool operator >= (const T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v>=t;
    }
    
    //<=
    template<typename T>
    friend bool operator <= (const T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v<=t;
    }
    
    //==
    template<typename T>
    friend bool operator == (const T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v==t;
    }
    
    //!=
    template<typename T>
    friend bool operator != (const T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v!=t;
    }
    
    //算术运算
    //+
    //...
    //-
    Type operator - ()
    {
        Type t;
        read(t);
        return -t;
    }
    
    //*
    template<typename T>
    Type operator * (const T &v)
    {
        Type t;
        read(t);
        return t*v;
    }
    
    ///
    template<typename T>
    Type operator / (const T &v)
    {
        Type t;
        read(t);
        return t/v;
    }
    
    //+
    template<typename T>
    Type operator + (const T &v)
    {
        Type t;
        read(t);
        return t+v;
    }
    
    //%
    template<typename T>
    Type operator % (const T &v)
    {
        Type t;
        read(t);
        return t%v;
    }
    
    //-
    template<typename T>
    Type operator - (const T &v)
    {
        Type t;
        read(t);
        return t-v;
    }
    
    //+
    //...
    
    //-
    //...
    
    //*
    template<typename T>
    friend Type operator * (const T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v*t;
    }
    
    ///
    template<typename T>
    friend Type operator / (const T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v/t;
    }
    
    //+
    template<typename T>
    friend Type operator + (const T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v+t;
    }
    
    //%
    template<typename T>
    friend Type operator % (const T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v%t;
    }
    
    //-
    template<typename T>
    friend Type operator - (const T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v-t;
    }
    
    //逻辑运算
    //&&
    template<typename T>
    bool operator && (const T &v)
    {
        Type t;
        read(t);
        return t&&v;
    }
    
    //||
    template<typename T>
    bool operator || (const T &v)
    {
        Type t;
        read(t);
        return t||v;
    }
    
    //!
    bool operator ! ()
    {
        Type t;
        read(t);
        return !t;
    }
    
    //&&
    template<typename T>
    friend bool operator && (T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v&&t;
    }

    //||
    template<typename T>
    friend bool operator || (T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        return v||t;
    }
    
    //复合赋值
    //+=
    template<typename T>
    void operator += (const T &v)
    {
        Type t;
        read(t);
        t += v;
        write(t);
    }
    
    //-=
    template<typename T>
    void operator -= (const T &v)
    {
        Type t;
        read(t);
        t -= v;
        write(t);
    }
    
    //*=
    template<typename T>
    void operator *= (const T &v)
    {
        Type t;
        read(t);
        t *= v;
        write(t);
    }
    
    ///=
    template<typename T>
    void operator /= (const T &v)
    {
        Type t;
        read(t);
        t /= v;
        write(t);
    }
    
    //%=
    template<typename T>
    void operator %= (const T &v)
    {
        Type t;
        read(t);
        t %= v;
        write(t);
    }
    
    //<<=
    template<typename T>
    void operator <<= (const T &v)
    {
        Type t;
        read(t);
        t <<= v;
        write(t);
    }
    
    //>>=
    template<typename T>
    void operator >>= (const T &v)
    {
        Type t;
        read(t);
        t >>= v;
        write(t);
    }
    
    //&=
    template<typename T>
    void operator &= (const T &v)
    {
        Type t;
        read(t);
        t &= v;
        write(t);
    }
    
    //|=
    template<typename T>
    void operator |= (const T &v)
    {
        Type t;
        read(t);
        t |= v;
        write(t);
    }
    
    //^=
    template<typename T>
    void operator ^= ( T &v)
    {
        Type t;
        read(t);
        t ^= v;
        write(t);
    }
    
    //+=
    template<typename T>
    friend void operator += (T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        v += t;
    }
    
    //-=
    template<typename T>
    friend void operator -= (T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        v -= t;
    }
    
    //*=
    template<typename T>
    friend void operator *= (T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        v *= t;
    }
    
    ///=
    template<typename T>
    friend void operator /= (T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        v /= t;
    }
    
    //%=
    template<typename T>
    friend void operator %= (T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        v %= t;
    }
    
    //<<=
    template<typename T>
    friend void operator <<= (T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        v <<= t;
    }
    
    //>>=
    template<typename T>
    friend void operator >>= (T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        v >>= t;
    }
    
    //&=
    template<typename T>
    friend void operator &= (T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        v &= t;
    }
    
    //|=
    template<typename T>
    friend void operator |= (T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        v |= t;
    }
    
    //^=
    template<typename T>
    friend void operator ^= (T &v,CEncrypData &obj)
    {
        Type t;
        obj.read(t);
        v ^= t;
    }
    
    //++?
    Type operator ++ ()
    {
        Type t;
        read(t);
        ++t;
        write(t);
        return t;
    }
    
    //--?
    Type operator -- ()
    {
        Type t;
        read(t);
        --t;
        write(t);
        return t;
    }
    
    //?++
    Type operator ++ (int)
    {
        Type t;
        read(t);
        Type t0 = t;
        t++;
        write(t);
        return t0;
    }
    
    //?--
    Type operator -- (int)
    {
        Type t;
        read(t);
        Type t0 = t;
        t--;
        write(t);
        return t0;
    }
    //...其他没有重载到的可通过get()处理
    
    
    //注意:目前在处理1>=nLength上不进行加密;如bool,char
private:
    void write(const Type &t)
    {
        char *pchBuf = (char*)&t;
        char *pchBufVal = (char*)&m_data;
        for (int i = 0; i < nLength; ++i)
        {
            pchBufVal[i] = pchBuf[nLength-1-i];
        }
    }
    
    void read(Type &t)const
    {
        char *pchBuf = (char*)&t;
        char *pchBufVal = (char*)&m_data;
        for (int i = 0; i < nLength; ++i)
        {
            pchBuf[i] = pchBufVal[nLength-1-i];
        }
    }
    
private:
    Type    m_data;
    
};

#endif
