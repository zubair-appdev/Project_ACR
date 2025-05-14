#include "health.h"
#include "ui_health.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QButtonGroup>

Health::Health(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Health)
{
    ui->setupUi(this);


    this->setMinimumSize(100, 100);
    // this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);  // No upper size limit
    // this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);
    this->showMaximized();

    // Only retrieve and embed the central widget in a scroll area
    QWidget *existingCentralWidget = takeCentralWidget(); // Take the existing central widget
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(existingCentralWidget);         // Embed it in the scroll area
    scrollArea->setWidgetResizable(true);                 // Allow resizing within the scroll area

    // Set the scroll area as the new central widget
    setCentralWidget(scrollArea);

    pageNumber = 1;

    QPixmap logo(":/new/prefix1/resourceFiles/logo.jpg");
   // ui->label_logo->setPixmap(logo);

    takeLayoutAndbool(ui->verticalLayout_29,false);
    takeLayoutAndbool(ui->verticalLayout_30,false);

    ui->lineEdit_Techne->setPlaceholderText("mCi");
    ui->lineEdit_METS->setPlaceholderText("METS");
    ui->lineEdit_Tc99->setPlaceholderText("mCi");

    ui->lineEdit_PriorStudy->setDisabled(true);
    ui->lineEdit_threadMill->setDisabled(true);
    ui->lineEdit_ChestEKG->setDisabled(true);

    //validations
    QDoubleValidator *decimalValidator = new QDoubleValidator(0.0,1000.0,4,this);
    ui->lineEdit_Techne->setValidator(decimalValidator);
    ui->lineEdit_METS->setValidator(decimalValidator);
    ui->lineEdit_Tc99->setValidator(decimalValidator);
    ui->lineEdit_computedEF->setValidator(decimalValidator);
    ui->lineEdit_ESV->setValidator(decimalValidator);
    ui->lineEdit_TID->setValidator(decimalValidator);

    //Grouping RadioButtons
    //------------------------------------------------------------------------------------------------------
    QButtonGroup *genderG = new QButtonGroup(centralWidget());
    genderG->addButton(ui->radioButton_m);
    genderG->addButton(ui->radioButton_fe);

    QButtonGroup *priorStudyG = new QButtonGroup(centralWidget());
    priorStudyG->addButton(ui->radioButton_yesP);
    priorStudyG->addButton(ui->radioButton_noP);

    QButtonGroup *norEnlg = new QButtonGroup(centralWidget());
    norEnlg->addButton(ui->radioButton_normalSIze);
    norEnlg->addButton(ui->radioButton_enlargedSize);

    QButtonGroup *threadmillg = new QButtonGroup(centralWidget());
    threadmillg->addButton(ui->radioButton_yesThread);
    threadmillg->addButton(ui->radioButton_noThread);

    QButtonGroup *chestg = new QButtonGroup(centralWidget());
    chestg->addButton(ui->radioButton_yesChest);
    chestg->addButton(ui->radioButton_noChest);

    QButtonGroup *revPer = new QButtonGroup(centralWidget());
    revPer->addButton(ui->radioButton_yesRevPer);
    revPer->addButton(ui->radioButton_noRevPer);

    QButtonGroup *revAnterior = new QButtonGroup(centralWidget());
    revAnterior->addButton(ui->radioButton_basalAnteriorRev);
    revAnterior->addButton(ui->radioButton_midAnteriorREv);
    revAnterior->addButton(ui->radioButton_distaAnteriorlRev);
    revAnterior->addButton(ui->radioButton_revAntNone);

    QButtonGroup *revInferior = new QButtonGroup(centralWidget());
    revInferior->addButton(ui->radioButton_basalInferiorRev);
    revInferior->addButton(ui->radioButton_midInferiorRev);
    revInferior->addButton(ui->radioButton_distalInferiorRev);
    revInferior->addButton(ui->radioButton_revInfNone);


    QButtonGroup *fixPer = new QButtonGroup(centralWidget());
    fixPer->addButton(ui->radioButton_yesFixPer);
    fixPer->addButton(ui->radioButton_noFixPer);

    QButtonGroup *fixAnterior = new QButtonGroup(centralWidget());
    fixAnterior->addButton(ui->radioButton_basalAnteriorFix);
    fixAnterior->addButton(ui->radioButton_midAnteriorFix);
    fixAnterior->addButton(ui->radioButton_distaAnteriorlFix);
    fixAnterior->addButton(ui->radioButton_fixAntNone);

    QButtonGroup *fixInferior = new QButtonGroup(centralWidget());
    fixInferior->addButton(ui->radioButton_basalInferiorFix);
    fixInferior->addButton(ui->radioButton_midInferiorFix);
    fixInferior->addButton(ui->radioButton_distalInferiorFix);
    fixInferior->addButton(ui->radioButton_fixInfNone);

    QButtonGroup *TID = new QButtonGroup(centralWidget());
    TID->addButton(ui->radioButton_TIDnormal);
    TID->addButton(ui->radioButton_TIDabnormal);

    QButtonGroup *RV = new QButtonGroup(centralWidget());
    RV->addButton(ui->radioButton_RVnormal);
    RV->addButton(ui->radioButton_RVabnormal);

    QButtonGroup *Con = new QButtonGroup(centralWidget());
    Con->addButton(ui->radioButton_ConNormal);
    Con->addButton(ui->radioButton_ConProNor);
    Con->addButton(ui->radioButton_ConMilAbn);
    Con->addButton(ui->radioButton_ConAbn);

    QButtonGroup *LV = new QButtonGroup(centralWidget());
    LV->addButton(ui->radioButton_LVnor);
    LV->addButton(ui->radioButton_LVabn);

    QButtonGroup *Recommend = new QButtonGroup(centralWidget());
    Recommend->addButton(ui->radioButton_yesRecommend);
    Recommend->addButton(ui->radioButton_noRecommend);

    QButtonGroup *typeOfTest = new QButtonGroup(centralWidget());
    typeOfTest->addButton(ui->radioButton_exercise);
    typeOfTest->addButton(ui->radioButton_lexiscan);

    //-------------------------------------------------------------------------------------------------------

    connect(ui->radioButton_yesP,SIGNAL(clicked()),this,SLOT(openLineEditPrior()));
    connect(ui->radioButton_noP,SIGNAL(clicked()),this,SLOT(closeLineEditPrior()));

    connect(ui->radioButton_yesThread,SIGNAL(clicked()),this,SLOT(openLineEditThread()));
    connect(ui->radioButton_noThread,SIGNAL(clicked()),this,SLOT(closeLineEditThread()));

    connect(ui->radioButton_yesChest,SIGNAL(clicked()),this,SLOT(openLineEditChest()));
    connect(ui->radioButton_noChest,SIGNAL(clicked()),this,SLOT(closeLineEditChest()));

    connect(ui->checkBox_chest,SIGNAL(stateChanged(int)),this,SLOT(indicationDataReceiver()));
    connect(ui->checkBox_Pre,SIGNAL(stateChanged(int)),this,SLOT(indicationDataReceiver()));
    connect(ui->checkBox_Dys,SIGNAL(stateChanged(int)),this,SLOT(indicationDataReceiver()));
    connect(ui->checkBox_has,SIGNAL(stateChanged(int)),this,SLOT(indicationDataReceiver()));
    connect(ui->checkBox_his,SIGNAL(stateChanged(int)),this,SLOT(indicationDataReceiver()));
    connect(ui->checkBox_sp,SIGNAL(stateChanged(int)),this,SLOT(indicationDataReceiver()));

    connect(ui->checkBox_age,SIGNAL(stateChanged(int)),this,SLOT(riskFactorCADdataReceiver()));
    connect(ui->checkBox_DM,SIGNAL(stateChanged(int)),this,SLOT(riskFactorCADdataReceiver()));
    connect(ui->checkBox_HTN,SIGNAL(stateChanged(int)),this,SLOT(riskFactorCADdataReceiver()));
    connect(ui->checkBox_COPD,SIGNAL(stateChanged(int)),this,SLOT(riskFactorCADdataReceiver()));
    connect(ui->checkBox_dyslip,SIGNAL(stateChanged(int)),this,SLOT(riskFactorCADdataReceiver()));
    connect(ui->checkBox_family,SIGNAL(stateChanged(int)),this,SLOT(riskFactorCADdataReceiver()));
    connect(ui->checkBox_PreDM,SIGNAL(stateChanged(int)),this,SLOT(riskFactorCADdataReceiver()));
    connect(ui->checkBox_former,SIGNAL(stateChanged(int)),this,SLOT(riskFactorCADdataReceiver()));
    connect(ui->checkBox_smoking,SIGNAL(stateChanged(int)),this,SLOT(riskFactorCADdataReceiver()));
    connect(ui->checkBox_stroke,SIGNAL(stateChanged(int)),this,SLOT(riskFactorCADdataReceiver()));

    connect(ui->checkBox_normalRhythm,SIGNAL(stateChanged(int)),this,SLOT(resultsDataReceiver()));
    connect(ui->checkBox_LAFB,SIGNAL(stateChanged(int)),this,SLOT(resultsDataReceiver()));
    connect(ui->checkBox_RBBB,SIGNAL(stateChanged(int)),this,SLOT(resultsDataReceiver()));
    connect(ui->checkBox_LBBB,SIGNAL(stateChanged(int)),this,SLOT(resultsDataReceiver()));
    connect(ui->checkBox_QWaves,SIGNAL(stateChanged(int)),this,SLOT(resultsDataReceiver()));
    connect(ui->checkBox_brady,SIGNAL(stateChanged(int)),this,SLOT(resultsDataReceiver()));
    connect(ui->checkBox_AFB,SIGNAL(stateChanged(int)),this,SLOT(resultsDataReceiver()));

    connect(ui->radioButton_yesRevPer,SIGNAL(clicked()),this,SLOT(onPressRevYes()));
    connect(ui->radioButton_noRevPer,SIGNAL(clicked()),this,SLOT(onPressRevNo()));

    connect(ui->checkBox_anteriorTerRev,SIGNAL(stateChanged(int)),this,SLOT(openAnteriorRev()));
    connect(ui->checkBox_inferiorTerRev,SIGNAL(stateChanged(int)),this,SLOT(openInferiorRev()));

    connect(ui->radioButton_yesFixPer,SIGNAL(clicked()),this,SLOT(onPressFixYes()));
    connect(ui->radioButton_noFixPer,SIGNAL(clicked()),this,SLOT(onPressFixNo()));

    connect(ui->checkBox_anteriorTerFix,SIGNAL(stateChanged(int)),this,SLOT(openAnteriorFix()));
    connect(ui->checkBox_inferiorTerFix,SIGNAL(stateChanged(int)),this,SLOT(openInferiorFix()));

    connect(ui->radioButton_lexiscan, &QRadioButton::toggled, this, &Health::onLexiscanToggled);
}

Health::~Health()
{
    delete ui;
}


void Health::on_pushButton_clicked()
{
    QDate today = ui->dateEdit_today->date();

    QString ref = ui->lineEdit_ref->text();

    QString sex;
    if(ui->radioButton_m->isChecked())
    {
        sex = ui->radioButton_m->text();
    }
    else if(ui->radioButton_fe->isChecked())
    {
        sex = ui->radioButton_fe->text();
    }
    else
    {
        //do nothing
    }

    QDate dob = ui->dateEdit_dob->date();

    QString pName = ui->lineEdit_pName->text();

    pageNumber = 1;

    notCreateWord = true;
    generatePDF(today,ref,sex,dob,pName);
}


void Health::openLineEditPrior()
{
    ui->lineEdit_PriorStudy->setDisabled(false);

}

void Health::closeLineEditPrior()
{
    ui->lineEdit_PriorStudy->setDisabled(true);
    ui->lineEdit_PriorStudy->setText("None Available.");
}

void Health::openLineEditThread()
{
    ui->lineEdit_threadMill->setDisabled(true);
}

void Health::closeLineEditThread()
{
    ui->lineEdit_threadMill->setDisabled(false);
}

void Health::openLineEditChest()
{
    ui->lineEdit_ChestEKG->setDisabled(true);
}

void Health::closeLineEditChest()
{
    ui->lineEdit_ChestEKG->setDisabled(false);
}

//function definition for enabling/disabling all widgets in a layout
void Health::takeLayoutAndbool(QLayout *layoutz, bool boolz)
{
    for (int i = 0; i < layoutz->count(); ++i) {
        QLayoutItem *item = layoutz->itemAt(i);
        if (item) {
            // Enable/Disables widgets directly
            QWidget *widget = item->widget();
            if (widget)
                widget->setEnabled(boolz);

            // Handle layouts separately if needed
            QLayout *layout = item->layout();
            if (layout) {
                enableWidgetsInLayout(layout, boolz);
            }
        }
    }
}

//function definition for if a layout has layout to enable/disable widgets that are contained in a layout
void Health::enableWidgetsInLayout(QLayout *layout, bool enable)
{
    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem *item = layout->itemAt(i);
        if (item) {
            QWidget *widget = item->widget();
            if (widget)
                widget->setEnabled(enable);
        }
    }
}

void Health::onPressRevYes()
{
    if(ui->radioButton_yesRevPer->isChecked())
    {
        takeLayoutAndbool(ui->verticalLayout_29,true);
    }
    ui->radioButton_basalAnteriorRev->setEnabled(false);
    ui->radioButton_midAnteriorREv->setEnabled(false);
    ui->radioButton_distaAnteriorlRev->setEnabled(false);
    ui->radioButton_revAntNone->setEnabled(false);

    ui->radioButton_basalInferiorRev->setEnabled(false);
    ui->radioButton_midInferiorRev->setEnabled(false);
    ui->radioButton_distalInferiorRev->setEnabled(false);
    ui->radioButton_revInfNone->setEnabled(false);

}

void Health::onPressRevNo()
{
    if (ui->radioButton_noRevPer->isChecked())
    {
        takeLayoutAndbool(ui->verticalLayout_29,false);
    }

}

void Health::openAnteriorRev()
{
    if(ui->checkBox_anteriorTerRev->isChecked())
    {
        ui->radioButton_basalAnteriorRev->setEnabled(true);
        ui->radioButton_midAnteriorREv->setEnabled(true);
        ui->radioButton_distaAnteriorlRev->setEnabled(true);
        ui->radioButton_revAntNone->setEnabled(true);
    }
    else
    {
        ui->radioButton_basalAnteriorRev->setEnabled(false);
        ui->radioButton_midAnteriorREv->setEnabled(false);
        ui->radioButton_distaAnteriorlRev->setEnabled(false);
        ui->radioButton_revAntNone->setEnabled(false);
    }
}

void Health::openInferiorRev()
{
    if(ui->checkBox_inferiorTerRev->isChecked())
    {
        ui->radioButton_basalInferiorRev->setEnabled(true);
        ui->radioButton_midInferiorRev->setEnabled(true);
        ui->radioButton_distalInferiorRev->setEnabled(true);
        ui->radioButton_revInfNone->setEnabled(true);
    }
    else
    {
        ui->radioButton_basalInferiorRev->setEnabled(false);
        ui->radioButton_midInferiorRev->setEnabled(false);
        ui->radioButton_distalInferiorRev->setEnabled(false);
        ui->radioButton_revInfNone->setEnabled(false);
    }
}

void Health::onPressFixYes()
{
    if(ui->radioButton_yesFixPer->isChecked())
    {
        takeLayoutAndbool(ui->verticalLayout_30,true);
    }
    ui->radioButton_basalAnteriorFix->setEnabled(false);
    ui->radioButton_midAnteriorFix->setEnabled(false);
    ui->radioButton_distaAnteriorlFix->setEnabled(false);
    ui->radioButton_fixAntNone->setEnabled(false);

    ui->radioButton_basalInferiorFix->setEnabled(false);
    ui->radioButton_midInferiorFix->setEnabled(false);
    ui->radioButton_distalInferiorFix->setEnabled(false);
    ui->radioButton_fixInfNone->setEnabled(false);
}

void Health::onPressFixNo()
{
    if(ui->radioButton_noFixPer->isChecked())
    {
        takeLayoutAndbool(ui->verticalLayout_30,false);
    }

}

void Health::openAnteriorFix()
{
    if(ui->checkBox_anteriorTerFix->isChecked())
    {
        ui->radioButton_basalAnteriorFix->setEnabled(true);
        ui->radioButton_midAnteriorFix->setEnabled(true);
        ui->radioButton_distaAnteriorlFix->setEnabled(true);
        ui->radioButton_fixAntNone->setEnabled(true);
    }
    else
    {
        ui->radioButton_basalAnteriorFix->setEnabled(false);
        ui->radioButton_midAnteriorFix->setEnabled(false);
        ui->radioButton_distaAnteriorlFix->setEnabled(false);
        ui->radioButton_fixAntNone->setEnabled(false);
    }

}

void Health::openInferiorFix()
{
    if(ui->checkBox_inferiorTerFix->isChecked())
    {
        ui->radioButton_basalInferiorFix->setEnabled(true);
        ui->radioButton_midInferiorFix->setEnabled(true);
        ui->radioButton_distalInferiorFix->setEnabled(true);
        ui->radioButton_fixInfNone->setEnabled(true);
    }
    else
    {
        ui->radioButton_basalInferiorFix->setEnabled(false);
        ui->radioButton_midInferiorFix->setEnabled(false);
        ui->radioButton_distalInferiorFix->setEnabled(false);
        ui->radioButton_fixInfNone->setEnabled(false);
    }
}

void Health::onLexiscanToggled(bool checked)
{
    if (checked) {
        ui->lineEdit_MAPHR->setDisabled(true);
        ui->timeEdit_treadMill->setDisabled(true);
        ui->lineEdit_METS->setDisabled(true);
    }
    else
    {
        ui->lineEdit_MAPHR->setDisabled(false);
        ui->timeEdit_treadMill->setDisabled(false);
        ui->lineEdit_METS->setDisabled(false);
    }
}

void Health::indicationDataReceiver()
{
    QStringList IndicationData;
    if(ui->checkBox_chest->isChecked())
    {
        IndicationData.append(ui->checkBox_chest->text());
    }
    if(ui->checkBox_Pre->isChecked())
    {
        IndicationData.append(ui->checkBox_Pre->text());
    }
    if(ui->checkBox_Dys->isChecked())
    {
        IndicationData.append(ui->checkBox_Dys->text());
    }
    if(ui->checkBox_has->isChecked())
    {
        IndicationData.append(ui->checkBox_has->text());
    }
    if(ui->checkBox_his->isChecked())
    {
        IndicationData.append(ui->checkBox_his->text());
    }
    if(ui->checkBox_sp->isChecked())
    {
        IndicationData.append(ui->checkBox_sp->text());
    }

    this->IndicationData = IndicationData;

}

void Health::riskFactorCADdataReceiver()
{
    QStringList RiskFactorCAD_Data;
    if(ui->checkBox_age->isChecked())
        RiskFactorCAD_Data.append(ui->checkBox_age->text());
    if(ui->checkBox_DM->isChecked())
        RiskFactorCAD_Data.append(ui->checkBox_DM->text());
    if(ui->checkBox_HTN->isChecked())
        RiskFactorCAD_Data.append(ui->checkBox_HTN->text());
    if(ui->checkBox_COPD->isChecked())
        RiskFactorCAD_Data.append(ui->checkBox_COPD->text());
    if(ui->checkBox_dyslip->isChecked())
        RiskFactorCAD_Data.append(ui->checkBox_dyslip->text());
    if(ui->checkBox_family->isChecked())
        RiskFactorCAD_Data.append(ui->checkBox_family->text());
    if(ui->checkBox_PreDM->isChecked())
        RiskFactorCAD_Data.append(ui->checkBox_PreDM->text());
    if(ui->checkBox_former->isChecked())
        RiskFactorCAD_Data.append(ui->checkBox_former->text());
    if(ui->checkBox_smoking->isChecked())
        RiskFactorCAD_Data.append(ui->checkBox_smoking->text());
    if(ui->checkBox_stroke->isChecked())
        RiskFactorCAD_Data.append(ui->checkBox_stroke->text());

    this->RiskFactorCAD_Data=RiskFactorCAD_Data;

}

void Health::resultsDataReceiver()
{
    QStringList ResultsData;

    if(ui->checkBox_normalRhythm->isChecked())
        ResultsData.append(ui->checkBox_normalRhythm->text());
    if(ui->checkBox_LAFB->isChecked())
        ResultsData.append(ui->checkBox_LAFB->text());
    if(ui->checkBox_RBBB->isChecked())
        ResultsData.append(ui->checkBox_RBBB->text());
    if(ui->checkBox_LBBB->isChecked())
        ResultsData.append(ui->checkBox_LBBB->text());
    if(ui->checkBox_QWaves->isChecked())
        ResultsData.append(ui->checkBox_QWaves->text());
    if(ui->checkBox_brady->isChecked())
        ResultsData.append(ui->checkBox_brady->text());

    if(ui->checkBox_AFB->isChecked())
    {
        ResultsData.append(ui->checkBox_AFB->text());
    }

    this->ResultsData = ResultsData;

}

void Health::generatePDF(const QDate &today, const QString &ref, const QString &sex,
                         const QDate &dob, const QString &PName)
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save PDF","","*.pdf");
    if(fileName.isEmpty())
    {
        return;
    }

    QPdfWriter writer(fileName);

    // Get page dimensions for dynamic calculations
    int pageWidth = writer.width();
    int pageHeight = writer.height();

    writer.setPageSize(QPageSize(QPageSize::A4));
    int leftMargin = static_cast<int>(pageWidth * (30.0 / pageWidth));   // 30 pixels as a percentage of page width
    int topMargin = static_cast<int>(pageHeight * (30.0 / pageHeight));   // 30 pixels as a percentage of page height
    int rightMargin = static_cast<int>(pageWidth * (30.0 / pageWidth));  // 30 pixels as a percentage of page width
    int bottomMargin = static_cast<int>(pageHeight * (30.0 / pageHeight)); // 30 pixels as a percentage of page height

    writer.setPageMargins(QMargins(leftMargin, topMargin, rightMargin, bottomMargin));


    QPainter painter(&writer);

    QPen pen;
    pen.setWidth(3);
    painter.setPen(pen);

    //For Normal Text
    QFont normalFont = painter.font();
    normalFont.setPointSize(10);

    //For Bold Text
    QFont boldFont = painter.font();
    boldFont.setPointSize(11);
    boldFont.setBold(true);

    //Loading logo image
    QImage logoImage(":/new/prefix1/resourceFiles/logo.jpg");

    int y = static_cast<int>(pageHeight * (600.0 / pageHeight));


    painter.drawImage(QRect(200,y,1400,1400),logoImage);
    y += static_cast<int>(pageHeight * (3300.0 / pageHeight));


    // Drawing that rectangle box
    int rectX = static_cast<int>(pageWidth * (2400.0 / pageWidth)); // Convert 2400 to dynamic value
    int rectY = static_cast<int>(pageHeight * (600.0 / pageHeight)); // Convert 600 to dynamic value
    int rectWidth = static_cast<int>(pageWidth * (5000.0 / pageWidth)); // Convert 5000 to dynamic value
    int rectHeight = static_cast<int>(pageHeight * (1800.0 / pageHeight)); // Convert 1800 to dynamic value

    painter.drawRect(rectX, rectY, rectWidth, rectHeight);


    // Writing text inside box -------------------------------------
    int boxY = static_cast<int>(pageHeight * (1000.0 / pageHeight)); // Convert 1000 to dynamic value
    int boxYHeight = static_cast<int>(pageHeight * (370.0 / pageHeight)); // Convert 370 to dynamic value

    painter.setFont(boldFont);
    painter.drawText(static_cast<int>(pageWidth * (2900.0 / pageWidth)), boxY, "Cardio Vascular Associates of North Houston");
    boxY += boxYHeight;
    painter.drawText(static_cast<int>(pageWidth * (3700.0 / pageWidth)), boxY, "Nitin Mahajan MD, MPH");
    boxY += boxYHeight;

    painter.setFont(normalFont);
    painter.drawText(static_cast<int>(pageWidth * (3200.0 / pageWidth)), boxY, "17070 Red Oak Dr., Ste. 305, Houston, TX 77090");
    boxY += boxYHeight;
    painter.drawText(static_cast<int>(pageWidth * (3500.0 / pageWidth)), boxY, "Phone-8322964203; Fax-2104050294");
    boxY += boxYHeight + static_cast<int>(pageHeight * (600.0 / pageHeight)); // Convert 600 to dynamic value

    painter.setFont(boldFont);

    if (ui->radioButton_lexiscan->isChecked())
    {
        painter.drawText(static_cast<int>(pageWidth * (3300.0 / pageWidth)), boxY, "Lexiscan Stress Testing");
        ui->lineEdit_MAPHR->setDisabled(true);
        ui->timeEdit_treadMill->setDisabled(true);
        ui->lineEdit_METS->setDisabled(true);
    }
    else
    {
        painter.drawText(static_cast<int>(pageWidth * (3000.0 / pageWidth)), boxY, "Exercise Myoview Stress Testing");
    }

    painter.setFont(normalFont);
    // Inside text completed ----------------------------------------

    lineHeight = static_cast<int>(pageHeight * (250.0 / pageHeight));
    currentY = boxY += lineHeight + static_cast<int>(pageHeight * (500.0 / pageHeight)); // Convert 500 to dynamic value


    //For Normal Text after box
    normalFont = painter.font();
    normalFont.setFamily("Courier New"); // Set the font family
    normalFont.setPointSize(11);

    //For Bold Text
    boldFont = painter.font();
    boldFont.setFamily("Courier New"); // Set the font family
    boldFont.setPointSize(11);
    boldFont.setBold(true);

    currentX = static_cast<int>(writer.width() * 0.04);
    int stationaryX = static_cast<int>(writer.width() * 0.04);

    //Tab 1 data
    //^^^^^^^^^^

    generatePdf(writer,painter,boldFont,"Date: ",currentX,1,1);
    generatePdf(writer,painter,normalFont,today.toString("MM/dd/yyyy"),currentX,1,1);
    // Calculate dynamic width based on page width
    int dynamicAgeWidth = static_cast<int>(pageWidth * (6000.0 / pageWidth));
    generatePdf(writer, painter, boldFont, "Age: ", dynamicAgeWidth, 1, 1);
    int Age = ui->dateEdit_dob->date().daysTo(ui->dateEdit_today->date());
    generatePdf(writer,painter,normalFont,QString::number(Age/365),currentX,1,1);

    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    generatePdf(writer ,painter,boldFont,"Re: ",stationaryX,1,1);
    generatePdf(writer,painter,normalFont,ref,currentX,1,1);
    int dynamicSexWidth = static_cast<int>(pageWidth * (6000.0 / pageWidth));
    generatePdf(writer,painter,boldFont,"Sex: ",dynamicSexWidth,1,1);
    generatePdf(writer,painter,normalFont,sex,currentX,1,1);

    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    generatePdf(writer,painter,boldFont,"Date of Birth: ",stationaryX,1,1);
    generatePdf(writer,painter,normalFont,dob.toString("MM/dd/yyyy"),currentX,1,1);
    int dynamicMRWidth = static_cast<int>(pageWidth * (6000.0 / pageWidth));
    generatePdf(writer,painter,boldFont,"MR: ",dynamicMRWidth,1,1);
    generatePdf(writer,painter,normalFont,PName,currentX,1,1);

    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    generatePdf(writer,painter,boldFont,"Referring MD: ",stationaryX,1,1);
    generatePdf(writer,painter,normalFont,"PCP",currentX,1,1);
    int dynamicTechWidth = static_cast<int>(pageWidth * (6000.0 / pageWidth));
    generatePdf(writer,painter,boldFont,"Tech: ",dynamicTechWidth,1,1);
    generatePdf(writer,painter,normalFont,"Patel",currentX,1,1);

    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    generatePdf(writer,painter,boldFont,"Prior Study: ",stationaryX,1,1);
    generatePdf(writer,painter,normalFont,ui->lineEdit_PriorStudy->text(),currentX,1,1);

    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    generatePdf(writer,painter,boldFont,"Interpreting Physician: ",stationaryX,1,1);
    generatePdf(writer,painter,normalFont,"Nitin Mahajan MD, MPH",currentX,1,1);

    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    qDebug()<<IndicationData;
    QString pdfIndicationData = IndicationData.join(", ") + ".";
    qDebug() << pdfIndicationData;

    generatePdf(writer,painter,boldFont,"Indication for Study: ",stationaryX,1,1);
    generatePdf(writer,painter,normalFont,pdfIndicationData,currentX,1,1);
    generatePdf(writer,painter,normalFont," Risk factors for CAD include ",currentX,1,1);

    qDebug()<<RiskFactorCAD_Data;
    QString pdfRiskFactorsData = RiskFactorCAD_Data.join(", ") + ".";
    qDebug() << pdfRiskFactorsData;

    generatePdf(writer,painter,normalFont,pdfRiskFactorsData,currentX,1,1);

    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax


    //Tab 2 data
    //^^^^^^^^^^

    generatePdf(writer,painter,boldFont,"Description of Procedure: ",stationaryX,1,1);
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    QString gender2,gender3,Tectnetium_99,treadmillBruce,METS,Tc99;
    Tectnetium_99 = ui->lineEdit_Techne->text();
    treadmillBruce = ui->timeEdit_treadMill->time().toString("mm:ss");
    METS = ui->lineEdit_METS->text();
    Tc99 = ui->lineEdit_Tc99->text();

    if(sex=="Male")
    {
        gender2 = "He";
        gender3 = "he";
    }
    else
    {
        gender2 = "She";
        gender3 = "she";
    }

    QString DespOfProString;
    if(ui->radioButton_lexiscan->isChecked())
    {
        DespOfProString = QString("The patient underwent Lexiscan Myoview Imaging study using one day protocol."
                                          "%1 was initially injected intravenously with %2 mCi of Tc MIBI "
                                          "at rest in left elbow by Patel and resting images acquired 30 min "
                                          "post injection. The patient was intravenously infused with 0.4mg/5ml of LEXISCAN in a bolus"
                                  " fashion over a minute. At peak hyperemia %3 was injected intravenously with %4"
                                          " mCi of Tc MIBI and tomographic images were acquired 25 min post"
                                          " injection.").arg(gender2).arg(Tectnetium_99).arg(gender3).arg(Tc99);
    }
    else
    {
        DespOfProString = QString("The patient underwent Exercise Myoview Imaging study using one day protocol."
                                          "%1 was initially injected intravenously with %2 mCi of Technetium-99M Sestamibi "
                                          "intravenously at rest in left elbow by Patel and resting images acquired 30 min "
                                          "post injection. The patient subsequently exercised on treadmill using Bruce protocol for "
                                          " %3 min achieving %4 METS. At peak exercise %5 was injected intravenously with %6"
                                          " mCi of Tc 99M Sestamibi and gated and tomographic images were acquired 25 min post"
                                          " injection.").arg(gender2).arg(Tectnetium_99).arg(treadmillBruce).arg(METS).arg(gender3).arg(Tc99);
    }


    qDebug()<<DespOfProString;
    generatePdf(writer,painter,normalFont,DespOfProString,stationaryX,1,1);

    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    generatePdf(writer,painter,boldFont,"Results: ",stationaryX,1,1);
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    QString pdfResultsString;


    pdfResultsString = ResultsData.join(", ") + ".";


    QString iniHr,iniBp,maxHr,MAPHR,maxBP;
    iniHr = ui->lineEdit_iniHR->text();
    iniBp = ui->lineEdit_iniBP->text();
    maxHr = ui->lineEdit_maxHR->text();
    MAPHR = ui->lineEdit_MAPHR->text();
    maxBP = ui->lineEdit_maxBP->text();

    QString ogResults;

    if(ui->radioButton_lexiscan->isChecked())
    {
        ogResults = QString("Baseline EKG showed %1 Initial HR was %2/min with BP of %3 mmHg. At peak hyperemia,"
                            " the maximal HR was %4/min"
                            " with BP of %5 mmHg.").arg(pdfResultsString).arg(iniHr).arg(iniBp).arg(maxHr).arg(maxBP);
    }
    else
    {
        ogResults = QString("Baseline EKG showed %1 Initial HR was %2/min with BP of %3 mmHg. At peak exercise,"
                            " the maximal HR was %4/min (%5 % MAPHR)"
                            " with BP of %6 mmHg.").arg(pdfResultsString).arg(iniHr).arg(iniBp).arg(maxHr)
                .arg(MAPHR).arg(maxBP);
    }

    qDebug()<<ogResults;

    generatePdf(writer,painter,normalFont,ogResults,stationaryX,1,1);
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    QString threadMillStat, chestStat,ogResults2;
    threadMillStat = ui->lineEdit_threadMill->text();
    chestStat = ui->lineEdit_ChestEKG->text();

    QString threadOrLexi;
    if(ui->radioButton_lexiscan->isChecked())
    {
        threadOrLexi = "Lexiscan infusion";
    }
    else
    {
        threadOrLexi = "treadmill";
    }

    if(ui->radioButton_yesThread->isChecked())
    {
        ogResults2.append(QString("During the %1 normal blood pressure and heart rate response was noted. ").arg(threadOrLexi));
    }
    else
    {
        ogResults2.append(threadMillStat);
    }

    if(ui->radioButton_yesChest->isChecked())
    {
        ogResults2.append("No chest pain or EKG changes seen.");
    }
    else
    {
        ogResults2.append(chestStat);
    }

    qDebug()<<ogResults2;

    generatePdf(writer,painter,normalFont,ogResults2,stationaryX,1,1);

    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    //Tab 3 data
    //^^^^^^^^^^

    int dynamic500Width = static_cast<int>(pageWidth * (500.0 / pageWidth)); // Convert 6000 to dynamic value
    int dynamic1000Width = static_cast<int>(pageWidth * (1000.0 / pageWidth)); // Convert 6000 to dynamic value

    generatePdf(writer,painter,boldFont,"Imaging Studies :",stationaryX,1,1);
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,boldFont,"1.Stress Images :",stationaryX+dynamic500Width,1,1);
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,boldFont,"a.SPECT Gated Images - ",stationaryX+dynamic1000Width,1,1);
    
    QString stressSpect;
    stressSpect.append("SPECT gated images show left ventricle was ");
    if(ui->radioButton_normalSIze->isChecked())
    {
        stressSpect.append(ui->radioButton_normalSIze->text()+". ");
    }
    if(ui->radioButton_enlargedSize->isChecked())
    {
        stressSpect.append(ui->radioButton_enlargedSize->text()+". ");
        
    }
    
    if(ui->checkBox_gating->isChecked())
    {
        stressSpect.append("There was "+ui->checkBox_gating->text()+". ");
    }
    
    stressSpect.append(QString("Computed EF by software was %1% "
                               "with ESV of %2 ml.").arg(ui->lineEdit_computedEF->text())
                       .arg(ui->lineEdit_ESV->text()));

    qDebug()<<stressSpect;

    generatePdf(writer,painter,normalFont,stressSpect,currentX,1,1);
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax


    //Tab 4 data
    //^^^^^^^^^^

    generatePdf(writer,painter,boldFont,"b.SPECT Tomographic Images - ",stationaryX+dynamic1000Width,1,1);

    QString stressSpectTomo;
    QStringList revData,fixData;

    if(ui->radioButton_yesRevPer->isChecked())
    {
        revData.append("reversible perfusion defect in");
        if(ui->checkBox_anteriorTerRev->isChecked())
        {
            if(ui->radioButton_basalAnteriorRev->isChecked())
            {
                revData.append("basal anterior territory");
            }
            if(ui->radioButton_midAnteriorREv->isChecked())
            {
                revData.append("mid anterior territory");
            }
            if(ui->radioButton_distaAnteriorlRev->isChecked())
            {
                revData.append("distal anterior territory");
            }
            if(ui->radioButton_revAntNone->isChecked())
            {
                revData.append("anterior territory");
            }
        }

        if(ui->checkBox_inferiorTerRev->isChecked())
        {
            if(ui->radioButton_basalInferiorRev->isChecked())
            {
                revData.append("basal inferior territory");
            }
            if(ui->radioButton_midInferiorRev->isChecked())
            {
                revData.append("mid inferior territory");
            }
            if(ui->radioButton_distalInferiorRev->isChecked())
            {
                revData.append("distal inferior territory");
            }
            if(ui->radioButton_revInfNone->isChecked())
            {
                revData.append("inferior territory");
            }
        }

        if(ui->checkBox_inferoLat_Rev->isChecked())
        {
            revData.append("infero lateral territory");
        }
        if(ui->checkBox_anteroLat_Rev->isChecked())
        {
            revData.append("antero lateral territory");
        }
        if(ui->checkBox_apicalRev->isChecked())
        {
            revData.append("apical territory");
        }
        if(ui->checkBox_anteroSepRev->isChecked())
        {
            revData.append("antero septal territory");
        }
        if(ui->checkBox_InferoSepRev->isChecked())
        {
            revData.append("infero septal territory");
        }

    }

    if(ui->radioButton_noRevPer->isChecked())
    {
        revData.append("Without any reversible ischemia");
    }

    //-----------------------------------------------------------------------------------

    if(ui->radioButton_yesFixPer->isChecked())
    {
        fixData.append("fixed perfusion defect in");
        if(ui->checkBox_anteriorTerFix->isChecked())
        {
            if(ui->radioButton_basalAnteriorFix->isChecked())
            {
                fixData.append("basal anterior territory");
            }
            if(ui->radioButton_midAnteriorFix->isChecked())
            {
                fixData.append("mid anterior territory");
            }
            if(ui->radioButton_distaAnteriorlFix->isChecked())
            {
                fixData.append("distal anterior territory");
            }
            if(ui->radioButton_fixAntNone->isChecked())
            {
                fixData.append("anterior territory");
            }
        }

        if(ui->checkBox_inferiorTerFix->isChecked())
        {
            if(ui->radioButton_basalInferiorFix->isChecked())
            {
                fixData.append("basal inferior territory");
            }
            if(ui->radioButton_midInferiorFix->isChecked())
            {
                fixData.append("mid inferior territory");
            }
            if(ui->radioButton_distalInferiorFix->isChecked())
            {
                fixData.append("distal inferior territory");
            }
            if(ui->radioButton_fixInfNone->isChecked())
            {
                fixData.append("inferior territory");
            }
        }

        if(ui->checkBox_inferoLat_Fix->isChecked())
        {
            fixData.append("infero lateral territory");
        }
        if(ui->checkBox_anteroLat_Fix->isChecked())
        {
            fixData.append("antero lateral territory");
        }
        if(ui->checkBox_apicalFix->isChecked())
        {
            fixData.append("apical territory");
        }
        if(ui->checkBox_anteroSepFix->isChecked())
        {
            fixData.append("antero septal territory");
        }
        if(ui->checkBox_InferoSepFix->isChecked())
        {
            fixData.append("infero septal territory");
        }
        if(ui->checkBox_FixDiaph->isChecked())
        {
            fixData.append("likely secondary to diaphragmatic attenuation");
        }
        if(ui->checkBox_FixBreastAtte->isChecked())
        {
            fixData.append("likely secondary to breast attenuation");
        }
        if(ui->checkBox_FixPartial->isChecked())
        {
            fixData.append("likely secondary to partial volume effect");
        }

    }

    if(ui->radioButton_noFixPer->isChecked())
    {
        fixData.append("normal uptake");
    }

    qDebug()<<revData;
    qDebug()<<endl;
    qDebug()<<fixData;

    QString revDataString = revData.join(", ") + ".";
    QString fixDataString = fixData.join(", ") + ".";

    if(revDataString.startsWith("Without any reversible ischemia"))
    {
        stressSpectTomo = QString("SPECT tomographic images showed %1 %2").arg(fixDataString).arg(revDataString);
    }
    else if(revDataString.startsWith("reversible perfusion defect")
            && fixDataString.startsWith("normal uptake"))
    {
        stressSpectTomo = QString("SPECT tomographic images showed %1").arg(revDataString);
    }
    else
    {
        stressSpectTomo = QString("SPECT tomographic images showed %1 %2").arg(revDataString).arg(fixDataString);
    }

    this->stressSpectTomo = stressSpectTomo;
    qDebug()<<stressSpectTomo;
    qDebug()<<this->editString+" :Updated 2";

    if(editString.isEmpty() || ui->plainTextEdit_spectTomo->toPlainText().isEmpty())
    {
        generatePdf(writer,painter,normalFont,stressSpectTomo,currentX,1,1);
    }
    else
    {
        generatePdf(writer,painter,normalFont,this->editString,currentX,1,1);
    }

    QString TidRatio = (ui->radioButton_TIDnormal->isChecked())?"normal at ":"abnormal at ";
    QString rvIntake = (ui->radioButton_RVnormal->isChecked())?"No abnormal increase":"There is abnormal increase";
    QString TID = QString("TID ratio was %1%2. %3 in RV uptake seen.").arg(TidRatio).arg(ui->lineEdit_TID->text())
            .arg(rvIntake);

    generatePdf(writer,painter,normalFont,TID,currentX,1,1);

    //Tab 5 data
    //^^^^^^^^^^
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    dynamic500Width = static_cast<int>(pageWidth * (500.0 / pageWidth));
    dynamic1000Width = static_cast<int>(pageWidth * (1000.0 / pageWidth));
    generatePdf(writer,painter,boldFont,"2.Resting Images :",stationaryX+dynamic500Width,1,1);
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    generatePdf(writer,painter,boldFont,"a.SPECT Gated Images - ",stationaryX+dynamic1000Width,1,1);

    QString EF = ui->lineEdit_SpectEF->text();

    QString restSpect = QString("SPECT gated images showed visually estimated EF of %1%.").arg(EF);
    generatePdf(writer,painter,normalFont,restSpect,currentX,1,1);

    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    generatePdf(writer,painter,boldFont,"b.SPECT Tomographic Images - ",stationaryX+dynamic1000Width,1,1);

    if(editString.isEmpty() || ui->plainTextEdit_spectTomo->toPlainText().isEmpty())
    {
        // Process `stressSpectTomo` to exclude the text after "without"
        QString modifiedString = stressSpectTomo;
        int index = modifiedString.indexOf("Without");
        if(index != -1)
        {
            modifiedString = modifiedString.left(index).trimmed();
        }

        if(modifiedString.contains("SPECT tomographic images showed reversible perfusion defect") &&
                !modifiedString.contains("fixed perfusion defect"))
        {
            generatePdf(writer, painter, normalFont,"SPECT tomographic images showed normal uptake.", currentX, 1, 1);
        }
        else if (modifiedString.contains("fixed perfusion defect"))
        {
            // Find the position of "fixed perfusion defect"
            int index = modifiedString.indexOf("fixed perfusion defect");

            // Extract the part after "fixed perfusion defect"
            QString afterFixedPerfusionDefect = modifiedString.mid(index);

            // Concatenate the desired starting text with the extracted part
            QString result = QString("SPECT tomographic images showed %1").arg(afterFixedPerfusionDefect);

            // Print the result
            qDebug() << result <<"rare condition";
            generatePdf(writer, painter, normalFont,result, currentX, 1, 1);
        }
        else
        {
            generatePdf(writer, painter, normalFont, modifiedString, currentX, 1, 1);
        }
        qDebug()<<modifiedString<<" :modifiedString";
    }
    else
    {
        // Process `this->editString` to exclude the text after "without"
        QString modifiedEditString = this->editString;
        int index = modifiedEditString.indexOf("without", 0, Qt::CaseInsensitive);
        if(index != -1)
        {
            modifiedEditString = modifiedEditString.left(index).trimmed();
        }

        if(modifiedEditString.contains("SPECT tomographic images showed reversible perfusion defect") &&
                !modifiedEditString.contains("fixed perfusion defect"))
        {
            generatePdf(writer, painter, normalFont,"SPECT tomographic images showed normal uptake.", currentX, 1, 1);
        }
        else if (modifiedEditString.contains("fixed perfusion defect"))
        {
            // Find the position of "fixed perfusion defect"
            int index = modifiedEditString.indexOf("fixed perfusion defect");

            // Extract the part after "fixed perfusion defect"
            QString afterFixedPerfusionDefect = modifiedEditString.mid(index);

            // Concatenate the desired starting text with the extracted part
            QString result = QString("SPECT tomographic images showed %1").arg(afterFixedPerfusionDefect);

            // Print the result
            qDebug() << result <<"rare condition";
            generatePdf(writer, painter, normalFont,result, currentX, 1, 1);
        }
        else
        {
            generatePdf(writer, painter, normalFont, modifiedEditString, currentX, 1, 1);
        }
        qDebug()<<modifiedEditString<<" :modifiedEditString";

    }


    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    generatePdf(writer,painter,boldFont,"Conclusion : ",stationaryX,1,1);

    QString options,LVOptions,spectConclusion,additionalNote,recommend;

    additionalNote = ui->lineEdit_additionalNote->text();

    if(additionalNote.isEmpty())
    {
        additionalNote = "";
    }
    else
    {
         additionalNote.append(",");
    }

    if (ui->radioButton_ConNormal->isChecked()) {
        options = ui->radioButton_ConNormal->text();
    } else if (ui->radioButton_ConProNor->isChecked()) {
        options = ui->radioButton_ConProNor->text();
    } else if (ui->radioButton_ConMilAbn->isChecked()) {
        options = ui->radioButton_ConMilAbn->text();
    } else if (ui->radioButton_ConAbn->isChecked()) {
        options = ui->radioButton_ConAbn->text();
    }

    LVOptions = (ui->radioButton_LVnor->isChecked())?"normal":"abnormal";

    if(editString.isEmpty() || ui->plainTextEdit_spectTomo->toPlainText().isEmpty())
    {
        // Remove "SPECT tomographic images showed" from stressSpectTomo if it exists
        QString modifiedString = stressSpectTomo;
        modifiedString.remove("SPECT tomographic images showed", Qt::CaseInsensitive);

        spectConclusion = modifiedString;
    }
    else
    {
        // Remove "SPECT tomographic images showed" from editString if it exists
        QString modifiedEditString = this->editString;
        modifiedEditString.remove("SPECT tomographic images showed", Qt::CaseInsensitive);

        spectConclusion = modifiedEditString;
    }

    recommend = (ui->radioButton_yesRecommend->isChecked())?"Recommend clinical correlation.":"";

    spectConclusion.chop(1);  // Remove the last character from spectConclusion

    QString rvConclusion = (ui->radioButton_RVnormal->isChecked())?"without any":"with";

    QString conclusion = QString("This is a %1 study showing %2 LV systolic function,"
                                 " %3, %4 %5 TID ratio %6 abnormal increase in RV uptake. %7")
            .arg(options)
            .arg(LVOptions)
            .arg(spectConclusion)
            .arg(additionalNote)
            .arg(TidRatio.remove(" at"))
            .arg(rvConclusion)
            .arg(recommend);

    qDebug()<<conclusion;

    generatePdf(writer, painter, normalFont, conclusion, currentX, 1, 1);

    if(MAPHR.toFloat() < 85 && ui->radioButton_exercise->isChecked())
    {
        generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
        generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
        QString specialString = "This is technically a non-diagnostic study due to inability to achieve 85% MAPHR.";
        generatePdf(writer, painter, normalFont,specialString , stationaryX, 1, 1);

    }

    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax

    generatePdf(writer,painter,boldFont,"Nitin Mahajan MD, MPH",stationaryX,1,1);
    generatePdf(writer,painter,normalFont,"",currentX,0,0); //New line syntax
    QString currentDate = ui->dateEdit_currentDate->date().toString("MM/dd/yyyy");
    generatePdf(writer,painter,boldFont,currentDate,stationaryX,1,1);

    // Footer text to be printed at the bottom of the page
    QString footerText = "Ps: Stress tests do not have 100% sensitivity and "
                         "specificity for excluding significant CAD, and do not "
                         "exclude the possibility of MI.";

    // Print the footer at the bottom of the page or on a new page
    //'true' to indicate it's a footer
    generatePdf(writer, painter, normalFont, footerText, currentX, 1, 1, true);

    painter.end();


    QString docxPath = fileName.endsWith(".pdf") ? fileName.left(fileName.length() - 4) + ".docx" : fileName + ".docx";
    qDebug()<<fileName<<"  "<<docxPath;

    if(notCreateWord == true)
    {
        convertPdfToWord(fileName, docxPath);
    }
}


void Health::on_actionExit_triggered()
{
    qApp->quit();
}

void Health::printPageNumber(QPdfWriter &pdfWriter, QPainter &painter, int pageNumber)
{
    // Define a font for the page number (e.g., Arial, size 10, non-bold)
    QFont pageNumberFont("Arial", 10);
    painter.setFont(pageNumberFont);

    // Get page dimensions
    int pageWidth = pdfWriter.width();
    int pageHeight = pdfWriter.height();

    // Draw the page number centered at the bottom of the page
    QString pageNumberText = QString("Page %1").arg(pageNumber);
    int dynamic50Width = static_cast<int>(pageWidth * (50.0 / pageWidth));
    painter.drawText(pageWidth / 2, pageHeight - dynamic50Width, pageNumberText);
}

void Health::generatePdf(QPdfWriter &pdfWriter, QPainter &painter, QFont getFont,
                         const QString &text, int getX, int noHeight, bool sameLineEdit,
                         bool isFooter)
{

    // Set getX as currentX
    currentX = getX;

    // Set font for the painter (main content)
    painter.setFont(getFont);

    // Get page dimensions for dynamic calculations
    int pageWidth = pdfWriter.width();
    int pageHeight = pdfWriter.height();

    // Calculate 95% of the page height for pagination trigger
    int maxHeight = static_cast<int>(pageHeight * 0.95);

    // Calculate 95% of the page width for text wrapping
    int maxWidth = static_cast<int>(pageWidth * 0.95);
    int initialStartX = static_cast<int>(pageWidth * 0.04); // 4% of page width as the starting X

    // Set the starting x position
    int x = sameLineEdit ? currentX : initialStartX;  // Use currentX if continuing on the same line
    QFontMetrics metrics(getFont);  // Get font metrics for the content font

    QStringList words = text.split(' ', QString::SkipEmptyParts);  // Split the text into words

    int dynamic1000Width = static_cast<int>(pageWidth * (1000.0 / pageWidth));
    int dynamic500Width = static_cast<int>(pageWidth * (500.0 / pageWidth));
    int dynamic200Width = static_cast<int>(pageWidth * (200.0 / pageWidth));
    int dynamic400Width = static_cast<int>(pageWidth * (400.0 / pageWidth));
    int dynamic300Width = static_cast<int>(pageWidth * (300.0 / pageWidth));



    for (const QString& word : words)
    {
        int wordWidth = metrics.horizontalAdvance(word + " ");  // Get the width of the word plus space

        // Check if the word exceeds the line width
        if (x + wordWidth > maxWidth)
        {
            currentY += lineHeight;  // Move to the next line
            //special cases-------------------------
            if(text.startsWith("SPECT gated images") || text.startsWith("SPECT tomographic images showed") ||
                    text.startsWith("TID ratio was"))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic1000Width;
            }
            else if(text.startsWith("1.Stress Images :"))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic500Width;
            }
            else if(text.startsWith("a.SPECT Gated Images - "))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic1000Width;
            }
            else if(text.startsWith("b.SPECT Tomographic Images - "))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic1000Width;
            }
            else if(text.startsWith("2.Resting Images :"))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic500Width;
            }
            else if(text.startsWith("a.SPECT Gated Images - "))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic1000Width;
            }
            else if(text.startsWith("b.SPECT Tomographic Images - "))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic1000Width;
            }
            else
            {
                x = initialStartX;       // Reset x for new line
            }
        }

        // Now check if we've exceeded the page height and need a new page
        if (currentY >= maxHeight)
        {
            // Before creating a new page, print the page number
            printPageNumber(pdfWriter, painter, pageNumber);

            // Create a new page
            pdfWriter.newPage();
            currentY = dynamic200Width;       // Reset Y position for new page

            //special cases-------------------------
            if(text.startsWith("SPECT gated images") || text.startsWith("SPECT tomographic images showed") ||
                    text.startsWith("TID ratio was"))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic1000Width;
            }
            else if(text.startsWith("1.Stress Images :"))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic500Width;
            }
            else if(text.startsWith("a.SPECT Gated Images - "))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic1000Width;
            }
            else if(text.startsWith("b.SPECT Tomographic Images - "))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic1000Width;
            }
            else if(text.startsWith("2.Resting Images :"))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic500Width;
            }
            else if(text.startsWith("a.SPECT Gated Images - "))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic1000Width;
            }
            else if(text.startsWith("b.SPECT Tomographic Images - "))
            {
                x = static_cast<int>(pdfWriter.width() * 0.04) + dynamic1000Width;
            }
            else
            {
                x = initialStartX;       // Reset x for new line
            }

            pageNumber++;         // Increment page number for the new page

            // Restore the main content font after printing the page number
            painter.setFont(getFont);
        }

        // Draw the text using the updated x and currentY values
        if(isFooter == false)
        {
            painter.drawText(x, currentY, word);
        }

        // Move X for the next word
        x += wordWidth;

        // Update global currentX for the next function call
        currentX = x;
    }

    // Update currentY based on noHeight
    if (noHeight != 1)
    {
        currentY += lineHeight;  // Move Y to the next line if noHeight is not 1
    }

    //###############       FOOTER      ###################//
    // If it's the last content and we're printing a footer, check if there's enough space for the footer
    if (isFooter) {
        // Calculate the height needed for two lines of the footer
        int footerHeight = metrics.height() * 2;  // Space for two lines

        // Check if there is enough space for the entire footer on the current page
        if (currentY + footerHeight + dynamic400Width > maxHeight) {
            // Print the page number before creating a new page
            printPageNumber(pdfWriter, painter, pageNumber);



            // Create a new page for the footer
            pdfWriter.newPage();
            currentY = dynamic200Width;  // Reset Y position for the new page
            pageNumber++;    // Increment page number
        }

        // Set the Y position to the bottom of the page, leaving room for the footer
        currentY = pageHeight - footerHeight - dynamic300Width;  // Adjust for footer positioning

        // Use a smaller font for the footer if needed
        QFont footerFont = getFont;
        footerFont.setPointSize(11);  // Use the desired font size
        painter.setFont(footerFont);

        // Set up text wrapping logic
        QString footerText = text;
        QStringList words = footerText.split(' ', QString::SkipEmptyParts);  // Split the text into words
        int footerX = initialStartX;  // Starting X position
        int lineY = currentY;  // Track the Y position for the line

        // Loop through words and wrap text to fit within the page width
        for (const QString& word : words) {
            int wordWidth = metrics.horizontalAdvance(word + " ");

            // Check if the word exceeds the maxWidth for wrapping
            if (footerX + wordWidth > maxWidth) {
                lineY += metrics.height();  // Move to the next line
                footerX = initialStartX;    // Reset X position for the new line

                // If we've already wrapped once (2nd line), stop further wrapping and break
                if (lineY >= currentY + footerHeight) {
                    break;  // We can only allow two lines
                }
            }

            // Draw the word in the footer
            painter.drawText(footerX, lineY, word);
            footerX += wordWidth;  // Move X for the next word
        }

        // Print the page number at the bottom after the footer
        printPageNumber(pdfWriter, painter, pageNumber);
    }
    //###############       FOOTER      ###################//


    // Debug output
    qDebug() << "Current X: " << currentX;
    qDebug() << "Current Y: " << currentY;
    qDebug() << "-----------------------------------------";

    qDebug()<<pdfWriter.height()<<"  "<<pdfWriter.width()<<" Pdf Max Height and Width";
}




void Health::on_pushButton_preview_clicked()
{
    QDate today = ui->dateEdit_today->date();

    QString ref = ui->lineEdit_ref->text();

    QString sex;
    if(ui->radioButton_m->isChecked())
    {
        sex = ui->radioButton_m->text();
    }
    else if(ui->radioButton_fe->isChecked())
    {
        sex = ui->radioButton_fe->text();
    }
    else
    {
        //do nothing
    }

    QDate dob = ui->dateEdit_dob->date();

    QString pName = ui->lineEdit_pName->text();

    pageNumber = 1;
    qDebug()<<"Preview ----------------------------------------------------";

    notCreateWord = false;

    generatePDF(today,ref,sex,dob,pName);
    ui->plainTextEdit_spectTomo->setPlainText(stressSpectTomo);
    qDebug()<<"---------------------------------------------------------End";
}

void Health::on_pushButton_save_clicked()
{
    editString = ui->plainTextEdit_spectTomo->toPlainText();
    qDebug()<<editString<<" : Updated";
    QMessageBox::information(this,"Update","Updated Successfully");
}

//Python script for converting pdf to word
void Health::convertPdfToWord(const QString &pdfPath, const QString &docxPath) {
    // Create the QProcess object
    QProcess *process = new QProcess(this);  // Using a pointer so it can be deleted later

    // Set up the command
    QString pythonExe = "pdf.exe";
    qDebug() << "Python executable path:" << pythonExe;

    QStringList arguments;
    arguments << pdfPath << docxPath; // Pass PDF and DOCX paths as arguments
    qDebug() << "Arguments:" << arguments;

    // Create and display a progress dialog
    QProgressDialog *progressDialog = new QProgressDialog("Please wait...", nullptr, 0, 0, this);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setCancelButton(nullptr); // Disable cancel button
    progressDialog->show();

    // Connect signals for process finished and error handling
    connect(process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        progressDialog->hide();
        QString output = process->readAllStandardOutput();
        QString errors = process->readAllStandardError();

        qDebug() << "Process output:" << output;
        if (!errors.isEmpty()) {
            qDebug() << "Errors:" << errors;
        }

        if (exitStatus == QProcess::NormalExit && exitCode == 0) {
            QMessageBox::information(this, "Success", "File saved successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Conversion failed.");
        }

        // Clean up process and progress dialog
        process->deleteLater();
        progressDialog->deleteLater();
    });

    connect(process, static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred),
            this, [=](QProcess::ProcessError error) {
        progressDialog->hide();
        qDebug() << "Python process error:" << error;
        QMessageBox::warning(this, "Error", "Conversion process failed with an error.");
        process->deleteLater();
        progressDialog->deleteLater();
    });

    // Start the Python process asynchronously
    process->start(pythonExe, arguments);
}




