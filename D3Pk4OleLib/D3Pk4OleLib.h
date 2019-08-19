

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Sun Nov 19 00:32:37 2006
 */
/* Compiler settings for .\D3Pk4OleLib.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __D3Pk4OleLib_h__
#define __D3Pk4OleLib_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ID3Treefs_FWD_DEFINED__
#define __ID3Treefs_FWD_DEFINED__
typedef interface ID3Treefs ID3Treefs;
#endif 	/* __ID3Treefs_FWD_DEFINED__ */


#ifndef __D3Treefs_FWD_DEFINED__
#define __D3Treefs_FWD_DEFINED__

#ifdef __cplusplus
typedef class D3Treefs D3Treefs;
#else
typedef struct D3Treefs D3Treefs;
#endif /* __cplusplus */

#endif 	/* __D3Treefs_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ID3Treefs_INTERFACE_DEFINED__
#define __ID3Treefs_INTERFACE_DEFINED__

/* interface ID3Treefs */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ID3Treefs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1D7BF72B-5737-4BF1-A853-3435CCD6A99B")
    ID3Treefs : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Addfs( 
            BSTR PathName,
            /* [retval][out] */ VARIANT_BOOL *ReturnVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetListfs( 
            /* [retval][out] */ VARIANT *ReturnVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SelectFile( 
            BSTR PathName,
            /* [retval][out] */ VARIANT_BOOL *ReturnVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_File( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FileSize( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FileData( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FileTime( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FileDataAsText( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ID3TreefsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ID3Treefs * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ID3Treefs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ID3Treefs * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ID3Treefs * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ID3Treefs * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ID3Treefs * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ID3Treefs * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            ID3Treefs * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Addfs )( 
            ID3Treefs * This,
            BSTR PathName,
            /* [retval][out] */ VARIANT_BOOL *ReturnVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetListfs )( 
            ID3Treefs * This,
            /* [retval][out] */ VARIANT *ReturnVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SelectFile )( 
            ID3Treefs * This,
            BSTR PathName,
            /* [retval][out] */ VARIANT_BOOL *ReturnVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_File )( 
            ID3Treefs * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileSize )( 
            ID3Treefs * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileData )( 
            ID3Treefs * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileTime )( 
            ID3Treefs * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileDataAsText )( 
            ID3Treefs * This,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } ID3TreefsVtbl;

    interface ID3Treefs
    {
        CONST_VTBL struct ID3TreefsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ID3Treefs_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ID3Treefs_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ID3Treefs_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ID3Treefs_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ID3Treefs_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ID3Treefs_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ID3Treefs_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ID3Treefs_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define ID3Treefs_Addfs(This,PathName,ReturnVal)	\
    (This)->lpVtbl -> Addfs(This,PathName,ReturnVal)

#define ID3Treefs_GetListfs(This,ReturnVal)	\
    (This)->lpVtbl -> GetListfs(This,ReturnVal)

#define ID3Treefs_SelectFile(This,PathName,ReturnVal)	\
    (This)->lpVtbl -> SelectFile(This,PathName,ReturnVal)

#define ID3Treefs_get_File(This,pVal)	\
    (This)->lpVtbl -> get_File(This,pVal)

#define ID3Treefs_get_FileSize(This,pVal)	\
    (This)->lpVtbl -> get_FileSize(This,pVal)

#define ID3Treefs_get_FileData(This,pVal)	\
    (This)->lpVtbl -> get_FileData(This,pVal)

#define ID3Treefs_get_FileTime(This,pVal)	\
    (This)->lpVtbl -> get_FileTime(This,pVal)

#define ID3Treefs_get_FileDataAsText(This,pVal)	\
    (This)->lpVtbl -> get_FileDataAsText(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ID3Treefs_Close_Proxy( 
    ID3Treefs * This);


void __RPC_STUB ID3Treefs_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ID3Treefs_Addfs_Proxy( 
    ID3Treefs * This,
    BSTR PathName,
    /* [retval][out] */ VARIANT_BOOL *ReturnVal);


void __RPC_STUB ID3Treefs_Addfs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ID3Treefs_GetListfs_Proxy( 
    ID3Treefs * This,
    /* [retval][out] */ VARIANT *ReturnVal);


void __RPC_STUB ID3Treefs_GetListfs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ID3Treefs_SelectFile_Proxy( 
    ID3Treefs * This,
    BSTR PathName,
    /* [retval][out] */ VARIANT_BOOL *ReturnVal);


void __RPC_STUB ID3Treefs_SelectFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ID3Treefs_get_File_Proxy( 
    ID3Treefs * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ID3Treefs_get_File_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ID3Treefs_get_FileSize_Proxy( 
    ID3Treefs * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB ID3Treefs_get_FileSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ID3Treefs_get_FileData_Proxy( 
    ID3Treefs * This,
    /* [retval][out] */ VARIANT *pVal);


void __RPC_STUB ID3Treefs_get_FileData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ID3Treefs_get_FileTime_Proxy( 
    ID3Treefs * This,
    /* [retval][out] */ DATE *pVal);


void __RPC_STUB ID3Treefs_get_FileTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ID3Treefs_get_FileDataAsText_Proxy( 
    ID3Treefs * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ID3Treefs_get_FileDataAsText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ID3Treefs_INTERFACE_DEFINED__ */



#ifndef __D3Pk4OleLibLib_LIBRARY_DEFINED__
#define __D3Pk4OleLibLib_LIBRARY_DEFINED__

/* library D3Pk4OleLibLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_D3Pk4OleLibLib;

EXTERN_C const CLSID CLSID_D3Treefs;

#ifdef __cplusplus

class DECLSPEC_UUID("7866C00B-DB1B-4063-9C16-73F6DC1B868C")
D3Treefs;
#endif
#endif /* __D3Pk4OleLibLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


