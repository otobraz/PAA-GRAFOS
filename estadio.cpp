#include "estadio.h"

/** @brief construtor da classe
 *
 * @param numLocais int: n�mero de locais
 *
 */
Estadio::Estadio(int numLocais){
    this->numLocais = numLocais;
    listaAdjUV = new std::vector<iTupla>[numLocais];
    listaAdjVU = new std::vector<iTupla>[numLocais];
}

/** @brief adiciona aresta (u,v) na lista de adjac�ncia
 *
 * @param cID int: id do corredor
 * @param u int: v�rtice
 * @param v int: v�rtice
 * @param peso int: peso da aresta
 * @return void
 *
 */
void Estadio::addCorredorUV(int cId, int u, int v, int peso){
    listaAdjUV[u-1].push_back(std::make_tuple(cId, v, peso));
}

/** @brief adiciona aresta (u,v) na lista de adjac�ncia reversa
 *
 * @param cID int: id do corredor
 * @param u int: v�rtice
 * @param v int: v�rtice
 * @param peso int: peso da aresta
 * @return void
 *
 */
void Estadio::addCorredorVU(int cId, int u, int v, int peso){
    listaAdjVU[v-1].push_back(std::make_tuple(cId, u, peso));
}

/** @brief retorna n�mero de locais do est�dio
 *
 * @return int: n�mero de locais
 *
 */
int Estadio::getNL(){
    return numLocais;
}


/** @brief retorna n�mero de corredores do est�dio
 *
 * @return long: n�mero de corredores
 *
 */
long Estadio::getNC(){
    return numCorredores;
}

/** @brief setter para numCorredores
 *
 * @param numCorredores int: n�mero de corredores
 * @return void
 *
 */
void Estadio::setNC(int numCorredores){
    this->numCorredores = numCorredores;
}

/** @brief retorna lista de adjac�ncias do grafo
 *
 * @return std::vector<iTupla>*: lista
 *
 */
std::vector<iTupla>* Estadio::getListaAdjUV(){
    return listaAdjUV;
}


/** @brief retorna lista de adjac�ncia reversa do grafo
 *
 * @return std::vector<iTupla>*: lista reversa
 *
 */
std::vector<iTupla>* Estadio::getListaAdjVU(){
    return listaAdjVU;
}
