
/* ----------------------------------- File Header -------------------------------------------*
	File				:	AddinManager.h
	Project Code		:	
	Author	    		:	Tom Thomas
	Created On	    	:	11/16/2004 12:00:47 PM
	Last Modified	   	:	11/16/2004 12:00:47 PM
----------------------------------------------------------------------------------------------*
	Type				:	Header File
	Description			:   
	Developer's Note	:	
	Bugs				:	
	See Also			:	
	Revision History	:	
	Traceability        :	
	Necessary Files		:	
---------------------------------------------------------------------------------------------*/

// AddinManager.h: interface for the CAddinManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ADDINMANAGER_H_INCLUDED__
#define __ADDINMANAGER_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PF_ADDIN_CMD_MIN_MSG_ID				WM_USER + 10000
#define PF_ADDIN_CMD_MAX_MSG_ID				WM_USER + 20000
			
/*----------------------------------- Class Header ------------------------------------------*
	Name				:	CAddinCommadInfo
	Date of Creation	:	10/1/2004 5:41:37 PM
	Type				:	BASE
	Functionality		:   
---------------------------------------------------------------------------------------------*/
class CAddinCommadInfo
{
public:
	CStringArray m_MenuStringsArray; //All Menus before coming to the leaf node
	UINT m_iCommandID;
	CString m_strMenuString;
	CString m_strHelpString;
	CString m_strToolTip;
	CString m_strFunction;
	CString m_strShortCutKey;
	int m_iToolbarIndex;
	UINT m_iSeparator;
	CArray<VARIANT,VARIANT> m_VarArgs;
	CAddinCommadInfo()
	{	
		m_iCommandID	= 0;
		m_iSeparator	= 0;
		m_iToolbarIndex	= -1;
	}
	CAddinCommadInfo(CAddinCommadInfo& AddinCommadInfo)
	{
		m_iCommandID	= AddinCommadInfo.m_iCommandID;
		m_strFunction	= AddinCommadInfo.m_strFunction;
		m_strMenuString	= AddinCommadInfo.m_strMenuString;
		m_strHelpString	= AddinCommadInfo.m_strHelpString;
		m_strToolTip	= AddinCommadInfo.m_strToolTip;
		m_strShortCutKey= AddinCommadInfo.m_strShortCutKey;
		m_iToolbarIndex	= AddinCommadInfo.m_iToolbarIndex;
		m_iSeparator	= AddinCommadInfo.m_iSeparator;

		m_VarArgs.Copy(AddinCommadInfo.m_VarArgs);
		m_MenuStringsArray.Copy(AddinCommadInfo.m_MenuStringsArray);

	}
	operator =(CAddinCommadInfo& AddinCommadInfo)
	{
		m_iCommandID	= AddinCommadInfo.m_iCommandID;
		m_strFunction	= AddinCommadInfo.m_strFunction;
		m_strMenuString	= AddinCommadInfo.m_strMenuString;
		m_strHelpString	= AddinCommadInfo.m_strHelpString;
		m_strToolTip	= AddinCommadInfo.m_strToolTip;
		m_strShortCutKey= AddinCommadInfo.m_strShortCutKey;
		m_iToolbarIndex	= AddinCommadInfo.m_iToolbarIndex;
		m_iSeparator	= AddinCommadInfo.m_iSeparator;

		m_VarArgs.Copy(AddinCommadInfo.m_VarArgs);
		m_MenuStringsArray.Copy(AddinCommadInfo.m_MenuStringsArray);
	}
};

/*----------------------------------- Class Header ------------------------------------------*
	Name				:	CAddinInfo
	Date of Creation	:	10/1/2004 1:20:53 PM
	Type				:	BASE
	Functionality		:   
---------------------------------------------------------------------------------------------*/

class CAddinInfo
{
public:
	CLSID m_clsID;
	CString m_strAddinName;
	long m_lInstanceHandle;
	long m_lToobarRes;
	BOOL m_bLoadAddin;
	UINT m_lToolbarButtonCount;
	IUnknown * m_pAddin;
	CArray<CAddinCommadInfo,CAddinCommadInfo> m_AddinCommadInfoArray;
public:
	CAddinInfo()
	{
		m_pAddin		= NULL;
		m_strAddinName	= "";
		m_bLoadAddin	= TRUE;
		m_lToolbarButtonCount= 0;
	}

	CAddinInfo(CAddinInfo & AddinInfo)
	{
		*this = AddinInfo;
	}

	operator =(CAddinInfo & AddinInfo)
	{
		m_clsID				  = AddinInfo.m_clsID;
		m_lInstanceHandle	  = AddinInfo.m_lInstanceHandle;
		m_lToobarRes		  = AddinInfo.m_lToobarRes;
		m_pAddin			  = AddinInfo.m_pAddin ;
		m_strAddinName		  = AddinInfo.m_strAddinName;
		m_bLoadAddin		  = AddinInfo.m_bLoadAddin;
		m_lToolbarButtonCount = AddinInfo.m_lToolbarButtonCount;

		m_AddinCommadInfoArray.SetSize(AddinInfo.m_AddinCommadInfoArray.GetSize());
		for(int i = 0; i < m_AddinCommadInfoArray.GetSize(); i++)
		{
			m_AddinCommadInfoArray[i] = AddinInfo.m_AddinCommadInfoArray[i];
		}
	}
};


/*----------------------------------- Class Header ------------------------------------------*
	Name				:	CAddinManager
	Date of Creation	:	10/1/2004 1:20:47 PM
	Type				:	BASE
	Functionality		:   
---------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
class CAddinManager
{
private:
	DWORD m_dwCookie;
	BOOL m_bLoadAllAdins;
	CArray<CAddinInfo,CAddinInfo> m_AddinInfoArray;
	CComPtr<IClass> m_pComObject;
	HRESULT LoadAllAdins(BSTR strAddinLoadingInfo,IClass * pObject, CATID catid, REFIID iidAddin);

	CLSID m_clsID;
	IID   m_iID;
public:
	
	BOOL InvokeAddinMenuItem(UINT iCommandID);
	BOOL SetAddinVisible(CString strAddinName, BOOL bVisible);
	const CAddinInfo& GetAddinInformation(UINT iCommandID);
	const CAddinCommadInfo& GetAddinCommadInfo(UINT iCommandID);
	CAddinCommadInfo GetAddinCommadInfo(long iAddinIndex,long lIndex);
	BOOL AddAddinCommandInfo(long iAddinIndex, CAddinCommadInfo AddinCommadInfo);
	BOOL SetAddinCount(long lCount);
	BOOL UnloadAllAddins();
	CAddinInfo GetAddinInfo(long iAddinIndex);
	BOOL SetAddinInfo(long iAddinIndex,CAddinInfo AddinInfo);
	long GetAddinCount();
	void SetLoadAllAddinStatus(BOOL bLoadAllAddins);
	virtual BOOL GetLoadAllAddinStatus();
	
public:	
	BOOL LoadAllAddins(REFCLSID clsID, REFIID iid);
	BOOL SaveAddinDefaultSettings();
	BOOL LoadAddinDefaultSettings();
	
	CAddinManager();
	virtual ~CAddinManager();
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template<class IClass, class IAddinClass>
CAddinManager<IClass, IAddinClass>::CAddinManager()
{
	m_bLoadAllAdins=TRUE;
}

template<class IClass, class IAddinClass>
CAddinManager<IClass, IAddinClass>::~CAddinManager()
{
	CoUninitialize();	
}


/*-------------------------------------------- Function Header ---------------------------------------------*
	Function Name	  : CAddinManager::LoadAddinDefaultSettings
	Description	      : 
	Return Type	      : BOOL 
	Warning           : 
------------------------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
BOOL CAddinManager<IClass, IAddinClass>::LoadAddinDefaultSettings()
{
	return TRUE;
}


/*-------------------------------------------- Function Header ---------------------------------------------*
	Function Name	  : CAddinManager::SaveAddinDefaultSettings
	Description	      : 
	Return Type	      : BOOL 
	Warning           : 
------------------------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
BOOL CAddinManager<IClass, IAddinClass>::SaveAddinDefaultSettings()
{
	return TRUE;
}


/*-------------------------------------------- Function Header ---------------------------------------------*
	Function Name	  : CAddinManager::LoadAllAddins
	Description	      : 
	Return Type	      : BOOL 
	Warning           : 
------------------------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
BOOL CAddinManager<IClass, IAddinClass>::LoadAllAddins(REFCLSID clsID, REFIID iid)
{
	//Load all the addins here by looking 
	//at the default settings from the registry

	HRESULT m_hCoInitialize;
	HRESULT hResult = S_OK;          
	//COM Iniialize
	hResult = CoInitialize(NULL);
	if(SUCCEEDED(hResult))
	{
		m_hCoInitialize = hResult;
		hResult = m_pComObject.CoCreateInstance(clsID);
	}
	if(!SUCCEEDED(hResult))
	{
		AfxMessageBox(_T("Addin in Server Object is Failed."));
		return FALSE;
	}
	AfxConnectionAdvise((LPUNKNOWN)m_pComObject,
								  iid,
								  AfxGetApp()->GetIDispatch(FALSE),
								  FALSE,&m_dwCookie);

	CString strAddinSettingsInfo(_T(""));
	if(LoadAllAdins(strAddinSettingsInfo.AllocSysString(),m_pComObject)==E_FAIL)
	{
		return FALSE;
	}

	m_clsID = clsID;
	m_iID   = iid;
	return TRUE;
}


/*-------------------------------------------- Function Header ---------------------------------------------*
	Function Name	  : CAddinManager::GetLoadAllAddinStatus
	Description	      : Override this function if you want to load all addin conditionally..
						That is depending upon the application settings.

	Return Type	      : BOOL 
	Warning           : 
------------------------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
BOOL CAddinManager<IClass, IAddinClass>::GetLoadAllAddinStatus()
{
	return m_bLoadAllAdins;
}


/*-------------------------------------------- Function Header ---------------------------------------------*
	Function Name	  : CAddinManager::SetLoadAllAddinStatus
	Description	      : 
	Return Type	      : void 
	Warning           : 
  Argument          : BOOL bLoadAllAddins
------------------------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
void CAddinManager<IClass, IAddinClass>::SetLoadAllAddinStatus(BOOL bLoadAllAddins)
{
	m_bLoadAllAdins = bLoadAllAddins;
}


/*-------------------------------------------- Function Header ---------------------------------------------*
	Function Name	  : CAddinManager::GetAddinCount
	Description	      : 
	Return Type	      : long 
	Warning           : 
------------------------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
long CAddinManager<IClass, IAddinClass>::GetAddinCount()
{
	return m_AddinInfoArray.GetSize();
}


/*-------------------------------------------- Function Header ---------------------------------------------*
	Function Name	  : CAddinManager::SetAddinInfo
	Description	      : 
	Return Type	      : BOOL 
	Warning           : 
  Argument          : long iAddinIndex
  Argument          : CAddinInfo AddinInfo
------------------------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
BOOL CAddinManager<IClass, IAddinClass>::SetAddinInfo(long iAddinIndex,CAddinInfo AddinInfo)
{
	m_AddinInfoArray.SetAt(iAddinIndex,AddinInfo);
	return TRUE;
}


/*-------------------------------------------- Function Header ---------------------------------------------*
	Function Name	  : CAddinManager::GetAddinInfo
	Description	      : 
	Return Type	      : CAddinInfo 
	Warning           : 
  Argument          : long iAddinIndex
------------------------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
CAddinInfo CAddinManager<IClass, IAddinClass>::GetAddinInfo(long iAddinIndex)
{
	//Iterate through the loop and 
	return m_AddinInfoArray[iAddinIndex];
}


template<class IClass, class IAddinClass>
BOOL CAddinManager<IClass, IAddinClass>::UnloadAllAddins()
{

	AfxConnectionUnadvise((LPUNKNOWN)m_pComObject,
												  m_iID,
												  AfxGetApp()->GetIDispatch(FALSE),
												  FALSE,m_dwCookie);
	m_pComObject.Release();
	for(int i=0;i<m_AddinInfoArray.GetSize();i++)
	{
		((IAddinClass)m_AddinInfoArray[i].m_pAddin)->Release();  
	}
	CoUninitialize();
	return TRUE;
}


template<class IClass, class IAddinClass>
BOOL CAddinManager<IClass, IAddinClass>::SetAddinCount(long lCount)
{
	m_AddinInfoArray.SetSize(lCount);
	return TRUE;
}


template<class IClass, class IAddinClass>
BOOL CAddinManager<IClass, IAddinClass>::AddAddinCommandInfo(long iAddinIndex, CAddinCommadInfo AddinCommadInfo)
{
	m_AddinInfoArray[iAddinIndex].m_AddinCommadInfoArray.Add(AddinCommadInfo);
	return TRUE;
}


template<class IClass, class IAddinClass>
CAddinCommadInfo CAddinManager<IClass, IAddinClass>::GetAddinCommadInfo(long iAddinIndex,long lIndex)
{
	return m_AddinInfoArray[iAddinIndex].m_AddinCommadInfoArray[lIndex];
}


/*-------------------------------------------- Function Header ---------------------------------------------*
	Function Name	  : CAddinManager::GetAddinCommadInfo
	Description	      : 
	Return Type	      : const CAddinCommadInfo& 
	Warning           : 
  Argument          : UINT iCommandID
------------------------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
const CAddinCommadInfo& CAddinManager<IClass, IAddinClass>::GetAddinCommadInfo(UINT iCommandID)
{
	static CAddinCommadInfo AddinCommadInfo;
	AddinCommadInfo.m_iCommandID=0; 
	for(int i=0;i<m_AddinInfoArray.GetSize();i++)
	{
		for(int j=0;j<m_AddinInfoArray[i].m_AddinCommadInfoArray.GetSize();j++)
		{
			if(m_AddinInfoArray[i].m_AddinCommadInfoArray[j].m_iCommandID==iCommandID)
			{
				return m_AddinInfoArray[i].m_AddinCommadInfoArray[j];
			}
		}
	}
	return AddinCommadInfo;
}


/*-------------------------------------------- Function Header ---------------------------------------------*
	Function Name	  : CAddinManager::GetAddinInformation
	Description	      : 
	Return Type	      : const CAddinInfo& 
	Warning           : 
  Argument          : UINT iCommandID
------------------------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
const CAddinInfo& CAddinManager<IClass, IAddinClass>::GetAddinInformation(UINT iCommandID)
{
	static CAddinInfo AddinInfo;
	for(int i=0;i<m_AddinInfoArray.GetSize();i++)
	{
		for(int j=0;j<m_AddinInfoArray[i].m_AddinCommadInfoArray.GetSize();j++)
		{
			if(m_AddinInfoArray[i].m_AddinCommadInfoArray[j].m_iCommandID==iCommandID)
			{
				return m_AddinInfoArray[i];
			}
		}
	}
	return AddinInfo;
}


template<class IClass, class IAddinClass>
BOOL CAddinManager<IClass, IAddinClass>::SetAddinVisible(CString strAddinName, BOOL bVisible)
{
	for(int i=0;i<m_AddinInfoArray.GetSize();i++)
	{
		if(m_AddinInfoArray[i].m_strAddinName==strAddinName)
		{
			m_AddinInfoArray[i].m_bLoadAddin=bVisible;
			return TRUE;
		}
	}
	return FALSE;
}


/*-------------------------------------------- Function Header ---------------------------------------------*
	Function Name	  : CAddinManager::InvokeAddinMenuItem
	Description	      : Ivokes function corresponding to addin menu item
	Return Type	      : BOOL 
	Warning           : 
  Argument          : UINT nID
------------------------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
BOOL CAddinManager<IClass, IAddinClass>::InvokeAddinMenuItem(UINT nID)
{
	try
	{
		const CAddinInfo& AddinInfo=GetAddinInformation(nID);
		const CAddinCommadInfo& AddinCommadInfo=GetAddinCommadInfo(nID);
		HRESULT hResult=E_FAIL;
		IDispatch * pDisp = (IDispatch *)NULL;
		DISPID dispid;
		CString strFunction=AddinCommadInfo.m_strFunction;
		//OLECHAR FAR* szMember = (OLECHAR FAR*)strFunction.GetBuffer(strFunction.GetLength());

		OLECHAR FAR* szMember = strFunction.AllocSysString() ;

		// Code that sets a pointer to the dispatch (pdisp) is omitted.
		
		DISPPARAMS  DispParams;
		memset(&DispParams, 0, sizeof DispParams);
		VARIANT VarResult;
		memset(&VarResult, 0, sizeof VarResult);
		EXCEPINFO ExcepInfo;
		memset(&ExcepInfo, 0, sizeof ExcepInfo);
		unsigned int uArgErr=0;
		
		hResult = ((IAddinClass)AddinInfo.m_pAddin)->QueryInterface(IID_IDispatch,(void **)&pDisp);
		if(SUCCEEDED(hResult))
		{
			hResult=pDisp->GetIDsOfNames(IID_NULL,&szMember, 1, LOCALE_SYSTEM_DEFAULT,&dispid);
			if(FAILED(hResult))
			{
				return FALSE;
			}
			//Get the DispId so call the invoke function after filling the parameters
			hResult=pDisp->Invoke(dispid,IID_NULL,LOCALE_SYSTEM_DEFAULT,DISPATCH_METHOD|DISPATCH_PROPERTYGET ,&DispParams,&VarResult,&ExcepInfo,&uArgErr); 
			if(FAILED(hResult))
			{
				CString strErrorString;
				strErrorString.Format("Faild to invoke %s function",strFunction);
				AfxMessageBox(strErrorString);
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	catch(...)
	{
		return FALSE;
	}
	return TRUE;
}


/*-------------------------------------------- Function Header ---------------------------------------------*
	Function Name	  : CAddinManager::LoadAllAdins
	Description	      : Loads all the addins belonging the certain catogory ID CATID_PROJECT_FRAMEWORK_ADDINS
	Return Type	      : HRESULT 
	Warning           : 
  Argument          : BSTR strAddinLoadingInfo , settings information from file/registry about which addin to load etc
  Argument          : IProjectFramework* pProjectFramework
------------------------------------------------------------------------------------------------------------*/
template<class IClass, class IAddinClass>
HRESULT CAddinManager<IClass, IAddinClass>::LoadAllAdins(BSTR strAddinLoadingInfo,IClass * pObject, CATID catid, REFIID iidAddin)
{
	try
	{
		CString strAddinLoadingInformation=strAddinLoadingInfo;
		if(strAddinLoadingInformation=="")
		{
			//Load All addins
			CLSID clsid[40];
			ICatInformation *pCatInfo = NULL;
			HRESULT hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 0,
						CLSCTX_SERVER, IID_ICatInformation, (void **)&pCatInfo);
			if (!SUCCEEDED(hr))
			{
				AfxMessageBox("Catagory Failed");
				return E_FAIL;
			}
			IEnumCLSID *pCLSID = NULL;
			CATID catids[1];
			catids[0] = catid;
			int iAddinCount=0;
			hr = pCatInfo->EnumClassesOfCategories(1, catids, -1, 0, &pCLSID);
			do
			{
				DWORD num = 0;
				hr = pCLSID->Next(40, clsid, &num);
				if(!SUCCEEDED(hr))
				{
					AfxMessageBox("Class ID  Failed");
					return E_FAIL;
				}
				iAddinCount=iAddinCount+num;
				CDrawApp::m_AddinManager.SetAddinCount(iAddinCount); 
				for (long i = 0; i < (long)num; i++)
				{
					IAddinClass * pAddin = NULL;
					
					hr = CoCreateInstance(clsid[i], NULL,CLSCTX_INPROC_SERVER,iidAddin, (void **)&pAddin);
					
					if(SUCCEEDED(hr))
					{
						CAddinInfo AddinInfo;
						AddinInfo.m_pAddin= (IUnknown*)pAddin;
						AddinInfo.m_clsID=clsid[i];
						CDrawApp::m_AddinManager.SetAddinInfo(i,AddinInfo);
						VARIANT bFirstTime;
						bFirstTime.vt =VT_BOOL;
						bFirstTime.boolVal=TRUE; 
						pAddin->Initialize(i, pObject,bFirstTime);
					}
					else
					{
																				
					}
				
				}
				
			} while (hr == S_OK);
			pCLSID->Release();
			pCatInfo->Release();
			//Fill the session manager with the routing table
			return S_OK;
		}
		else
		{
			//Load selected addins
		}
	}
	
	catch(...)
	{
		AfxMessageBox("Generic Error");
		return E_FAIL;
	}

	return S_OK;
}

#endif // !defined(__ADDINMANAGER_H_INCLUDED__)
