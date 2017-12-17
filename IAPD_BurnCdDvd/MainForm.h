#pragma once
#include <ShlObj.h>
#include <Windows.h>
#include "OpticalDrive.h"
#include "DiscImage.h"
#include <imapi2.h>
#include <imapi2error.h>
#include <imapi2fs.h>
#include <imapi2fserror.h>
#include <codecvt>
#define MB_SIZE 1048576
#define IMAPI_MESSAGE_VALIDATION 2315
#define IMAPI_MESSAGE_FORMATTING 2316
#define IMAPI_MESSAGE_INITIALIZING_HARDWARE 2317
#define IMAPI_MESSAGE_CALIBRATING_POWER 2318
#define IMAPI_MESSAGE_WRITING_DATA 2319
#define IMAPI_MESSAGE_ACTION_FINALIZATION 2320
#define IMAPI_MESSAGE_ACTION_COMPLETED 2321
#define CD_SIZE 740
#define DVD_SIZE 4700

namespace IAPDBurnCdDvd {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
		}

	protected:
		~MainForm()
		{
			if (image)
				delete image;
			if (components)
			{
				delete components;
			}
		}
		virtual void WndProc(Message% m) override 
		{
			switch (m.Msg)
			{
			case IMAPI_MESSAGE_VALIDATION:
			{
				eventValidatingMedia();
				break;
			}

			case IMAPI_MESSAGE_FORMATTING:
			{
				eventFormattingMedia();
				break;
			}

			case IMAPI_MESSAGE_INITIALIZING_HARDWARE:
			{
				eventInitHardware();
				break;
			}

			case IMAPI_MESSAGE_CALIBRATING_POWER:
			{
				eventCalibrating();
				break;
			}

			case IMAPI_MESSAGE_WRITING_DATA:
			{
				Int32 value = m.LParam.ToInt32();
				eventWriting(value);
				break;
			}

			case IMAPI_MESSAGE_ACTION_FINALIZATION:
			{
				eventWritingFinal();
				break;
			}

			case IMAPI_MESSAGE_ACTION_COMPLETED:
			{
				eventSuccess();
				break;
			}

			case WM_DEVICECHANGE:
			{
				vector<string> devices = OpticalDrive::getAvailableDrives();
				OpticalDrive drive(devices[comboDrive->SelectedIndex]);
				string typeOfMedia;
				while (true)
				{
					typeOfMedia = drive.getMediaType();
					if (typeOfMedia != "")
						break;
				}
				String ^mediaTypeVal = gcnew String(typeOfMedia.c_str());
				this->mediaTypeValue->Text = mediaTypeVal;
				if (mediaTypeVal == "Unknown")
				{
					buttonBurn->Enabled = false;
					comboTemplate->Enabled = true;
					comboTemplate->SelectedIndex = 0;
				}
				else
				{
					buttonBurn->Enabled = true;
					comboTemplate->Enabled = false;
					double totalSize = drive.getTotalMediaSize();
					if (totalSize == 0)
						break;
					this->labelTotalValue->Text = (gcnew Double(totalSize))->ToString("#.##");
					if (!this->image->changeMaxSize(totalSize))
					{
						MessageBox::Show("There are no enough space :(", "Error", MessageBoxButtons::OK);
						break;
					}
					double free = image->getFreeSize();
					this->labelFreeValue->Text = (gcnew Double(free))->ToString("#.##");
				}
				break;
			}

			default:
				Form::WndProc(m);
			}
		}
	private: 
		NotifyIcon ^icon;
		DiscImage *image;
		DateTime ^lastNotification;
		TableLayoutPanel^ tableLayoutPanel1;
		Label^ mediaType;
		Label^ mediaTypeValue;
		ProgressBar^ progressBar1;
		TextBox ^box;
		Label^ labelTotal;
		Label^ labelFree;
		Label^ labelUsed;
		Label^ labelTotalValue;
		Label^ labelFreeValue;
		Label^ labelUsedValue;
		Button^ buttonAddFile;
		Button^ buttonBurn;
		TreeView^ treeView1;
		ComboBox ^comboTemplate;
		ComboBox ^comboDrive;
		System::ComponentModel::Container ^components;

		void formResize(System::Object^  sender, System::EventArgs^  e)
		{
			if (FormWindowState::Minimized == this->WindowState)
			{
				icon->Visible = true;
				this->Hide();
			}
			else
			{
				icon->Visible = false;
			}
		}

		void comboTemplateChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if (image)
				delete image;
			treeView1->Nodes->Clear();
			Double size = comboTemplate->SelectedItem == "CD" ? CD_SIZE : DVD_SIZE;
			image = new DiscImage(size);
			this->labelTotalValue->Text = (gcnew Double(size))->ToString("#.##");
			double free = image->getFreeSize();
			this->labelFreeValue->Text = (gcnew Double(free))->ToString("#.##");
			double used = image->getCurrentSize();
			this->labelUsedValue->Text = (gcnew Double(used))->ToString("#.##");
		}

		void comboDriveChanged(System::Object^  sender, System::EventArgs^  e)
		{
			buttonBurn->Enabled = true;
			vector<string> devices = OpticalDrive::getAvailableDrives();
			OpticalDrive device(devices[comboDrive->SelectedIndex]);
			string mediaTypeStr = device.getMediaType();
			this->mediaTypeValue->Text = gcnew String(mediaTypeStr.c_str());
			if (mediaTypeStr == "Unknown")
			{
				buttonBurn->Enabled = false;
				comboTemplate->Enabled = true;
				comboTemplate->SelectedIndex = 0;
			}
			else
			{
				double size = device.getTotalMediaSize();
				if (size == 0)
					return;
				if (!image->changeMaxSize(size))
				{
					MessageBox::Show("There are no enough space :(", "Error", MessageBoxButtons::OK);
					return;
				}
				this->labelTotalValue->Text = (gcnew Double(size))->ToString("#.##");
				double free = image->getFreeSize();
				this->labelFreeValue->Text = (gcnew Double(free))->ToString("#.##");
				double used = image->getCurrentSize();
				this->labelUsedValue->Text = (gcnew Double(used))->ToString("#.##");
			}
			
		}


		void iconDoubleClick(System::Object^ sender, System::EventArgs^  e)
		{
			this->ShowInTaskbar = true;
			this->Show();
			this->WindowState = FormWindowState::Normal;
		}

		void InitializeComponent(void)
		{
			vector<string> devices = OpticalDrive::getAvailableDrives();
			lastNotification = gcnew DateTime();
			this->icon = (gcnew System::Windows::Forms::NotifyIcon());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->box = (gcnew System::Windows::Forms::TextBox());
			this->buttonAddFile = (gcnew System::Windows::Forms::Button());
			this->mediaType = (gcnew System::Windows::Forms::Label());
			this->mediaTypeValue = (gcnew System::Windows::Forms::Label());
			this->labelTotal = (gcnew System::Windows::Forms::Label());
			this->labelFree = (gcnew System::Windows::Forms::Label());
			this->labelUsed = (gcnew System::Windows::Forms::Label());
			this->labelTotalValue = (gcnew System::Windows::Forms::Label());
			this->labelFreeValue = (gcnew System::Windows::Forms::Label());
			this->labelUsedValue = (gcnew System::Windows::Forms::Label());
			this->buttonBurn = (gcnew System::Windows::Forms::Button());
			this->comboTemplate = (gcnew System::Windows::Forms::ComboBox());
			this->comboDrive = (gcnew System::Windows::Forms::ComboBox());
			comboTemplate->Items->Add("CD");
			comboTemplate->Items->Add("DVD");
			comboTemplate->SelectedValueChanged += gcnew System::EventHandler(this, &MainForm::comboTemplateChanged);
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			this->progressBar1->Maximum = 100;
			this->Resize += gcnew System::EventHandler(this, &MainForm::formResize);
			icon->Visible = true;
			icon->Text = "CD Burn";
			icon->Icon = this->Icon;
			icon->DoubleClick += gcnew System::EventHandler(this, &MainForm::iconDoubleClick);
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel1->Controls->Add(this->treeView1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->progressBar1, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->box, 0, 2);
			this->tableLayoutPanel1->Location = System::Drawing::Point(12, 12);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 3;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 60.0F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 10.0F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 30.0F)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(658, 368);
			this->tableLayoutPanel1->TabIndex = 0;

			this->box->Dock = System::Windows::Forms::DockStyle::Fill;
			this->box->Enabled = false;
			this->box->Multiline = true;

			this->treeView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->treeView1->Location = System::Drawing::Point(3, 3);
			this->treeView1->Name = L"treeView1";
			this->treeView1->Size = System::Drawing::Size(652, 331);
			this->treeView1->TabIndex = 0;

			this->progressBar1->Location = System::Drawing::Point(3, 340);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(652, 23);
			this->progressBar1->TabIndex = 1;

			this->buttonAddFile->Location = System::Drawing::Point(673, 205);
			this->buttonAddFile->Name = L"buttonAddFile";
			this->buttonAddFile->Size = System::Drawing::Size(75, 23);
			this->buttonAddFile->TabIndex = 9;
			this->buttonAddFile->Text = L"Add file";
			this->buttonAddFile->UseVisualStyleBackColor = true;
			this->buttonAddFile->Click += gcnew System::EventHandler(this, &MainForm::buttonAddFile_Click);

			this->mediaType->AutoSize = true;
			this->mediaType->Location = System::Drawing::Point(673, 15);
			this->mediaType->Name = L"mediaType";
			this->mediaType->Size = System::Drawing::Size(80, 13);
			this->mediaType->TabIndex = 1;
			this->mediaType->Text = L"Type of media: ";

			OpticalDrive drive(devices[0]);
			this->mediaTypeValue->AutoSize = true;
			this->mediaTypeValue->Location = System::Drawing::Point(785, 15);
			this->mediaTypeValue->Name = L"mediaTypeValue";
			this->mediaTypeValue->Size = System::Drawing::Size(0, 13);
			this->mediaTypeValue->TabIndex = 2;
			string mediaTypeStr = drive.getMediaType();
			this->mediaTypeValue->Text = gcnew String(mediaTypeStr.c_str());

			this->labelTotal->AutoSize = true;
			this->labelTotal->Location = System::Drawing::Point(674, 36);
			this->labelTotal->Name = L"labelTotal";
			this->labelTotal->Size = System::Drawing::Size(80, 13);
			this->labelTotal->TabIndex = 3;
			this->labelTotal->Text = L"Total size, MB: ";

			this->comboTemplate->Location = System::Drawing::Point(680, 150);
			this->comboTemplate->Width = 160;
			this->comboTemplate->Enabled = false;

			this->comboDrive->Location = System::Drawing::Point(680, 100);
			this->comboDrive->Width = 160;
			

			this->labelFree->AutoSize = true;
			this->labelFree->Location = System::Drawing::Point(676, 58);
			this->labelFree->Name = L"labelFree";
			this->labelFree->Size = System::Drawing::Size(53, 13);
			this->labelFree->TabIndex = 4;
			this->labelFree->Text = L"Free, MB:";
 
			this->labelUsed->AutoSize = true;
			this->labelUsed->Location = System::Drawing::Point(676, 82);
			this->labelUsed->Name = L"labelUsed";
			this->labelUsed->Size = System::Drawing::Size(57, 13);
			this->labelUsed->TabIndex = 5;
			this->labelUsed->Text = L"Used, MB:";

			this->labelTotalValue->AutoSize = true;
			this->labelTotalValue->Location = System::Drawing::Point(784, 36);
			this->labelTotalValue->Name = L"labelTotalValue";
			this->labelTotalValue->Size = System::Drawing::Size(35, 13);
			this->labelTotalValue->TabIndex = 6;
			double totalSize = drive.getTotalMediaSize();
			this->labelTotalValue->Text = (gcnew Double(totalSize))->ToString("#.##");

			this->labelFreeValue->AutoSize = true;
			this->labelFreeValue->Location = System::Drawing::Point(785, 58);
			this->labelFreeValue->Name = L"labelFreeValue";
			this->labelFreeValue->Size = System::Drawing::Size(35, 13);
			this->labelFreeValue->TabIndex = 7;
			double freeSize = drive.getFreeMediaSize();
			this->labelFreeValue->Text = (gcnew Double(freeSize))->ToString("#.##");

			this->labelUsedValue->AutoSize = true;
			this->labelUsedValue->Location = System::Drawing::Point(785, 82);
			this->labelUsedValue->Name = L"labelUsedValue";
			this->labelUsedValue->Size = System::Drawing::Size(35, 13);
			this->labelUsedValue->TabIndex = 8;
			double usedSize = totalSize - freeSize;
			this->labelUsedValue->Text = (gcnew Double(usedSize))->ToString();
	 
			this->buttonBurn->Location = System::Drawing::Point(765, 357);
			this->buttonBurn->Name = L"buttonBurn";
			this->buttonBurn->Size = System::Drawing::Size(75, 23);
			this->buttonBurn->TabIndex = 11;
			this->buttonBurn->Text = L"Burn!";
			this->buttonBurn->UseVisualStyleBackColor = true;
			this->buttonBurn->Click += gcnew System::EventHandler(this, &MainForm::buttonBurn_Click);

			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(847, 392);
			this->Controls->Add(this->buttonBurn);
			this->Controls->Add(this->labelUsedValue);
			this->Controls->Add(this->buttonAddFile);
			this->Controls->Add(this->labelFreeValue);
			this->Controls->Add(this->labelTotalValue);
			this->Controls->Add(this->labelUsed);
			this->Controls->Add(this->labelFree);
			this->Controls->Add(this->labelTotal);
			this->Controls->Add(this->mediaTypeValue);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->mediaType);
			this->Controls->Add(this->comboTemplate);
			this->Controls->Add(this->comboDrive);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"MainForm";
			this->Text = L"CD Burn";
			this->tableLayoutPanel1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

			if (mediaTypeStr == "Unknown")
			{
				buttonBurn->Enabled = false;
				comboTemplate->Enabled = true;
				comboTemplate->SelectedIndex = 0;
			}
			initComboDrive();
			if (!image)
			image = new DiscImage(totalSize);
			this->comboDrive->SelectedValueChanged += gcnew System::EventHandler(this, &MainForm::comboDriveChanged);
		}

		void buttonAddFile_Click(System::Object^  sender, System::EventArgs^  e) {
			CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
			progressBar1->Value = 0;
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
				String ^pathManaged = gcnew String(widePath.c_str());
				String ^name = Path::GetFileName(pathManaged);
				cli::array<TreeNode ^> ^nodes = treeView1->Nodes->Find(Path::GetFileName(pathManaged), false);
				if (nodes->Length != 0)
					return;
				double size = (double)getSizeOfSelected(pathManaged) / MB_SIZE;
				if (!System::IO::File::Exists(pathManaged))
				{
					if (!image->addDirData(widePath, size))
					{
						MessageBox::Show("Error", "Error", MessageBoxButtons::OK);
						return;
					}
					addDirectoryToTree(pathManaged);
				}
				else
				{
					if (!image->addFileData(widePath, size))
					{
						MessageBox::Show("Error", "Error", MessageBoxButtons::OK);
						return;
					}
					addFileToTree(pathManaged);
				}
				double free = image->getFreeSize();
				double current = image->getCurrentSize();
				this->labelFreeValue->Text = (gcnew Double(free))->ToString("#.##");
				this->labelUsedValue->Text = (gcnew Double(current))->ToString("#.##");
			};

			return;
		}

		long long getSizeOfSelected(String ^path)	
		{
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

		void addFileToTree(String ^path)
		{
			FileInfo ^fileInfo = gcnew FileInfo(path);
			TreeNode ^node = gcnew TreeNode(fileInfo->Name);
			node->Name = fileInfo->Name;
			treeView1->Nodes->Add(node);
		}
		
		void addDirectoryToTree(String ^path)
		{
			Generic::Stack<TreeNode ^>^ stack = gcnew Generic::Stack<TreeNode ^>();
			DirectoryInfo ^rootDirectory = gcnew DirectoryInfo(path);
			TreeNode ^node = gcnew TreeNode(rootDirectory->Name);
			node->Tag = rootDirectory;
			node->Name = rootDirectory->Name;
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
		
		void buttonBurn_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			if (comboDrive->Items->Count == 0)
			{
				MessageBox::Show("Error", "Error", MessageBoxButtons::OK);
				return;
			}
			vector<string> devices = OpticalDrive::getAvailableDrives();
			OpticalDrive disc(devices.at(comboDrive->SelectedIndex));
			IFileSystemImage *im = image->getImage();
			disc.startBurn(im, static_cast<HWND>(Handle.ToPointer()));
			buttonBurn->Enabled = false;
		}

		void eventValidatingMedia()
		{
			progressBar1->Value = 0;
			box->AppendText(DateTime::Now.ToString() + " : Validating that the current media is supported.");
			box->AppendText(Environment::NewLine);
			box->ScrollToCaret();
			if (icon->Visible == true)
			{
				icon->BalloonTipText = "Validating that the current media is supported.";
				icon->ShowBalloonTip(500);
			}
			return;
		}

		void eventFormattingMedia()
		{
			box->AppendText(DateTime::Now.ToString() + ": Formatting media.");
			box->AppendText(Environment::NewLine);
			box->ScrollToCaret();
			if (icon->Visible == true)
			{
				icon->BalloonTipText = "Formatting media.";
				icon->ShowBalloonTip(500);
			}
			return;
		}

		void eventInitHardware()
		{
			box->AppendText(DateTime::Now.ToString() + " : Initializing the hardware.");
			box->AppendText(Environment::NewLine);
			box->ScrollToCaret();
			if (icon->Visible == true)
			{
				icon->BalloonTipText = "Initializing the hardware.";
				icon->ShowBalloonTip(500);
			}
			return;
		}

		void eventCalibrating()
		{
			box->AppendText(DateTime::Now.ToString() + " : Optimizing laser intensity for writing to the media...");
			box->AppendText(Environment::NewLine);
			box->ScrollToCaret();
			if (icon->Visible == true)
			{
				icon->BalloonTipText = "Optimizing laser intensity for writing to the media...";
				icon->ShowBalloonTip(500);
			}
			return;
		}

		void eventWriting(Int32 percent)
		{
			box->AppendText(DateTime::Now.ToString() + " : Writing data to the media...");
			box->AppendText(Environment::NewLine);
			box->ScrollToCaret();
			if (percent == 0 && icon->Visible == true)
			{
				icon->BalloonTipText = "Writing data started...";
				icon->ShowBalloonTip(500);
			}
			
			if ((DateTime::Now - *lastNotification).Seconds > 10 && icon->Visible == true)
			{
				icon->BalloonTipText = "Writing data... " + percent + "%";
				icon->ShowBalloonTip(300);
				lastNotification = DateTime::Now;
			}
			progressBar1->Value = percent;
			return;
		}

		void eventWritingFinal()
		{
			box->AppendText(DateTime::Now.ToString() + " : Finalizing the write.");
			box->AppendText(Environment::NewLine);
			box->ScrollToCaret();
			return;
		}

		void eventSuccess()
		{
			box->AppendText(DateTime::Now.ToString() + " : Successfully finished!\n");
			box->AppendText(Environment::NewLine);
			box->ScrollToCaret();
			progressBar1->Value = 100;
			if (icon->Visible == true)
			{
				icon->BalloonTipText = "Successfully finished!";
				icon->ShowBalloonTip(1000);
			}
			buttonBurn->Enabled = true;
			box->ScrollToCaret();
			return;
		}

		void initComboDrive()
		{
			vector<string> devices = OpticalDrive::getAvailableDrives();
			if (devices.size() == 0)
			{
				comboDrive->Enabled = false;
			}
			for (int i = 0; i < devices.size(); i++)
			{
				vector<string> nameOfDevice = OpticalDrive::getDriveName(devices[i]);
				for (int j = 0; j < nameOfDevice.size(); j++)
				{
					String ^nameManaged = gcnew String(nameOfDevice[j].c_str());
					comboDrive->Items->Add(nameManaged);
				}
			}
			comboDrive->SelectedIndex = 0;
		}
};

}
