#include "mainwindow.h"
#include "ui_mainwindow.h"  // Includes the UI file for MainWindow
#include "bstpage.h"
#include "rbtpage.h"
#include "avlpage.h"
#include "phonepage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the button clicks to the slot
    connect(ui->binarysearchtreebutton, &QPushButton::clicked, this, &MainWindow::showbstpage);
    connect(ui->binarysearchtreebutton_2, &QPushButton::clicked, this, &MainWindow::showrbtpage);
    connect(ui->binarysearchtreebutton_3, &QPushButton::clicked, this, &MainWindow::showavlpage);
    connect(ui->binarysearchtreebutton_4, &QPushButton::clicked, this, &MainWindow::showphonepage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showbstpage()
{
    // Create and show an instance of BSTPage
    ui->binarysearchtreebutton->hide();
    ui->binarysearchtreebutton_2->hide();
    ui->binarysearchtreebutton_3->hide();
    bstpage *bstPage = new bstpage(this);
    bstPage->show();
}

void MainWindow::showrbtpage()
{
    // Create and show an instance of RBTPage
    ui->binarysearchtreebutton_2->hide();
    ui->binarysearchtreebutton->hide();
    ui->binarysearchtreebutton_3->hide();
    ui->binarysearchtreebutton_4->hide();
    rbtpage *rbtPage = new rbtpage(this);
    rbtPage->show();
}

void MainWindow::showavlpage()
{
    // Create and show an instance of AVLPage
    ui->binarysearchtreebutton_2->hide();
    ui->binarysearchtreebutton->hide();
    ui->binarysearchtreebutton_3->hide();
    ui->binarysearchtreebutton_4->hide();
    avlpage *avlPage = new avlpage(this);
    avlPage->show();
}

void MainWindow::showphonepage()
{
    // Create and show an instance of PhonePage
    ui->binarysearchtreebutton->hide();
    ui->binarysearchtreebutton_2->hide();
    ui->binarysearchtreebutton_3->hide();
    ui->binarysearchtreebutton_4->hide();
    phonepage *phonePage = new phonepage(this);
    phonePage->show();
}
