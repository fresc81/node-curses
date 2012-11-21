/*
 * ptrwrap.h
 *
 *  Created on: 21.11.2012
 *      Author: Paul Bottin
 */

#ifndef PTRWRAP_H_
#define PTRWRAP_H_

#define BUILDING_NODE_EXTENSION

#include <node.h>
#include <v8.h>

#include <typeinfo>

template<class T>
class PtrWrap: public node::ObjectWrap
{
private:
	T *ptr_;
	static v8::Persistent<v8::Function> constructor;

public:
	static inline void Init()
	{
		v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
		tpl->SetClassName(v8::String::NewSymbol(typeid(T).name()));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	}

	inline T * Unwrap ()
	{
		return ptr_;
	}

	inline T * operator &()
	{
		return ptr_;
	}

	static inline v8::Handle<v8::Value> New(T* ptr)
	{
		v8::HandleScope scope;
		const unsigned argc = 1;
		v8::Local<v8::Number> arg0 = v8::Number::New(reinterpret_cast<long>(ptr));
		v8::Handle<v8::Value> argv[argc] = {
				arg0
		};
		v8::Local<v8::Object> instance = constructor->NewInstance(argc, argv);
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

	static inline v8::Handle<v8::Value> New(const v8::Arguments& args)
	{
		v8::HandleScope scope;
		PtrWrap<T>* obj = new PtrWrap<T>();
		obj->ptr_ = (T*)(args[0]->IsUndefined() ? 0 : args[0]->IntegerValue());
		obj->Wrap(args.This());
		return args.This();
	}

};

#endif /* PTRWRAP_H_ */
