#pragma once
#include <Windows.h>

namespace BillSender {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Summary for EmailSettings
	/// </summary>
	public ref class EmailSettings : public System::Windows::Forms::Form
	{
	public:
		EmailSettings(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EmailSettings()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  EmailAddress;
	protected:
	private: System::Windows::Forms::TextBox^  Password;
	private: System::Windows::Forms::Button^  UpdateBtn;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->EmailAddress = (gcnew System::Windows::Forms::TextBox());
			this->Password = (gcnew System::Windows::Forms::TextBox());
			this->UpdateBtn = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// EmailAddress
			// 
			this->EmailAddress->Location = System::Drawing::Point(144, 12);
			this->EmailAddress->Name = L"EmailAddress";
			this->EmailAddress->Size = System::Drawing::Size(200, 20);
			this->EmailAddress->TabIndex = 0;
			// 
			// Password
			// 
			this->Password->Location = System::Drawing::Point(144, 72);
			this->Password->Name = L"Password";
			this->Password->PasswordChar = '*';
			this->Password->Size = System::Drawing::Size(200, 20);
			this->Password->TabIndex = 1;
			// 
			// UpdateBtn
			// 
			this->UpdateBtn->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->UpdateBtn->Location = System::Drawing::Point(254, 111);
			this->UpdateBtn->Name = L"UpdateBtn";
			this->UpdateBtn->Size = System::Drawing::Size(85, 47);
			this->UpdateBtn->TabIndex = 2;
			this->UpdateBtn->Text = L"Update";
			this->UpdateBtn->UseVisualStyleBackColor = true;
			this->UpdateBtn->Click += gcnew System::EventHandler(this, &EmailSettings::UpdateBtn_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(7, 12);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(111, 20);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Email Address";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(40, 72);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(78, 20);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Password";
			// 
			// EmailSettings
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(356, 165);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->UpdateBtn);
			this->Controls->Add(this->Password);
			this->Controls->Add(this->EmailAddress);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"EmailSettings";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Email Settings";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

public: String^ dir;
private: System::Void UpdateBtn_Click(System::Object^  sender, System::EventArgs^  e) {
			//Create emailSettings.ini file if it doesn't already exist, then close the handle so we can work with it
			 HANDLE HR = CreateFile(L"emailSettings.ini", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
			 CloseHandle(HR);

			 if (String::IsNullOrEmpty(EmailAddress->Text))
			 {
				 MessageBox::Show("Please input an email address.", "NO EMAIL ADDRESS", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				 return;
			 }

			 if (String::IsNullOrEmpty(Password->Text))
			 {
				 MessageBox::Show("Please enter a password.", "NO PASSWORD", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				 return;
			 }

			//Open the file, clearing out the previous contents
			 StreamWriter^ out = gcnew StreamWriter("emailSettings.ini", false);
			 out->WriteLine(EmailAddress->Text);
			 out->Write(Password->Text);
			 out->Close();

			 this->Close();
}
};
}
