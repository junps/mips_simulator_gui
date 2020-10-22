#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();
    void displayFile(QString file_name);

    void heigh_light_row(int row);

private:
    Ui::Form *ui;
};

#endif // FORM_H
