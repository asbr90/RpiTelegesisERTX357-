/**
 * @author	Artjom Siebert
 * @date	04.01.2015
 */

#include <nan.h>
#include "HueWrapper.h"


void Init(Handle<Object> exports, Handle<Object> module) {
	NanScope();

	HueWrapper::Init(exports);


}
NODE_MODULE(hue, Init)

