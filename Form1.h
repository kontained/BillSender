#pragma once

namespace BillSender {

	using namespace System;
	using namespace System::IO;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Net::Mail;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
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
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  EmailTo;


	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ListView^  RecentRecipList;

	private: System::Windows::Forms::Button^  SendBill;
	private: System::Windows::Forms::PictureBox^  ScannedImage;


	private: System::Windows::Forms::Button^  ScanImage;




	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  EmailSubject;

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

	private: String^ FileValue;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->EmailSubject = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->RecentRecipList = (gcnew System::Windows::Forms::ListView());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->EmailTo = (gcnew System::Windows::Forms::TextBox());
			this->SendBill = (gcnew System::Windows::Forms::Button());
			this->ScannedImage = (gcnew System::Windows::Forms::PictureBox());
			this->ScanImage = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ScannedImage))->BeginInit();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->EmailSubject);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->RecentRecipList);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->EmailTo);
			this->groupBox1->Location = System::Drawing::Point(620, 34);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(258, 402);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Sending Options";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(1, 62);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(61, 18);
			this->label3->TabIndex = 5;
			this->label3->Text = L"Subject:";
			// 
			// EmailSubject
			// 
			this->EmailSubject->Location = System::Drawing::Point(68, 62);
			this->EmailSubject->Name = L"EmailSubject";
			this->EmailSubject->Size = System::Drawing::Size(176, 20);
			this->EmailSubject->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(94, 96);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(128, 18);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Recent Recipients";
			// 
			// RecentRecipList
			// 
			this->RecentRecipList->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->RecentRecipList->LabelWrap = false;
			this->RecentRecipList->Location = System::Drawing::Point(68, 117);
			this->RecentRecipList->MultiSelect = false;
			this->RecentRecipList->Name = L"RecentRecipList";
			this->RecentRecipList->Size = System::Drawing::Size(176, 277);
			this->RecentRecipList->TabIndex = 2;
			this->RecentRecipList->TabStop = false;
			this->RecentRecipList->UseCompatibleStateImageBehavior = false;
			this->RecentRecipList->View = System::Windows::Forms::View::List;
			this->RecentRecipList->DoubleClick += gcnew System::EventHandler(this, &Form1::RecentRecipList_DoubleClick);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(32, 21);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(30, 18);
			this->label1->TabIndex = 1;
			this->label1->Text = L"To:";
			// 
			// EmailTo
			// 
			this->EmailTo->Location = System::Drawing::Point(68, 21);
			this->EmailTo->Name = L"EmailTo";
			this->EmailTo->Size = System::Drawing::Size(176, 20);
			this->EmailTo->TabIndex = 0;
			// 
			// SendBill
			// 
			this->SendBill->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->SendBill->Location = System::Drawing::Point(717, 549);
			this->SendBill->Name = L"SendBill";
			this->SendBill->Size = System::Drawing::Size(105, 65);
			this->SendBill->TabIndex = 3;
			this->SendBill->Text = L"Send Email";
			this->SendBill->UseVisualStyleBackColor = true;
			this->SendBill->Click += gcnew System::EventHandler(this, &Form1::SendBill_Click);
			// 
			// ScannedImage
			// 
			this->ScannedImage->Location = System::Drawing::Point(12, 12);
			this->ScannedImage->Name = L"ScannedImage";
			this->ScannedImage->Size = System::Drawing::Size(602, 602);
			this->ScannedImage->TabIndex = 2;
			this->ScannedImage->TabStop = false;
			// 
			// ScanImage
			// 
			this->ScanImage->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->ScanImage->Location = System::Drawing::Point(717, 463);
			this->ScanImage->Name = L"ScanImage";
			this->ScanImage->Size = System::Drawing::Size(105, 65);
			this->ScanImage->TabIndex = 2;
			this->ScanImage->Text = L"Scan Image";
			this->ScanImage->UseVisualStyleBackColor = true;
			this->ScanImage->Click += gcnew System::EventHandler(this, &Form1::ScanImage_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(890, 626);
			this->Controls->Add(this->ScanImage);
			this->Controls->Add(this->ScannedImage);
			this->Controls->Add(this->SendBill);
			this->Controls->Add(this->groupBox1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Bill Sender";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ScannedImage))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
		public: System::Void SendBill_Click(System::Object^  sender, System::EventArgs^  e);
		public: System::Void ScanImage_Click(System::Object^  sender, System::EventArgs^  e);
		public: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e);
		public: System::Void RecentRecipList_DoubleClick(System::Object^  sender, System::EventArgs^  e);
	};
}

