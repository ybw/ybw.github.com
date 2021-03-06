/*
  Copyright (c) 2009 Dave Gamble
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef json__h
#define json__h

#ifdef __cplusplus
extern "C"
{
#endif

// json Types:
#define json_False 0
#define json_True 1
#define json_NULL 2
#define json_Number 3
#define json_String 4
#define json_Array 5
#define json_Object 6
	
#define json_IsReference 256

// The json structure:
typedef struct json {
	struct json *next,*prev;	// next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem
	struct json *child;		// An array or object item will have a child pointer pointing to a chain of the items in the array/object.

	int type;					// The type of the item, as above.

	char *valuestring;			// The item's string, if type==json_String
	int valueint;				// The item's number, if type==json_Number
	double valuedouble;			// The item's number, if type==json_Number

	char *string;				// The item's name string, if this item is the child of, or is in the list of subitems of an object.
} json;

typedef struct json_Hooks {
      void *(*malloc_fn)(size_t sz);
      void (*free_fn)(void *ptr);
} json_Hooks;

// Supply malloc, realloc and free functions to json
extern void json_InitHooks(json_Hooks* hooks);


// Supply a block of JSON, and this returns a json object you can interrogate. Call json_Delete when finished.
extern json *json_Parse(const char *value);
// Render a json entity to text for transfer/storage. Free the char* when finished.
extern char  *json_Print(json *item);
// Render a json entity to text for transfer/storage without any formatting. Free the char* when finished.
extern char  *json_PrintUnformatted(json *item);
// Delete a json entity and all subentities.
extern void   json_Delete(json *c);

// Returns the number of items in an array (or object).
extern int	  json_GetArraySize(json *array);
// Retrieve item number "item" from array "array". Returns NULL if unsuccessful.
extern json *json_GetArrayItem(json *array,int item);
// Get item "string" from object. Case insensitive.
extern json *json_GetObjectItem(json *object,const char *string);
	
// These calls create a json item of the appropriate type.
extern json *json_CreateNull();
extern json *json_CreateTrue();
extern json *json_CreateFalse();
extern json *json_CreateNumber(double num);
extern json *json_CreateString(const char *string);
extern json *json_CreateArray();
extern json *json_CreateObject();

// These utilities create an Array of count items.
extern json *json_CreateIntArray(int *numbers,int count);
extern json *json_CreateFloatArray(float *numbers,int count);
extern json *json_CreateDoubleArray(double *numbers,int count);
extern json *json_CreateStringArray(const char **strings,int count);

// Append item to the specified array/object.
extern void json_AddItemToArray(json *array, json *item);
extern void	json_AddItemToObject(json *object,const char *string,json *item);
// Append reference to item to the specified array/object. Use this when you want to add an existing json to a new json, but don't want to corrupt your existing json.
extern void json_AddItemReferenceToArray(json *array, json *item);
extern void	json_AddItemReferenceToObject(json *object,const char *string,json *item);

// Remove/Detatch items from Arrays/Objects.
extern json *json_DetachItemFromArray(json *array,int which);
extern void   json_DeleteItemFromArray(json *array,int which);
extern json *json_DetachItemFromObject(json *object,const char *string);
extern void   json_DeleteItemFromObject(json *object,const char *string);
	
// Update array items.
extern void json_ReplaceItemInArray(json *array,int which,json *newitem);
extern void json_ReplaceItemInObject(json *object,const char *string,json *newitem);

#define json_AddNullToObject(object,name)	json_AddItemToObject(object, name, json_CreateNull())
#define json_AddTrueToObject(object,name)	json_AddItemToObject(object, name, json_CreateTrue())
#define json_AddFalseToObject(object,name)		json_AddItemToObject(object, name, json_CreateFalse())
#define json_AddNumberToObject(object,name,n)	json_AddItemToObject(object, name, json_CreateNumber(n))
#define json_AddStringToObject(object,name,s)	json_AddItemToObject(object, name, json_CreateString(s))

#ifdef __cplusplus
}
#endif

#endif
