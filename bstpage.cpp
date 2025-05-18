#include "bstpage.h"
#include "mainwindow.h"
#include "ui_bstpage.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QHash>
#include <queue>
#include <QPropertyAnimation>

bstpage::bstpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::bstpage),
    root(nullptr)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText("Enter number");
    ui->traversalLabel->setText("Traversal");

    ui->traversalLabel->setWordWrap(true);
    ui->traversalLabel->setMaximumWidth(ui->scrollArea->width());

    ui->scrollArea->setWidgetResizable(true);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    connect(ui->insertbutton, &QPushButton::clicked, this, [=]() {
        long long int value = ui->lineEdit->text().toLongLong();
        root = insertNode(root, value);
        ui->lineEdit->clear();
        drawTree();
    });


    connect(ui->deletebutton, &QPushButton::clicked, this, [=]() {
        int value = ui->lineEdit->text().toInt();
        root = deleteNode(root, value);
        ui->lineEdit->clear();
        drawTree();
    });

    connect(ui->preorderButton, &QPushButton::clicked, this, &bstpage::displayPreorder);
    connect(ui->inorderButton, &QPushButton::clicked, this, &bstpage::displayInorder);
    connect(ui->postorderButton, &QPushButton::clicked, this, &bstpage::displayPostorder);
    connect(ui->backButton, &QPushButton::clicked, this, &bstpage::goBack);

}

bstpage::~bstpage()
{
    delete ui;
}

TreeNode* bstpage::insertNode(TreeNode* node, long long int value) {
    if (node == nullptr) {
        return new TreeNode(value);
    }
    if (value < node->value) {
        node->left = insertNode(node->left, value);
    } else if (value > node->value) {
        node->right = insertNode(node->right, value);
    }
    return node;
}

TreeNode* bstpage::deleteNode(TreeNode* node, int value) {
    if (node == nullptr) return nullptr;

    if (value < node->value) {
        node->left = deleteNode(node->left, value);
    } else if (value > node->value) {
        node->right = deleteNode(node->right, value);
    } else {


        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }

        else if (node->left == nullptr) {
            TreeNode* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            TreeNode* temp = node->left;
            delete node;
            return temp;
        }


        else {

            TreeNode* minNode = node->right;
            while (minNode->left != nullptr) {
                minNode = minNode->left;
            }
            node->value = minNode->value;
            node->right = deleteNode(node->right, minNode->value);
        }
    }
    return node;
}

void bstpage::drawTree() {

    ui->graphicsView->scene()->clear();


    if (root != nullptr) {
        drawNode(root, 400, 30, 200);
    }
}

void bstpage::drawNode(TreeNode* node, int x, int y, int xOffset) {
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

QString bstpage::preOrder(TreeNode* node) {
    if (!node) return "";
    return QString::number(node->value) + " " + preOrder(node->left) + preOrder(node->right);
}

QString bstpage::inOrder(TreeNode* node) {
    if (!node) return "";
    return inOrder(node->left) + QString::number(node->value) + " " + inOrder(node->right);
}

QString bstpage::postOrder(TreeNode* node) {
    if (!node) return "";
    return postOrder(node->left) + postOrder(node->right) + QString::number(node->value) + " ";
}

void bstpage::displayPreorder() {
    QString result = preOrder(root);
    ui->traversalLabel->setText("Pre-Order: " + result);
}

void bstpage::displayInorder() {
    QString result = inOrder(root);
    ui->traversalLabel->setText("In-Order: " + result);
}

void bstpage::displayPostorder() {
    QString result = postOrder(root);
    ui->traversalLabel->setText("Post-Order: " + result);
}

void bstpage::goBack() {
    this->close();
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
}
