#ifndef META_H
#define META_H

#include <stdlib.h>     // malloc, realloc, free
#include <string.h>     // strlen

#define meta_offset(T, E) ((size_t)(&(((T*)(0))->E)))
#define meta_to_str(T) ((const char*)#T)

typedef enum meta_property_type
{
    META_PROPERTY_TYPE_U8 = 0x00,
    META_PROPERTY_TYPE_U16,
    META_PROPERTY_TYPE_U32,
    META_PROPERTY_TYPE_U64,
    META_PROPERTY_TYPE_S8,
    META_PROPERTY_TYPE_S16,
    META_PROPERTY_TYPE_S32,
    META_PROPERTY_TYPE_S64,
    META_PROPERTY_TYPE_F32,
    META_PROPERTY_TYPE_F64,
    META_PROPERTY_TYPE_SIZE_T,
    META_PROPERTY_TYPE_STR,
    META_PROPERTY_TYPE_COUNT
} meta_property_type;

typedef struct meta_property_type_info_t 
{
    const char* name;       // Display name
    uint32_t id;            // Matches up to property type, used for lookups and switch statements
} meta_property_type_info_t;

extern meta_property_type_info_t _meta_property_type_info_decl_impl(const char* name, uint32_t id);

#define meta_property_type_info_decl(T, PROP_TYPE)\
    _meta_property_type_info_decl_impl(meta_to_str(T), (PROP_TYPE))

#define META_PROPERTY_TYPE_INFO_U8      meta_property_type_info_decl(uint8_t, META_PROPERTY_TYPE_U8)
#define META_PROPERTY_TYPE_INFO_S8      meta_property_type_info_decl(int8_t, META_PROPERTY_TYPE_S8)
#define META_PROPERTY_TYPE_INFO_U16     meta_property_type_info_decl(uint16_t, META_PROPERTY_TYPE_U16)
#define META_PROPERTY_TYPE_INFO_S16     meta_property_type_info_decl(int16_t, META_PROPERTY_TYPE_S16)
#define META_PROPERTY_TYPE_INFO_U32     meta_property_type_info_decl(uint32_t, META_PROPERTY_TYPE_U32)
#define META_PROPERTY_TYPE_INFO_S32     meta_property_type_info_decl(int32_t, META_PROPERTY_TYPE_S32)
#define META_PROPERTY_TYPE_INFO_U64     meta_property_type_info_decl(uint64_t, META_PROPERTY_TYPE_U64)
#define META_PROPERTY_TYPE_INFO_S64     meta_property_type_info_decl(int64_t, META_PROPERTY_TYPE_S64)
#define META_PROPERTY_TYPE_INFO_F32     meta_property_type_info_decl(float, META_PROPERTY_TYPE_F32)
#define META_PROPERTY_TYPE_INFO_F64     meta_property_type_info_decl(double, META_PROPERTY_TYPE_F64)
#define META_PROPERTY_TYPE_INFO_SIZE_T  meta_property_type_info_decl(size_t, META_PROPERTY_TYPE_SIZE_T)
#define META_PROPERTY_TYPE_INFO_STR     meta_property_type_info_decl(char*, META_PROPERTY_TYPE_STR)

typedef struct meta_property_t 
{
    const char* name;                   // Display name of field
    size_t offset;                      // Offset in bytes to struct
    meta_property_type_info_t type;     // Type info
} meta_property_t;

extern meta_property_t _meta_property_impl(const char* name, size_t offset, meta_property_type_info_t type);

#define meta_property(CLS, FIELD, TYPE)\
    _meta_property_impl(meta_to_str(FIELD), meta_offset(CLS, FIELD), TYPE)

typedef struct meta_class_t 
{
    const char* name;            // Display name
    uint32_t property_count;     // Count of all properties in list
    meta_property_t* properties; // List of all properties
} meta_class_t;

typedef struct meta_registry_t 
{
    struct {uint64_t key; meta_class_t value;}* classes;
} meta_registry_t;

typedef struct meta_class_decl_t 
{
    meta_property_t* properties;        // Array of properties
    size_t size;                        // Size of array in bytes
} meta_class_decl_t;

// Functions
extern meta_registry_t meta_registry_new();
extern void meta_registry_free(meta_registry_t* meta);
extern uint64_t _meta_registry_register_class_impl(meta_registry_t* meta, const char* name, const meta_class_decl_t* decl);
extern meta_class_t* _meta_class_getp_impl(meta_registry_t* meta, const char* name);

#define meta_registry_register_class(META, T, DECL)\
    _meta_registry_register_class_impl((META), meta_to_str(T), (DECL))

#define meta_registry_class_get(META, T)\
    _meta_class_getp_impl(META, meta_to_str(T))

#define meta_registry_getvp(OBJ, T, PROP)\
    ((T*)((uint8_t*)(OBJ) + (PROP)->offset))

#define meta_registry_getv(OBJ, T, PROP)\
    (*((T*)((uint8_t*)(OBJ) + (PROP)->offset)))

/* Implementation */

#ifdef META_IMPL

meta_registry_t meta_registry_new()
{
    meta_registry_t meta = {0};
    return meta;
}

void meta_registry_free(meta_registry_t* meta)
{
    // Handle in a little bit
}

uint64_t _meta_registry_register_class_impl(meta_registry_t* meta, const char* name, const meta_class_decl_t* decl)
{
    meta_class_t cls = {0};

    // Make value for pair
    uint32_t ct = decl->size / sizeof(meta_property_t);
    cls.name = name;
    cls.property_count = ct; 
    cls.properties = malloc(decl->size); 
    memcpy(cls.properties, decl->properties, decl->size);

    // Make key for pair
    // Hash our name into a 64-bit identifier
    uint64_t id = (uint64_t)stbds_hash_string(name, 0);

    // Insert key/value pair into our hash table
    hmput(meta->classes, id, cls);
    return id;
}

meta_property_t _meta_property_impl(const char* name, size_t offset, meta_property_type_info_t type)
{
    meta_property_t mp = {0};
    mp.name = name;
    mp.offset = offset;
    mp.type = type;
    return mp;
}

meta_property_type_info_t _meta_property_type_info_decl_impl(const char* name, uint32_t id)
{
    meta_property_type_info_t info = {0};
    info.name = name;
    info.id = id;
    return info;
}

meta_class_t* _meta_class_getp_impl(meta_registry_t* meta, const char* name)
{
    uint64_t id = (uint64_t)stbds_hash_string(name, 0);
    return (&hmgetp(meta->classes, id)->value);
}

#endif
#endif // META_H




















