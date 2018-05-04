#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <string>
#include <exception>
#include <climits>

#define INF LLONG_MAX

#include "estadio.h"

/** @brief Comparador para inserção de elementos na fila de prioridades
 */
class menorDistancia{

public:
    bool operator()(iPar p1, iPar p2){
        return p1.second > p2.second;
    }
};

/** @brief Definição de tipos de dados complexos
 */
typedef std::priority_queue<iPar,std::vector<iPar>,menorDistancia> filaPrioridade;  /**< vetor de pares de inteiros */
typedef std::vector<iTupla> vITupla;        /**< vetor de tuplas de inteiros */
typedef vITupla::iterator it;       /**< iterator para os vetores vITupla */

/** @brief Cria grafo a partir do arquivo .txt lido
 *
 * @param path std::string - caminho para arquivo
 * @return objeto Estadio
 *
 */
Estadio criarGrafo(std::string path){
    int numLocais, numCorredores;
    int u, v, w;
    std::ifstream in(path);
    in.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    if (in.is_open()){
        in >> numLocais;
        in >> numCorredores;
        Estadio estadio(numLocais);
        estadio.setNC(numCorredores);
        for(int i = 1; i <= numCorredores; i++){
            in >> u;
            in >> v;
            in >> w;
            estadio.addCorredorUV(i, u, v, w);
            estadio.addCorredorVU(i, u, v, w);
        }
        in.close();
        return estadio;
    }
}


/** @brief Inicializa os vetores das distâncias
 *
 * @param nL int: número de locais
 * @param s int: vértice de origem
 * @param d[] long long: vetor das distâncias a partir da origem
 * @param dT[] long long: vetor das distâncias a partir do destino
 * @return void
 *
 */
void inicializar(int nL, int s, long long d[], long long dT[]){
    for(int i = 0; i < nL; i++){
        d[i] = INF;
        dT[i] = INF;
    }
    d[s-1] = 0;
    dT[nL-1] = 0;
}

/** @brief Relaxa as arestas
 *
 * @param d[] int: vetor de distâncias
 * @param corredor iTupla - tupla de 3 inteiros que define um corredor
 * @param u int - vértice origem do corredor
 * @param q filaPrioridade& - fila de prioridade
 * @return void
 *
 */
void relaxar(long long d[], iTupla corredor, int u, filaPrioridade &q){
    int v = std::get<1>(corredor)-1;
    int w = std::get<2>(corredor);
    if(d[v] > d[u] + w){
        d[v] = d[u] + w;
        q.push(std::make_pair(v+1, d[v]));  /**< faz nova adição do vértice com prioridade atualizada */
    }
}

/** @brief Encontra os corredores que possivelmente serão utilizados (CPU)
 *
 * @param e Estadio: grafo
 * @param d[] long long: vetor de distâncias da origem ao destino
 * @param dT[] long long: vetor de distâncias do destino à origem
 * @param nCP int&: contador pro número de corredores
 * @param c[] int: vetor para marcar os CPUs
 * @return Estadio: grafo composto por arestas dos caminhos mínimos
 *
 */
Estadio encontraCPU(Estadio &e, long long d[], long long dT[], int &nCP, int c[]){
    Estadio camMin(e.getNL());
    for(int i = 0; i < e.getNL(); i++){
        for(vITupla::iterator j = e.getListaAdjUV()[i].begin(); j != e.getListaAdjUV()[i].end(); j++){
            int w = std::get<2>(*j);
            if(d[e.getNL()-1] == d[i] + dT[std::get<1>(*j)-1] + w){
                int cId = std::get<0>(*j);
                int v = std::get<1>(*j);
                camMin.addCorredorUV(cId, v, i+1, w);
                camMin.addCorredorUV(cId, i+1, v, w);
                c[cId-1] = 1;
                nCP++;
            }
        }
    }
    camMin.setNC(nCP);
    return camMin;
}

/** @brief Encontra os corredores que definitivamente serão utilizados (CDU)
 *
 * @param e Estadio: grafo
 * @param nCD int&: contador pro número de CDUs
 * @param c[] int: vetor para marcar os corredores
 * @return void
 *
 */
void encontraCDU(Estadio &e, int &nCD, int c[]){

    int nL = e.getNL();             /**< número de locais */

	bool *visited = new bool[nL];   /**< marca vértices visitados */
	int *caminho = new int[nL];     /**< mantem controle do caminho percorrido na árvore */
	int *d = new int[nL];           /**< marca tempo de descoberta do vértice */
	int *l = new int[nL];           /**< armazena menor vértice alcançável */
	int *p = new int[nL];           /**< armazena predecessores */
	it *ite = new it[nL];           /**< iterators para as listas de adjência */

	int posV = 0, u = 0, k = 0, v;  /**< variáveis de controle */

	static int t = 0;               /**< contador para os tempos de descoberta */

    // inicializa vetores de predecessores, visitados e iterators
	for (int i = 0; i < nL; i++){
		p[i] = 0;
		visited[i] = false;
		ite[i] = e.getListaAdjUV()[i].begin();
	}

    // enquanto houverem vértices a serem processados
	while (true){

		if (k == 0) { /**< se o vértice u está sendo visitado pela primeira vez */
			caminho[posV] = u;
			posV++;
			k = 1;
			visited[u] = true;
			d[u] = l[u] = ++t;
		}

        // verifica elemento na lista de adjacência
		while(ite[u] != e.getListaAdjUV()[u].end()){
			v = std::get<1>(*ite[u])-1;
			ite[u]++;       /**< avança iterator para veriricar o próximo elemento depois */
			if(!visited[v]){
				p[v] = u;
				u = v;
				k = 0;
				break;      /**< interrompe o while para explorar o vértice "v" */
			}else if(v != p[u]){
                l[u] = std::min(l[u], d[v]);
			}
		}
		if (k == 1 && posV == 1){ /**< se não há mais vértices a serem explorados */
			return;
		}else if (k == 1){ /**< se o vértice "u" não possui mais vértices adjacentes para visitar */
			v = u;
			posV--;
			u = caminho[posV-1]; /**< faz "u" ser o vértice anterior no vetor de caminho */
			l[u] = std::min(l[u], l[v]);
			if (l[v] > d[u]){
                for(it i = e.getListaAdjUV()[u].begin(); i != e.getListaAdjUV()[u].end(); i++){
                    if(std::get<1>(*i) == v+1){
                        c[std::get<0>(*i)-1] = 2;       /**< marca o corredor como CDU = 2 */
                    }
                }
                nCD++;      /**< incrementa contador */
			}
		}
	}
}

/** @brief Gera o arquivo de saída a partir do vetor c[]
 *
 * @param nC int: número de corredores
 * @param nCPU int: número de CPUs identificados
 * @param nCDU int: número de CDUs identificados
 * @param c[] int: vetor onde os corredores foram marcados
 * @param path std::string: caminho pro arquivo de saída
 * @return void
 *
 */
void geraSaida(int nC, int nCPU, int nCDU, int c[], std::string path){
    std::ostringstream ssCPU, ssCDU;
    std::ofstream out;

    for(int i = 0; i < nC-1; i++){
        if(c[i] != 0){ /**< Se o corredor foi marcado com 1 ou 2 */
            ssCPU << i+1 << " "; /**< concatena o identificador à ostringstream dos CPUs */
        }
        if(c[i] == 2){   /**< Se o corredor foi marcado com 2 */
            ssCDU << i+1 << " ";  /**< concatena o identificador à ostringstream dos CDUs */
        }
    }

    // faz o mesmo processo pro último corredor separadamente para evitar espaço em branco no final
    if(c[nC-1] != 0){
        ssCPU << nC;
    }
    if(c[nC-1] == 2){
        ssCDU << nC;
    }

    // escreve no arquivo
    out.open(path);
    out << nCPU << "\n" << ssCPU.str() << "\n" << nCDU << "\n" << ssCDU.str() << "\n";
    out.close();
}

/** @brief Função que executa Dijkstra a partir da origem e a partir do destino
 *
 * @param s int: origem
 * @param e Estadio: grafo
 * @param d[] long long: vetor de distâncias a partir da origem
 * @param dT[] long long: vetor de disâncias a partir do destino
 * @return void
 *
 */
void twoWayDijkstra(int s, Estadio e, long long d[], long long dT[]){

    int nL = e.getNL();
    int counter = 0;    /**< contador pro número de iterações nos loops */
    long long dis;      /**< variável para armazenar distância do elemento retirado da fila */
    inicializar(nL, s, d, dT);      /**< inicializa as distâncias */

    // cria filas de prioridade e as inicializa
    filaPrioridade q, qT;
    for(int i = 0; i < nL; i++){
        q.push(std::make_pair(i+1, d[i]));
        qT.push(std::make_pair(i+1, dT[i]));
    }

    // calcula distâncias mínimas a partir da origem
    while (counter < nL && !q.empty()){
        int u = q.top().first-1;    /**< id do vértice */
        dis = q.top().second;     /**< distância */
        q.pop();
        if(d[u] == INF || d[u] < dis) continue;     /**< se é um vértices com prioridade desatualizada */
        for(it i = e.getListaAdjUV()[u].begin(); i != e.getListaAdjUV()[u].end(); i++){
            relaxar(d, *i, u, q);
        }
        counter++;

    }

    // se o destino é inalcansável a partir da origem
    if(d[nL-1] == INF){
        std::cout << "Nao ha caminho entre a origem e destino";
        return;
    }

    counter = 0;

    // calcula distâncias mínimas a partir do destino
    while (counter < nL && !qT.empty()){
        int u = qT.top().first-1;       /**< id do vértice */
        dis = qT.top().second;        /**< distância */
        qT.pop();
        if(dT[u] == INF || dT[u] < dis) continue;       /**< se é um vértices com prioridade desatualizada */
        for(it i = e.getListaAdjVU()[u].begin(); i != e.getListaAdjVU()[u].end(); i++){
            relaxar(dT, *i, u, qT);
        }
        counter++;
    }
}

/** @brief Faz execução dos algoritmos e gera a saída ao final
 *
 * @param e Estadio&: grafo
 * @param s int: origem
 * @param saida std::string: caminho para o arquivo de saída
 * @return void
 *
 */
void caminhosMinimos(Estadio &e, int s, std::string saida){
    int nL = e.getNL();     /**< nº de locais/vértices */
    int nC = e.getNC();     /**< nº de corredores/arestas */
    long long *d = new long long[nL];       /**< vetores para distâncias mínimas */
    long long *dT = new long long[nL];
    int *c = new int[nC];        /**< vetor para marcação dos corredores - 0:não utilizado, 1:CPU, 2:CDU */
    int nCPU = 0, nCDU = 0; /**< contadores para CPUs e CDUs*/

    // inicializa vetor de marcação
    for(int i = 0; i < nC; i++){
        c[i] = 0;
    }

    // calcula distâncias d (a partir da origem "s")
    // calcula distâncias dT(a partir do destino "nL")
    twoWayDijkstra(s, e, d, dT);

    // monta grafo com arestas dos caminhos mínimos
    // conta número de CPUs e os marca no array "c"
    Estadio camMin = encontraCPU(e, d, dT, nCPU, c);

    // conta número de CDUs e os marca no array "c"
    encontraCDU(camMin, nCDU, c);

    // gera a saída
    geraSaida(nC, nCPU, nCDU, c, saida);
}

int main (int argc, char* argv[]){

    std::string entrada = argv[1], saida = argv[2]; /**< armazena entrada e saída passados na execução */

    try{
        Estadio estadio = criarGrafo(entrada);      /**< lê arquivo de entra e monta o grafo */
        caminhosMinimos(estadio, 1, saida);         /**< faz todo o processamento e gera saída */
    }catch(std::exception& e){
        std::cout << "Erro ao abrir o arquivo especificado";
    }
    return 0;
}
