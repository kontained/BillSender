// BillSender.cpp : main project file.
#include "stdafx.h"
#include "Form1.h"
#include <shlobj.h>
#include <vcclr.h>
#include <Windows.h>
#include <Wia.h>
#include <Objbase.h>
#include <comdef.h>

using namespace BillSender;
TCHAR DIRECTORY[MAX_PATH];
_bstr_t DevID;
[STAThreadAttribute]

#pragma comment(lib, "shell32.lib")

HRESULT CreateWiaDeviceManager(IWiaDevMgr2** ppWiaDevMgr)
{
	//
	// Validate arguments
	//
	if (NULL == ppWiaDevMgr)
	{
		return E_INVALIDARG;
	}

	//
	// Initialize out variables
	//
	*ppWiaDevMgr = NULL;

	//
	// Create an instance of the device manager
	//
	HRESULT hr = CoCreateInstance(CLSID_WiaDevMgr2, NULL, CLSCTX_LOCAL_SERVER, IID_IWiaDevMgr2, (void**)ppWiaDevMgr);

	//
	// Return the result of creating the device manager
	//
	return hr;
}
HRESULT CreateWiaDevice( IWiaDevMgr2 *pWiaDevMgr, BSTR bstrDeviceID, IWiaItem2 **ppWiaDevice ) 
{
	//
	// Validate arguments
	//
	if (NULL == pWiaDevMgr || NULL == bstrDeviceID || NULL == ppWiaDevice)
	{
		return E_INVALIDARG;
	}

	//
	// Initialize out variables
	//
	*ppWiaDevice = NULL;

	//
	// Create the WIA Device
	//
	HRESULT hr = pWiaDevMgr->CreateDevice(0, bstrDeviceID, ppWiaDevice );

	//
	// Return the result of creating the device
	//
	return hr;
}
HRESULT ReadSomeWiaProperties( IWiaPropertyStorage *pWiaPropertyStorage )
{
	//
	// Validate arguments
	//
	if (NULL == pWiaPropertyStorage)
	{
		return E_INVALIDARG;
	}

	//
	// Declare PROPSPECs and PROPVARIANTs, and initialize them to zero.
	//
	PROPSPEC PropSpec[3] = {0};
	PROPVARIANT PropVar[3] = {0};

	//
	// How many properties are you querying for?
	//
	const ULONG c_nPropertyCount = sizeof(PropSpec)/sizeof(PropSpec[0]);

	//
	// Define which properties you want to read:
	// Device ID.  This is what you would use to create
	// the device.
	//
	PropSpec[0].ulKind = PRSPEC_PROPID;
	PropSpec[0].propid = WIA_DIP_DEV_ID;

	//
	// Device Name
	//
	PropSpec[1].ulKind = PRSPEC_PROPID;
	PropSpec[1].propid = WIA_DIP_DEV_NAME;

	//
	// Device description
	//
	PropSpec[2].ulKind = PRSPEC_PROPID;
	PropSpec[2].propid = WIA_DIP_DEV_DESC;

	//
	// Ask for the property values
	//
	HRESULT hr = pWiaPropertyStorage->ReadMultiple( c_nPropertyCount, PropSpec, PropVar );
	if (SUCCEEDED(hr))
	{
		//
		// IWiaPropertyStorage::ReadMultiple will return S_FALSE if some
		// properties could not be read, so you have to check the return
		// types for each requested item.
		//

		//
		// Check the return type for the device ID
		//
		if (VT_BSTR == PropVar[0].vt)
		{
			DevID = PropVar[0].bstrVal;
		}

		//
		// Check the return type for the device name
		//
		if (VT_BSTR == PropVar[1].vt)
		{
			//
			// Do something with the device name
			//
			//_tprintf( TEXT("WIA_DIP_DEV_NAME: %ws\n"), PropVar[1].bstrVal );
		}

		//
		// Check the return type for the device description
		//
		if (VT_BSTR == PropVar[2].vt)
		{
			//
			// Do something with the device description
			//
			//_tprintf( TEXT("WIA_DIP_DEV_DESC: %ws\n"), PropVar[2].bstrVal );
		}

		//
		// Free the returned PROPVARIANTs
		//
		FreePropVariantArray( c_nPropertyCount, PropVar );
	}

	//
	// Return the result of reading the properties
	//
	return hr;
}
HRESULT GetWiaDevices( IWiaDevMgr2 *pWiaDevMgr ) //Vista or later
{
	//
	// Validate arguments
	//
	if (NULL == pWiaDevMgr)
	{
		return E_INVALIDARG;
	}

	//
	// Get a device enumerator interface
	//
	IEnumWIA_DEV_INFO *pWiaEnumDevInfo = NULL;
	HRESULT hr = pWiaDevMgr->EnumDeviceInfo( WIA_DEVINFO_ENUM_LOCAL, &pWiaEnumDevInfo );
	if (SUCCEEDED(hr))
	{
		//
		// Loop until you get an error or pWiaEnumDevInfo->Next returns
		// S_FALSE to signal the end of the list.
		//
		while (S_OK == hr)
		{
			//
			// Get the next device's property storage interface pointer
			//
			IWiaPropertyStorage *pWiaPropertyStorage = NULL;
			hr = pWiaEnumDevInfo->Next( 1, &pWiaPropertyStorage, NULL );

			//
			// pWiaEnumDevInfo->Next will return S_FALSE when the list is
			// exhausted, so check for S_OK before using the returned
			// value.
			//
			if (hr == S_OK)
			{
				//
				// Do something with the device's IWiaPropertyStorage*
				//
				ReadSomeWiaProperties(pWiaPropertyStorage);

				//
				// Release the device's IWiaPropertyStorage*
				//
				pWiaPropertyStorage->Release();
				pWiaPropertyStorage = NULL;
			}
		}

		//
		// If the result of the enumeration is S_FALSE (which
		// is normal), change it to S_OK.
		//
		if (S_FALSE == hr)
		{
			hr = S_OK;
		}

		//
		// Release the enumerator
		//
		pWiaEnumDevInfo->Release();
		pWiaEnumDevInfo = NULL;
	}

	//
	// Return the result of the enumeration
	//
	return hr;
}
int GetFileCount()
{
	String^ line;
	int value = 0;

	//Convert DIRECTORY to string, then add "\\BillSender\\" to it
	String^ dir = "";
	String^ dir1 = "\\BillSender\\";
	TCHAR tmp[MAX_PATH];
	wmemset(tmp, L'', MAX_PATH);

	for (int i = 0; i < MAX_PATH; i++)
	{
		if (DIRECTORY[i] == NULL)
		{
			dir = dir + dir1;
			break;
		}
		else
			dir = dir + DIRECTORY[i];
	}

	for (int i = 0; i < dir->Length; i++)
	{
		if (dir[i] == NULL)
			break;
		else
			tmp[i] = dir[i];
	}

	try
	{
		wcsncat_s(tmp, L"count.ini", wcslen(tmp) + 16);
		CreateFile(tmp, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

		StreamReader^ in = gcnew StreamReader(dir + "count.ini");

		line = in->ReadLine();
		in->Close();

		value = Convert::ToInt32(line);
		value++;

		StreamWriter^ out = gcnew StreamWriter(dir + "count.ini");

		out->Write(Convert::ToString(value));
		out->Close();
	}
	catch (...){}

	return value;
}
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}
/*Handles attaching the files to the email then sends it*/
void Form1::SendBill_Click(System::Object^  sender, System::EventArgs^  e) {
	//gmail server info
	int port = 587;
	String^ server = "smtp.gmail.com";

	//Convert DIRECTORY to string, then add "\\BillSender\\" to it
	String^ dir = "";
	String^ dir1 = "\\BillSender\\";
	TCHAR tmp[MAX_PATH];
	wmemset(tmp, L'', MAX_PATH);

	for (int i = 0; i < MAX_PATH; i++)
	{
		if (DIRECTORY[i] == NULL)
		{
			dir = dir + dir1;
			break;
		}
		else
			dir = dir + DIRECTORY[i];
	}

	for (int i = 0; i < dir->Length; i++)
	{
		if (dir[i] == NULL)
			break;
		else
			tmp[i] = dir[i];
	}

	//To field empty
	if (String::IsNullOrEmpty(EmailTo->Text))
	{
		MessageBox::Show("Please input an email address.", "NO EMAIL ADDRESS", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}

	//Subject field empty
	if (String::IsNullOrEmpty(EmailSubject->Text))
	{
		MessageBox::Show("Please input a subject.", "NO SUBJECT", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}

	//Set the from, to, subject, and body of the email
	String^ From = L"jprucool@gmail.com";

	MailMessage^ msg = gcnew MailMessage(From, EmailTo->Text);

	msg->Subject = EmailSubject->Text;
	msg->Body = L"Please see attached files.";

	//creat the smtp client that will send the message
	SmtpClient^ client = gcnew SmtpClient();

	client->Host = server;
	client->Port = port;

	//set up the credentials for the gmail account
	client->UseDefaultCredentials = false;
	client->DeliveryMethod = SmtpDeliveryMethod::Network;
	client->Credentials = gcnew System::Net::NetworkCredential("test", "test");

	//gmail requires ssl
	client->EnableSsl = true;

	//Attach the image
	Attachment^ data = gcnew Attachment(dir + FileValue + ".jpg");
	msg->Attachments->Add(data);

	//send the message
	try
	{
		client->Send(msg);
	}
	catch (SmtpFailedRecipientException^ e)
	{
		MessageBox::Show("Failed to send email to recipient. \n Please make sure email address is correct.", "ERROR", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}
	catch (...)
	{
		MessageBox::Show("Error sending Email. \n Please try again.", "ERROR", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}
	client->~SmtpClient();

	//Write the email to the recent email list for easy access next time to emails.
	StreamWriter^ out = gcnew StreamWriter(dir + "recentEmails.txt", true);
	out->WriteLine(EmailTo->Text);
	out->Close();

	ScannedImage->Hide();

	EmailTo->Text = "";
	EmailSubject->Text = "";
}
/*Gets the device information, scans in the image, and saves it to disk*/
void Form1::ScanImage_Click(System::Object ^sender, System::EventArgs ^e) {
	IWiaDevMgr2* devMgr;
	IWiaItem2* item;
	HRESULT hr;
	LONG x = 0L;
	int fileCount = 0;
	BSTR *files = new BSTR[0];

	//Convert DIRECTORY to string, then add "\\BillSender\\" to it
	String^ dir = "";
	String^ dir1 = "\\BillSender\\";
	TCHAR tmp[MAX_PATH];
	wmemset(tmp, L'', MAX_PATH);

	for (int i = 0; i < MAX_PATH; i++)
	{
		if (DIRECTORY[i] == NULL)
		{
			dir = dir + dir1;
			break;
		}
		else
			dir = dir + DIRECTORY[i];
	}

	for (int i = 0; i < dir->Length; i++)
	{
		if (dir[i] == NULL)
			break;
		else
			tmp[i] = dir[i];
	}

	FileValue = gcnew String("TMS");

	fileCount = GetFileCount();

	FileValue = FileValue + fileCount;

	pin_ptr<const wchar_t> wch = PtrToStringChars(FileValue);
	printf_s("%S\n", wch);

	hr = CreateWiaDeviceManager(&devMgr);

	if (hr == S_OK)
		hr = GetWiaDevices(devMgr);

	if (hr == S_OK)
		hr = CreateWiaDevice(devMgr, DevID, &item);


	_bstr_t FolderName(tmp);
	_bstr_t FileName(wch);

	try
	{
		//Displays a dialog that allows the user to select properties, then scan.
		hr = item->DeviceDlg(0, GetActiveWindow(), FolderName, FileName, &x, &files, &item);
	}
	catch (...)
	{
		MessageBox::Show("Failed to scan the image. \n Please make sure the scanner is on.", "ERROR", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}

	//Display the image to the screen
	ScannedImage->SizeMode = PictureBoxSizeMode::StretchImage;
	ScannedImage->Load(dir + FileValue + ".jpg");
	ScannedImage->Show();
}
/*Load recent recipients box and display the welcome image
Also retrieves the 'My Documents' folder for the user*/
void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e)
{
	//Get the my documents directory for the logged in user
	HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, DIRECTORY);

	//Convert DIRECTORY to string, then add "\\BillSender\\" to it
	String^ dir = "";
	String^ dir1 = "\\BillSender\\";
	TCHAR tmp[MAX_PATH];
	wmemset(tmp, L'', MAX_PATH);

	for (int i = 0; i < MAX_PATH; i++)
	{
		if (DIRECTORY[i] == NULL)
		{
			dir = dir + dir1;
			break;
		}
		else
			dir = dir + DIRECTORY[i];
	}

	for (int i = 0; i < dir->Length; i++)
	{
		if (dir[i] == NULL)
			break;
		else
			tmp[i] = dir[i];
	}

	//create the BillSender directory if it doesn't exist
	CreateDirectory(tmp, NULL);

	//Show Welcome image
	try
	{
		ScannedImage->SizeMode = PictureBoxSizeMode::StretchImage;
		ScannedImage->Load(dir + "WelcomePage.jpg");
		ScannedImage->Show();
	}
	catch (...){}

	//read in recent recipients
	try
	{
		//concat "recentEmails.txt" to the end of the string so we can create the file
		wcsncat_s(tmp, L"recentEmails.txt", wcslen(tmp) + 16);
		CreateFile(tmp, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

		StreamReader^ in = gcnew StreamReader(dir + "recentEmails.txt");

		String^ Recipient;

		while ((Recipient = in->ReadLine()) != nullptr)
			RecentRecipList->Items->Add(Recipient);

		in->Close();
	}
	catch (...){}
}
/*Double click an email to place it into the email field*/
void Form1::RecentRecipList_DoubleClick(System::Object^  sender, System::EventArgs^  e)
{
	//Clear text box
	EmailTo->Text = "";

	String^ Recipient = RecentRecipList->FocusedItem->Text;

	EmailTo->Text = Recipient;
}