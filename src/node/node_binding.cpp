#include "node_binding.h"
#include <vector>
#include <iostream>
#include <functional>
#include <memory>

#include <nan.h>
#include <v8.h>
#include <nan_typedarray_contents.h>
#include <uv.h>

#include "ubinder/wrapper_interface.h"
#include "loops_tasks_queue.h"

namespace ubinder {

    NodeBinding::NodeBinding(uv_loop_t* loop)
        : _tasksToQueue(loop) {
    }

    void NodeBinding::SendRequest(std::vector<uint8_t>&& reqData, Callback onResponse) {
        // callback will be called from different thread, so we need to push a task on event loop
        auto& queue = _tasksToQueue;
        server->SendRequest(std::move(reqData), [onResponse, &queue] (std::vector<uint8_t>&& data){
            queue.PushTask([d{ std::move(data) }, onResponse]() mutable { onResponse(std::move(d)); });
        });
    }

    void NodeBinding::SendNotification(std::vector<uint8_t>&& reqData) {
        server->SendNotification(std::forward<std::vector<uint8_t>>(reqData));
    }
};

static std::unique_ptr<ubinder::NodeBinding> nodeBinding;

NAN_METHOD(getLength){
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::cout << buff.length() << std::endl;
}

NAN_METHOD(thereAndBack) {
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::vector<uint8_t> cpp(*buff, *buff + buff.length());
    info.GetReturnValue().Set(Nan::CopyBuffer((char*)cpp.data(), cpp.size()).ToLocalChecked());
}

NAN_METHOD(Method) {
    std::vector<uint8_t> some_data {0,1,2,3,4,5,6,7,8,9,10};
    info.GetReturnValue().Set(Nan::CopyBuffer((char*)some_data.data(), some_data.size()).ToLocalChecked());
}

NAN_METHOD(sendRequest) {
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::vector<uint8_t> cpp(*buff, *buff + buff.length());
    auto callback = std::make_shared<Nan::Callback>(info[1].As<v8::Function>());
    ubinder::Callback lmb([callback](std::vector<uint8_t>&& data){
        Nan::HandleScope scope;
        v8::Local<v8::Value> argv[] = { Nan::CopyBuffer((char*)data.data(), data.size()).ToLocalChecked() };
        callback->Call(1, argv);
    });
    nodeBinding->SendRequest(std::move(cpp), std::move(lmb));
}

NAN_METHOD(sendNotification) {
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::vector<uint8_t> cpp(*buff, *buff + buff.length());
    nodeBinding->SendNotification(std::move(cpp));
}

NAN_METHOD(responseCallback) {
    info.GetReturnValue().Set(Nan::New("hello world").ToLocalChecked());
}

void CreateFunction(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    

    // omit this to make it anonymous
    fn->SetName(Nan::New("theFunction").ToLocalChecked());

    info.GetReturnValue().Set(fn);
}

NAN_METHOD(registerLib) {
    auto onRequest = std::make_shared<Nan::Callback>(info[0].As<v8::Function>());
    auto onNotification = std::make_shared<Nan::Callback>(info[1].As<v8::Function>());
    ubinder::OnRequest lmbRequest([onRequest](std::vector<uint8_t> && data, Callback onResponse) {
        Nan::HandleScope scope;
        v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(responseCallback);
        v8::Local<v8::Function> fn = tpl->GetFunction();
        v8::Local<v8::Value> argv[] = {
            Nan::CopyBuffer((char*)data.data(), data.size()).ToLocalChecked(),
            fn
        };
        onRequest->Call(2, argv);
        });
    nodeBinding->RegisterServer(std::move(lmbRequest), std::move(lmb));
}





NAN_MODULE_INIT(Init) {
    nodeBinding = std::make_unique<ubinder::NodeBinding>(uv_default_loop());
    NAN_EXPORT(target, Method);
    NAN_EXPORT(target, getLength);
    NAN_EXPORT(target, thereAndBack);
    NAN_EXPORT(target, sendRequest);
}

NODE_MODULE(hello, Init)