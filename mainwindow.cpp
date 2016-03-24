#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <qdebug.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->txtPathLDIF->setReadOnly(true);

    // Definindo os atributos padrão
    ui->chkNome->setChecked( true );
    ui->chkCPF->setChecked( true );
    //ui->chkSIAPE->setChecked( true );
    //ui->chkCodigoUsuario->setChecked( true );
    //ui->chkTelefone->setChecked( true );
    //ui->chkLotacao->setChecked( true );
    //ui->chkTipoVinculo->setChecked( true );
    ui->chkLoginUnico->setChecked( true );
    ui->chkEmail->setChecked( true );

}

MainWindow::~MainWindow()
{
    delete ui;
    delete listAtributos;
}

void MainWindow::on_btnEscolherLDIF_clicked()
{
    pathFileLDIF = QFileDialog::getOpenFileName(this,"Selecionar Arquivo","", "Arquivo LDIF (*.ldif)");
    if ( ! pathFileLDIF.isNull() )
        ui->txtPathLDIF->setText( pathFileLDIF );
}

void MainWindow::on_btnExportarSMB4_clicked()
{

}

void MainWindow::on_btnProcessarLDIF_clicked()
{

    if ( pathFileLDIF.isNull() || pathFileLDIF.isEmpty() ){
        QMessageBox::information(this, "Escolher arquivo LDIF", "Favor escolha o arquivo LDIF a ser processado");
        ui->btnEscolherLDIF->setFocus();
        return;
    }
    else{

        lerAtributos();
        //usuario = new UFGUser();
        userCount = 0;

        QFile file ( pathFileLDIF );

        if(!file.open( QIODevice::ReadOnly | QIODevice::Text ))
            return;


        QTextStream in( &file );
        // if( codificacao() == "UTF-8" ){
        in.setCodec("UTF-8");

        //}else if( codificacao() == "ISO-8859-1"){
        //    in.setCodec("ISO-8859-1");

        //}else{

        //}
        QString sep = ":";
        // listUsuario.clear();


        try{

            while( !in.atEnd() ){
                QString line;
                QStringList list;

                // Le a linha e adiciona a variavel Qstring
                line = in.readLine();
                list = QStringList() << line.split( sep );
                //qDebug() << list.length();
                //QMessageBox::information(this, "List", QString(int(list.length())) );
                filtrarDados(list);

                //listAtributos->setContador(7);

                if ( usuario.getNome().length() > 3 && usuario.getCpf().length() >0 && usuario.getTipoVinculo().length() >1 && usuario.getLoginUnico().length() >3 && usuario.getEmail().length() >5 ){
                    //if ( listAtributos->getContador() == userCount ){
                    // listUsuario.append( usuario );
                    qDebug() << "Nome: " << usuario.getNome();
                    qDebug() << "CPF: " << usuario.getCpf();
                    qDebug() << "Vinculo: "<< usuario.getTipoVinculo();
                    qDebug() << "Login: " << usuario.getLoginUnico();
                    qDebug() << "Email: " << usuario.getEmail();

                    if ( usuario.getCodigo().length() > 0 ){
                        qDebug() << "Código: " << usuario.getCodigo();
                    }

                    if ( usuario.getLotacao() != "" ){
                        qDebug() << "Lotação: " << usuario.getLotacao();
                    }

                    qDebug() << "";

                    usuario.limpar();
                    userCount = 0;
                    flagLotacao = 0;
                    flagEmployee = 0;

                }

            }

        }catch (...){

        }

    }

    //qDebug() << QString( listAtributos->toFormate());
    //QMessageBox::information(this, "TeS", QString ( listAtributos->toFormate() ));

}

void MainWindow::lerAtributos(){

    listAtributos = new Atributos();
    int attrNum = 0;

    if ( ui->chkNome->isChecked() ) {
        listAtributos->setNome( true );
        attrNum += 1;
    }
    if ( ui->chkCPF->isChecked() ) {
        listAtributos->setCpf( true );
        attrNum += 1;
    }
    if ( ui->chkSIAPE->isChecked() ){
        listAtributos->setSiape( true );
        attrNum += 1;
    }
    if ( ui->chkCodigoUsuario->isChecked() ){
        listAtributos->setCodigo( true );
        attrNum += 1;
    }
    if ( ui->chkTelefone->isChecked() ){
        listAtributos->setTelefone(true );
        attrNum += 1;
    }
    if ( ui->chkLotacao->isChecked() ){
        listAtributos->setLotacao( true );
        attrNum += 1;
    }
    if ( ui->chkTipoVinculo->isChecked() ){
        listAtributos->setTipoVinculo( true );
        attrNum += 1;
    }
    if ( ui->chkLoginUnico->isChecked() ){
        listAtributos->setLoginUnico( true );
        attrNum += 1;
    }
    if ( ui->chkEmail->isChecked() ){
        listAtributos->setEmail( true );
        attrNum += 1;
    }

    listAtributos->setContador(attrNum);
    //qDebug() << "QTD attributos: "  << listAtributos->getContador();

}

void MainWindow::filtrarDados(QStringList value) {
    QString aux = "";

    // Nome Completo
    if ( value.at(0) == "displayName" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setNome( aux );
            userCount ++;
        }
    }
    // CPF
    if ( value.at(0) == "brPersonCPF" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setCpf( aux );
            userCount ++;
        }
    }
    // SIAPE
    if ( value.at(0) == "siape" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setSiape( aux );
            userCount ++;
        }
    }
    // Código do Usuário
    if ( value.at(0) == "employeeNumber" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setCodigo( aux );
            userCount ++;
        }
    }
    // Telefone
    if ( value.at(0) == "telephoneNumber" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setTelefone( aux );
            userCount ++;
        }
    }
    // Lotação
    if ( value.at(0) == "l" ){
        aux = value.at(1);
        if ( aux != "" && aux != "NULL" ){
            if ( flagLotacao == 0 ) {
                aux.remove( 0, 1 );
                usuario.setLotacao( aux );
                userCount ++;
                flagLotacao ++;
            }
        }
    }
    // Tipo de Vinculo = TA/Aluno/Professor/Outros
    if ( value.at(0) == "employeeType" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            if ( flagEmployee == 0 ){
                //qDebug() << "Entrou em employeeType: " << value.at(1);
                aux.remove( 0, 1 );
                usuario.setTipoVinculo( aux );
                userCount ++;
                flagEmployee ++;
            }
        }
    }
    // Login Único
    if ( value.at(0) == "uid" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setLoginUnico( aux );
            userCount ++;
        }
    }
    // E-mail
    if ( value.at(0) == "mail" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setEmail( aux );
            userCount ++;
        }
    }

    //qDebug() << "Dados: " << usuario << endl;

}
