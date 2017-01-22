#ifndef __XNORMAL__3DSMAX__SBM__IMPORTER__HEADER__416a708b__498b0b94__
    #define __XNORMAL__3DSMAX__SBM__IMPORTER__HEADER__416a708b__498b0b94__

    #include <max.h>
    
    /**
    * The interface for the xNormal's .SBM mesh importer for Autodesk 3dsmax
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
    * Once you finished with the SBM importer, delete it using the DeleteThis() method
    * \code
    * sbmExp->DeleteThis()
    * sbmExp = 0;
    * \endcode
    */
    class IxNormal3DSMaxSBMImporter : public SceneImport
    {
    public:
        /**
        * Safe multiple C CRT destructor.
        * Call DeleteThis() to delete the plugin
        */
        virtual void DeleteThis() throw() = 0;

    public:
        /**
        * Get the Class_ID of the SBM mesh exporter plugin
        */
        static Class_ID GetClassID()
        {
            return Class_ID(0x416a708b, 0x498b0b94);
        }

    protected:
        /**
        * Protected virtual destructor.
        * Call DeleteThis() method to delete the plugin. That is required to avoid memory
        * problems with different C CRTs. That's why we marked the destructor as "protected",
        * to avoid the programmers to call the destructor directly.
        */
        virtual ~IxNormal3DSMaxSBMImporter() throw()
        {
        }
    };

#endif //__XNORMAL__3DSMAX__SBM__EXPORTER__HEADER__40cd2402__49557079

