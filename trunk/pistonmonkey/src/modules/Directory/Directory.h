#ifndef PISTONMONKEY_DIRECTORY_H_
#define PISTONMONKEY_DIRECTORY_H_

typedef struct
{
} Directory_data;

void	Directory_preinit();
void 	Directory_init(JSContext * cx, JSObject * obj);

LM_PROPERTY_SET_FUNCTION_REF(Directory_property_path_set)

JSBool 	Directory_constructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval);
void 	Directory_deconstructor(JSContext * cx, JSObject * obj);

LM_FUNCTION_REF(Directory_function_exists)
LM_FUNCTION_REF(Directory_function_exists_static)

LM_FUNCTION_REF(Directory_function_isDirectory)
LM_FUNCTION_REF(Directory_function_isDirectory_static)

LM_FUNCTION_REF(Directory_function_isFile)
LM_FUNCTION_REF(Directory_function_isFile_static)

#endif /* PISTONMONKEY_DIRECTORY_H_ */
