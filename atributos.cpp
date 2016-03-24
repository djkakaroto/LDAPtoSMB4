#include "atributos.h"

Atributos::Atributos()
{
    nome = false;
    cpf = false;
    siape = false;
    codigo = false;
    telefone = false;
    lotacao = false;
    tipoVinculo = false;
    loginUnico = false;
    email = false;

}

bool Atributos::getNome() const
{
    return nome;
}

void Atributos::setNome(bool value)
{
    nome = value;
}

bool Atributos::getCpf() const
{
    return cpf;
}

void Atributos::setCpf(bool value)
{
    cpf = value;
}

bool Atributos::getSiape() const
{
    return siape;
}

void Atributos::setSiape(bool value)
{
    siape = value;
}

bool Atributos::getTelefone() const
{
    return telefone;
}

void Atributos::setTelefone(bool value)
{
    telefone = value;
}

bool Atributos::getLotacao() const
{
    return lotacao;
}

void Atributos::setLotacao(bool value)
{
    lotacao = value;
}

bool Atributos::getTipoVinculo() const
{
    return tipoVinculo;
}

void Atributos::setTipoVinculo(bool value)
{
    tipoVinculo = value;
}

bool Atributos::getLoginUnico() const
{
    return loginUnico;
}

void Atributos::setLoginUnico(bool value)
{
    loginUnico = value;
}

bool Atributos::getEmail() const
{
    return email;
}

void Atributos::setEmail(bool value)
{
    email = value;
}

bool Atributos::getCodigo() const
{
    return codigo;
}

void Atributos::setCodigo(bool value)
{
    codigo = value;
}

int Atributos::getContador()
{
    return contador;
}

void Atributos::setContador(int value)
{
    contador = value;
}
