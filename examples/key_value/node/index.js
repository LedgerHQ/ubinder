const key_store_node = require('bindings')('key_store_node')
const ubinder = require('../../../src/node/ubinder');
const messages = require('./generated/messages_pb')

const logger = (method, ...params) => {
    const now = new Date();
    console.log(`${now.toISOString()}: Calling ${method} with params: ${params}`)
}

const OnRequest = (data, callback) => {
    logger("OnRequest", data);
}

const OnNotification = data => {
    logger("OnNotification", data);
}

const callbacker = new ubinder.Callbacker(key_store_node, OnRequest, OnNotification);

const createAddValue = (key, value) => {
    logger("createAddValue", key, value);
    const addVal = new messages.AddValueRequest();
    addVal.setKey(key);
    addVal.setValue(value);
    const req = new messages.Request();
    req.setRequestType(messages.RequestType.ADD_VALUE);
    req.setRequestBody(addVal.serializeBinary());
    return req;
}

const createGetValue = key => {
    logger("createGetValue", key);
    const getVal = new messages.GetValueRequest();
    getVal.setKey(key);
    const req = new messages.Request();
    req.setRequestType(messages.RequestType.GET_VALUE);
    req.setRequestBody(getVal.serializeBinary());
    return req;
}

const sendAddValue = async (key, value) => {
    logger("sendAddValue", key, value);
    const msg = createAddValue(key, value).serializeBinary();
    const data = await callbacker.sendRequest(msg);
    const resp = messages.AddValueResponse.deserializeBinary(data);
    if (resp.getError()!='') {
        throw new Error(resp.getError());
    }
}

const sendGetValue = async key => {
    logger("sendGetValue", key);
    const msg = createGetValue(key).serializeBinary();
    const data = await callbacker.sendRequest(msg);
    const resp = messages.GetValueResponse.deserializeBinary(data);
    if (resp.getError()=='') {
        return resp.getValue();
    }
    throw new Error(resp.getError());
}


const run_test = async () => {
    logger("run_test");
    const send1 = sendAddValue("key1", "value1")
        .then(() => sendGetValue("key1"))
        .then(val => console.log(`Received ${val} from key1`))
        .catch(x=> console.log(x));
    const send2 = sendAddValue("key2", "value2")
        .then(() => sendGetValue("key2"))
        .then(val => console.log(`Received ${val} from key2`))
        .catch(x=> console.log(x));
    const send3 = sendAddValue("key3", "value3")
        .then(() => sendGetValue("key3"))
        .then(val => console.log(`Received ${val} from key3`))
        .catch(x=> console.log(x));
    await Promise.all([send1, send2, send3]);

    // TODO: this should exit
    //process.exit(0);
    // But it seems that there are some handles preventing from that
    // so the node process hangs
    const handles = process._getActiveHandles();
    console.log(requests)
}

run_test();
