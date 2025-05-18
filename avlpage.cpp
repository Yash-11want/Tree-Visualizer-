#include "avlpage.h"
#include "ui_avlpage.h"
// #include "mainwindow.h"
#include "mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QPen>

avlpage::avlpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::avlpage),
    root(nullptr)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    connect(ui->insertbutton, &QPushButton::clicked, this, [=]() {
        int value = ui->lineEdit->text().toInt();
        if (value != 0) {
            root = insertNode(root, value);
            drawTree();
        }
        ui->lineEdit->clear();
    });

    connect(ui->deletebutton, &QPushButton::clicked, this, [=]() {
        int value = ui->lineEdit->text().toInt();
        if (value != 0) {
            root = deleteNode(root, value);
            drawTree();
        }
        ui->lineEdit->clear();
    });

    connect(ui->inorderButton, &QPushButton::clicked, this, &avlpage::displayInorderTraversal);
    connect(ui->preorderButton, &QPushButton::clicked, this, &avlpage::displayPreorderTraversal);
    connect(ui->postorderButton, &QPushButton::clicked, this, &avlpage::displayPostorderTraversal);

    connect(ui->backButton, &QPushButton::clicked, this, &avlpage::goBack);

}

avlpage::~avlpage()
{
    delete ui;
}

AVLTreeNode* avlpage::insertNode(AVLTreeNode* node, int value) {
    if (node == nullptr) {
        return new AVLTreeNode(value);
    }

    if (value < node->value) {
        node->left = insertNode(node->left, value);
    } else if (value > node->value) {
        node->right = insertNode(node->right, value);
    } else {
        return node;
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = getBalanceFactor(node);

    if (balance > 1 && value < node->left->value) {
        return rightRotate(node);
    }
    if (balance < -1 && value > node->right->value) {
        return leftRotate(node);
    }
    if (balance > 1 && value > node->left->value) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && value < node->right->value) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

AVLTreeNode* avlpage::deleteNode(AVLTreeNode* root, int value) {
    if (root == nullptr) {
        return root;
    }

    if (value < root->value) {
        root->left = deleteNode(root->left, value);
    } else if (value > root->value) {
        root->right = deleteNode(root->right, value);
    } else {
        if (root->left == nullptr || root->right == nullptr) {
            AVLTreeNode* temp = root->left ? root->left : root->right;

            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp;
            }
            delete temp;
        } else {
            AVLTreeNode* temp = minValueNode(root->right);
            root->value = temp->value;
            root->right = deleteNode(root->right, temp->value);
        }
    }

    if (root == nullptr) {
        return root;
    }

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getBalanceFactor(root);

    if (balance > 1 && getBalanceFactor(root->left) >= 0) {
        return rightRotate(root);
    }
    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalanceFactor(root->right) <= 0) {
        return leftRotate(root);
    }
    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

int avlpage::getHeight(AVLTreeNode* node) {
    return node ? node->height : 0;
}

int avlpage::getBalanceFactor(AVLTreeNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

AVLTreeNode* avlpage::rightRotate(AVLTreeNode* y) {
    AVLTreeNode* x = y->left;
    AVLTreeNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

AVLTreeNode* avlpage::leftRotate(AVLTreeNode* x) {
    AVLTreeNode* y = x->right;
    AVLTreeNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

int avlpage::max(int a, int b) {
    return (a > b) ? a : b;
}

AVLTreeNode* avlpage::minValueNode(AVLTreeNode* node) {
    AVLTreeNode* current = node;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

void avlpage::drawTree() {
    ui->graphicsView->scene()->clear();
    if (root != nullptr) {
        drawNode(root, 400, 30, 200);
    }
}

void avlpage::drawNode(AVLTreeNode* node, int x, int y, int xOffset) {
    if (node == nullptr) return;

    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(x - 15, y - 15, 30, 30);
    circle->setBrush(Qt::green);

    QGraphicsTextItem* textItem = new QGraphicsTextItem(QString::number(node->value), circle);
    textItem->setDefaultTextColor(Qt::white);
    textItem->setPos(x - 7, y - 10);

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


void avlpage::updateTraversalDisplay(const QString &text) {
    ui->traversalLabel->setText(text);
}


void avlpage::displayInorderTraversal() {
    QString result;
    inorderTraversal(root, result);
    updateTraversalDisplay("Inorder Traversal:\n" + result);
}

void avlpage::displayPreorderTraversal() {
    QString result;
    preorderTraversal(root, result);
    updateTraversalDisplay("Preorder Traversal:\n" + result);
}

void avlpage::displayPostorderTraversal() {
    QString result;
    postorderTraversal(root, result);
    updateTraversalDisplay("Postorder Traversal:\n" + result);
}


void avlpage::inorderTraversal(AVLTreeNode* node, QString &result) {
    if (node == nullptr) return;
    inorderTraversal(node->left, result);
    result += QString::number(node->value) + " ";
    inorderTraversal(node->right, result);
}

void avlpage::preorderTraversal(AVLTreeNode* node, QString &result) {
    if (node == nullptr) return;
    result += QString::number(node->value) + " ";
    preorderTraversal(node->left, result);
    preorderTraversal(node->right, result);
}

void avlpage::postorderTraversal(AVLTreeNode* node, QString &result) {
    if (node == nullptr) return;
    postorderTraversal(node->left, result);
    postorderTraversal(node->right, result);
    result += QString::number(node->value) + " ";
}

void avlpage::goBack() {
    this->close();

    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
}

