{
  'variables': {
  	'target_module_name': 'key_store_node',
    'conditions': [
        ['OS=="win"', {"wrapper_libs": "../../../../build/examples/key_value/wrapped_lib/Debug/wrapped_lib.lib"}],
        ['OS=="linux"',{"wrapper_libs": "../../../../build/examples/key_value/wrapped_lib/libwrapped_lib.so"}],
        ['OS=="mac"',{"wrapper_libs": "../../../../build/examples/key_value/wrapped_lib/libwrapped_lib.dylib"}]
    ],
  },
  'includes': ['../../../src/node/ubinder.gypi'],
  'copies':
   [
	  {
		'destination': '<(module_root_dir)/build/Release/',
		'files': [ '<(module_root_dir)/ubinder/<(wrapper_libs)' ]
	  }
   ]
}
