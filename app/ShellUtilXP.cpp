////////////////////////////////////////////////////////////////////////////// 
// 
// ShellUtilXP.cpp -- Emulates some useful Shell functions not available on  
//                    Windows XP (but available on Windows 7) 
// 
// 
// by Giovanni Dicanio <giovanni.dicanio@gmail.com> 
// 
// 2011 October 13th 
// 
// 
////////////////////////////////////////////////////////////////////////////// 
 
 
 
//============================================================================ 
//                              Includes 
//============================================================================ 
 
#include "stdafx.h"             // Pre-compiled headers 
 
#include "ShellUtilXP.h"        // Module header 
 

 
//============================================================================ 
//          Anonymous namespace for local module helpers 
//============================================================================ 
namespace { 
 
 
//---------------------------------------------------------------------------- 
// Helper function for ShellUtilXP_BindToObjectEx(). 
// "psf" must point to a valid interface (not NULL!). 
//---------------------------------------------------------------------------- 
inline HRESULT __declspec(nothrow) __stdcall BindToObjectInternalHelper( 
    __in IShellFolder *psf,  
    __in PCUIDLIST_RELATIVE pidl,  
    __in_opt IBindCtx *pbc, 
    __in REFIID riid,  
    __deref_out void **ppv) 
{ 
    // 
    // Check parameters 
    // 
 
    if (psf == NULL) 
        return E_POINTER; 
 
    if (pidl == NULL) 
        return E_POINTER; 
 
    if (ppv == NULL) 
        return E_POINTER; 
 
 
 
    // 
    // Clear output parameters 
    // 
 
    *ppv = NULL; 
 
 
 
    // 
    // http://blogs.msdn.com/b/oldnewthing/archive/2011/08/30/10202076.aspx 
    // 
    // Special check: 
    //  
    // "The subtlety here is in the test for pidl->mkid.cb.  
    // The IShellFolder::BindToObject method is for binding to child objects  
    // (or grandchildren or deeper descendants).  
    // If the object you want is the desktop itself, then you can't use  
    // IShellFolder::BindToObject since the desktop is not a child of itself.  
    // In fact, if the object you want is the desktop itself, then you  
    // already have the desktop, so we just QueryInterface for it." 
    //  
     
    HRESULT hr;     // Result of operations 
     
    if (pidl->mkid.cb)  
    { 
        // Bind to child object 
        hr = psf->BindToObject(pidl, pbc, riid, ppv); 
    }  
    else  
    { 
        // 'psf' is not a child of itself: so just call QueryInterface() 
        hr = psf->QueryInterface(riid, ppv); 
    } 
 
    return hr; 
} 
 
 
 
//---------------------------------------------------------------------------- 
// RAII wrapper to ::CoTaskMemFree(). 
// 
// Shell API functions use COM memory allocator, so ::CoTaskMemFree() is  
// required to release memory resources allocated from the Shell API. 
// For example, the memory allocated by ILClone() must be freed by the 
// caller using ::CoTaskMemFree(). 
// This class provides a convenient way to release these memory resources, 
// because its destructor automatically calls ::CoTaskMemFree() on the 
// wrapped pointer. 
// 
// This class does not throw C++ exceptions (in fact, its methods are marked 
// with "nothrow()"). 
//---------------------------------------------------------------------------- 
class ComMemoryDeleter 
{ 
    //------------------------------------------------------------------------ 
    //                      Class Public Interface 
    //------------------------------------------------------------------------ 
public: 
 
    // Constructs the object pointing to nothing. 
    ComMemoryDeleter() throw() 
        : m_ptr(NULL) 
    { 
    } 
 
 
    // Constructs the object safe-guarding the input pointer. 
    // When this object goes out of scope, the memory 
    // pointed is freed with CoTaskMemFree(). 
    explicit ComMemoryDeleter(__in_opt void * ptr) throw() 
        : m_ptr(ptr) 
    { 
    } 
 
 
    // Safely releases allocated memory (if any) 
    ~ComMemoryDeleter() throw() 
    { 
        Clear(); 
    } 
 
     
    // Accesses the guarded pointer. 
    // (Returns NULL if there is no pointer guarded) 
    void * Ptr() const throw() 
    { 
        return m_ptr; 
    } 
 
 
    // If there is guarded pointer, frees its memory. 
    // Then, guards another pointer (if a non-NULL value is passed). 
    void Reset(__in_opt void * ptr = NULL) throw() 
    { 
        Clear(); 
        m_ptr = ptr; 
    } 
 
 
 
    //------------------------------------------------------------------------ 
    //                          Implementation 
    //------------------------------------------------------------------------ 
private: 
 
    // Guarded pointer 
    void * m_ptr; 
 
 
    // Frees memory pointed by the guarded pointer using CoTaskMemFree(). 
    void Clear() throw() 
    { 
        // Free memory pointed by the guarded pointer. 
        if ( m_ptr != NULL ) 
        { 
            ::CoTaskMemFree(m_ptr); 
            m_ptr = NULL; 
        } 
    } 
 
 
    // 
    // BAN COPY 
    // 
private: 
    ComMemoryDeleter(const ComMemoryDeleter &); 
    ComMemoryDeleter & operator=(const ComMemoryDeleter &); 
 
}; 
 
 
} // anonymous namespace 
 
 
 
 
//============================================================================ 
//                          Function Implementations 
//============================================================================ 
 
 
extern "C" HRESULT __stdcall ShellUtilXP_BindToObjectEx( 
    __in_opt IShellFolder *psf,  
    __in PCUIDLIST_RELATIVE pidl,  
    __in_opt IBindCtx *pbc, 
    __in REFIID riid,  
    __deref_out void **ppv) 
{ 
    // 
    // Check parameters 
    // 
 
    if (pidl == NULL) 
        return E_POINTER; 
 
    if (ppv == NULL) 
        return E_POINTER; 
 
 
 
    // 
    // Clear output parameters 
    // 
     
    *ppv = NULL; 
 
 
 
    // 
    // Retrieve IShellFolder for the desktop folder if requested, 
    // and safely bind to object using the internal helper function. 
    // 
 
    HRESULT hr;         // result of operations 
 
    if (psf == NULL) 
    { 
        // psf == NULL ---> The parent is the Desktop folder, 
        // which is the root of the Shell namespace. 
 
        // Retrieve the IShellFolder interface for the desktop folder 
        IShellFolder * psfDesktop; 
        hr = SHGetDesktopFolder(&psfDesktop); 
        if (SUCCEEDED(hr)) 
        { 
            hr = BindToObjectInternalHelper(psfDesktop, pidl, pbc, riid, ppv); 
            psfDesktop->Release(); 
        } 
    } 
    else 
    { 
        hr = BindToObjectInternalHelper(psf, pidl, pbc, riid, ppv); 
    } 
 
    return hr; 
} 
 
 
 
extern "C" HRESULT __stdcall ShellUtilXP_BindToObject( 
    __in_opt IShellFolder *psf,  
    __in PCUIDLIST_RELATIVE pidl,  
    __in REFIID riid,  
    __deref_out void **ppv) 
{ 
    // Call the corresponding "...Ex()" function with a NULL IBindCtx. 
    return ShellUtilXP_BindToObjectEx(psf, pidl, NULL, riid, ppv); 
} 
 
 
 
extern "C" HRESULT __stdcall ShellUtilXP_BindToFolderIDListParentEx( 
    __in_opt IShellFolder *psfRoot,  
    __in PCUIDLIST_RELATIVE pidl,  
    __in_opt IBindCtx *pbc,  
    __in REFIID riid,  
    __deref_out void **ppv,  
    __deref_opt_out PCUITEMID_CHILD *ppidlLast) 
{ 
    // 
    // Check parameters 
    // 
 
    if (ppv == NULL) 
        return E_POINTER; 
 
 
 
    // 
    // Clear output parameters 
    // 
 
    *ppv = NULL; 
 
    if (ppidlLast != NULL) 
    { 
        *ppidlLast = NULL; 
    } 
 
 
 
    // 
    // Get the parent PIDL of the specified PIDL 
    // 
 
    // Deep-copy the input PIDL because we have to modify it 
    PIDLIST_RELATIVE pidlParent = ILClone(pidl);  
 
    // ILClone() allocated memory which must be freed using CoTaskMemFree(). 
    ComMemoryDeleter pidlParentDeleter(pidlParent); 
 
    // The parent PIDL is the same as the original PIDL,  
    // excluding the last SHITEMID 
    if ( ! ILRemoveLastID(pidlParent) ) 
    { 
        return E_FAIL; 
    } 
 
 
 
    // 
    // Bind to this (parent) object, specified by pidlParent. 
    // 
    HRESULT hr; 
    hr = ShellUtilXP_BindToObjectEx(psfRoot, pidlParent, pbc, riid, ppv); 
    if (FAILED(hr)) 
    { 
        return hr; 
    } 
 
 
    // 
    // Pass pointer to the last ID in the input PIDL 
    // 
    if (ppidlLast != NULL) 
    { 
        *ppidlLast = ILFindLastID(pidl); 
        // No need to release memory with ILFindLastID(). 
    } 
 
 
    return hr; 
} 
 
 
 
extern "C" HRESULT __stdcall ShellUtilXP_BindToFolderIDListParent( 
    __in_opt IShellFolder *psfRoot,  
    __in PCUIDLIST_RELATIVE pidl,  
    __in REFIID riid,  
    __deref_out void **ppv,  
    __deref_opt_out PCUITEMID_CHILD *ppidlLast) 
{ 
    // Call the corresponding "...Ex()" function with a NULL IBindCtx. 
    return ShellUtilXP_BindToFolderIDListParentEx( 
        psfRoot, pidl, NULL, riid, ppv, ppidlLast); 
} 
 
 
 
//////////////////////////////////////////////////////////////////////////////