/**
 * @author	Artjom Siebert
 * @date	04.01.2015
 *
 * @brief
 */

#ifndef	HUEWRAPPER_H_
#define HUEWRAPPER_H_

/*Includes*/

#include <nan.h>
extern "C" {
	#include "ETRX357.h"
}

using namespace v8;
using namespace std;

class HueWrapper : public node::ObjectWrap {

private:
static v8::Persistent<v8::Function> constructor;

explicit HueWrapper();
~HueWrapper();

double counter_;
static NAN_METHOD(New);
static NAN_METHOD(ConnectHue);

public:
static void Init(v8::Handle<v8::Object> exports);

};
#endif
