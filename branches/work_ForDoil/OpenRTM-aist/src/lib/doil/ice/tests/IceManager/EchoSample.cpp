// **********************************************************************
//
// Copyright (c) 2003-2011 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.4.2
//
// <auto-generated>
//
// Generated from file `EchoSample.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#include <EchoSample.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 304
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 2
#       error Ice patch level mismatch!
#   endif
#endif

static const ::std::string __Demo__EchoSample__echo_name = "echo";

::Ice::Object* IceInternal::upCast(::Demo::EchoSample* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::Demo::EchoSample* p) { return p; }

void
Demo::__read(::IceInternal::BasicStream* __is, ::Demo::EchoSamplePrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::Demo::EchoSample;
        v->__copyFrom(proxy);
    }
}

void
IceProxy::Demo::EchoSample::echo(const ::std::string& msg, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::Demo::EchoSample* __del = dynamic_cast< ::IceDelegate::Demo::EchoSample*>(__delBase.get());
            __del->echo(msg, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, true, __cnt);
        }
    }
}

::Ice::AsyncResultPtr
IceProxy::Demo::EchoSample::begin_echo(const ::std::string& msg, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __Demo__EchoSample__echo_name, __del, __cookie);
    try
    {
        __result->__prepare(__Demo__EchoSample__echo_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->__getOs();
        __os->write(msg);
        __os->endWriteEncaps();
        __result->__send(true);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __result->__exceptionAsync(__ex);
    }
    return __result;
}

void
IceProxy::Demo::EchoSample::end_echo(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __Demo__EchoSample__echo_name);
}

const ::std::string&
IceProxy::Demo::EchoSample::ice_staticId()
{
    return ::Demo::EchoSample::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::Demo::EchoSample::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::Demo::EchoSample);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::Demo::EchoSample::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::Demo::EchoSample);
}

::IceProxy::Ice::Object*
IceProxy::Demo::EchoSample::__newInstance() const
{
    return new EchoSample;
}

void
IceDelegateM::Demo::EchoSample::echo(const ::std::string& msg, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __Demo__EchoSample__echo_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(msg);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateD::Demo::EchoSample::echo(const ::std::string& msg, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& msg, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_msg(msg)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::Demo::EchoSample* servant = dynamic_cast< ::Demo::EchoSample*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->echo(_m_msg, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_msg;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __Demo__EchoSample__echo_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(msg, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::Ice::ObjectPtr
Demo::EchoSample::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __Demo__EchoSample_ids[2] =
{
    "::Demo::EchoSample",
    "::Ice::Object"
};

bool
Demo::EchoSample::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__Demo__EchoSample_ids, __Demo__EchoSample_ids + 2, _s);
}

::std::vector< ::std::string>
Demo::EchoSample::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__Demo__EchoSample_ids[0], &__Demo__EchoSample_ids[2]);
}

const ::std::string&
Demo::EchoSample::ice_id(const ::Ice::Current&) const
{
    return __Demo__EchoSample_ids[0];
}

const ::std::string&
Demo::EchoSample::ice_staticId()
{
    return __Demo__EchoSample_ids[0];
}

::Ice::DispatchStatus
Demo::EchoSample::___echo(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string msg;
    __is->read(msg);
    __is->endReadEncaps();
    echo(msg, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __Demo__EchoSample_all[] =
{
    "echo",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
Demo::EchoSample::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__Demo__EchoSample_all, __Demo__EchoSample_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __Demo__EchoSample_all)
    {
        case 0:
        {
            return ___echo(in, current);
        }
        case 1:
        {
            return ___ice_id(in, current);
        }
        case 2:
        {
            return ___ice_ids(in, current);
        }
        case 3:
        {
            return ___ice_isA(in, current);
        }
        case 4:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
Demo::EchoSample::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
Demo::EchoSample::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

// COMPILERFIX: Stream API is not supported with VC++ 6
#if !defined(_MSC_VER) || (_MSC_VER >= 1300)
void
Demo::EchoSample::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type Demo::EchoSample was not generated with stream support";
    throw ex;
}

void
Demo::EchoSample::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type Demo::EchoSample was not generated with stream support";
    throw ex;
}
#endif

void 
Demo::__patch__EchoSamplePtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::Demo::EchoSamplePtr* p = static_cast< ::Demo::EchoSamplePtr*>(__addr);
    assert(p);
    *p = ::Demo::EchoSamplePtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::Demo::EchoSample::ice_staticId(), v->ice_id());
    }
}
