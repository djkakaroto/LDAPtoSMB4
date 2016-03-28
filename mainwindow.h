#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ufguser.h"
#include "atributos.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnEscolherLDIF_clicked();

    void on_btnExportarSMB4_clicked();

    void on_btnProcessarLDIF_clicked();

private:
    Ui::MainWindow *ui;
    //QFile *inputFileLDIF;
    QString pathFileLDIF;
    Atributos *listAtributos;
    UFGUser usuario;
    QList <UFGUser> listUsuario;
    int flagEmployee;
    int flagLotacao;

//Metodos
    void inserirDadosTabela( const QList <UFGUser> &value );
    void lerAtributos();
    void filtrarDados(QStringList value);

};

#endif // MAINWINDOW_H
