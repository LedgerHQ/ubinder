mkdir -p ./java/JavaKV/src/main/java
mkdir -p ./wrapped_lib/generated
protoc -I ./proto --cpp_out=./wrapped_lib/generated --java_out=./java/JavaKV/src/main/java --js_out=import_style=commonjs,binary:./node/generated ./proto/messages.proto
