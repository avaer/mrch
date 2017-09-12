#include <node.h>
#include "march.h"
#include <iostream>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Value;
using v8::Array;
using v8::ArrayBuffer;
using v8::Float32Array;
using v8::Uint32Array;
 
void MarchCubes(const FunctionCallbackInfo<Value>& args) {
  unsigned int positionIndex;
  unsigned int faceIndex;

  Local<String> bufferString = String::NewFromUtf8(args.GetIsolate(), "buffer");
  Local<String> byteOffsetString = String::NewFromUtf8(args.GetIsolate(), "byteOffset");
  Local<String> positionsString = String::NewFromUtf8(args.GetIsolate(), "positions");
  Local<String> indicesString = String::NewFromUtf8(args.GetIsolate(), "indices");

  Local<Array> dimsArg = Local<Array>::Cast(args[0]);
  Local<ArrayBuffer> potentialBuffer = Local<ArrayBuffer>::Cast(args[1]->ToObject()->Get(bufferString));
  unsigned int potentialByteOffset = args[1]->ToObject()->Get(byteOffsetString)->Uint32Value();
  Local<Array> shiftArg = Local<Array>::Cast(args[2]);
  Local<ArrayBuffer> positionsBuffer = Local<ArrayBuffer>::Cast(args[3]->ToObject()->Get(bufferString));
  unsigned int positionsByteOffset = args[3]->ToObject()->Get(byteOffsetString)->Uint32Value();
  Local<ArrayBuffer> facesBuffer = Local<ArrayBuffer>::Cast(args[4]->ToObject()->Get(bufferString));
  unsigned int facesByteOffset = args[4]->ToObject()->Get(byteOffsetString)->Uint32Value();

  int dims[3] = {
    dimsArg->Get(0)->Int32Value(),
    dimsArg->Get(1)->Int32Value(),
    dimsArg->Get(2)->Int32Value()
  };
  int shift[3] = {
    Local<Array>::Cast(shiftArg->Get(0))->Get(0)->Int32Value(),
    Local<Array>::Cast(shiftArg->Get(0))->Get(1)->Int32Value(),
    Local<Array>::Cast(shiftArg->Get(0))->Get(2)->Int32Value()
  };

  float *potential = (float *)((char *)potentialBuffer->GetContents().Data() + potentialByteOffset);
  float *positions = (float *)((char *)positionsBuffer->GetContents().Data() + positionsByteOffset);
  unsigned int *faces = (unsigned int *)((char *)facesBuffer->GetContents().Data() + facesByteOffset);

  // std::cout << "got init " << potential[99] << ":" << potential[100] << ":" << shift[0] << ":" << shift[1] << ":" << shift[2] << "\n";

  marchingCubes(dims, potential, shift, positions, faces, positionIndex, faceIndex);

  Local<Object> result = Object::New(args.GetIsolate());
  result->Set(positionsString, Float32Array::New(positionsBuffer, positionsByteOffset, positionIndex));
  result->Set(indicesString, Uint32Array::New(facesBuffer, facesByteOffset, faceIndex));
  args.GetReturnValue().Set(result);
}

void InitAll(Local<Object> exports, Local<Object> module) {
  NODE_SET_METHOD(module, "exports", MarchCubes);
}

NODE_MODULE(addon, InitAll)
