#ifndef __XNORMAL__3DSMAX__SBM__EXPORTER__HEADER__40cd2402__49557079
    #define __XNORMAL__3DSMAX__SBM__EXPORTER__HEADER__40cd2402__49557079

    #include <max.h>
    
    /**
    * The interface for the xNormal's .SBM mesh exporter for Autodesk 3dsmax
    *
    * To create a new xNormal SBM mesh exporter from 3dsmax do this:
    * \code
    * #include <MaxSBMExporter.h>
    * ...
    * IxNormal3DSMaxSBMExporter* sbmExp = reinterpret_cast<IxNormal3DSMaxSBMExporter*>(
    *   interface->CreateInstance(SCENE_EXPORT_CLASS_ID, IxNormal3DSMaxSBMExporter::GetClassID());
    *   //interface is the Max core interface (Interface class)
    * \endcode
    *
    * To export a scene without user intervention do this:
    * \code
    * std::wstring l_strLog;
    * sbmExp->ExportWithOptions(_T("c:\\exportedMeshes\\hola.sbm"),&expInterface,interface,
        false,false,false,true,false,true,false,&l_strLogTxt);
    *  
    * \endcode
    * 
    * If you want to export a scene with user intervention do this:
    * \code
    * Class_ID cid(IxNormal3DSMaxSBMExporter::GetClassID());
    * interface->ExportToFile(_T("c:\\exportedMeshes\\hola.sbm"),0,0,&cid);
    * \endcode
    *
    * Once you finished with the SBM exporter, delete it using the DeleteThis() method
    * \code
    * sbmExp->DeleteThis()
    * sbmExp = 0;
    * \endcode
    */
    class IxNormal3DSMaxSBMExporter : public SceneExport
    {
    public:
        /**
        * Safe multiple C CRT destructor.
        * Call DeleteThis() to delete the plugin
        */
        virtual void DeleteThis() throw() = 0;

    public:
        /**
        * Get the Class_ID of the SBMP mesh exporter plugin
        */
        static Class_ID GetClassID()
        {
            return Class_ID(0x40cd2402, 0x49557079);
        }

    public:
        /**
        * Method to export the scene without user intervention
        * @param fileName [in] The complete output filename path where to store the .SBM mesh file.
        *   For example _T("c:\\meshes\\chair.SBM"). Cannot be NULL or empty.
        *
        * @param exportSelectedOnly [in] TRUE to export only the selected nodes(non-hidden)
        *       or zero to export all(non-hidden) nodes in the scene.
        *
        * @param exportUVs [in] TRUE to export the texture coordinates of the meshes or FALSE to ignore them
        *
        * @param exportNormals [in] TRUE to export the vertex normals of the meshes or FALSE to ignore them
        * @param smoothNormals [in] TRUE to average the meshes's vertex normals or FALSE to export the
        *       vertex normal with their assigned smoothing groups intact. Notice this parameter will be
        *       ignored unless you set the "exportNormals" to TRUE.
        *
        * @param exportTangentBasis [in] TRUE to export the vertex tangent basis of the meshes or FALSE to
        *  ignore them. Notice this parameter will be ignored unless you set the "exportUVs" and also the
        *   "exportNormals" ( UVs and normals are required to compute the tangent basis ).
        *
        * @param exportCage [in] TRUE to export the cage data(Projection modified over an Editable Mesh) or
        *       FALSE to ignore it. Notice if this is set to "TRUE" the cage data will be only exported
        *       if the mesh is triangulated(Editable mesh). Editable poly(quad or n-gon faces) or any
        *       mesh type different than the "Editable mesh" aren't supported... so sure your artists
        *       put an "Editable mesh" modifier before the "Projection modifier" or the caga data
        *       won't be exported.
        *
        * @param exportVCols [in] TRUE to export the vertex colors
        *
        * @param logText [in] An optional pointer to an STL string to be filled with the log text. 
        *   The string WON'T be cleared... so clear it manually before calling this method if you need
        *   it empty.
        *
        * @return Will return TRUE is no error happens. FALSE if any error occures.
        *   No exception will be thrown.
        *
        * @remarks Tell your artists to use a "Reset XForm" before exporting to update the vertices
        *       with unapplied rotations or transformations.
        */
        virtual bool ExportWithOptions ( const TCHAR *fileName, 
            const bool exportSelectedOnly = false, const bool exportUVs = false, 
            const bool exportNormals = false, const bool smoothNormals = false, 
            const bool exportTangentBasis = false, const bool exportCage = false,
            const bool exportVCols=false, CStr *logText=0 ) throw() = 0;


        /**
        * Method to export the scene without user intervention
        * @param fileName [in] The complete output filename path where to store the .SBM mesh file.
        *   For example _T("c:\\meshes\\chair.SBM"). Cannot be NULL or empty.
        *
        * @param nodesToExport [in] A list of nodes to export. Cannot be NULL.
        *
        * @param exportUVs [in] TRUE to export the texture coordinates of the meshes or FALSE to ignore them
        *
        * @param exportNormals [in] TRUE to export the vertex normals of the meshes or FALSE to ignore them
        * @param smoothNormals [in] TRUE to average the meshes's vertex normals or FALSE to export the
        *       vertex normal with their assigned smoothing groups intact. Notice this parameter will be
        *       ignored unless you set the "exportNormals" to TRUE.
        *
        * @param exportTangentBasis [in] TRUE to export the vertex tangent basis of the meshes or FALSE to
        *  ignore them. Notice this parameter will be ignored unless you set the "exportUVs" and also the
        *   "exportNormals" ( UVs and normals are required to compute the tangent basis ).
        *
        * @param exportCage [in] TRUE to export the cage data(Projection modified over an Editable Mesh) or
        *       FALSE to ignore it. Notice if this is set to "TRUE" the cage data will be only exported
        *       if the mesh is triangulated(Editable mesh). Editable poly(quad or n-gon faces) or any
        *       mesh type different than the "Editable mesh" aren't supported... so sure your artists
        *       put an "Editable mesh" modifier before the "Projection modifier" or the caga data
        *       won't be exported.
        *
        * @param exportVCols [in] TRUE to export the vertex colors
        * @param logText [in] An optional pointer to an STL string to be filled with the log text. 
        *   The string WON'T be cleared... so clear it manually before calling this method if you need
        *   it empty.
        *
        * @return Will return TRUE is no error happens. FALSE if any error occures.
        *   No exception will be thrown.
        */
        virtual bool ExportNodesWithOptions ( const TCHAR *fileName, 
            const INodeTab *nodesToExport, const bool exportUVs = false, 
            const bool exportNormals = false, const bool smoothNormals = false, 
            const bool exportTangentBasis = false, const bool exportCage = false,
            const bool exportVCols=false, CStr* logText=0 ) throw() = 0;

    protected:
        /**
        * Protected virtual destructor.
        * Call DeleteThis() method to delete the plugin. That is required to avoid memory
        * problems with different C CRTs. That's why we marked the destructor as "protected",
        * to avoid the programmers to call the destructor directly.
        */
        virtual ~IxNormal3DSMaxSBMExporter()
        {
        }
    };

#endif //__XNORMAL__3DSMAX__SBM__EXPORTER__HEADER__40cd2402__49557079

