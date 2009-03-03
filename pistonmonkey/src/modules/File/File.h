#ifndef PISTONMONKEY_FILE_H_
#define PISTONMONKEY_FILE_H_

typedef struct
{
	FILE * 	fp;
} File_data;

void	File_preinit();
void 	File_init(JSContext * cx, JSObject * obj);

LM_PROPERTY_SET_FUNCTION_REF(File_property_path_set)

JSBool 	File_constructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval);
void 	File_deconstructor(JSContext * cx, JSObject * obj);

LM_FUNCTION_REF(File_function_exists)
LM_FUNCTION_REF(File_function_exists_static)

LM_FUNCTION_REF(File_function_isDirectory)
LM_FUNCTION_REF(File_function_isDirectory_static)

LM_FUNCTION_REF(File_function_isFile)
LM_FUNCTION_REF(File_function_isFile_static)

LM_FUNCTION_REF(File_function_readAll)
LM_FUNCTION_REF(File_function_readAll_static)

bool 	File_function_size_primitive(char * path, off_t * size);
JSBool 	File_function_size_internal(JSContext * cx, char * path, jsval * rval);
LM_FUNCTION_REF(File_function_size)
LM_FUNCTION_REF(File_function_size_static)

#endif /* PISTONMONKEY_FILE_H_ */
