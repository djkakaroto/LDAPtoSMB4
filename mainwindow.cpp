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
    QString pathExportLDIF;
    QFile file;

    if ( ui->txtRealm->text().isEmpty() || ui->txtRealm->text().isNull() ){
        QMessageBox::information(this, "Informe o REALM/DOMINIO", "Favor digitar o REALM/DOMINIO de acordo com o exemplo abaixo:\n\n DC=dominio,DC=net,DC=local\n ou\n dominio.net.local" );
        ui->txtRealm->setFocus();
    }else{

        // Escolhe o local a ser salvo e adiciona o caminho ao valor da variável
        pathExportLDIF = QFileDialog::getSaveFileName( this, "Escolha o local para salvar o arquivo." );

        if ( ! pathExportLDIF.isNull() ){
            file.setFileName( pathExportLDIF );

            if(!file.open( QIODevice::WriteOnly | QIODevice::Text ))
                return;

            QTextStream out ( &file );

            // Definindo a codificação do arquivo para UTF-8
            out.setCodec("UTF-8");

            UFGUser smbUser;
            QString codigo, cpf, nome, vinculo, lotacao, loginUnico, email, telefone, siape = "";
            QString realm, dominioKrb, dnRealm = "";
            QStringList dominio;

            if ( ui->txtRealm->text() != "" ){
                realm = ui->txtRealm->text().toLower();

                // Verifica a composição com DOMINIO/REALM
                if ( realm.contains(".") ){
                    dominio = realm.split(".");
                    dominioKrb = realm;

                    switch ( dominio.length() ) {
                    case 1:
                        dnRealm = "DC=" + dominio.at(0);
                        break;
                    case 2:
                        dnRealm = "DC=" + dominio.at(0) + "," + "DC=" + dominio.at(1);
                        break;
                    case 3:
                        dnRealm = "DC=" + dominio.at(0) + "," + "DC=" + dominio.at(1) + "," + "DC=" + dominio.at(2);
                        break;
                    case 4:
                        dnRealm = "DC=" + dominio.at(0) + "," + "DC=" + dominio.at(1) + "," + "DC=" + dominio.at(2) + "," + "DC=" + dominio.at(3);
                        break;
                    default:
                        break;
                    }

                }else{
                    dnRealm = realm;
                    realm = realm.remove( "dc=" );
                    dominio = realm.split(",");

                    switch ( dominio.length() ) {
                    case 1:
                        dominioKrb = dominio.at(0);
                        break;
                    case 2:
                        dominioKrb = dominio.at(0) + "." + dominio.at(1);
                        break;
                    case 3:
                        dominioKrb = dominio.at(0) + "." + dominio.at(1) + "." + dominio.at(2);
                        break;
                    case 4:
                        dominioKrb = dominio.at(0) + "." + dominio.at(1) + "." + dominio.at(2) + "." + dominio.at(3);
                        break;
                    default:
                        break;
                    }

                }
            }

            for ( int linha = 0; linha < listUsuario.length(); linha++ ){

                smbUser = listUsuario.at( linha );

                if ( smbUser.getLoginUnico() != "" ){
                    loginUnico = smbUser.getLoginUnico();
                    out << "dn: " << "CN=" << loginUnico << ",CN=Users," << dnRealm << "\n";
                    out << "objectClass: top\n";
                    out << "objectClass: person\n";
                    out << "objectClass: organizationalPerson\n";
                    out << "objectClass: user\n";
                    out << "cn: " + loginUnico << "\n";
                    out << "sAMAccountName: " + loginUnico << "\n";
                    out << "userPrincipalName: " + loginUnico << "@" << dominioKrb << "\n";
                }

                if ( smbUser.getNome() != "" ){
                    nome = smbUser.getNome();
                    out << "name: " + nome << "\n";
                    out << "displayName: " + nome << "\n";
                }

                if ( smbUser.getTipoVinculo() != "" ){
                    vinculo = smbUser.getTipoVinculo();
                    out << "description: " + vinculo << "\n";
                }

                if ( smbUser.getLotacao() != "" ){
                    lotacao = smbUser.getLotacao();
                    out << "physicalDeliveryOfficeName: " + lotacao << "\n";
                }

                out << "objectCategory: " << "CN=Person,CN=Schema,CN=Configuration," << dnRealm << "\n";
                out << "userAccountControl: 512" << "\n";
                out << "distinguishedName: " << "CN=" << loginUnico << ",CN=Users," << dnRealm << "\n";

                if ( smbUser.getEmail() != "" ){
                    email = smbUser.getEmail();
                    out << "mail: " + email << "\n";
                }

                if ( smbUser.getTelefone() != "" ){
                    telefone = smbUser.getTelefone();
                    out << "telephoneNumber: " + telefone << "\n";
                }

                if ( smbUser.getSiape() != "" ){
                    siape = smbUser.getSiape();
                    //out << "siape: " + siape << "\n";
                }

                if ( smbUser.getCpf() != "" ){
                    cpf = smbUser.getCpf();
                    out << "info: " << "CPF: " + cpf;
                }

                if (smbUser.getCodigo() != "" ){
                    codigo = smbUser.getCodigo();
                    out << " Código: " + codigo << "\n";
                }else{
                    out << "\n";
                }

                out << "" << endl;

            }

            QMessageBox::information(this, "Resultado", "Arquivo salvo com sucesso em: " + pathExportLDIF );
        }

    }
    file.close();
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

                    try {
                        listUsuario.append( usuario );
                    }catch (...){
                        QMessageBox::critical( this, "Erro Inesperado", "Não foi possível adicionar o(s) usuários a visualização!" );
                    }

                    usuario.limpar();
                    flagLotacao = 0;
                    flagEmployee = 0;

                }

            }

        }catch (...){
            QMessageBox::critical(this, "Erro Critico", "Ocorreu um erro ao ler o arquivo até o final.");
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
    QStandardItemModel *model = new QStandardItemModel(value.length(),7,this);

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

    if ( listAtributos->getTelefone() ){
        model->setHorizontalHeaderItem(7, new QStandardItem(QString("Telefone")));
    }

    if ( listAtributos->getSiape() ){
        model->setHorizontalHeaderItem(8, new QStandardItem(QString("SIAPE")));
    }

    // Percorre o QList e retira os usuarios para o preenchimento da tabela
    for ( int linha = 0; linha < value.length(); linha++ ){

        user = value.at( linha );

        for ( int col = 0; col < 9; col++ ){

            switch (col) {
            case 0:
                model->setItem( linha, col, new QStandardItem(QString ( user.getCodigo() ) ) );
                break;
            case 1:
                model->setItem( linha, col, new QStandardItem(QString ( user.getCpf() ) ) );
                break;
            case 2:
                model->setItem( linha, col, new QStandardItem(QString ( user.getNome() ) ) );
                break;
            case 3:
                model->setItem( linha, col, new QStandardItem(QString ( user.getTipoVinculo() ) ) );
                break;
            case 4:
                model->setItem( linha, col, new QStandardItem(QString ( user.getLotacao() ) ) );
                break;
            case 5:
                model->setItem( linha, col, new QStandardItem(QString ( user.getLoginUnico() ) ) );
                break;
            case 6:
                model->setItem( linha, col, new QStandardItem(QString ( user.getEmail() ) ) );
                break;
            case 7:
                if ( user.getTelefone() != "" ){
                    model->setItem( linha, col, new QStandardItem(QString ( user.getTelefone() ) ) );
                }
                break;
            case 8:
                if ( user.getSiape() != "" ){
                    model->setItem( linha, col, new QStandardItem(QString ( user.getSiape() ) ) );
                }
                break;
            default:
                break;
            }
        }
    }

    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0, 70);
    ui->tableView->setColumnWidth(1, 100);
    ui->tableView->setColumnWidth(2, 310);
    ui->tableView->setColumnWidth(3, 130);
    ui->tableView->setColumnWidth(4, 150);
    ui->tableView->setColumnWidth(5, 150);
    ui->tableView->setColumnWidth(6, 200);

    //ui->tableView->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );

}
