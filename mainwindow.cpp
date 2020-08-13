#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::requestFromServer()
{
    QNetworkRequest networkRequest;
    QByteArray postData;
    QString url_ws;

    url_ws=ui->lineEdit->text(); // request data from server
    networkRequest.setUrl(QUrl(url_ws));
    networkReply = networkManager.post(networkRequest,postData);
    connect(networkReply,SIGNAL(finished()),this,SLOT(receiveFromServer())); // execute this slot when finished receive data
}

void MainWindow::receiveFromServer()
{
    QString qString;
    std::string stdString;
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;
    QStringList stringList,jsonKeys;
    QByteArray postData;
    int j,fieldCount;

    // first part
    postData=networkReply->readAll(); // put received data into QByteArray
    stdString=postData.toStdString(); // copy to std string
    qString=QString::fromStdString(stdString); // finally to a QString
    ui->plainTextEdit->setPlainText(qString); // dump in a plain text

    // second part
    jsonDocument = QJsonDocument::fromJson(postData); // always put in a QJsonDocument
    jsonObject = jsonDocument.object(); // parse to QJsonObject

    ui->tableWidget->setColumnCount(2); // we need to columns
    stringList << "FIELD" << "VALUE"; // column headers
    ui->tableWidget->setHorizontalHeaderLabels(stringList); // set the column headers
    ui->tableWidget->setColumnWidth(0,200); // width of first column
    ui->tableWidget->setColumnWidth(1,200); // width of second column

    // third part
    jsonKeys=jsonObject.keys(); // dump the field names in a QStringList
    fieldCount=jsonKeys.count(); // get the field count
    ui->tableWidget->setRowCount(fieldCount); // set the row count
    for (j=0; j<fieldCount; j++)
    {   // set the string fields in the first column (index 0)
        ui->tableWidget->setItem(j, 0, new QTableWidgetItem(jsonKeys[j]));
    }

    // fourth part
    fieldCount=jsonObject.length(); // field count again (is not necesary)
    for (j=0; j<fieldCount; j++)
    {   // set the values in the second column (index 1)
        qString=jsonObject.value(jsonKeys[j]).toString();
        ui->tableWidget->setItem(j, 1, new QTableWidgetItem(qString));
    }
}

void MainWindow::on_pushButton_clicked()
{
    requestFromServer();
}
