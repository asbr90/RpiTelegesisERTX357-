/**
 * @author	Artjom Siebert
 * @date	04.01.2015
 *
 */
#include <nan.h>
#include "HueWrapper.h"

using namespace v8;

HueWrapper::HueWrapper() {
}
;
HueWrapper::~HueWrapper() {
}
;

Persistent<Function> HueWrapper::constructor;

void HueWrapper::Init(v8::Handle<v8::Object> exports) {
	Local < FunctionTemplate > tpl = NanNew < FunctionTemplate > (New);

	tpl->SetClassName(NanNew("HueWrapper"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	tpl->PrototypeTemplate()->Set(NanNew("connect"),
			NanNew < FunctionTemplate > (ConnectHue)->GetFunction());

	NanAssignPersistent(constructor, tpl->GetFunction());
	exports->Set(NanNew("HueWrapper"), tpl->GetFunction());
}

NAN_METHOD(HueWrapper::New) {
	NanScope();

	HueWrapper* obj = new HueWrapper();

	NanReturnNull();
}

NAN_METHOD(HueWrapper::ConnectHue) {
	NanScope();
	bootload();
	NanReturnValue(NanNew<String>("Scaned network"));
}
