/*
 * ptrwrap.h
 *
 *  Created on: 21.11.2012
 *      Author: Paul Bottin
 */

#ifndef PTRWRAP_H_
#define PTRWRAP_H_

#ifndef BUILDING_NODE_EXTENSION
#	define BUILDING_NODE_EXTENSION
#endif

#include <node.h>
#include <v8.h>

/**
 * declares a PtrWrap class for the type t
 */
#define PTRWRAP_DECLARE(t)													\
		template <> v8::Persistent<v8::Function> PtrWrap< t >::ctor_;		\

/**
 * initializes a PtrWrap class for the type t, to be called within the module's
 * initialization function
 */
#define PTRWRAP_INIT(t, o)													\
		PtrWrap< t >::Init( #t , o );										\


/**
 * wraps a pointer to the type T into a JSObject
 */
template<class T>
class PtrWrap : public node::ObjectWrap
{
private:
	/**
	 * the wrapped pointer
	 */
	T *ptr_;

	/**
	 * constructor function
	 */
	static v8::Persistent<v8::Function> ctor_;

public:
	/**
	 * test, if obj is an instance of this class, no inheritance
	 */
	static inline bool InstanceOf(v8::Handle<v8::Value> obj)
	{
		v8::HandleScope scope;
		if (obj.IsEmpty()) return false;
		if (!obj->IsObject()) return false;
		v8::Handle<v8::Object> asObj = obj.As<v8::Object>();
		return asObj->Get(v8::String::NewSymbol("constructor")) == ctor_;
	}

	/**
	 * initializes this class, will be called through PTRWRAP_INIT macro
	 */
	static inline v8::Persistent<v8::Function> Init(const char *name, v8::Handle<v8::Object> target)
	{
		v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
		v8::Local<v8::String> symbol = v8::String::NewSymbol(name);
		tpl->SetClassName(symbol);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		tpl->PrototypeTemplate()->Set(
				v8::String::NewSymbol("toString"),
				v8::FunctionTemplate::New(ToString)
		);
		tpl->ReadOnlyPrototype();
		ctor_ = v8::Persistent<v8::Function>::New(tpl->GetFunction());
		target->SetHiddenValue(symbol, ctor_);
		return ctor_;
	}

	/**
	 * unwrap the pointer, obj must be an instance of this class
	 */
	static inline T * Unwrap (v8::Handle<v8::Value> obj)
	{
		v8::HandleScope scope;
		return node::ObjectWrap::Unwrap<PtrWrap<T> >(obj->ToObject())->ptr_;
	}

	/**
	 * this class never will be instantiated from the VM,
	 * calls the private New method through the constructor field
	 */
	static inline v8::Handle<v8::Value> New(T* ptr)
	{
		v8::HandleScope scope;
		v8::Local<v8::Value> arg = v8::External::Wrap(ptr);
		v8::Local<v8::Object> instance = ctor_->NewInstance(1, &arg);
		return scope.Close(instance);
	}

private:
	inline PtrWrap() :
			ptr_(0)
	{
	}

	virtual ~PtrWrap()
	{
	}

	/**
	 * called by the constructor
	 */
	static v8::Handle<v8::Value> New(const v8::Arguments& args)
	{
		v8::HandleScope scope;
		if (!args.IsConstructCall()) {
			ThrowException(Exception::TypeError(String::New("Not constructed via new keyword.")));
			return scope.Close(v8::Undefined());
		}

		PtrWrap<T>* obj = new PtrWrap<T>();
		obj->ptr_ = (T*)(args[0]->IsUndefined() ? 0 : v8::External::Unwrap(args[0]));
		obj->Wrap(args.This());

		return args.This();
	}

	/**
	 * to string method
	 */
	static v8::Handle<v8::Value> ToString(const v8::Arguments& args) {
		v8::HandleScope scope;
		char buff[256];
		memset(buff, 0, 256);
		sprintf(buff, "[object WINDOW(%p)]", Unwrap(args.This()));
		return scope.Close(v8::String::New(buff));
	}

};

#endif /* PTRWRAP_H_ */
