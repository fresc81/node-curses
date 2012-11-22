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
#define PTRWRAP_DECLARE(t)																					\
		template <> v8::Persistent<v8::Function> PtrWrap< t >::ctor_ = PtrWrap< t >::Init( #t );			\


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
	static inline v8::Persistent<v8::Function> Init(const char *name)
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
		return v8::Persistent<v8::Function>::New(tpl->GetFunction());
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
		v8::Local<v8::Object> instance = ctor_->NewInstance();

		// ptr_ will always be NULL if instantiated by VM
		node::ObjectWrap::Unwrap<PtrWrap<T> >(instance)->ptr_ = ptr;

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
			v8::ThrowException(v8::Exception::TypeError(v8::String::New("Not constructed via new keyword.")));
			return scope.Close(v8::Undefined());
		}

		PtrWrap<T>* obj = new PtrWrap<T>();
		obj->ptr_ = NULL;
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
		v8::String::Utf8Value tp(args.This()->GetConstructorName());
		sprintf(buff, "[object %s(%p)]", *tp, Unwrap(args.This()));
		return scope.Close(v8::String::New(buff));
	}

};

#endif /* PTRWRAP_H_ */
