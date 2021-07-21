#include <stdio.h>
#include <stdint.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define META_IMPL
#include "meta.h"

typedef struct other_thing_t {
    int32_t s_val; 
} other_thing_t;

#define META_PROPERTY_TYPE_OBJ          (META_PROPERTY_TYPE_COUNT + 1)
#define META_PROPERTY_TYPE_INFO_OBJ     meta_property_type_info_decl(other_thing_t, META_PROPERTY_TYPE_OBJ)

typedef struct thing_t {
    uint32_t u_val;
    float f_val;
    const char* name;
    other_thing_t o_val;
} thing_t;

void print_obj(meta_registry_t* meta, void* obj, meta_class_t* cls)
{
    for (uint32_t i = 0; i < cls->property_count; ++i)
    {
        meta_property_t* prop = &cls->properties[i];

        switch (prop->type.id)
        {
            case META_PROPERTY_TYPE_U32: 
            {
                uint32_t* v = meta_registry_getvp(obj, uint32_t, prop);
                printf("%s (%s): %zu\n", prop->name, prop->type.name, *v);
            } break;

            case META_PROPERTY_TYPE_S32: 
            {
                int32_t* v = meta_registry_getvp(obj, int32_t, prop);
                printf("%s (%s): %d\n", prop->name, prop->type.name, *v);
            } break;

            case META_PROPERTY_TYPE_F32: 
            {
                float* v = meta_registry_getvp(obj, float, prop);
                printf("%s (%s): %.2f\n", prop->name, prop->type.name, *v);
            } break;

            case META_PROPERTY_TYPE_STR: 
            {
                const char* v = meta_registry_getv(obj, const char*, prop);
                printf("%s (%s): %s\n", prop->name, prop->type.name, v);
            } break;

            case META_PROPERTY_TYPE_OBJ:
            {
                printf("%s (%s):\n", prop->name, prop->type.name);
                const other_thing_t* v = meta_registry_getvp(obj, other_thing_t, prop);
                const meta_class_t* clz = meta_registry_class_get(meta, other_thing_t);
                print_obj(meta, v, clz); 
            } break;
        } 
    }
}

int32_t main(int32_t argc, char** argv)
{
    meta_registry_t meta = meta_registry_new();

    thing_t thing = {
        .u_val = 42069,
        .f_val = 3.145f,
        .name = "THING",
        .o_val = (other_thing_t) {
            .s_val = -380 
        }
    };

    // thing_t registry
    meta_registry_register_class(&meta, thing_t, (&(meta_class_decl_t){
        .properties = (meta_property_t[]) {
            meta_property(thing_t, u_val, META_PROPERTY_TYPE_INFO_U32),
            meta_property(thing_t, f_val, META_PROPERTY_TYPE_INFO_F32),
            meta_property(thing_t, name, META_PROPERTY_TYPE_INFO_STR),
            meta_property(thing_t, o_val, META_PROPERTY_TYPE_INFO_OBJ)
        },
        .size = 4 * sizeof(meta_property_t)
    }));

    // other_thing_t registry
    meta_registry_register_class(&meta, other_thing_t, (&(meta_class_decl_t){
        .properties = (meta_property_t[]) {
            meta_property(other_thing_t, s_val, META_PROPERTY_TYPE_INFO_S32)
        },
        .size = 1 * sizeof(meta_property_t)
    }));

    meta_class_t* cls = meta_registry_class_get(&meta, thing_t);
    print_obj(&meta, &thing, cls);    

    return 0;
}








