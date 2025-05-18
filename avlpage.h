#ifndef AVLPAGE_H
#define AVLPAGE_H

#include <QWidget>
#include <QGraphicsScene>

struct AVLTreeNode {
    int value;
    AVLTreeNode* left;
    AVLTreeNode* right;
    int height;

    AVLTreeNode(int val)
        : value(val), left(nullptr), right(nullptr), height(1) {}
};

namespace Ui {
class avlpage;
}

class avlpage : public QWidget
{
    Q_OBJECT

public:
    explicit avlpage(QWidget *parent = nullptr);
    ~avlpage();

    Ui::avlpage *ui;
    AVLTreeNode* root;

    AVLTreeNode* insertNode(AVLTreeNode* node, int value);
    AVLTreeNode* deleteNode(AVLTreeNode* node, int value);

    int getHeight(AVLTreeNode* node);
    int getBalanceFactor(AVLTreeNode* node);
    AVLTreeNode* leftRotate(AVLTreeNode* x);
    AVLTreeNode* rightRotate(AVLTreeNode* y);
    int max(int a, int b);
    AVLTreeNode* minValueNode(AVLTreeNode* node);

    void drawTree();
    void drawNode(AVLTreeNode* node, int x, int y, int xOffset);

private slots:
    void displayInorderTraversal();
    void displayPreorderTraversal();
    void displayPostorderTraversal();
    void goBack();

private:
    void inorderTraversal(AVLTreeNode* node, QString &result);
    void preorderTraversal(AVLTreeNode* node, QString &result);
    void postorderTraversal(AVLTreeNode* node, QString &result);
    void updateTraversalDisplay(const QString &text);
};

#endif // AVLPAGE_H
