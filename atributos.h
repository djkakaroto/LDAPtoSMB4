#ifndef ATRIBUTOS_H
#define ATRIBUTOS_H

#include <QString>

class Atributos
{
public:
    Atributos();

    bool getNome() const;
    void setNome(bool value);

    bool getCpf() const;
    void setCpf(bool value);

    bool getSiape() const;
    void setSiape(bool value);

    bool getTelefone() const;
    void setTelefone(bool value);

    bool getLotacao() const;
    void setLotacao(bool value);

    bool getTipoVinculo() const;
    void setTipoVinculo(bool value);

    bool getLoginUnico() const;
    void setLoginUnico(bool value);

    bool getEmail() const;
    void setEmail(bool value);

    bool getCodigo() const;
    void setCodigo(bool value);

    int getContador();
    void setContador(int value);

private:

    bool nome;
    bool cpf;
    bool siape;
    bool codigo;
    bool telefone;
    bool lotacao;
    bool tipoVinculo;
    bool loginUnico;
    bool email;
    int contador;

};

#endif // ATRIBUTOS_H
