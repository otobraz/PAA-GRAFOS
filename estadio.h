#ifndef ESTADIO_H
#define ESTADIO_H

#include <vector>
#include <tuple>

/** @brief definição de tipos complexos de dados
 */
typedef std::pair<int, int> iPar;
typedef std::tuple<int, int, int> iTupla;


/** @brief Classe Estádio
 */
class Estadio{

private:
    int numLocais;
    long numCorredores;
    std::vector<iTupla> *listaAdjUV;
    std::vector<iTupla> *listaAdjVU;

public:
    Estadio(int numLocais);
    void addCorredorUV(int cId, int u, int v, int peso);
    void addCorredorVU(int cId, int u, int v, int peso);
    int getNL();
    long getNC();
    std::vector<iTupla>* getListaAdjUV();
    std::vector<iTupla>* getListaAdjVU();
    void setNC(int numCorredores);
};

#endif // ESTADIO_H
