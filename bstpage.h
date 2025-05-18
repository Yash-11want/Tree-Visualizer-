#ifndef BSTPAGE_H
#define BSTPAGE_H

#include <QWidget>
#include <QGraphicsScene>
#include <string>
#include <QLabel>

struct TreeNode{
    long long int value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(long long int val): value(val),left(nullptr),right(nullptr){}
};

namespace Ui {
class bstpage;
}

class bstpage : public QWidget
{
    Q_OBJECT

public:
    explicit bstpage(QWidget *parent = nullptr);
    ~bstpage();


    Ui::bstpage *ui;
    TreeNode* root;
    QLabel* traversalLabel;

    TreeNode* insertNode(TreeNode* node, long long int value);
    TreeNode* deleteNode(TreeNode* node, int value);
    void drawTree();
    void drawNode(TreeNode* node, int x, int y, int xOffset);
    QString preOrder(TreeNode* node);
    QString inOrder(TreeNode* node);
    QString postOrder(TreeNode* node);
    void displayInorder();
    void displayPreorder();
    void displayPostorder();

private slots:
    void goBack();
};

#endif // BSTPAGE_H
