#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include "estruturas.h"

using namespace std;

// Vetores com todos os dados necessários.
vector<shared_ptr<Sala>> salas;
vector<shared_ptr<Horario>> horarios;
vector<shared_ptr<Professor>> professores;
vector<shared_ptr<Turma>> turmas;
vector<shared_ptr<Disciplina>> disciplinas;

int aleatorio(int max) {
    static random_device rd;
    static mt19937 motor(rd());

    uniform_int_distribution<int> distribuicao(0, max-1);

    return distribuicao(motor);
}

// Função de fitness usada para avaliar os indivíduos.
double funcao_fitness(int conflitos){
    return 1.0/(conflitos+1);
}

// Construtor padrão.
Cronograma::Cronograma(){
    fitness = 0.0;
}

// Cria vetor de aulas com as disciplinas na ordem em que
// foram passadas no vector, com valores aleatórios nos outros campos.
Cronograma::Cronograma(const vector<shared_ptr<Disciplina>>& disciplinas){
    int i;
    for(auto disciplina : disciplinas){
        Aula aula;
        i = aleatorio(N_HORARIOS);
        aula.horario = horarios[i];
        i = aleatorio(N_SALAS);
        aula.sala = salas[i];
        aula.disciplina = disciplina;

        aulas.push_back(aula);
    }
}

// Adiciona uma aula ao cronograma.
void Cronograma::adicionar_aula(Aula aula){
    aulas.push_back(aula);
}

// Calcula o valor da função de fitness para o indivíduo e o guarda.
// RESTRIÇÕES:
// Um professor não pode dar duas aulas ao mesmo tempo.
// Uma turma não pode ter duas aulas ao mesmo tempo.
// Uma sala não pode ser usada para duas aulas ao mesmo tempo.
void Cronograma::calcular_fitness(){
    int conflitos = 0;
    for(size_t i = 0; i < aulas.size()-1; i++){
        for(size_t j = i+1; j < aulas.size(); j++){
        if(aulas[i].horario->id == aulas[j].horario->id){
            if(aulas[i].disciplina->professor->id == aulas[j].disciplina->professor->id)
                conflitos ++;
            if(aulas[i].disciplina->turma->id == aulas[j].disciplina->turma->id)
                conflitos ++;
            if(aulas[i].sala->id == aulas[j].sala->id)
                conflitos ++;
        }
        }
    }
    fitness = funcao_fitness(conflitos);
}

// Retorna o valor salvo do fitness do indivíduo.
double Cronograma::get_fitness() const {
    return fitness;
}

// Retorna a aula i do cronograma.
Aula Cronograma::get_aula(size_t i) const {
    return aulas[i];
}

// Gera a população inicial aleatoriamente.
Populacao::Populacao(){
    melhor = -1;
    fitness_melhor = -1;
    for(int i = 0; i < TAM_POP; i++){
        Cronograma individuo = Cronograma(disciplinas);
        individuos.push_back(individuo);
    }
}

// Calcula o fitness de todos os indivíduos.
void Populacao::calcular_fitness_populacao(){
    size_t i = 0;
    fitness_melhor = -1;
    for (auto& individuo : individuos) {
        individuo.calcular_fitness();
        if(individuo.get_fitness() > fitness_melhor){
            melhor = i;
            fitness_melhor = individuo.get_fitness();
        }
        i++;
    }
}

// Retorna o indivíduo i.
Cronograma Populacao::get_individuo(size_t i) const {
    return individuos[i];
}

// Retorna o índice do melhor indivíduo.
int Populacao::get_melhor() const {
    return melhor;
}

// Implementa Torneio para escolher os pais.
size_t Torneio(int grau, const Populacao& pop){
    size_t ind, melhor;
    melhor = (size_t) aleatorio(TAM_POP);
    for(int i = 1; i < grau; i++){
        ind = (size_t) aleatorio(TAM_POP);
        melhor = (pop.get_individuo(ind).get_fitness() > pop.get_individuo(melhor).get_fitness()) ? ind : melhor;
    }

    return melhor;
}

// Gera um filho por meio de uniform crossover.
// Para cada gene (aula), um número aleatório é gerado.
// Se 1, o filho herda o gene do pai1.
// Se 0, herda do pai2.
Cronograma uniform_crossover(const Cronograma& pai1, const Cronograma& pai2){
    Cronograma filho;

    for(int i = 0; i < N_AULAS; i++){
        if(aleatorio(2))
            filho.adicionar_aula(pai1.get_aula(i)); // Se 1, gene i vem do pai1.
        else
            filho.adicionar_aula(pai2.get_aula(i)); // Senão, vem do pai2.
    }

    return filho;
}

// Inicia o algoritmo evolutivo sobre a população.
// Usa Torneio para escolher os pais.
void Populacao::evoluir_populacao(){

    Cronograma filho;
    size_t i;
    size_t indice_pai1, indice_pai2;
    int grau = 2; // Número de indivíduos por torneio.

    calcular_fitness_populacao();

    for(int gen = 0; gen < MAX_GEN; gen++){
     
        vector<Cronograma> individuos_novos;

        // Adiciona o melhor indivíduo à nova população.
        Cronograma melhor_individuo = individuos[melhor];
        individuos_novos.push_back(melhor_individuo);

        // Gera os indivíduos restantes (o melhor já está na população).
        for(i = 0; i < TAM_POP - 1; i++){
            indice_pai1 = Torneio(grau, *this);
            indice_pai2 = Torneio(grau, *this);

            const Cronograma& pai1 = individuos[indice_pai1];
            const Cronograma& pai2 = individuos[indice_pai2];

            filho = uniform_crossover(pai1, pai2);

            // IMPLEMENTAR MUTAÇÃO AQUI

            individuos_novos.push_back(filho);
        }

        individuos = individuos_novos;

        calcular_fitness_populacao();

        if(fitness_melhor == 1.0) // Um cronograma válido foi gerado.
            break;
    }
}

int main(void){

    return 0;
}