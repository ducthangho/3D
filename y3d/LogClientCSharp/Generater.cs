using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogClientCSharp
{
    class Generater
    {


        //const int TYPE_FLOAT = 0;     //!< Identifies a floating point parameter type.
        //const int TYPE_INT = 1;       //!< Identifies an integer parameter type.
        //const int TYPE_RGBA = 2;      //!< Identifies a Color parameter type. \note Does not contain an alpha channel despite the name.
        //const int TYPE_POINT3 = 3;  //!< Identifies a Point3 parameter type.
        //const int TYPE_BOOL = 4;      //!< Identifies a BOOL parameter type.
        //const int  TYPE_USER = 5;      //!< Used to define user types 
       

        //enum ParamType2
        //{
        //    // TYPE_FLOAT,  
        //    // TYPE_INT,
        //    // TYPE_RGBA,
        //    // TYPE_POINT3,
        //    // TYPE_BOOL,
        //    TYPE_ANGLE = TYPE_BOOL + 1, //!< A floating point value with an implied stdAngleDim dimension.
        //    TYPE_PCNT_FRAC,             //!< A floating point with an implied stdPercentDim dimension.
        //    TYPE_WORLD,                 //!< A floating point specifying a parameter represents world distance units. This implies a parameter dimension of stdWorldDim.
        //    TYPE_STRING,                //!< A character string. The string has a local copy made and managed by the parameter block.
        //    TYPE_FILENAME,              //!< Used to identify file names (const MCHAR*).  An AssetType should also be specified - see ParamTags.p_assetTypeID and ParamTags.p_assetTypeName.
        //    TYPE_HSV,                   //!< This option is obsolete.
        //    TYPE_COLOR_CHANNEL,         //!< A single floating point value with an implied stdColor255Dim dimension.
        //    TYPE_TIMEVALUE,             //!< A single integer value used as a TimeValue -- implies a stdTimeDim dimension.
        //    TYPE_RADIOBTN_INDEX,        //!< This is used as integer for parameters represented as radio buttons in the UI.
        //    TYPE_MTL,                   //!< A pointer to a material object. This can be one of three types: a reference owned by the parameter block, a reference owned by the block owner, or no reference management (just a copy of the pointer).
        //    TYPE_TEXMAP,                //!< A pointer to a texture map object. This can be one of three types: a reference owned by the parameter block, a reference owned by the block owner, or no reference management (just a copy of the pointer). 
        //    TYPE_BITMAP,                //!< A pointer to a bitmap object. This can be one of three types: a reference owned by the parameter block, a reference owned by the block owner, or no reference management (just a copy of the pointer).
        //    TYPE_INODE,                 //!< A pointer to a node. This can be one of three types: a reference owned by the parameter block, a reference owned by the block owner, or no reference management (just a copy of the pointer). 
        //    TYPE_REFTARG,               //!< A pointer to a reference target. All reference targets in this group can be one of three types: reference owned by parameter block, reference owned by block owner, or no reference management (just a copy of the pointer).
        //                                // new for R4...
        //    TYPE_INDEX,                 //!< Used for parameters that are 0-based, but exposed to MAXScript as 1-based. For example a vertex index.
        //    TYPE_MATRIX3,               //!< A standard 3ds Max matrix.
        //    TYPE_PBLOCK2,               //!< A pointer to an IParamBlock2 object. Note "TYPE_PBLOCK2_TYPE" is not defined.
        //                                // new for R6...
        //    TYPE_POINT4,
        //    TYPE_FRGBA,

        //    // only for published function parameter types, not pblock2 parameter types...
        //    TYPE_ENUM,
        //    TYPE_VOID,
        //    TYPE_INTERVAL,
        //    TYPE_ANGAXIS,
        //    TYPE_QUAT,
        //    TYPE_RAY,
        //    TYPE_POINT2,
        //    TYPE_BITARRAY,
        //    TYPE_CLASS,
        //    TYPE_MESH,
        //    TYPE_OBJECT,
        //    TYPE_CONTROL,
        //    TYPE_POINT,
        //    TYPE_TSTR,
        //    TYPE_IOBJECT,
        //    TYPE_INTERFACE,
        //    TYPE_HWND,
        //    TYPE_NAME,
        //    TYPE_COLOR,
        //    TYPE_FPVALUE,
        //    TYPE_VALUE,
        //    TYPE_DWORD,
        //    TYPE_bool,
        //    TYPE_INTPTR,
        //    TYPE_INT64,
        //    TYPE_DOUBLE,
        //    TYPE_BOX3,
        //    TYPE_BEZIERSHAPE,

        //    // Tab<>s of above  (MUST be in same order as base types)
        //    TYPE_FLOAT_TAB = TYPE_FLOAT + TYPE_TAB, //!< A table of floating point values.
        //    TYPE_INT_TAB,           //!< A table of integer values
        //    TYPE_RGBA_TAB,          //!< A table of Point3 values with an implied stdColor255Dim dimension.
        //    TYPE_POINT3_TAB,        //!< A table of Point3 data type values.
        //    TYPE_BOOL_TAB,          //!< A table of integers used as a set of boolean values.
        //    TYPE_ANGLE_TAB,         //!< A table of floating point value with an implied stdAngleDim dimension.
        //    TYPE_PCNT_FRAC_TAB,     //!< A table of same as TYPE_STRING, but is used with TYPE_FILEOPENBUTTON and TYPE_FILESAVEBUTTON parameter map controls.
        //    TYPE_WORLD_TAB,         //!< A table of parameters that represents world distance units. This implies a parameter dimension of stdWorldDim.
        //    TYPE_STRING_TAB,        //!< A table of character strings (const MCHAR*).
        //    TYPE_FILENAME_TAB,      //!< A table of filenames (see TYPE_FILENAME).
        //    TYPE_HSV_TAB,           //!< This option is obsolete.
        //    TYPE_COLOR_CHANNEL_TAB, //!< A table of floating point values with an implied stdColor255Dim dimension.
        //    TYPE_TIMEVALUE_TAB,     //!< A table of integer value used as a TimeValue -- implies stdTimeDim dimension.
        //    TYPE_RADIOBTN_INDEX_TAB,//!< This is currently unused.
        //    TYPE_MTL_TAB,           //!< A table of material object pointers (see TYPE_MTL).
        //    TYPE_TEXMAP_TAB,        //!< A table of texmap object pointers (see TYPE_TEXMAP).
        //    TYPE_BITMAP_TAB,        //!< A table of TYPE_BITMAP values.
        //    TYPE_INODE_TAB,         //!< A table of TYPE_INODE values.
        //    TYPE_REFTARG_TAB,       //!< A table of TYPE_REFTARG values.
        //                            // new for R4...
        //    TYPE_INDEX_TAB,         //!< A table of TYPE_INDEX values. 
        //    TYPE_MATRIX3_TAB,       //!< A table of TYPE_MATRIX3 values.
        //    TYPE_PBLOCK2_TAB,       //!< A table of TYPE_BLOCK2 values.
        //                            // new for R6...
        //    TYPE_POINT4_TAB,
        //    TYPE_FRGBA_TAB,
        //    // Only for published function parameter types, not pblock2 parameter types.
        //    TYPE_ENUM_TAB,
        //    TYPE_VOID_TAB,
        //    TYPE_INTERVAL_TAB,
        //    TYPE_ANGAXIS_TAB,
        //    TYPE_QUAT_TAB,
        //    TYPE_RAY_TAB,
        //    TYPE_POINT2_TAB,
        //    TYPE_BITARRAY_TAB,
        //    TYPE_CLASS_TAB,
        //    TYPE_MESH_TAB,
        //    TYPE_OBJECT_TAB,
        //    TYPE_CONTROL_TAB,
        //    TYPE_POINT_TAB,
        //    TYPE_TSTR_TAB,
        //    TYPE_IOBJECT_TAB,
        //    TYPE_INTERFACE_TAB,
        //    TYPE_HWND_TAB,
        //    TYPE_NAME_TAB,
        //    TYPE_COLOR_TAB,
        //    TYPE_FPVALUE_TAB,
        //    TYPE_VALUE_TAB,
        //    TYPE_DWORD_TAB,
        //    TYPE_bool_TAB,
        //    TYPE_INTPTR_TAB,
        //    TYPE_INT64_TAB,
        //    TYPE_DOUBLE_TAB,
        //    TYPE_BOX3_TAB,
        //    TYPE_BEZIERSHAPE_TAB,

        //    // pass by-ref types, implies & parameters, int& & float& are passed via .ptr fields, only for FnPub use

        //    TYPE_FLOAT_BR = TYPE_FLOAT + TYPE_BY_REF,
        //    TYPE_INT_BR = TYPE_INT + TYPE_BY_REF,
        //    TYPE_BOOL_BR = TYPE_BOOL + TYPE_BY_REF,
        //    TYPE_ANGLE_BR = TYPE_ANGLE + TYPE_BY_REF,
        //    TYPE_PCNT_FRAC_BR = TYPE_PCNT_FRAC + TYPE_BY_REF,
        //    TYPE_WORLD_BR = TYPE_WORLD + TYPE_BY_REF,
        //    TYPE_COLOR_CHANNEL_BR = TYPE_COLOR_CHANNEL + TYPE_BY_REF,
        //    TYPE_TIMEVALUE_BR = TYPE_TIMEVALUE + TYPE_BY_REF,
        //    TYPE_RADIOBTN_INDEX_BR = TYPE_RADIOBTN_INDEX + TYPE_BY_REF,
        //    TYPE_INDEX_BR = TYPE_INDEX + TYPE_BY_REF,
        //    TYPE_RGBA_BR = TYPE_RGBA + TYPE_BY_REF,
        //    TYPE_BITMAP_BR = TYPE_BITMAP + TYPE_BY_REF,
        //    TYPE_POINT3_BR = TYPE_POINT3 + TYPE_BY_REF,
        //    TYPE_HSV_BR = TYPE_HSV + TYPE_BY_REF,
        //    TYPE_REFTARG_BR = TYPE_REFTARG + TYPE_BY_REF,
        //    TYPE_MATRIX3_BR = TYPE_MATRIX3 + TYPE_BY_REF,
        //    TYPE_POINT4_BR = TYPE_POINT4 + TYPE_BY_REF,
        //    TYPE_FRGBA_BR = TYPE_FRGBA + TYPE_BY_REF,
        //    TYPE_ENUM_BR = TYPE_ENUM + TYPE_BY_REF,
        //    TYPE_INTERVAL_BR = TYPE_INTERVAL + TYPE_BY_REF,
        //    TYPE_ANGAXIS_BR = TYPE_ANGAXIS + TYPE_BY_REF,
        //    TYPE_QUAT_BR = TYPE_QUAT + TYPE_BY_REF,
        //    TYPE_RAY_BR = TYPE_RAY + TYPE_BY_REF,
        //    TYPE_POINT2_BR = TYPE_POINT2 + TYPE_BY_REF,
        //    TYPE_BITARRAY_BR = TYPE_BITARRAY + TYPE_BY_REF,
        //    TYPE_MESH_BR = TYPE_MESH + TYPE_BY_REF,
        //    TYPE_POINT_BR = TYPE_POINT + TYPE_BY_REF,
        //    TYPE_TSTR_BR = TYPE_TSTR + TYPE_BY_REF,
        //    TYPE_COLOR_BR = TYPE_COLOR + TYPE_BY_REF,
        //    TYPE_FPVALUE_BR = TYPE_FPVALUE + TYPE_BY_REF,
        //    TYPE_DWORD_BR = TYPE_DWORD + TYPE_BY_REF,
        //    TYPE_bool_BR = TYPE_bool + TYPE_BY_REF,
        //    TYPE_INTPTR_BR = TYPE_INTPTR + TYPE_BY_REF,
        //    TYPE_INT64_BR = TYPE_INT64 + TYPE_BY_REF,
        //    TYPE_DOUBLE_BR = TYPE_DOUBLE + TYPE_BY_REF,
        //    TYPE_BOX3_BR = TYPE_BOX3 + TYPE_BY_REF,
        //    TYPE_BEZIERSHAPE_BR = TYPE_BEZIERSHAPE + TYPE_BY_REF,

        //    // pass by-ref Tab<> types - tab is passed by ref, data type in tab is TYPE_XXXX.
        //    // For example: TYPE_FLOAT_TAB_BR = Tab<float>& , TYPE_INODE_TAB_BR_TYPE = Tab<INode*>&
        //    TYPE_FLOAT_TAB_BR = TYPE_FLOAT + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_INT_TAB_BR = TYPE_INT + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_RGBA_TAB_BR = TYPE_RGBA + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_POINT3_TAB_BR = TYPE_POINT3 + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_BOOL_TAB_BR = TYPE_BOOL + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_ANGLE_TAB_BR = TYPE_ANGLE + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_PCNT_FRAC_TAB_BR = TYPE_PCNT_FRAC + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_WORLD_TAB_BR = TYPE_WORLD + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_STRING_TAB_BR = TYPE_STRING + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_FILENAME_TAB_BR = TYPE_FILENAME + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_HSV_TAB_BR = TYPE_HSV + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_COLOR_CHANNEL_TAB_BR = TYPE_COLOR_CHANNEL + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_TIMEVALUE_TAB_BR = TYPE_TIMEVALUE + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_RADIOBTN_INDEX_TAB_BR = TYPE_RADIOBTN_INDEX + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_MTL_TAB_BR = TYPE_MTL + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_TEXMAP_TAB_BR = TYPE_TEXMAP + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_BITMAP_TAB_BR = TYPE_BITMAP + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_INODE_TAB_BR = TYPE_INODE + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_REFTARG_TAB_BR = TYPE_REFTARG + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_INDEX_TAB_BR = TYPE_INDEX + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_MATRIX3_TAB_BR = TYPE_MATRIX3 + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_POINT4_TAB_BR = TYPE_POINT4 + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_FRGBA_TAB_BR = TYPE_FRGBA + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_TSTR_TAB_BR = TYPE_TSTR + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_ENUM_TAB_BR = TYPE_ENUM + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_INTERVAL_TAB_BR = TYPE_INTERVAL + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_ANGAXIS_TAB_BR = TYPE_ANGAXIS + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_QUAT_TAB_BR = TYPE_QUAT + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_RAY_TAB_BR = TYPE_RAY + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_POINT2_TAB_BR = TYPE_POINT2 + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_BITARRAY_TAB_BR = TYPE_BITARRAY + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_CLASS_TAB_BR = TYPE_CLASS + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_MESH_TAB_BR = TYPE_MESH + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_OBJECT_TAB_BR = TYPE_OBJECT + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_CONTROL_TAB_BR = TYPE_CONTROL + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_POINT_TAB_BR = TYPE_POINT + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_IOBJECT_TAB_BR = TYPE_IOBJECT + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_INTERFACE_TAB_BR = TYPE_INTERFACE + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_HWND_TAB_BR = TYPE_HWND + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_NAME_TAB_BR = TYPE_NAME + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_COLOR_TAB_BR = TYPE_COLOR + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_FPVALUE_TAB_BR = TYPE_FPVALUE + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_VALUE_TAB_BR = TYPE_VALUE + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_DWORD_TAB_BR = TYPE_DWORD + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_bool_TAB_BR = TYPE_bool + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_INTPTR_TAB_BR = TYPE_INTPTR + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_INT64_TAB_BR = TYPE_INT64 + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_DOUBLE_TAB_BR = TYPE_DOUBLE + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_BOX3_TAB_BR = TYPE_BOX3 + TYPE_TAB + TYPE_BY_REF,
        //    TYPE_BEZIERSHAPE_TAB_BR = TYPE_BEZIERSHAPE + TYPE_TAB + TYPE_BY_REF,

        //    // pass by-value types, implies dereferencing the (meaningful) pointer-based values, only for FnPub use
        //    TYPE_RGBA_BV = TYPE_RGBA + TYPE_BY_VAL, //[TBA]
        //    TYPE_POINT3_BV = TYPE_POINT3 + TYPE_BY_VAL,
        //    TYPE_HSV_BV = TYPE_HSV + TYPE_BY_VAL,
        //    TYPE_INTERVAL_BV = TYPE_INTERVAL + TYPE_BY_VAL,
        //    TYPE_BITMAP_BV = TYPE_BITMAP + TYPE_BY_VAL,
        //    TYPE_MATRIX3_BV = TYPE_MATRIX3 + TYPE_BY_VAL,
        //    TYPE_POINT4_BV = TYPE_POINT4 + TYPE_BY_VAL,
        //    TYPE_FRGBA_BV = TYPE_FRGBA + TYPE_BY_VAL,
        //    TYPE_ANGAXIS_BV = TYPE_ANGAXIS + TYPE_BY_VAL,
        //    TYPE_QUAT_BV = TYPE_QUAT + TYPE_BY_VAL,
        //    TYPE_RAY_BV = TYPE_RAY + TYPE_BY_VAL,
        //    TYPE_POINT2_BV = TYPE_POINT2 + TYPE_BY_VAL,
        //    TYPE_BITARRAY_BV = TYPE_BITARRAY + TYPE_BY_VAL,
        //    TYPE_MESH_BV = TYPE_MESH + TYPE_BY_VAL,
        //    TYPE_POINT_BV = TYPE_POINT + TYPE_BY_VAL,
        //    TYPE_TSTR_BV = TYPE_TSTR + TYPE_BY_VAL,
        //    TYPE_COLOR_BV = TYPE_COLOR + TYPE_BY_VAL,
        //    TYPE_FPVALUE_BV = TYPE_FPVALUE + TYPE_BY_VAL,
        //    TYPE_CLASS_BV = TYPE_CLASS + TYPE_BY_VAL,
        //    TYPE_BOX3_BV = TYPE_BOX3 + TYPE_BY_VAL,
        //    TYPE_BEZIERSHAPE_BV = TYPE_BEZIERSHAPE + TYPE_BY_VAL,

        //    // pass by-val Tab<> types - tab is passed by val, data type in tab is TYPE_XXXX.
        //    // For example: TYPE_FLOAT_TAB_BV = Tab<float> , TYPE_INODE_TAB_BV = Tab<INode*>
        //    TYPE_FLOAT_TAB_BV = TYPE_FLOAT + TYPE_TAB + TYPE_BY_VAL, //[TBA]
        //    TYPE_INT_TAB_BV = TYPE_INT + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_RGBA_TAB_BV = TYPE_RGBA + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_POINT3_TAB_BV = TYPE_POINT3 + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_BOOL_TAB_BV = TYPE_BOOL + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_ANGLE_TAB_BV = TYPE_ANGLE + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_PCNT_FRAC_TAB_BV = TYPE_PCNT_FRAC + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_WORLD_TAB_BV = TYPE_WORLD + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_STRING_TAB_BV = TYPE_STRING + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_FILENAME_TAB_BV = TYPE_FILENAME + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_HSV_TAB_BV = TYPE_HSV + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_COLOR_CHANNEL_TAB_BV = TYPE_COLOR_CHANNEL + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_TIMEVALUE_TAB_BV = TYPE_TIMEVALUE + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_RADIOBTN_INDEX_TAB_BV = TYPE_RADIOBTN_INDEX + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_MTL_TAB_BV = TYPE_MTL + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_TEXMAP_TAB_BV = TYPE_TEXMAP + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_BITMAP_TAB_BV = TYPE_BITMAP + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_INODE_TAB_BV = TYPE_INODE + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_REFTARG_TAB_BV = TYPE_REFTARG + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_INDEX_TAB_BV = TYPE_INDEX + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_MATRIX3_TAB_BV = TYPE_MATRIX3 + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_POINT4_TAB_BV = TYPE_POINT4 + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_FRGBA_TAB_BV = TYPE_FRGBA + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_PBLOCK2_TAB_BV = TYPE_PBLOCK2 + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_VOID_TAB_BV = TYPE_VOID + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_TSTR_TAB_BV = TYPE_TSTR + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_ENUM_TAB_BV = TYPE_ENUM + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_INTERVAL_TAB_BV = TYPE_INTERVAL + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_ANGAXIS_TAB_BV = TYPE_ANGAXIS + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_QUAT_TAB_BV = TYPE_QUAT + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_RAY_TAB_BV = TYPE_RAY + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_POINT2_TAB_BV = TYPE_POINT2 + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_BITARRAY_TAB_BV = TYPE_BITARRAY + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_CLASS_TAB_BV = TYPE_CLASS + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_MESH_TAB_BV = TYPE_MESH + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_OBJECT_TAB_BV = TYPE_OBJECT + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_CONTROL_TAB_BV = TYPE_CONTROL + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_POINT_TAB_BV = TYPE_POINT + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_IOBJECT_TAB_BV = TYPE_IOBJECT + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_INTERFACE_TAB_BV = TYPE_INTERFACE + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_HWND_TAB_BV = TYPE_HWND + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_NAME_TAB_BV = TYPE_NAME + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_COLOR_TAB_BV = TYPE_COLOR + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_FPVALUE_TAB_BV = TYPE_FPVALUE + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_VALUE_TAB_BV = TYPE_VALUE + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_DWORD_TAB_BV = TYPE_DWORD + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_bool_TAB_BV = TYPE_bool + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_INTPTR_TAB_BV = TYPE_INTPTR + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_INT64_TAB_BV = TYPE_INT64 + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_DOUBLE_TAB_BV = TYPE_DOUBLE + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_BOX3_TAB_BV = TYPE_BOX3 + TYPE_TAB + TYPE_BY_VAL,
        //    TYPE_BEZIERSHAPE_TAB_BV = TYPE_BEZIERSHAPE + TYPE_TAB + TYPE_BY_VAL,

        //    // pass by-pointer types for int & float types, implies * parameters, int* & float* are passed via .ptr fields, only for FnPub use
        //    TYPE_FLOAT_BP = TYPE_FLOAT + TYPE_BY_PTR, //[TBA]
        //    TYPE_INT_BP = TYPE_INT + TYPE_BY_PTR,
        //    TYPE_BOOL_BP = TYPE_BOOL + TYPE_BY_PTR,
        //    TYPE_ANGLE_BP = TYPE_ANGLE + TYPE_BY_PTR,
        //    TYPE_PCNT_FRAC_BP = TYPE_PCNT_FRAC + TYPE_BY_PTR,
        //    TYPE_WORLD_BP = TYPE_WORLD + TYPE_BY_PTR,
        //    TYPE_COLOR_CHANNEL_BP = TYPE_COLOR_CHANNEL + TYPE_BY_PTR,
        //    TYPE_TIMEVALUE_BP = TYPE_TIMEVALUE + TYPE_BY_PTR,
        //    TYPE_RADIOBTN_INDEX_BP = TYPE_RADIOBTN_INDEX + TYPE_BY_PTR,
        //    TYPE_INDEX_BP = TYPE_INDEX + TYPE_BY_PTR,
        //    TYPE_ENUM_BP = TYPE_ENUM + TYPE_BY_PTR,
        //    TYPE_DWORD_BP = TYPE_DWORD + TYPE_BY_PTR,
        //    TYPE_bool_BP = TYPE_bool + TYPE_BY_PTR,
        //    TYPE_INTPTR_BP = TYPE_INTPTR + TYPE_BY_PTR,
        //    TYPE_INT64_BP = TYPE_INT64 + TYPE_BY_PTR,
        //    TYPE_DOUBLE_BP = TYPE_DOUBLE + TYPE_BY_PTR,

        //    // there are no specific by-pointer Tab<> types, all Tab<> types are by-pointer by default

        //    TYPE_MAX_TYPE,

        //    // MAXScript internal types
        //    TYPE_KEYARG_MARKER = 253,
        //    TYPE_MSFLOAT,
        //    TYPE_UNSPECIFIED,

        //};
    }
}
