#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <qdebug.h>
#include <QStandardItemModel>


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
    ui->chkCodigoUsuario->setChecked( true );
    //ui->chkTelefone->setChecked( true );
    ui->chkLotacao->setChecked( true );
    ui->chkTipoVinculo->setChecked( true );
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

        QFile file ( pathFileLDIF );

        if(!file.open( QIODevice::ReadOnly | QIODevice::Text ))
            return;

        QTextStream in( &file );

        // Definindo a codificação do arquivo para UTF-8
        in.setCodec("UTF-8");
        QString sep = ":";

        try{

            listUsuario.clear();

            while( !in.atEnd() ){
                QString line;
                QStringList list;

                // Le a linha e adiciona a variavel Qstring
                line = in.readLine();
                list = QStringList() << line.split( sep );

                // Realiza a separação dos dados
                filtrarDados(list);

                if ( usuario.getNome().length() > 3 && usuario.getCpf().length() >0 && usuario.getTipoVinculo().length() >1 && usuario.getLoginUnico().length() >3 && usuario.getEmail().length() >5 ){
                    //if ( listAtributos->getContador() == userCount ){
                    // listUsuario.append( usuario );
                    //                    qDebug() << "Nome: " << usuario.getNome();
                    //                    qDebug() << "CPF: " << usuario.getCpf();
                    //                    qDebug() << "Vinculo: "<< usuario.getTipoVinculo();
                    //                    qDebug() << "Login: " << usuario.getLoginUnico();
                    //                    qDebug() << "Email: " << usuario.getEmail();

                    //                    if ( usuario.getCodigo().length() > 0 ){
                    //                        qDebug() << "Código: " << usuario.getCodigo();
                    //                    }

                    //                    if ( usuario.getLotacao() != "" ){
                    //                        qDebug() << "Lotação: " << usuario.getLotacao();
                    //                    }

                    //                    qDebug() << "";

                    try {
                        listUsuario.append( usuario );
                    }catch (...){

                    }

                    usuario.limpar();
                    flagLotacao = 0;
                    flagEmployee = 0;

                }

            }

        }catch (...){

        }

        // Envia o objeto listUsuario para inserir na tabela
        inserirDadosTabela( listUsuario );

        // Fecha o arquivo LDIF
        file.close();
    }

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
}

void MainWindow::filtrarDados(QStringList value) {
    QString aux = "";

    // Nome Completo
    if ( value.at(0) == "displayName" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setNome( aux );
        }
    }

    // CPF
    if ( value.at(0) == "brPersonCPF" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setCpf( aux );
        }
    }

    // SIAPE
    if ( value.at(0) == "siape" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setSiape( aux );
        }
    }

    // Código do Usuário
    if ( value.at(0) == "employeeNumber" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setCodigo( aux );
        }
    }

    // Telefone
    if ( value.at(0) == "telephoneNumber" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setTelefone( aux );
        }
    }

    // Lotação
    if ( value.at(0) == "l" ){
        aux = value.at(1);
        if ( aux != "" && aux != "NULL" ){
            if ( flagLotacao == 0 ) {
                aux.remove( 0, 1 );
                usuario.setLotacao( aux );
                flagLotacao ++;
            }
        }
    }

    // Tipo de Vinculo = TA/Aluno/Professor/Outros
    if ( value.at(0) == "employeeType" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            if ( flagEmployee == 0 ){
                aux.remove( 0, 1 );
                usuario.setTipoVinculo( aux );
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
        }
    }
    // E-mail
    if ( value.at(0) == "mail" ){
        aux = value.at(1);
        if ( aux != "" || aux != "NULL" ){
            aux.remove( 0, 1 );
            usuario.setEmail( aux );
        }
    }

}

void MainWindow::inserirDadosTabela( const QList <UFGUser> &value ){
    UFGUser user;
    QStandardItemModel *model = new QStandardItemModel(value.length(),9,this);

    if ( listAtributos->getCodigo() ){
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Código")));
    }

    if ( listAtributos->getCpf() ){
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("CPF")));
    }

    if ( listAtributos->getNome() ){
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Nome Completo")));
    }

    if ( listAtributos->getTipoVinculo() ){
        model->setHorizontalHeaderItem(3, new QStandardItem(QString("Vinculo")));
    }

    if ( listAtributos->getLotacao() ){
        model->setHorizontalHeaderItem(4, new QStandardItem(QString("Lotação")));
    }

    if ( listAtributos->getLoginUnico() ){
        model->setHorizontalHeaderItem(5, new QStandardItem(QString("Login Único")));
    }

    if ( listAtributos->getEmail() ){
        model->setHorizontalHeaderItem(6, new QStandardItem(QString("E-mail")));
    }

    if ( listAtributos->getTelefone() || listAtributos->getTelefone() == false ){
        model->setHorizontalHeaderItem(7, new QStandardItem(QString("Telefone")));
    }

    if ( listAtributos->getSiape() || listAtributos->getSiape() == false ){
        model->setHorizontalHeaderItem(8, new QStandardItem(QString("SIAPE")));
    }

    // Percorre o QList e retira os usuarios para o preenchimento da tabela
    for ( int linha = 0; linha < value.length(); linha++ ){

        user = value.at( linha );

        for ( int col = 0; col < 9; col++ ){

            switch (col) {
            case 0:
                //item->setText( QString( user.getCodigo() ) );
                model->setItem( linha, col, new QStandardItem(QString ( user.getCodigo() ) ) );
                break;
            case 1:
                //item->setText( QString( user.getCpf() ) );
                model->setItem( linha, col, new QStandardItem(QString ( user.getCpf() ) ) );
                break;
            case 2:
                //item->setText( QString( user.getNome() ) );
                model->setItem( linha, col, new QStandardItem(QString ( user.getNome() ) ) );
                break;
            case 3:
                //item->setText( QString( user.getTipoVinculo() ) );
                model->setItem( linha, col, new QStandardItem(QString ( user.getTipoVinculo() ) ) );
                break;
            case 4:
                //item->setText( QString( user.getLotacao() ) );
                model->setItem( linha, col, new QStandardItem(QString ( user.getLotacao() ) ) );
                break;
            case 5:
                //item->setText( QString( user.getLoginUnico() ) );
                model->setItem( linha, col, new QStandardItem(QString ( user.getLoginUnico() ) ) );
                break;
            case 6:
                //item->setText( QString( user.getEmail() ) );
                model->setItem( linha, col, new QStandardItem(QString ( user.getEmail() ) ) );
                break;
            case 7:
                //item->setText( QString( user.getTelefone() ) );
                model->setItem( linha, col, new QStandardItem(QString ( user.getTelefone() ) ) );
                break;
            case 8:
                //item->setText( QString( user.getSiape() ) );
                model->setItem( linha, col, new QStandardItem(QString ( user.getSiape() ) ) );
                break;
            default:
                break;
            }

            //model->setItem( linha, col, item );

        }
    }

    ui->tableView->setModel(model);

}
