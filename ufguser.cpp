#include "ufguser.h"

UFGUser::UFGUser()
{
    nome = "";
    cpf = "";
    siape = "";
    codigo = "";
    telefone = "";
    lotacao = "";
    tipoVinculo = "";
    loginUnico = "";
    email = "";
}

QString UFGUser::getNome() const
{
    return nome;
}

void UFGUser::setNome(const QString &value)
{
    nome = value;
}

QString UFGUser::getCpf() const
{
    return cpf;
}

void UFGUser::setCpf(const QString &value)
{
    cpf = value;
}

QString UFGUser::getSiape() const
{
    return siape;
}

void UFGUser::setSiape(const QString &value)
{
    siape = value;
}

QString UFGUser::getTelefone() const
{
    return telefone;
}

void UFGUser::setTelefone(const QString &value)
{
    telefone = value;
}

QString UFGUser::getLotacao() const
{
    return lotacao;
}

void UFGUser::setLotacao(const QString &value)
{
    lotacao = value;
}

QString UFGUser::getTipoVinculo() const
{
    return tipoVinculo;
}

void UFGUser::setTipoVinculo(const QString &value)
{
    tipoVinculo = value;
}

QString UFGUser::getLoginUnico() const
{
    return loginUnico;
}

void UFGUser::setLoginUnico(const QString &value)
{
    loginUnico = value;
}

QString UFGUser::getEmail() const
{
    return email;
}

void UFGUser::setEmail(const QString &value)
{
    email = value;
}

void UFGUser::limpar()
{
    nome = "";
    cpf = "";
    siape = "";
    codigo = "";
    telefone = "";
    lotacao = "";
    tipoVinculo = "";
    loginUnico = "";
    email = "";
}

QString UFGUser::getCodigo() const
{
    return codigo;
}

void UFGUser::setCodigo(const QString &value)
{
    codigo = value;
}
