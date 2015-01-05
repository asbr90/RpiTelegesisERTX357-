#include <nan.h>

using namespace v8;

NAN_METHOD(Method) {
  NanScope();
  NanReturnValue(String::New("Hello world"));
}

void Init(Handle<Object> exports) {
  exports->Set(NanSymbol("hellos"), FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(hello, Init)
