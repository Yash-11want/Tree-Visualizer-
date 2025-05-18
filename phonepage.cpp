#include "phonepage.h"
#include "mainwindow.h"
#include "ui_phonepage.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QHash>
#include <queue>

phonepage::phonepage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::phonepage),
    root(nullptr)
{
    ui->setupUi(this);

    ui->lineEdit->setPlaceholderText("Enter your contact");
    ui->lineEdit_2->setPlaceholderText("Enter your name");
    ui->lineEdit_3->setPlaceholderText("Enter your email");

    ui->graphicsView->setVisible(false);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    connect(ui->insertbutton, &QPushButton::clicked, this, [=]() {
        long long int value = ui->lineEdit->text().toLongLong();
        std::string nm = ui->lineEdit_2->text().toStdString();
        std::string em = ui->lineEdit_3->text().toStdString();
        root = insertNode(root, value,nm,em);
        addContactToList(value, nm);
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        drawTree();
    });

    connect(ui->deletebutton, &QPushButton::clicked, this, [=]() {
        long long int value = ui->lineEdit->text().toLongLong();
        root = deleteNode(root, value);
        ui->lineEdit->clear();
        removeContactFromList(value);
        drawTree();
    });


    connect(ui->searchbutton, &QPushButton::clicked, this, [=]() {
        long long int phoneNumber = ui->lineEdit->text().toLongLong();
        std::string result = searchContact(phoneNumber);
        ui->label->setText(QString::fromStdString(result));
        ui->lineEdit->clear();
        ui->treeVisibilityCheckbox->setChecked(true);
    });

    connect(ui->searchByNameButton, &QPushButton::clicked, this, [=]() {
        QString name = ui->lineEdit_2->text();
        PTreeNode* result = searchByName(root, name);




        if (result) {

            QString details = QString("<b>Name:</b>  %1 <br><b></b> <br><b>Phone:</b> %2 <br><b></b> <br><b>Email:</b> %3")
                                  .arg(QString::fromStdString(result->name))
                                  .arg(result->value)
                                  .arg(QString::fromStdString(result->email));

            ui->graphicsView->scene()->clear();
            QGraphicsTextItem* textItem = new QGraphicsTextItem(details);
            textItem->setDefaultTextColor(Qt::black);
            textItem->setPos(50, 50);

            textItem->setHtml(details);
            textItem->setDefaultTextColor(Qt::darkBlue);
            QFont font("Arial", 20, QFont::Bold);
            textItem->setFont(font);
            textItem->setTextWidth(280);
            textItem->setPos(60, 60);

            QGraphicsRectItem* background = new QGraphicsRectItem(0, 0, 300, 150);
            background->setBrush(QColor(230, 230, 250));
            background->setPen(QPen(Qt::darkBlue, 2));
            background->setRect(50, 50, 300, 150);
            background->setTransformOriginPoint(background->boundingRect().center());


            ui->graphicsView->scene()->addItem(background);
            ui->graphicsView->scene()->addItem(textItem);

            ui->treeVisibilityCheckbox->setChecked(true);

        }

        else{
            ui->label->setText("Contact not found");
        }
        ui->lineEdit_2->clear();
    });

    connect(ui->backbutton, &QPushButton::clicked, this, &phonepage::onBackButtonClicked);
    connect(ui->treeVisibilityCheckbox, &QCheckBox::toggled, this, &phonepage::toggleTreeVisibility);
}




phonepage::~phonepage()
{
    delete ui;
}


void phonepage::onBackButtonClicked() {

    this->close();


    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
}

void phonepage::toggleTreeVisibility(bool checked) {

    ui->graphicsView->setVisible(checked);
    ui->contactListWidget->setVisible(!checked);
}

PTreeNode* phonepage::insertNode(PTreeNode* node, long long int value,std::string nm, std::string em) {
    if (node == nullptr) {
        return new PTreeNode(value,nm,em);
    }
    if (value < node->value) {
        node->left = insertNode(node->left, value,nm,em);
    } else if (value > node->value) {
        node->right = insertNode(node->right, value,nm,em);
    }
    return node;
}

PTreeNode* phonepage::deleteNode(PTreeNode* node, long long int value) {
    if (node == nullptr) return nullptr;


    if (value < node->value) {
        node->left = deleteNode(node->left, value);
    } else if (value > node->value) {
        node->right = deleteNode(node->right, value);
    } else {

        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        } else if (node->left == nullptr) {
            PTreeNode* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            PTreeNode* temp = node->left;
            delete node;
            return temp;
        } else {
            PTreeNode* minNode = node->right;
            while (minNode->left != nullptr) {
                minNode = minNode->left;
            }
            node->value = minNode->value;
            node->name = minNode->name;
            node->email = minNode->email;
            node->right = deleteNode(node->right, minNode->value);
        }
    }
    return node;
}


void phonepage::drawTree() {

    ui->graphicsView->scene()->clear();


    if (root != nullptr) {
        drawNode(root, 400, 30, 200);
    }
}

void phonepage::drawNode(PTreeNode* node, int x, int y, int xOffset) {
    if (node == nullptr) return;


    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(x-15, y-15, 30, 30);
    circle->setBrush(Qt::yellow);


    QGraphicsTextItem* textItem = new QGraphicsTextItem(QString::number(node->value), circle);
    textItem->setDefaultTextColor(Qt::black);
    textItem->setPos(x-7,y-10);


    ui->graphicsView->scene()->addItem(circle);
    ui->graphicsView->scene()->addItem(textItem);


    if (node->left != nullptr) {
        drawNode(node->left, x - xOffset, y + 50, xOffset / 2);

        QGraphicsLineItem* line = new QGraphicsLineItem(x, y, x - xOffset, y + 50);
        ui->graphicsView->scene()->addItem(line);
    }

    if (node->right != nullptr) {
        drawNode(node->right, x + xOffset, y + 50, xOffset / 2);

        QGraphicsLineItem* line = new QGraphicsLineItem(x, y, x + xOffset, y + 50);
        ui->graphicsView->scene()->addItem(line);
    }
}

std::string phonepage::searchContact(long long int phoneNumber) {
    PTreeNode* current = root;
    while (current != nullptr) {
        if (phoneNumber == current->value) {

            QString details = QString("<b>Name:</b>  %1 <br><b></b> <br><b>Phone:</b> %2 <br><b></b> <br><b>Email:</b> %3")
            .arg(QString::fromStdString(current->name))
                .arg(current->value)
                .arg(QString::fromStdString(current->email));

            ui->graphicsView->scene()->clear();
            QGraphicsTextItem* textItem = new QGraphicsTextItem(details);

            textItem->setHtml(details);
            textItem->setDefaultTextColor(Qt::darkBlue);
            QFont font("Arial", 20, QFont::Bold);
            textItem->setFont(font);
            textItem->setTextWidth(280);
            textItem->setPos(60, 60);

            QGraphicsRectItem* background = new QGraphicsRectItem(0, 0, 300, 150);
            background->setBrush(QColor(230, 230, 250));
            background->setPen(QPen(Qt::darkBlue, 2));
            background->setRect(50, 50, 300, 150);
            background->setTransformOriginPoint(background->boundingRect().center());

            ui->graphicsView->scene()->addItem(background);
            ui->graphicsView->scene()->addItem(textItem);



            return current->name;
        } else if (phoneNumber < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }


    QString details = QString("%1").arg(QString::fromStdString("Contact not found"));

    ui->graphicsView->scene()->clear();
    QGraphicsTextItem* textItem = new QGraphicsTextItem(details);

    textItem->setHtml(details);
    textItem->setDefaultTextColor(Qt::darkBlue);
    QFont font("Arial", 20, QFont::Bold);
    textItem->setFont(font);
    textItem->setTextWidth(280);
    textItem->setPos(60, 60);

    QGraphicsRectItem* background = new QGraphicsRectItem(0, 0, 300, 150);
    background->setBrush(QColor(230, 230, 250));
    background->setPen(QPen(Qt::darkBlue, 2));
    background->setRect(50, 50, 300, 150);
    background->setTransformOriginPoint(background->boundingRect().center());

    ui->graphicsView->scene()->addItem(background);
    ui->graphicsView->scene()->addItem(textItem);
    return "Contact not found";
}

void phonepage::addContactToList(long long int value, const std::string& name) {

    QFont font("Arial", 20, QFont::Bold);
    QString contactText = QString("%1 - %2").arg(QString::number(value)).arg(QString::fromStdString(name));

    QListWidgetItem* newItem = new QListWidgetItem(contactText, ui->contactListWidget);
    newItem->setFont(font);

    ui->contactListWidget->addItem(newItem);
}

void phonepage::removeContactFromList(long long int value) {

    for (int i = 0; i < ui->contactListWidget->count(); i++) {
        QListWidgetItem* item = ui->contactListWidget->item(i);
        if (item->text().contains(QString::number(value))) {
            delete item;
            break;
        }
    }
}

PTreeNode* phonepage::searchByName(PTreeNode* node, const QString& name) {
    if (node == nullptr) return nullptr;

    if (QString::fromStdString(node->name).compare(name, Qt::CaseInsensitive) ==0){
        return node;
    }

    PTreeNode* foundNode = searchByName(node->left, name);
    if (foundNode == nullptr) {
        foundNode = searchByName(node->right, name);
    }
    return foundNode;
}

