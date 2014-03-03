// BillSender.cpp : main project file.
#include "stdafx.h"
#include "Form1.h"
#include <shlobj.h>
#include <vcclr.h>
#include <Windows.h>
#include <Wia.h>
#include <Objbase.h>
#include <comdef.h>
#include <string>

using namespace BillSender;
TCHAR DIRECTORY[MAX_PATH];
_bstr_t DevID;
[STAThreadAttribute]

#pragma comment(lib, "shell32.lib")

HRESULT CreateWiaDeviceManager(IWiaDevMgr2** ppWiaDevMgr)
{
	// Validate arguments
	if (NULL == ppWiaDevMgr)
	{
		return E_INVALIDARG;
	}

	// Initialize out variables
	*ppWiaDevMgr = NULL;

	// Create an instance of the device manager
	HRESULT hr = CoCreateInstance(CLSID_WiaDevMgr2, NULL, CLSCTX_LOCAL_SERVER, IID_IWiaDevMgr2, (void**)ppWiaDevMgr);

	// Return the result of creating the device manager
	return hr;
}
HRESULT CreateWiaDevice( IWiaDevMgr2 *pWiaDevMgr, BSTR bstrDeviceID, IWiaItem2 **ppWiaDevice ) 
{
	// Validate arguments
	if (NULL == pWiaDevMgr || NULL == bstrDeviceID || NULL == ppWiaDevice)
	{
		return E_INVALIDARG;
	}

	// Initialize out variables
	*ppWiaDevice = NULL;

	// Create the WIA Device
	HRESULT hr = pWiaDevMgr->CreateDevice(0, bstrDeviceID, ppWiaDevice );

	// Return the result of creating the device
	return hr;
}
HRESULT ReadSomeWiaProperties( IWiaPropertyStorage *pWiaPropertyStorage )
{
	// Validate arguments
	if (NULL == pWiaPropertyStorage)
	{
		return E_INVALIDARG;
	}

	// Declare PROPSPECs and PROPVARIANTs, and initialize them to zero.
	PROPSPEC PropSpec[3] = {0};
	PROPVARIANT PropVar[3] = {0};

	//
	// How many properties are you querying for?
	//
	const ULONG c_nPropertyCount = sizeof(PropSpec)/sizeof(PropSpec[0]);

	// Define which properties you want to read:
	// Device ID.  This is what you would use to create
	// the device.
	PropSpec[0].ulKind = PRSPEC_PROPID;
	PropSpec[0].propid = WIA_DIP_DEV_ID;

	// Device Name
	PropSpec[1].ulKind = PRSPEC_PROPID;
	PropSpec[1].propid = WIA_DIP_DEV_NAME;

	// Device description
	PropSpec[2].ulKind = PRSPEC_PROPID;
	PropSpec[2].propid = WIA_DIP_DEV_DESC;

	// Ask for the property values
	HRESULT hr = pWiaPropertyStorage->ReadMultiple( c_nPropertyCount, PropSpec, PropVar );
	if (SUCCEEDED(hr))
	{
		// IWiaPropertyStorage::ReadMultiple will return S_FALSE if some
		// properties could not be read, so you have to check the return
		// types for each requested item.

		// Check the return type for the device ID
		if (VT_BSTR == PropVar[0].vt)
		{
			DevID = PropVar[0].bstrVal;
		}

		// Check the return type for the device name
		if (VT_BSTR == PropVar[1].vt)
		{
			// Do something with the device name
			//_tprintf( TEXT("WIA_DIP_DEV_NAME: %ws\n"), PropVar[1].bstrVal );
		}

		// Check the return type for the device description
		if (VT_BSTR == PropVar[2].vt)
		{
			// Do something with the device description
			//_tprintf( TEXT("WIA_DIP_DEV_DESC: %ws\n"), PropVar[2].bstrVal );
		}

		// Free the returned PROPVARIANTs
		FreePropVariantArray( c_nPropertyCount, PropVar );
	}
	// Return the result of reading the properties
	return hr;
}
HRESULT GetWiaDevices( IWiaDevMgr2 *pWiaDevMgr ) //Vista or later
{
	// Validate arguments
	if (NULL == pWiaDevMgr)
	{
		return E_INVALIDARG;
	}

	// Get a device enumerator interface
	IEnumWIA_DEV_INFO *pWiaEnumDevInfo = NULL;
	HRESULT hr = pWiaDevMgr->EnumDeviceInfo( WIA_DEVINFO_ENUM_LOCAL, &pWiaEnumDevInfo );
	if (SUCCEEDED(hr))
	{
		// Loop until you get an error or pWiaEnumDevInfo->Next returns
		// S_FALSE to signal the end of the list.
		while (S_OK == hr)
		{
			// Get the next device's property storage interface pointer
			IWiaPropertyStorage *pWiaPropertyStorage = NULL;
			hr = pWiaEnumDevInfo->Next( 1, &pWiaPropertyStorage, NULL );

			// pWiaEnumDevInfo->Next will return S_FALSE when the list is
			// exhausted, so check for S_OK before using the returned
			// value.
			if (hr == S_OK)
			{
				// Do something with the device's IWiaPropertyStorage*
				ReadSomeWiaProperties(pWiaPropertyStorage);

				// Release the device's IWiaPropertyStorage*
				pWiaPropertyStorage->Release();
				pWiaPropertyStorage = NULL;
			}
		}

		// If the result of the enumeration is S_FALSE (which
		// is normal), change it to S_OK.
		if (S_FALSE == hr)
		{
			hr = S_OK;
		}

		// Release the enumerator
		pWiaEnumDevInfo->Release();
		pWiaEnumDevInfo = NULL;
	}

	// Return the result of the enumeration
	return hr;
}
//Get the file count so we can number the files
int GetFileCount()
{
	String^ line;
	int value = 0;

	try
	{
		HANDLE HR = CreateFile(L"count.ini", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		CloseHandle(HR);

		StreamReader^ in = gcnew StreamReader("count.ini");

		line = in->ReadLine();
		in->Close();

		value = Convert::ToInt32(line);
		value++;

		StreamWriter^ out = gcnew StreamWriter("count.ini");

		out->Write(Convert::ToString(value));
		out->Close();
	}
	catch (...){}

	return value;
}
//Return user name from emailSettings.ini
String^ UserName()
{
	String^ username;
	StreamReader^ in = gcnew StreamReader("emailSettings.ini");

	username = in->ReadLine();
	in->Close();

	return username;
}
//Return password from emailSettings.ini
String^ Password()
{
	String^ password;
	StreamReader^ in = gcnew StreamReader("emailSettings.ini");

	password = in->ReadLine();
	in->Close();

	return password;
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
	String^ From = UserName();

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
	client->Credentials = gcnew System::Net::NetworkCredential(UserName(), Password());

	//gmail requires ssl
	client->EnableSsl = true;

	//Attach the image
	for (int i = 0; i < AttachmentList.Count; i++)
	{
		Attachment^ data = gcnew Attachment(AttachmentList[i] +".jpg");
		msg->Attachments->Add(data);
	}

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
	StreamWriter^ out = gcnew StreamWriter("recentEmails.txt", true);
	out->WriteLine(EmailTo->Text);
	out->Close();

	ScannedImage->Hide();

	EmailTo->Text = "";
	EmailSubject->Text = "";

	MessageBox::Show("Message sent!", "Success!", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
}
/*Gets the device information, scans in the image, and saves it to disk*/
void Form1::ScanImage_Click(System::Object ^sender, System::EventArgs ^e) {
	IWiaDevMgr2* devMgr;
	IWiaItem2* item;
	HRESULT hr;
	LONG x = 0L;
	int fileCount = 0;
	BSTR *files = new BSTR[0];
	bool moreImages = true;

	//Setup WIA devices
	hr = CreateWiaDeviceManager(&devMgr);

	if (hr == S_OK)
		hr = GetWiaDevices(devMgr);

	if (hr == S_OK)
		hr = CreateWiaDevice(devMgr, DevID, &item);

	//set folder name
	_bstr_t FolderName(DIRECTORY);

	while (moreImages)
	{
		//Generate new filenames for the image.
		FileValue = gcnew String("TMS");

		fileCount = GetFileCount();

		FileValue = FileValue + fileCount;

		pin_ptr<const wchar_t> wch = PtrToStringChars(FileValue);
		printf_s("%S\n", wch);
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

		//add filename to attachment list
		AttachmentList.Add(FileValue);

		//Display the image to the screen
		ScannedImage->SizeMode = PictureBoxSizeMode::StretchImage;
		ScannedImage->Load(FileValue + ".jpg");
		ScannedImage->Show();

		//See if the user has more images to scan
		System::Windows::Forms::DialogResult dr;
		dr = MessageBox::Show("Would you like to scan another image?", "More Images?", MessageBoxButtons::YesNo, MessageBoxIcon::Question);

		if (dr == ::DialogResult::Yes)
		{
			moreImages = true;
		}
		else if (dr == ::DialogResult::No)
		{
			moreImages = false;
		}
	}
}
/*Load recent recipients box and display the welcome image
Also retrieves the 'My Documents' folder for the user*/
void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e)
{
	//Get the my documents directory for the logged in user
	HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, DIRECTORY);

	//add "\\BillSender\\" to the end of the string to create our directory
	std::wstring tmp = L"\\BillSender\\";
	int i = 0;
	bool flag = true;

	while (flag)
	{
		if (DIRECTORY[i] == NULL)
		{
			for (int x = 0; x < tmp.size(); x++)
			{
				if (DIRECTORY[i + x] == NULL)
				{
					DIRECTORY[i + x] = tmp[x];
				}
			}
			flag = false;
		}
		i++;
	}

	//create the BillSender directory if it doesn't exist, then set the current directory
	CreateDirectory(DIRECTORY, NULL);
	SetCurrentDirectory(DIRECTORY);

	//Show Welcome image
	try
	{
		ScannedImage->SizeMode = PictureBoxSizeMode::StretchImage;
		ScannedImage->Load("WelcomePage.jpg");
		ScannedImage->Show();
	}
	catch (...){}

	//read in recent recipients
	try
	{
		//create recentEmails.txt file
		HANDLE HR = CreateFile(L"recentEmails.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		CloseHandle(HR);

		StreamReader^ in = gcnew StreamReader("recentEmails.txt");

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
//Open the email settings form
void Form1::emailSettingsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	EmailSettings^ form = gcnew EmailSettings;
	form->Show();
}