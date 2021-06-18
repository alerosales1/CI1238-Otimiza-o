#include <iostream>
#include <list>

//Classe que representa o ator
class Ator {
public:
    int id; //Identificador do ator
    int custo; //Custo pra contratar o ator
    std::list<int> grupo; //Lista de grupos aos quais este ator pertence

    //Construtor da classe
    Ator(int id, int custo) {
        this->id = id;
        this->custo = custo;
    }

    //Método que adiciona um grupo à lista de grupos do ator
    void adicionarGrupo(int g) {
        grupo.push_back(g);
    }
};

//Classe que representa o nó da árvore
class No {
public:
    int custo; //Soma do custo de todos os atores contratados
    std::list<Ator*> escolhidos; //Lista de atores contratados
    std::list<No> filhos; //Lista dos filhos deste nó
    bool* grupoPresente; //Array que diz quais grupos foram representados

    //Construtor da classe
    No(int numeroGrupos) {
        int i;
        custo = 0;

        //Inicializando todos os elementos do array como false
        grupoPresente = new bool[numeroGrupos];
        for (i = 0; i < numeroGrupos; i++) {
            grupoPresente[i] = false;
        }
    }

    //Método que gera todos os possíveis filhos deste nó
    void gerarFilhos(std::list<Ator>& ator, int numeroPersonagens, int limite, int numeroGrupos) {

        //Declarando todas as variáveis do método
        std::list<Ator>::iterator it1;
        std::list<Ator*>::iterator it2;
        std::list<Ator>::iterator it3;
        std::list<int>::iterator it4;
        bool presente, todosGruposRepresentados;
        int i, custoMinimo, numeroEscolhidos;
        bool* grupoPresenteAux = new bool[numeroGrupos];

        //Verificando, entre todos os atores disponívies, quais eu posso escolher para gerar um filho
        for (it1 = ator.begin(); it1 != ator.end(); it1++) {
            for (i = 0; i < numeroGrupos; i++) {
                grupoPresenteAux[i] = grupoPresente[i];
            }
            for (it4 = it1->grupo.begin(); it4 != it1->grupo.end(); it4++) {
                grupoPresenteAux[*it4 - 1] = true;
            }
            presente = false;
            custoMinimo = it1->custo;

            //Verificando se o possível novo ator já foi escolhido, ou se podemos inserir ele
            for (it2 = escolhidos.begin(); it2 != escolhidos.end(); it2++) {
                if (it1->id == (*it2)->id) {
                    presente = true;
                    break;
                }
                custoMinimo += (*it2)->custo;
            }
            if (presente) {
                continue;
            }
            numeroEscolhidos = escolhidos.size() + 1;
            it3 = ator.begin();

            //Calculando o menor custo possível (custoMinimo), caso este caminho seja percorrido
            //Isso é feito adicionando vários outros atores com os menores custos
            while (true) {

                //Verificando se o número de atores escolhidos já é igual ao número de personagens
                if (numeroEscolhidos == numeroPersonagens) {

                    //Verificando se o menor custo possível é maior do que o limite
                    if (custoMinimo <= limite) {

                        //Verificando se todos os grupos tem representantes
                        todosGruposRepresentados = true;
                        for (i = 0; i < numeroGrupos; i++) {
                            if (!grupoPresenteAux[i]) {
                                todosGruposRepresentados = false;
                                break;
                            }
                        }

                        //Caso todos os grupos tenham representantes, pode criar o filho
                        if (todosGruposRepresentados) {
                            No novo(numeroGrupos);
                            novo.custo = custo + it1->custo;
                            novo.escolhidos = escolhidos;
                            novo.escolhidos.push_back(&*it1);
                            for (i = 0; i < numeroGrupos; i++) {
                                novo.grupoPresente[i] = grupoPresente[i];
                            }
                            for (it4 = it1->grupo.begin(); it4 != it1->grupo.end(); it4++) {
                                novo.grupoPresente[*it4 - 1] = true;
                            }
                            filhos.push_back(novo);
                        }
                    }
                    break;
                }
                else if (it3->id == it1->id) { //Verificando se o possível novo ator já foi escolhido
                    it3++;
                    continue;
                }
                else { //Adicionando mais um ator para simular o menor custo possível
                    presente = false;
                    for (it2 = escolhidos.begin(); it2 != escolhidos.end(); it2++) {
                        if (it3->id == (*it2)->id) {
                            presente = true;
                            break;
                        }
                    }
                    if (presente) {
                        it3++;
                        continue;
                    }
                    custoMinimo += it3->custo;
                    for (it4 = it3->grupo.begin(); it4 != it3->grupo.end(); it4++) {
                        grupoPresenteAux[*it4 - 1] = true;
                    }
                    numeroEscolhidos++;
                }
                it3++;
            }
        }
        //delete[] grupoPresenteAux;
    }
};

int main()
{
    //Declarando as variáveis
    int i, j, numeroGrupos, numeroAtores, numeroPersonagens, custo, gruposQuePertence, grupo, limite;
    std::list<Ator> ator;
    std::list<No*> fila;

    //Lendo os dados de entrada
    printf("Entrada:" "\n");
    std::cin >> numeroGrupos >> numeroAtores >> numeroPersonagens;
    for (i = 0; i < numeroAtores; i++) {
        std::cin >> custo >> gruposQuePertence;
        Ator novo(i, custo);
        for (j = 0; j < gruposQuePertence; j++) {
            std::cin >> grupo;
            novo.adicionarGrupo(grupo);
        }
        ator.push_back(novo);
    }
    ator.sort([](Ator a, Ator b) {return a.custo < b.custo; });

    std::list<Ator>::iterator it1;
    std::list<int>::iterator it2;
    limite = 1000;

    //Construindo a árvore
    No raiz(numeroGrupos);
    fila.push_back(&raiz);
    No* atual;
    std::list<No>::iterator it3;
    std::list<No*>::iterator it4;
    No* solucao = nullptr;
    while (!fila.empty()) {
        atual = fila.front();
        fila.pop_front();
        if (atual->escolhidos.size() == numeroPersonagens) {
            if (solucao == nullptr) {
                solucao = atual;
            }
            else if (solucao->custo > atual->custo) {
                solucao = atual;
            }
            continue;
        }
        atual->gerarFilhos(ator, numeroPersonagens, limite, numeroGrupos);
        for (it3 = atual->filhos.begin(); it3 != atual->filhos.end(); it3++) {
            if (fila.empty()) {
                fila.push_back(&*it3);
                continue;
            }
            for (it4 = fila.begin(); it4 != fila.end(); it4++) {
                if (it3->custo > (*it4)->custo) {
                    fila.insert(it4, &*it3);
                    break;
                }
            }
        }
    }

    //Exibindo a solução do problema
    printf("Saida:" " \n");
    std::list<Ator*>::iterator it5;
    if (solucao == nullptr) {
        std::cout << "Inviavel";
    }
    else {
        for (it5 = solucao->escolhidos.begin(); it5 != solucao->escolhidos.end(); it5++) {
            if (it5 != solucao->escolhidos.begin()) {
                std::cout << " ";
            }
            std::cout << (*it5)->id + 1;
        }
        std::cout << "\n" <<  solucao->custo;
    }
    printf("\n");
    return 0;
}