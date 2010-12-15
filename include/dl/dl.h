/* copyright (c) 2010 Fredrik Kihlander, see LICENSE for more info */

#ifndef DL_DL_H_INCLUDED
#define DL_DL_H_INCLUDED

/*
	File: dl.h
*/

#include <dl/dl_bit.h>
#include <dl/dl_defines.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/*
	Handle: dl_ctx_t
*/
typedef struct dl_context* dl_ctx_t;

/*
	Enum: dl_error_t
		Error-codes from DL

	DL_ERROR_OK                                            - All went ok!
	DL_ERROR_MALFORMED_DATA                                - The data was not valid DL-data.
	DL_ERROR_VERSION_MISMATCH                              - Data created with another version of DL.
	DL_ERROR_OUT_OF_LIBRARY_MEMORY                         - Out of memory.
	DL_ERROR_OUT_OF_INSTANCE_MEMORY                        - Out of instance memory.
	DL_ERROR_DYNAMIC_SIZE_TYPES_AND_NO_INSTANCE_ALLOCATOR  - DL would need to do a dynamic allocation but has now allocator set for that.
	DL_ERROR_TYPE_MISMATCH                                 - Expected type A but found type B.
	DL_ERROR_TYPE_NOT_FOUND                                - Could not find a requested type. Is the correct type library loaded?
	DL_ERROR_MEMBER_NOT_FOUND                              - Could not find a requested member of a type.
	DL_ERROR_BUFFER_TO_SMALL                               - Provided buffer is to small.
	DL_ERROR_ENDIAN_MISMATCH                               - Endianness of provided data is not the same as the platforms.
	DL_ERROR_UNSUPPORTED_OPERATION                         - The operation is not supported by dl-function.

	DL_ERROR_TXT_PARSE_ERROR                               - Syntax error while parsing txt-file. Check log for details.
	DL_ERROR_TXT_MEMBER_MISSING                            - A member is missing in a struct and in do not have a default value.
	DL_ERROR_TXT_MEMBER_SET_TWICE                          - A member is set twice in one struct.

	DL_ERROR_UTIL_FILE_NOT_FOUND                           - A argument-file is not found.
	DL_ERROR_UTIL_FILE_TYPE_MISMATCH                       - File type specified to read do not match file content.

	DL_ERROR_INTERNAL_ERROR                                - Internal error, contact dev!
*/
enum dl_error_t
{
	DL_ERROR_OK,
	DL_ERROR_MALFORMED_DATA,
	DL_ERROR_VERSION_MISMATCH,
	DL_ERROR_OUT_OF_LIBRARY_MEMORY,
	DL_ERROR_OUT_OF_INSTANCE_MEMORY,
	DL_ERROR_DYNAMIC_SIZE_TYPES_AND_NO_INSTANCE_ALLOCATOR,
	DL_ERROR_TYPE_MISMATCH,
	DL_ERROR_TYPE_NOT_FOUND,
	DL_ERROR_MEMBER_NOT_FOUND,
	DL_ERROR_BUFFER_TO_SMALL,
	DL_ERROR_ENDIAN_MISMATCH,
	DL_ERROR_INVALID_PARAMETER,
	DL_ERROR_UNSUPPORTED_OPERATION,

	DL_ERROR_TXT_PARSE_ERROR,
	DL_ERROR_TXT_MEMBER_MISSING,
	DL_ERROR_TXT_MEMBER_SET_TWICE,

	DL_ERROR_UTIL_FILE_NOT_FOUND,
	DL_ERROR_UTIL_FILE_TYPE_MISMATCH,

	DL_ERROR_INTERNAL_ERROR
};

/*
	Enum: EDLType
		Enumeration that describes a specific type in DL.
*/
enum dl_type_t
{
	// Type-layout
	DL_TYPE_ATOM_MIN_BIT            = 0,
	DL_TYPE_ATOM_MAX_BIT            = 7,
	DL_TYPE_STORAGE_MIN_BIT         = 8,
	DL_TYPE_STORAGE_MAX_BIT         = 15,
	DL_TYPE_BITFIELD_SIZE_MIN_BIT   = 16,
	DL_TYPE_BITFIELD_SIZE_MAX_BIT   = 23,
	DL_TYPE_BITFIELD_OFFSET_MIN_BIT = 24,
	DL_TYPE_BITFIELD_OFFSET_MAX_BIT = 31,

	// Field sizes
	DL_TYPE_BITFIELD_SIZE_BITS_USED   = DL_TYPE_BITFIELD_SIZE_MAX_BIT + 1   - DL_TYPE_BITFIELD_SIZE_MIN_BIT,
	DL_TYPE_BITFIELD_OFFSET_BITS_USED = DL_TYPE_BITFIELD_OFFSET_MAX_BIT + 1 - DL_TYPE_BITFIELD_OFFSET_MIN_BIT,

	// Masks
	DL_TYPE_ATOM_MASK            = DL_BITRANGE(DL_TYPE_ATOM_MIN_BIT,            DL_TYPE_ATOM_MAX_BIT),
	DL_TYPE_STORAGE_MASK         = DL_BITRANGE(DL_TYPE_STORAGE_MIN_BIT,         DL_TYPE_STORAGE_MAX_BIT),
	DL_TYPE_BITFIELD_SIZE_MASK   = DL_BITRANGE(DL_TYPE_BITFIELD_SIZE_MIN_BIT,   DL_TYPE_BITFIELD_SIZE_MAX_BIT),
	DL_TYPE_BITFIELD_OFFSET_MASK = DL_BITRANGE(DL_TYPE_BITFIELD_OFFSET_MIN_BIT, DL_TYPE_BITFIELD_OFFSET_MAX_BIT),

	// Atomic types
	DL_TYPE_ATOM_POD          = DL_INSERT_BITS(0x00000000, 1, DL_TYPE_ATOM_MIN_BIT, DL_TYPE_ATOM_MAX_BIT + 1),
	DL_TYPE_ATOM_ARRAY        = DL_INSERT_BITS(0x00000000, 2, DL_TYPE_ATOM_MIN_BIT, DL_TYPE_ATOM_MAX_BIT + 1),
	DL_TYPE_ATOM_INLINE_ARRAY = DL_INSERT_BITS(0x00000000, 3, DL_TYPE_ATOM_MIN_BIT, DL_TYPE_ATOM_MAX_BIT + 1),
	DL_TYPE_ATOM_BITFIELD     = DL_INSERT_BITS(0x00000000, 4, DL_TYPE_ATOM_MIN_BIT, DL_TYPE_ATOM_MAX_BIT + 1),

	// Storage type
	DL_TYPE_STORAGE_INT8   = DL_INSERT_BITS(0x00000000,  1, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_INT16  = DL_INSERT_BITS(0x00000000,  2, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_INT32  = DL_INSERT_BITS(0x00000000,  3, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_INT64  = DL_INSERT_BITS(0x00000000,  4, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_UINT8  = DL_INSERT_BITS(0x00000000,  5, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_UINT16 = DL_INSERT_BITS(0x00000000,  6, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_UINT32 = DL_INSERT_BITS(0x00000000,  7, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_UINT64 = DL_INSERT_BITS(0x00000000,  8, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_FP32   = DL_INSERT_BITS(0x00000000,  9, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_FP64   = DL_INSERT_BITS(0x00000000, 10, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_STR    = DL_INSERT_BITS(0x00000000, 11, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_PTR    = DL_INSERT_BITS(0x00000000, 12, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_STRUCT = DL_INSERT_BITS(0x00000000, 13, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),
	DL_TYPE_STORAGE_ENUM   = DL_INSERT_BITS(0x00000000, 14, DL_TYPE_STORAGE_MIN_BIT, DL_TYPE_STORAGE_MAX_BIT + 1),

	DL_TYPE_FORCE_32_BIT = 0x7FFFFFFF
};

enum dl_endian_t
{
	DL_ENDIAN_BIG,
	DL_ENDIAN_LITTLE,
};

DL_FORCEINLINE dl_endian_t dl_endian_host()
{
	union { unsigned char c[4]; unsigned int  i; } test;
	test.i = 0xAABBCCDD;
	return test.c[0] == 0xAA ? DL_ENDIAN_BIG : DL_ENDIAN_LITTLE;
}

#define DL_ENDIAN_HOST dl_endian_host()

/*
	Struct: dl_create_params_t
		Passed with initialization parameters to dl_context_create.
		This struct is open to change in later versions of dl.

	Members:
		alloc_func - function called by dl to allocate memory, set to 0x0 to use malloc
		free_func  - function called by dl to free memory, set to 0x0 to use free
		alloc_ctx  - parameter passed to alloc_func/free_func for userdata.
*/
typedef struct dl_create_params
{
	void* (*alloc_func)( unsigned int size, unsigned int alignment, void* alloc_ctx );
	void  (*free_func) ( void* ptr, void* alloc_ctx );
	void* alloc_ctx;
} dl_create_params_t;

/*
	Macro: DL_CREATE_PARAMS_SET_DEFAULT
		The preferred way to initialize dl_create_params_t is with this
		This macro will set default values that might not be optimal but
		is supposed to support all usecases of dl.
		This should be used to not get uninitialized members if create_params_t
		is extended.

	Example:
		dl_create_params_t p;
		DL_CREATE_PARAMS_SET_DEFAULT(p);
		p.alloc_func = my_func
*/
#define DL_CREATE_PARAMS_SET_DEFAULT( params ) \
		params.alloc_func = 0x0; \
		params.free_func  = 0x0; \
		params.alloc_ctx  = 0x0;

/*
	Group: Context
*/

/*
	Function: dl_context_create
		Creates a context.

	Parameters:
		dl_ctx        - Ptr to instance to create.
		create_params - Parameters to control the construction of the dl context. See DL_CREATE_PARAMS_SET_DEFAULT
		                for usage.
*/
dl_error_t DL_DLL_EXPORT dl_context_create( dl_ctx_t* dl_ctx, dl_create_params_t* create_params );

/*
	Function: dl_context_destroy
		Destroys a context and free all memory allocated with the DLAllocFuncs-functions.
*/
dl_error_t DL_DLL_EXPORT dl_context_destroy( dl_ctx_t dl_ctx );

/*
	Function: dl_context_load_type_library
		Load a type-library from bin-data into the context for use.
		One context can have multiple type libraries loaded and reference types within the different ones.

	Parameters:
		dl_ctx        - Context to load type-library into.
		lib_data      - Pointer to binary-data with type-library.
		lib_data_size - Size of _pData.
*/
dl_error_t DL_DLL_EXPORT dl_context_load_type_library( dl_ctx_t dl_ctx, const unsigned char* lib_data, unsigned int lib_data_size );


/*
	Group: Load
*/

/*
	Function: dl_instance_load
		Load instances inplace from a binary blob of data.
		Loads the instance to the memory area pointed to by _pInstance. Will return error if type is not constant size!

	Parameters:
		dl_ctx               - Context to load type-library into.
		dl_typeid            - Type of instance in the packed data.
		instance             - Ptr where to load the instance to.
		packed_instance      - Ptr to binary data to load from.
		packed_instance_size - Size of _pData.

	Note:
		Packed instance to load is required to be in current platform endian, if not DL_ERROR_ENDIAN_ERROR will be returned.
*/
dl_error_t DL_DLL_EXPORT dl_instance_load( dl_ctx_t dl_ctx, dl_typeid_t type, void* instance, const unsigned char* packed_instance, unsigned int packed_instance_size );

/*
	Group: Store
*/
/*
	Function: dl_instace_calc_size
		Calculate size needed to store instance.

	Parameters:
		dl_ctx   - Context to load type-library into.
		type     - Type id for type to store.
		instance - Ptr to instance to calculate size of.
		out_size - Ptr where to store the amount of bytes needed to store the instances.
*/
dl_error_t DL_DLL_EXPORT dl_instance_calc_size( dl_ctx_t dl_ctx, dl_typeid_t type, void* instance, unsigned int* out_size);

/*
	Function: dl_instace_store
		Store the instances.

	Parameters:
		dl_ctx            - Context to load type-library into.
		type              - Type id for type to store.
		instance          - Ptr to instance to store.
		out_instance      - Ptr to memory-area where to store the instances.
		out_instance_size - Size of _pData.

	Note:
		The instance after pack will be in current platform endian.
*/
dl_error_t DL_DLL_EXPORT dl_instance_store( dl_ctx_t dl_ctx, dl_typeid_t type, void* instance, unsigned char* out_instance, unsigned int out_instance_size );


/*
	Group: Util
*/

/*
	Function: dl_error_to_string
		Converts EDLError to string.
*/
DL_DLL_EXPORT const char* dl_error_to_string( dl_error_t error );

typedef struct dl_instance_info
{
	unsigned int ptrsize;
	dl_endian_t  endian;
	dl_typeid_t  root_type;
} dl_instance_info_t;

/*
	Function: dl_instance_get_info
		Fetch information about a packed dl-instance.

	Parameters:
		packed_instance      - Ptr to memory-area where packed instance is to be found.
		packed_instance_size - Size of packed_instance.
		out_info             - Ptr to dl_instance_info where to return info.

	Return:
		DL_ERROR_OK on success.
*/
dl_error_t dl_instance_get_info( const unsigned char* packed_instance, unsigned int packed_instance_size, dl_instance_info_t* out_info );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DL_DL_H_INCLUDED
