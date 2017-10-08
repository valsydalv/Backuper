#include "pch.h"

#include "QWizardNewProfile.h"
#include "BackupManager.h"

WizardNewProfile::WizardNewProfile(QWidget *parent)
	:QWizard(parent)
{
	addPage(new WizardIntroPage);
	addPage(new WizardInfoProfilePage);
	addPage(new WizardFilesPage);
	addPage(new WizardDateBackupProfile);
	addPage(new WizardSpecDateBackupPage);
	addPage(new WizardDateEveryWeekBackupPage);
	addPage(new WizardConclusionPage);

	setMinimumHeight(600);
	setMaximumHeight(600);
	setMinimumWidth(600);
	setMaximumWidth(600);

	QPalette Pal(palette());
	Pal.setColor(QPalette::Background, Qt::red);
	setAutoFillBackground(true);
	setPalette(Pal);

	setWindowTitle(tr("Creating new profile"));
}

WizardIntroPage::WizardIntroPage(QWidget *parent)
	:QWizardPage(parent)
{
	QPixmap pix(":/images/images/IntroPage.png");

	labForIntroPict_ = new QLabel;
	labForIntroPict_->setPixmap(pix);
	labForIntroPict_->setMask(pix.mask());

	labFotIntroInfo_ = new QLabel(tr("This wizard will help you create profile with "
		"certain backup settings for saving or restoring him."));
	labFotIntroInfo_->setWordWrap(true);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(labForIntroPict_);
	layout->addWidget(labFotIntroInfo_);
	setLayout(layout);

	setTitle(tr("Introduction"));
}

WizardInfoProfilePage::WizardInfoProfilePage(QWidget *parent)
	:QWizardPage(parent)
{
	QPixmap pix(":/images/images/LogoForBackup.png");

	labInfoPage_ = new QLabel(tr("This wizard will help you: \n\n"
		"- create a new empty profile to memory;\n"
		"- set the compression level to backup;\n"
		"- sets the size of one data chunk in MB."));

	labPicture_ = new QLabel;
	labPicture_->setPixmap(pix);
	labPicture_->setMask(pix.mask());

	labProfileName_ = new QLabel(tr("&Name of profile: "));
	lineProfileName_ = new QLineEdit;
	labProfileName_->setBuddy(lineProfileName_);

	labComprLvl_ = new QLabel(tr("&Compression level:"));
	sliderComprLvl_ = new QSlider(Qt::Horizontal);
	labViewComprLvl_ = new QLabel("-1");
	labComprLvl_->setBuddy(sliderComprLvl_);

	sliderComprLvl_->setRange(-1, 10);
	sliderComprLvl_->setPageStep(1);
	sliderComprLvl_->setValue(-1);
	sliderComprLvl_->setTickInterval(1);
	sliderComprLvl_->setTickPosition(QSlider::TicksBelow);

	labChunkSize_ = new QLabel(tr("&Chunk size:"));
	sbChunkSize_ = new QSpinBox();
	sbChunkSize_->setValue(64);
	sbChunkSize_->setMaximum(10000);
	labChunkSize_->setBuddy(sbChunkSize_);
	sbChunkSize_->setRange(0, 65536);

	connect(sliderComprLvl_, SIGNAL(valueChanged(int)), labViewComprLvl_, SLOT(setNum(int)));

	registerField("profileName*", lineProfileName_);
	registerField("comprLvl", sliderComprLvl_);
	registerField("chunkSize", sbChunkSize_);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *layoutLogo = new QHBoxLayout;
	QHBoxLayout *layoutProfileName = new QHBoxLayout;
	QHBoxLayout *layoutComprLvl = new QHBoxLayout;
	QVBoxLayout *layoutChunkSize = new QVBoxLayout;

	layoutLogo->addWidget(labInfoPage_);
	layoutLogo->addSpacing(100);
	layoutLogo->addWidget(labPicture_);

	layoutProfileName->addWidget(labProfileName_);
	layoutProfileName->addWidget(lineProfileName_);

	layoutComprLvl->addWidget(labComprLvl_);
	layoutComprLvl->addWidget(sliderComprLvl_);
	layoutComprLvl->addWidget(labViewComprLvl_);

	layoutChunkSize->addWidget(labChunkSize_);
	layoutChunkSize->addWidget(sbChunkSize_);

	mainLayout->addLayout(layoutLogo);
	mainLayout->addSpacing(100);
	layout->addLayout(layoutProfileName);
	layout->addSpacing(0);
	layout->addLayout(layoutChunkSize);
	layout->addSpacing(10);
	layout->addLayout(layoutComprLvl);
	mainLayout->addLayout(layout);

	setLayout(mainLayout);
}

WizardFilesPage::WizardFilesPage(QWidget *parent)
	:QWizardPage(parent)
{
	QPixmap pix(":/images/images/LogoForBackup.png");

	labInfoPage_ = new QLabel(tr("This wizard will help you: \n\n"
		"- set the path to result backup file;\n"
		"- add a directory to backup list (all directory's\n"
		" files and folders are added to backup list."));

	labPicture_ = new QLabel;
	labPicture_->setPixmap(pix);
	labPicture_->setMask(pix.mask());

	labResultFile_ = new QLabel(tr("Result file: "));
	labResultFile_->setFixedWidth(80);
	labPathResFile_ = new QLabel(tr("Not selected"));

	QPalette pal(Qt::lightGray);
	labPathResFile_->setAutoFillBackground(true);
	labPathResFile_->setPalette(pal);
	labPathResFile_->setStyleSheet("color: rgb(255,250,250)");

	labListSelectedFiles_ = new QLabel(tr("Added files"));
	labListSelectedFiles_->setAlignment(Qt::AlignCenter);

	butResultFile_ = new QPushButton(tr("Set"));
	butResultFile_->setFixedWidth(80);

	butAddFile_ = new QPushButton(tr("Add file"));
	butAddFile_->setFixedWidth(120);
	butAddFile_->setFixedHeight(50);

	butAddDirectory_ = new QPushButton(tr("Add directory"));
	butAddDirectory_->setFixedWidth(120);
	butAddDirectory_->setFixedHeight(50);

	QStringList files;
	files << "Added files";

	listFiles_ = new QListWidget;
	listFiles_->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(butResultFile_, SIGNAL(clicked()), SLOT(ClickedResultFile()));
	connect(butAddFile_, SIGNAL(clicked()), SLOT(ClickedAddFile()));
	connect(butAddDirectory_, SIGNAL(clicked()), SLOT(ClickedAddDirectory()));

	registerField("strList", this, "theStringList");

	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *layoutLogo = new QHBoxLayout;
	QHBoxLayout *layoutResultFile = new QHBoxLayout;
	QHBoxLayout *layoutAdd = new QHBoxLayout;
	QVBoxLayout *layoutButtonsAdds = new QVBoxLayout;
	QVBoxLayout *layoutAddedFiles = new QVBoxLayout;

	layoutLogo->addWidget(labInfoPage_);
	layoutLogo->addSpacing(100);
	layoutLogo->addWidget(labPicture_);

	layoutResultFile->addWidget(labResultFile_);
	layoutResultFile->addWidget(labPathResFile_);
	layoutResultFile->addWidget(butResultFile_);

	layoutButtonsAdds->addSpacing(20);
	layoutButtonsAdds->addWidget(butAddFile_);
	layoutButtonsAdds->addWidget(butAddDirectory_);
	layoutButtonsAdds->addSpacing(100);

	layoutAddedFiles->addWidget(labListSelectedFiles_);
	layoutAddedFiles->addWidget(listFiles_);

	layoutAdd->addLayout(layoutButtonsAdds);
	layoutAdd->addLayout(layoutAddedFiles);


	mainLayout->addLayout(layoutLogo);
	mainLayout->addSpacing(20);
	mainLayout->addLayout(layoutAdd);

	setLayout(mainLayout);
}

WizardDateBackupProfile::WizardDateBackupProfile(QWidget *parent)
	:QWizardPage(parent)
{
	QPixmap pix(":/images/images/LogoForBackup.png");

	labInfoPage_ = new QLabel(tr("This wizard will help you: \n\n"
		"- set the date and time for profile backup\n"));
	labPicture_ = new QLabel;
	labPicture_->setPixmap(pix);
	labPicture_->setMask(pix.mask());

	labTimeDate_ = new QLabel(tr("Specify the date and time: "));
	time_ = new QTimeEdit;

	grb_ = new QGroupBox("&Backup repeat");
	grb_->setStyleSheet("color: rgb(50, 0, 70)");

	rbEveryDay_ = new QRadioButton("Every day");
	rbEveryWeek_ = new QRadioButton("Every week");
	rbSpecDate_ = new QRadioButton("On the specified date");
	rbNoDate_ = new QRadioButton("Dafault profile without date to backup");
	rbNoDate_->setChecked(true);

	registerField("time", time_);
	registerField("backupRepeat", this, "theString");

	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *layoutLogo = new QHBoxLayout;
	QHBoxLayout *layoutTime = new QHBoxLayout;
	QVBoxLayout *layoutDateRepeat = new QVBoxLayout;

	layoutLogo->addWidget(labInfoPage_);
	layoutLogo->addSpacing(100);
	layoutLogo->addWidget(labPicture_);

	layoutTime->addWidget(labTimeDate_);
	layoutTime->addWidget(time_);

	layoutDateRepeat->addWidget(rbEveryDay_);
	layoutDateRepeat->addWidget(rbEveryWeek_);
	layoutDateRepeat->addWidget(rbSpecDate_);
	layoutDateRepeat->addWidget(rbNoDate_);
	grb_->setLayout(layoutDateRepeat);

	mainLayout->addLayout(layoutLogo);
	mainLayout->addSpacing(20);
	mainLayout->addLayout(layoutTime);
	mainLayout->addWidget(grb_);

	setLayout(mainLayout);
}

WizardDateEveryWeekBackupPage::WizardDateEveryWeekBackupPage(QWidget *parent)
	:QWizardPage(parent)
{
	QPixmap pix(":/images/images/LogoForBackup.png");

	labInfoPage_ = new QLabel(tr("This wizard will help you specify \n"
		"day of week for backup repeat"));

	labPicture_ = new QLabel;
	labPicture_->setPixmap(pix);
	labPicture_->setMask(pix.mask());

	strListDaysWeek_ << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday" << "Saturday" << "Sunday";

	labCombDaysWeek_ = new QLabel(tr("Select day of week for profile backup:"));
	combDaysWeek_ = new QComboBox();

	combDaysWeek_->clear();

	for (auto i = 0; i<strListDaysWeek_.size(); i++)
	{
		combDaysWeek_->addItem(strListDaysWeek_[i]);
	}

	registerField("dayWeek", this, "theStringDayWeek");

	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *layoutLogo = new QHBoxLayout;
	QHBoxLayout *layoutDaysWeek = new QHBoxLayout;

	layoutDaysWeek->addWidget(labCombDaysWeek_);
	layoutDaysWeek->addWidget(combDaysWeek_);

	layoutLogo->addWidget(labInfoPage_);
	layoutLogo->addSpacing(100);
	layoutLogo->addWidget(labPicture_);

	mainLayout->addLayout(layoutLogo);
	mainLayout->addLayout(layoutDaysWeek);
	setLayout(mainLayout);
}

WizardSpecDateBackupPage::WizardSpecDateBackupPage(QWidget *parent)
	:QWizardPage(parent)
{
	QPixmap pix(":/images/images/LogoForBackup.png");

	labInfoPage_ = new QLabel(tr("This wizard will help you specify \n"
		"the date for profile backup."));
	labPicture_ = new QLabel;
	labPicture_->setPixmap(pix);
	labPicture_->setMask(pix.mask());
	labCalendar_ = new QLabel(tr("Specify date: "));

	calendar_ = new QCalendarWidget;
	calendar_->setMinimumDate(QDate(1900, 1, 1));
	calendar_->setMaximumDate(QDate(3000, 1, 1));
	calendar_->setGridVisible(true);

	registerField("date", this, "theStringDateBackup");

	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *layoutLogo = new QHBoxLayout;
	QVBoxLayout *layoutCal = new QVBoxLayout;

	layoutCal->addWidget(labCalendar_);
	layoutCal->addWidget(calendar_);

	layoutLogo->addWidget(labInfoPage_);
	layoutLogo->addSpacing(100);
	layoutLogo->addWidget(labPicture_);
	mainLayout->addLayout(layoutLogo);
	mainLayout->addLayout(layoutCal);
	setLayout(mainLayout);
}

WizardConclusionPage::WizardConclusionPage(QWidget *parent)
	:QWizardPage(parent)
{
	QPixmap pix(":/images/images/IntroPage.png");

	labForIntroPict_ = new QLabel;
	labForIntroPict_->setPixmap(pix);
	labForIntroPict_->setMask(pix.mask());

	labConclInfo_ = new QLabel(tr("You are now successfully registered a new profile of backup."
		"Keep your data safe!"));
	labConclInfo_->setWordWrap(true);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(labForIntroPict_);
	layout->addWidget(labConclInfo_);
	setLayout(layout);

	setTitle(tr("Conclusion"));
}

void WizardNewProfile::reject()
{
	QMessageBox::StandardButton resBtn = QMessageBox::warning(this, "STOP CREATING PROFILE",
		tr("If you exit now, the process of creating a new profile will be stopped.\n"
			"You will have to start creating anew.\n"
			"Are you sure?\n"),
		QMessageBox::Cancel | QMessageBox::Yes,
		QMessageBox::Yes);

	if (resBtn == QMessageBox::Yes) {
		QDialog::reject();
	}
	else
	{
		QDialog::IgnoreMask;
	}
}

void WizardNewProfile::accept()
{
	BackupManager::BackupManager backuper;

	QString profileName = field("profileName").toString();
	int comprLvl = field("comprLvl").toInt();
	int chunkSize = field("chunkSize").toInt();
	QStringList strListFiles = field("strList").toStringList();

	QString time = field("time").toString();
	QString backupRepeat = field("backupRepeat").toString();

	backuper.CreateProfile(profileName);
	backuper.ModifyProfile(profileName).SetCompressionLevel(comprLvl);
	backuper.ModifyProfile(profileName).SetChunkSize(chunkSize);

	for (const auto& i : strListFiles)
	{
		if (i.contains("."))
		{
			backuper.ModifyProfile(profileName).AddBackupedFilePath(i);
		}
		else
			backuper.ModifyProfile(profileName).AddBackupedFolderPath(i);
	}

	backuper.GetProfile(profileName).SaveProfile();

	if (backupRepeat == "Every day")
	{

	}

	if (backupRepeat == "Every week")
	{
		QString dayWeek = field("dayWeek").toString();
		qDebug() << dayWeek << endl;
	}

	if (backupRepeat == "On the specified date")
	{
		QString dateBackup = field("date").toString();
		qDebug() << dateBackup << endl;
	}

	if (backupRepeat == "Dafault profile without date to backup")
	{

	}

	QDialog::accept();
}

int WizardFilesPage::nextId() const
{
	if (/*(labPathResFile->text() == "Not selected" || labPathResFile->text() == "")
		||*/ count_ == 0)
		return FILES_PAGE;
	else
		return DATE_BACKUP_PROFILE;
}

QStringList WizardFilesPage::theStringList() const
{
	QStringList strListFiles;

	for (auto i = 0; i < listFiles_->count(); ++i)
	{
		strListFiles << listFiles_->item(i)->text();
	}

	return strListFiles;
}

void WizardFilesPage::ClickedResultFile()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		tr("Choose result file"),
		"C://",
		"Text file (*.txt*);;All files (*.*)"
	);
	labPathResFile_->setText(filename);
}

void WizardFilesPage::ClickedAddFile()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		tr("Select file"),
		"C://",
		"All files (*.*)"
	);
	if (filename != "")
	{
		listFiles_->addItem(filename);
		count_++;
	}
}

void WizardFilesPage::ClickedAddDirectory()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	if (dir != "")
	{
		listFiles_->addItem(dir);
		count_++;
	}
}

int WizardDateBackupProfile::nextId() const
{
	if (rbEveryDay_->isChecked() == true)
	{
		return CONCLUSION_PAGE;
	}

	if (rbEveryWeek_->isChecked() == true)
	{
		return DATE_EVERY_WEEK_BACKUP_PAGE;
	}

	if (rbSpecDate_->isChecked() == true)
	{
		return SPEC_DATE_BACKUP_PAGE;
	}

	return CONCLUSION_PAGE;
}

QString WizardDateBackupProfile::theString() const
{
	QString strDate;

	if (rbEveryDay_->isChecked() == true)
	{
		strDate = rbEveryDay_->text();

		return strDate;
	}

	if (rbEveryWeek_->isChecked() == true)
	{
		strDate = rbEveryWeek_->text();

		return strDate;
	}

	if (rbSpecDate_->isChecked() == true)
	{
		strDate = rbSpecDate_->text();

		return strDate;
	}

	strDate = rbNoDate_->text();

	return strDate;
}

QString WizardDateEveryWeekBackupPage::theStringDayWeek() const
{
	QString strDayWeek;
	strDayWeek = combDaysWeek_->itemText(combDaysWeek_->currentIndex());

	return strDayWeek;
}

int WizardSpecDateBackupPage::nextId() const
{
	if (calendar_->selectedDate()<QDate::currentDate())
	{
		QMessageBox::warning(0, "Warning", "You have selected an unavailable date. \t");

		return SPEC_DATE_BACKUP_PAGE;
	}

	return CONCLUSION_PAGE;
}

QString WizardSpecDateBackupPage::theStringDateBackup() const
{
	QString strDate;
	strDate = calendar_->selectedDate().toString();

	return strDate;
}
