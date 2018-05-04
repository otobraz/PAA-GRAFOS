#include "estadio.h"

/** @brief construtor da classe
 *
 * @param numLocais int: número de locais
 *
 */
Estadio::Estadio(int numLocais){
    this->numLocais = numLocais;
    listaAdjUV = new std::vector<iTupla>[numLocais];
    listaAdjVU = new std::vector<iTupla>[numLocais];
}

/** @brief adiciona aresta (u,v) na lista de adjacência
 *
 * @param cID int: id do corredor
 * @param u int: vértice
 * @param v int: vértice
 * @param peso int: peso da aresta
 * @return void
 *
 */
void Estadio::addCorredorUV(int cId, int u, int v, int peso){
    listaAdjUV[u-1].push_back(std::make_tuple(cId, v, peso));
}

/** @brief adiciona aresta (u,v) na lista de adjacência reversa
 *
 * @param cID int: id do corredor
 * @param u int: vértice
 * @param v int: vértice
 * @param peso int: peso da aresta
 * @return void
 *
 */
void Estadio::addCorredorVU(int cId, int u, int v, int peso){
    listaAdjVU[v-1].push_back(std::make_tuple(cId, u, peso));
}

/** @brief retorna número de locais do estádio
 *
 * @return int: número de locais
 *
 */
int Estadio::getNL(){
    return numLocais;
}


/** @brief retorna número de corredores do estádio
 *
 * @return long: número de corredores
 *
 */
long Estadio::getNC(){
    return numCorredores;
}

/** @brief setter para numCorredores
 *
 * @param numCorredores int: número de corredores
 * @return void
 *
 */
void Estadio::setNC(int numCorredores){
    this->numCorredores = numCorredores;
}

/** @brief retorna lista de adjacências do grafo
 *
 * @return std::vector<iTupla>*: lista
 *
 */
std::vector<iTupla>* Estadio::getListaAdjUV(){
    return listaAdjUV;
}


/** @brief retorna lista de adjacência reversa do grafo
 *
 * @return std::vector<iTupla>*: lista reversa
 *
 */
std::vector<iTupla>* Estadio::getListaAdjVU(){
    return listaAdjVU;
}
