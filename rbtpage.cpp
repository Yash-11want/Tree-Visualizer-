#include "rbtpage.h"
#include "ui_rbtpage.h"
#include "mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QHash>
#include <queue>
#include <QPen>

rbtpage::rbtpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rbtpage),
    root(nullptr)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    connect(ui->insertbutton, &QPushButton::clicked, this, [=]() {
        int value = ui->lineEdit->text().toInt();

        if (value != 0) {
            root = insertNode(root, value);
            insertFixup(root);
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

    connect(ui->inorderButton, &QPushButton::clicked, this, &rbtpage::displayInorderTraversal);
    connect(ui->preorderButton, &QPushButton::clicked, this, &rbtpage::displayPreorderTraversal);
    connect(ui->postorderButton, &QPushButton::clicked, this, &rbtpage::displayPostorderTraversal);
    connect(ui->backButton, &QPushButton::clicked, this, &rbtpage::goBack);

}

rbtpage::~rbtpage()
{
    delete ui;
}

RBTreeNode* rbtpage::insertNode(RBTreeNode* root, int value) {

    RBTreeNode* newNode = new RBTreeNode(value);


    RBTreeNode* y = nullptr;
    RBTreeNode* x = root;
    while (x != nullptr) {
        y = x;
        if (value < x->value) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    newNode->parent = y;
    if (y == nullptr) {
        root = newNode;
    } else if (value < y->value) {
        y->left = newNode;
    } else {
        y->right = newNode;
    }

    newNode->color = RED;
    insertFixup(newNode);

    return root;
}



void rbtpage::insertFixup(RBTreeNode* node) {
    while (node != root && node->parent && node->parent->color == RED) {

        if (!node->parent->parent) {
            break;
        }


        RBTreeNode* grandparent = node->parent->parent;

        if (node->parent == grandparent->left) {

            RBTreeNode* uncle = grandparent->right;

            if (uncle && uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }
            else{

                if (node == node->parent->right) {
                    node = node->parent;
                    leftRotate(node);
                }

                else{
                    node->parent->color = BLACK;
                    grandparent->color = RED;
                    rightRotate(grandparent);
                }
            }
        }

        else{


            RBTreeNode* uncle = grandparent->left;

            if (uncle && uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }

            else{

                if (node == node->parent->left) {
                    node = node->parent;
                    rightRotate(node);
                }

                else{
                    node->parent->color = BLACK;
                    grandparent->color = RED;
                    leftRotate(grandparent);
                }
            }
        }
    }
    if (root){
        root->color = BLACK;
    }
}


void rbtpage::leftRotate(RBTreeNode* x) {

    RBTreeNode* y = x->right;
    x->right = y->left;

    if (y->left != nullptr) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}


void rbtpage::rightRotate(RBTreeNode* y) {
    RBTreeNode* x = y->left;
    y->left = x->right;
    if (x->right != nullptr) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == nullptr) {
        root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

RBTreeNode* rbtpage::deleteNode(RBTreeNode* root, int value) {
    RBTreeNode* node = root;
    RBTreeNode* target = nullptr;
    RBTreeNode* child = nullptr;
    RBTreeNode* temp = nullptr;
    bool originalColor;

    while (node != nullptr) {
        if (value == node->value) {
            target = node;
            break;
        } else if (value < node->value) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    if (target == nullptr) {
        return root;
    }

    originalColor = target->color;
    if (target->left == nullptr) {
        child = target->right;
        transplant(root, target, target->right);
    } else if (target->right == nullptr) {
        child = target->left;
        transplant(root, target, target->left);
    } else {
        temp = minimum(target->right);
        originalColor = temp->color;
        child = temp->right;
        if (temp->parent == target) {
            if (child) {
                child->parent = temp;
            }
        } else {
            transplant(root, temp, temp->right);
            temp->right = target->right;
            temp->right->parent = temp;
        }
        transplant(root, target, temp);
        temp->left = target->left;
        temp->left->parent = temp;
        temp->color = target->color;
    }

    delete target;

    if (originalColor == BLACK) {
        deleteFixup(root, child);
    }

    return root;
}

void rbtpage::transplant(RBTreeNode*& root, RBTreeNode* u, RBTreeNode* v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

RBTreeNode* rbtpage::minimum(RBTreeNode* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

void rbtpage::deleteFixup(RBTreeNode*& root, RBTreeNode* node) {
    while (node != root && (node == nullptr || node->color == BLACK)) {
        if (node == node->parent->left) {
            RBTreeNode* sibling = node->parent->right;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                leftRotate(node->parent);
                sibling = node->parent->right;
            }
            if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                (sibling->right == nullptr || sibling->right->color == BLACK)) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling->right == nullptr || sibling->right->color == BLACK) {
                    if (sibling->left != nullptr) {
                        sibling->left->color = BLACK;
                    }
                    sibling->color = RED;
                    rightRotate(sibling);
                    sibling = node->parent->right;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                if (sibling->right != nullptr) {
                    sibling->right->color = BLACK;
                }
                leftRotate(node->parent);
                node = root;
            }
        } else {
            RBTreeNode* sibling = node->parent->left;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rightRotate(node->parent);
                sibling = node->parent->left;
            }
            if ((sibling->right == nullptr || sibling->right->color == BLACK) &&
                (sibling->left == nullptr || sibling->left->color == BLACK)) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling->left == nullptr || sibling->left->color == BLACK) {
                    if (sibling->right != nullptr) {
                        sibling->right->color = BLACK;
                    }
                    sibling->color = RED;
                    leftRotate(sibling);
                    sibling = node->parent->left;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                if (sibling->left != nullptr) {
                    sibling->left->color = BLACK;
                }
                rightRotate(node->parent);
                node = root;
            }
        }
    }
    if (node != nullptr) {
        node->color = BLACK;
    }
}


RBTreeNode* rbtpage::findNode(RBTreeNode* root, int value) {
    while (root != nullptr && root->value != value) {
        if (value < root->value) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return root;
}



void rbtpage::drawTree() {
    ui->graphicsView->scene()->clear();

    if (root != nullptr) {
        drawNode(root, 400, 30, 200);
    }
}

void rbtpage::drawNode(RBTreeNode* node, int x, int y, int xOffset) {
    if (node == nullptr) return;


    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(x-15, y-15, 30, 30);
    circle->setBrush(node->color == RED ? Qt::red : Qt::black);


    QGraphicsTextItem* textItem = new QGraphicsTextItem(QString::number(node->value), circle);
    textItem->setDefaultTextColor(Qt::white);
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


void rbtpage::updateTraversalDisplay(const QString &text) {
    ui->traversalLabel->setText(text);
}


void rbtpage::displayInorderTraversal() {
    QString result;
    inorderTraversal(root, result);
    updateTraversalDisplay("Inorder Traversal:\n" + result);
}

void rbtpage::displayPreorderTraversal() {
    QString result;
    preorderTraversal(root, result);
    updateTraversalDisplay("Preorder Traversal:\n" + result);
}

void rbtpage::displayPostorderTraversal() {
    QString result;
    postorderTraversal(root, result);
    updateTraversalDisplay("Postorder Traversal:\n" + result);
}


void rbtpage::inorderTraversal(RBTreeNode* node, QString &result) {
    if (node == nullptr) return;
    inorderTraversal(node->left, result);
    result += QString::number(node->value) + " ";
    inorderTraversal(node->right, result);
}

void rbtpage::preorderTraversal(RBTreeNode* node, QString &result) {
    if (node == nullptr) return;
    result += QString::number(node->value) + " ";
    preorderTraversal(node->left, result);
    preorderTraversal(node->right, result);
}

void rbtpage::postorderTraversal(RBTreeNode* node, QString &result) {
    if (node == nullptr) return;
    postorderTraversal(node->left, result);
    postorderTraversal(node->right, result);
    result += QString::number(node->value) + " ";
}


void rbtpage::goBack() {
    this->close();

    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
}
