/*
 *
 * wrapped_lib
 *
 * Created by El Khalil Bellakrid on 26/06/2019.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ledger
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */


#ifndef UBINDER_WRAPPED_LIB_H
#define UBINDER_WRAPPED_LIB_H

#include <vector>
#include <functional>

#include "stlab/concurrency/default_executor.hpp"
#include "stlab/concurrency/immediate_executor.hpp"
#include "stlab/concurrency/future.hpp"
#include "ubinder/wrapper_interface.h"
#include "ubinder/function_types.h"
#include "ubinder/cpp_wrapper.hpp"

#include "key_value_store.h"
#include "messages.pb.h"

using namespace key_value_protoc;

class KeyValueStorageLib {
public:
    std::string OnAddValue(const AddValueRequest& req);

    std::string OnGetValue(const GetValueRequest& req);

    void OnRequest(std::vector<uint8_t>&& data, std::function<void(std::vector<uint8_t>&&)> && callback);

    void OnNotification(std::vector<uint8_t>&& data);

public:
    key_value_store::Storage storage;
};


static ubinder::CppWrapper<KeyValueStorageLib> CppWrapperInstance;

void OnRequestFunc(uint32_t request, const char* data, size_t dataSize) {
    CppWrapperInstance.onRequest(request, data, dataSize);
}

void OnResponseFunc(uint32_t request, const char* data, size_t dataSize) {
    CppWrapperInstance.onResponse(request, data, dataSize);
}

void OnNotificationFunc(const char* data, size_t dataSize) {
    CppWrapperInstance.onNotification(data, dataSize);
}


#endif //UBINDER_WRAPPED_LIB_H
