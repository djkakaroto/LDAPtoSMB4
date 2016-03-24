#ifndef UFGUSER_H
#define UFGUSER_H
#include <QString>


class UFGUser
{
public:
    UFGUser();

    QString getNome() const;
    void setNome(const QString &value);

    QString getCpf() const;
    void setCpf(const QString &value);

    QString getSiape() const;
    void setSiape(const QString &value);

    QString getMatricula() const;
    void setMatricula(const QString &value);

    QString getTelefone() const;
    void setTelefone(const QString &value);

    QString getLotacao() const;
    void setLotacao(const QString &value);

    QString getTipoVinculo() const;
    void setTipoVinculo(const QString &value);

    QString getLoginUnico() const;
    void setLoginUnico(const QString &value);

    QString getEmail() const;
    void setEmail(const QString &value);

private:
    QString nome;
    QString cpf;
    QString siape;
    QString matricula;
    QString telefone;
    QString lotacao;
    QString tipoVinculo;
    QString loginUnico;
    QString email;

};

#endif // UFGUSER_H
