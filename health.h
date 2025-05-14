#ifndef HEALTH_H
#define HEALTH_H

#include <QMainWindow>
#include <QScrollArea>
#include <QPixmap>
#include <QPdfWriter>
#include <QPainter>
#include <cmath>
#include <QDebug>
#include <QProcess>
#include <QProgressDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Health; }
QT_END_NAMESPACE

class Health : public QMainWindow
{
    Q_OBJECT

public:
    Health(QWidget *parent = nullptr);
    ~Health();

    void generatePdf(QPdfWriter &pdfWriter, QPainter &painter, QFont getFont,
                             const QString &text, int getX, int noHeight, bool sameLineEdit = false,
                             bool isFooter = false);

    void printPageNumber(QPdfWriter &pdfWriter, QPainter &painter, int pageNumber);

    void convertPdfToWord(const QString &pdfPath, const QString &docxPath);

private slots:
    void on_pushButton_clicked();

    void openLineEditPrior();
    void closeLineEditPrior();

    void openLineEditThread();
    void closeLineEditThread();

    void openLineEditChest();
    void closeLineEditChest();

    void takeLayoutAndbool(QLayout*,bool);
    void enableWidgetsInLayout(QLayout*,bool);

    void onPressRevYes();
    void onPressRevNo();
    void openAnteriorRev();
    void openInferiorRev();

    void onPressFixYes();
    void onPressFixNo();   
    void openAnteriorFix();
    void openInferiorFix();
    void onLexiscanToggled(bool checked);


    void indicationDataReceiver();

    void riskFactorCADdataReceiver();

    void resultsDataReceiver();

    void on_actionExit_triggered();

    void on_pushButton_preview_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::Health *ui;
    void generatePDF(const QDate &today,const QString &ref, const QString &sex,
                     const QDate &dob, const QString &PName);

    QStringList IndicationData;
    QStringList RiskFactorCAD_Data;
    QStringList ResultsData;

    QString stressSpectTomo;
    QString editString;


    int lineHeight;
    int currentY;
    int currentX;
    int pageNumber;

    bool notCreateWord;

};
#endif // HEALTH_H
