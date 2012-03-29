#include "luv_stream.h" // uv_stream_prototype
#include "luv_tty.h"

#include "luv.h"
#include "candor.h"
#include "uv.h"

#include <assert.h> // assert

using namespace candor;

static Value* luv_create_tty(uint32_t argc, Value* argv[]) {
  assert(argc > 0);
  int fd = argv[0]->ToNumber()->IntegralValue();
  UVData* data = new UVData(sizeof(uv_tty_t), uv_tty_prototype());
  uv_tty_init(uv_default_loop(), (uv_tty_t*)data->handle, fd, argc > 1 ? argv[1]->ToBoolean()->IsTrue() : fd == 0);
  return *data->obj;
}

static Value* luv_tty_set_mode(uint32_t argc, Value* argv[]) {
  assert(argc == 2);
  Object* obj = argv[0]->As<Object>();
  uv_tty_t* handle = UVData::ObjectTo<uv_tty_t>(obj);
  int status = uv_tty_set_mode(handle, argv[1]->As<Boolean>()->IsTrue());
  return Number::NewIntegral(status);
}

static Handle<Object> module;
Object* uv_tty_module() {
  if (!module.IsEmpty()) return *module;
  module.Wrap(Object::New());
  module->Set("create", Function::New(luv_create_tty));
  return *module;
}

static Handle<Object> prototype;
Object* uv_tty_prototype() {
  if (!prototype.IsEmpty()) return *prototype;
  prototype.Wrap(uv_stream_prototype()->Clone());
  prototype->Set("setMode", Function::New(luv_tty_set_mode));
  return *prototype;
}


