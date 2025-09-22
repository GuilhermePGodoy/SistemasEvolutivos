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
    return 1/(conflitos+1);
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
double Cronograma::get_fitness(){
    return fitness;
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

// Retorna o índice do melhor indivíduo.
int Populacao::get_melhor(){
    return melhor;
}
// Inicia o algoritmo evolutivo sobre a população.
void Populacao::evoluir_populacao(){
 return;
}

int main(void){

    return 0;
}