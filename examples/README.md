* key_value - simple library that simulate key-value store, each call to it takes 1s

NodeJS:
- First, you should build project (please refer to top `README.md`),
- Run `key_value/gen_proto.sh` or `key_value/genProto.cmd` script to generate protobuf files needed by this example,
- Then:
```
cd key_value/node
yarn 
``` 
- You can test by running:
```
node index.js
```