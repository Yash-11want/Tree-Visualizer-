#ifndef PHONEPAGE_H
#define PHONEPAGE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QListWidget>
#include <string>


struct PTreeNode{
    long long int value;
    PTreeNode* left;
    PTreeNode* right;
    std::string name;
    std::string email;

    PTreeNode(long long int val, std::string nm, std::string em): value(val),left(nullptr),right(nullptr),name(nm), email(em){}
};

namespace Ui {
class phonepage;
}

class phonepage : public QWidget
{
    Q_OBJECT

private slots:
    void onBackButtonClicked();
    void toggleTreeVisibility(bool checked);


public:
    explicit phonepage(QWidget *parent = nullptr);
    ~phonepage();


    Ui::phonepage *ui;
    PTreeNode* root;
    // QPushButton *backbutton;

    PTreeNode* insertNode(PTreeNode* node, long long int value, std::string nm, std::string em);
    PTreeNode* deleteNode(PTreeNode* node, long long int value);
    void drawTree(); // Method to draw the tree
    void drawNode(PTreeNode* node, int x, int y, int xOffset);
    std::string searchContact(long long int phoneNumber);
    void addContactToList(long long int value, const std::string& name);
    void removeContactFromList(long long int value);
    PTreeNode* searchByName(PTreeNode* node, const QString& name);
};

#endif // PHONEPAGE_H
