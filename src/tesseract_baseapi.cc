/*
 * Copyright by Alexander Makarenko
 *
 * See license text in LICENSE file
 */

#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "tesseract_baseapi.h"
#include "leptonica_pix.h"

using namespace v8;


BaseApi::BaseApi() {
  ocr = new tesseract::TessBaseAPI();
};


BaseApi::~BaseApi() {
  ocr->Clear();
  ocr->End();
};


void BaseApi::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  t->SetClassName(String::NewSymbol("BaseApi"));
  t->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  t->PrototypeTemplate()->Set(
    String::NewSymbol("clear"),
    FunctionTemplate::New(Clear)->GetFunction());

  t->PrototypeTemplate()->Set(
    String::NewSymbol("end"),
    FunctionTemplate::New(End)->GetFunction());

  t->PrototypeTemplate()->Set(
    String::NewSymbol("init"),
    FunctionTemplate::New(Init)->GetFunction());

  t->PrototypeTemplate()->Set(
    String::NewSymbol("setImage"),
    FunctionTemplate::New(SetImage)->GetFunction());

  t->PrototypeTemplate()->Set(
    String::NewSymbol("recognize"),
    FunctionTemplate::New(Recognize)->GetFunction());

  t->PrototypeTemplate()->Set(
    String::NewSymbol("getText"),
    FunctionTemplate::New(GetText)->GetFunction());

  Persistent<Function> constructor = Persistent<Function>::New(t->GetFunction());
  target->Set(String::NewSymbol("BaseApi"), constructor);
}


Handle<Value> BaseApi::New(const Arguments& args) {
  HandleScope scope;

  BaseApi* obj = new BaseApi();
  obj->Wrap(args.This());

  return args.This();
}


Handle<Value> BaseApi::Init(const Arguments& args) {
  HandleScope scope;
  REQUIRE_STRING(arg_lang, 0);

  char *lang;
  if (arg_lang.length()) {
    lang = strdup(*arg_lang);
  } else {
    lang = (char *) "eng";
  }

  BaseApi* obj = ObjectWrap::Unwrap<BaseApi>(args.This());
  int ret = obj->ocr->Init(NULL, lang);
  return scope.Close(Integer::New(ret));
}


Handle<Value> BaseApi::SetImage(const Arguments& args) {
  HandleScope scope;
  REQUIRE_ARG_NUM(1);

  PIX* pix;
  if (args[0]->IsString()) {
    pix = pixRead(strdup(*(String::Utf8Value(args[0]))));
  } else {
    PixWrap* pixWrap = ObjectWrap::Unwrap<PixWrap>(args[0]->ToObject());
    pix = pixWrap->data();
  }

  if (pix == NULL) {
    return ThrowException(Exception::Error(
    String::New("Image was not found or has unsupported format.")));
  }

  BaseApi* obj = ObjectWrap::Unwrap<BaseApi>(args.This());
  obj->ocr->SetImage(pix);
  return scope.Close(Null());
}


Handle<Value> BaseApi::Recognize(const Arguments& args) {
  HandleScope scope;

  BaseApi* obj = ObjectWrap::Unwrap<BaseApi>(args.This());
  obj->ocr->Recognize(NULL);
  return scope.Close(Null());
}


Handle<Value> BaseApi::GetText(const Arguments& args) {
  HandleScope scope;

  BaseApi* obj = ObjectWrap::Unwrap<BaseApi>(args.This());
  char *text = obj->ocr->GetUTF8Text();
  return scope.Close(String::New(text));
}


Handle<Value> BaseApi::Clear(const Arguments& args) {
  HandleScope scope;

  BaseApi* obj = ObjectWrap::Unwrap<BaseApi>(args.This());
  obj->ocr->Clear();
  return scope.Close(Null());
}


Handle<Value> BaseApi::End(const Arguments& args) {
  HandleScope scope;

  BaseApi* obj = ObjectWrap::Unwrap<BaseApi>(args.This());
  obj->ocr->End();
  return scope.Close(Null());
}