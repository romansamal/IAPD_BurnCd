#pragma once
#include <ShlObj.h>
#include <Windows.h>
#include "OpticalDisc.h"
#include "DiscImage.h"
#include <imapi2.h>
#include <imapi2error.h>
#include <imapi2fs.h>
#include <imapi2fserror.h>
#define IMAPI_MEDIA_TYPE_CDROM "CD-ROM"
namespace IAPDBurnCdDvd {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	/// <summary>
	/// —водка дл€ MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Label^  mediaType;
	private: System::Windows::Forms::Label^  mediaTypeValue;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Button^  button1;

	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::TreeView^  treeView1;

	protected: virtual void WndProc(Message% m) override {

			switch (m.Msg)
			{
			case WM_DEVICECHANGE:
			{
				OpticalDisc disc;
				String ^mediaTypeVal = gcnew String(disc.getMediaType().c_str());
				this->mediaTypeValue->Text = mediaTypeVal;
				break;
			}

			default:
				Form::WndProc(m);
			}
		}


	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->mediaType = (gcnew System::Windows::Forms::Label());
			this->mediaTypeValue = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel1->Controls->Add(this->treeView1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->progressBar1, 0, 1);
			this->tableLayoutPanel1->Location = System::Drawing::Point(12, 12);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 2;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 91.84782F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.152174F)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(658, 368);
			this->tableLayoutPanel1->TabIndex = 0;
			// 
			// treeView1
			// 
			this->treeView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->treeView1->Location = System::Drawing::Point(3, 3);
			this->treeView1->Name = L"treeView1";
			this->treeView1->Size = System::Drawing::Size(652, 331);
			this->treeView1->TabIndex = 0;
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(3, 340);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(652, 23);
			this->progressBar1->TabIndex = 1;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(673, 323);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 9;
			this->button1->Text = L"Add file";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// mediaType
			// 
			this->mediaType->AutoSize = true;
			this->mediaType->Location = System::Drawing::Point(673, 15);
			this->mediaType->Name = L"mediaType";
			this->mediaType->Size = System::Drawing::Size(80, 13);
			this->mediaType->TabIndex = 1;
			this->mediaType->Text = L"Type of media: ";
			// 
			// mediaTypeValue
			// 
			this->mediaTypeValue->AutoSize = true;
			this->mediaTypeValue->Location = System::Drawing::Point(762, 15);
			this->mediaTypeValue->Name = L"mediaTypeValue";
			this->mediaTypeValue->Size = System::Drawing::Size(0, 13);
			this->mediaTypeValue->TabIndex = 2;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(674, 36);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(80, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Total size, MB: ";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(676, 58);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Free, MB:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(676, 82);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(57, 13);
			this->label3->TabIndex = 5;
			this->label3->Text = L"Used, MB:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(784, 36);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(35, 13);
			this->label4->TabIndex = 6;
			this->label4->Text = L"label4";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(785, 58);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(35, 13);
			this->label5->TabIndex = 7;
			this->label5->Text = L"label5";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(785, 82);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(35, 13);
			this->label6->TabIndex = 8;
			this->label6->Text = L"label6";
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(765, 357);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 11;
			this->button3->Text = L"Burn!";
			this->button3->UseVisualStyleBackColor = true;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(847, 392);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->mediaTypeValue);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->mediaType);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->tableLayoutPanel1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	WCHAR szDir[MAX_PATH];
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.pidlRoot = NULL;
	bInfo.pszDisplayName = szDir;
	bInfo.lpszTitle = L"Please, select a folder/file"; 
	bInfo.ulFlags =  BIF_NEWDIALOGSTYLE | BIF_SHAREABLE | BIF_BROWSEINCLUDEFILES;
	bInfo.lpfn = NULL;
	bInfo.lParam = 0;
	bInfo.iImage = -1;
	LPITEMIDLIST lpItem = SHBrowseForFolder(&bInfo);
	if (lpItem != NULL)
	{
		SHGetPathFromIDList(lpItem, szDir);
		wstring widePath(szDir);
		string path(widePath.begin(), widePath.end());
		String ^pathManaged = gcnew String(path.c_str());
		getSizeOfSelected(pathManaged);
		addDirectoryToTree(pathManaged);
	};
	return;
}
private: long long getSizeOfSelected(String ^path)	{
	long long value;
	Scripting::FileSystemObject ^fso = gcnew Scripting::FileSystemObject();
	if (!System::IO::File::Exists(path))
	{
		Scripting::Folder ^folder = fso->GetFolder(path);
		value = Convert::ToInt64(folder->Size);
	}
	else
	{
		Scripting::File ^file = fso->GetFile(path);
		value = Convert::ToDouble(file->Size);
		
	}
	return value;
	}

private: void addDirectoryToTree(String ^path)
{
	Generic::Stack<TreeNode ^>^ stack = gcnew Generic::Stack<TreeNode ^>();
	DirectoryInfo ^rootDirectory = gcnew DirectoryInfo(path);
	TreeNode ^node = gcnew TreeNode(rootDirectory->Name);
	node->Tag = rootDirectory;
	stack->Push(node);
	while (stack->Count > 0)
	{
		TreeNode ^ currentNode = stack->Pop();
		DirectoryInfo ^directoryInfo = (DirectoryInfo ^)currentNode->Tag;
		for each(DirectoryInfo ^ directory in directoryInfo->GetDirectories())
		{
			TreeNode ^ childDirectoryNode = gcnew TreeNode(directory->Name);
			childDirectoryNode->Tag = directory;
			currentNode->Nodes->Add(childDirectoryNode);
			stack->Push(childDirectoryNode);
		}
		for each(FileInfo ^file in directoryInfo->GetFiles())
			currentNode->Nodes->Add(gcnew TreeNode(file->Name));
	}
	treeView1->Nodes->Add(node);
}
};

}
