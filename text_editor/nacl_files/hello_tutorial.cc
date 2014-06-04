// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include "rope.h"
#include <string>
#include <cstdlib>
#include <cstdio>

#define SPLAY 's'
#define INSERT 'i'
#define REMOVE 'r'
#define QUERY 'q'
#define GET 'g'
#define JSON 'j'

namespace {
  Rope *r = NULL;
}  // namespace

class HelloTutorialInstance : public pp::Instance {
 public:
  explicit HelloTutorialInstance(PP_Instance instance)
      : pp::Instance(instance) {}
  virtual ~HelloTutorialInstance() {}

  virtual void HandleMessage(const pp::Var& var_message) {
    // Ignore the message if it is not a string.
    if (!var_message.is_string())
      return;

    std::string message = var_message.AsString();

    if (r == NULL) {
      bool splay = (message[0] == SPLAY);
      r = new Rope(splay);
      return;
    }

    int space = message.find_first_of(' ');
    switch (message[0]) {
      case INSERT: {
        int index = atoi(message.substr(1, space).c_str());
        std::string *append = new std::string(message.substr(space + 1));
        r = r->insert(index, append);
        pp::Var var_reply(r->report(0, r->length()));
        PostMessage(var_reply);
        break;
                   }
      case REMOVE: {
        int start = atoi(message.substr(1, space).c_str());
        int end = atoi(message.substr(space + 1).c_str());
        r = r->remove(start, end);
        pp::Var var_reply(r->report(0, r->length()));
        PostMessage(var_reply);
        break;
                   }
      case QUERY: {
        int index = atoi(message.substr(1, space).c_str());
        r->report(index, index + 1);
        pp::Var var_reply(r->json());
        PostMessage(var_reply);
        break;
                  }
      case GET: {
        pp::Var var_reply(r->report(0, r->length()));
        PostMessage(var_reply);
        break;
                 }
      case JSON: {
        pp::Var var_reply(r->json());
        PostMessage(var_reply);
        break;
                 }
    }
  }
};

class HelloTutorialModule : public pp::Module {
 public:
  HelloTutorialModule() : pp::Module() {}
  virtual ~HelloTutorialModule() {}

  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new HelloTutorialInstance(instance);
  }
};

namespace pp {

Module* CreateModule() {
  return new HelloTutorialModule();
}

}  // namespace pp
