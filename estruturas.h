#pragma once // Evita que o header seja incluído múltiplas vezes

#include <string>
#include <vector>
#include <memory>

constexpr int TAM_POP = 100;
constexpr int N_AULAS = 20;
constexpr int MAX_GEN = 1000;

constexpr double TAXA_MUTACAO = 0.01;

struct Sala{
    int id;
    std::string numero;
    int capacidade;
};

struct Horario{
    int id;
    std::string horario;
};

struct Professor{
    int id;
    std::string nome;
};

struct Disciplina{
    int id;
    std::shared_ptr<Professor> professor;
};

struct Turma{
    int id;
    std::string nome;
};

struct Aula{
    std::shared_ptr<Horario> horario;
    std::shared_ptr<Sala> sala;
    std::shared_ptr<Turma> turma;
    std::shared_ptr<Disciplina> disciplina;
};

class Cronograma{
private:
    std::vector<Aula> aulas;
    double fitness;

public:
    // Cria vetor de aulas com as disciplinas e turmas na ordem em que
    // foram passadas nos vectors, com valores aleatórios nos outros campos.
    Cronograma(const std::vector<std::string>& disciplinas, const std::vector<std::string>& turmas);
    // Calcula o valor da função de fitness para o indivíduo e o guarda.
    void calcular_fitness();
    // Retorna o valor salvo do fitness do indivíduo.
    double fitness();
};

class Populacao{
private:
    std::vector<Cronograma> individuos;
    int melhor;
    double fitness_melhor;

public:
    // Gera a população inicial aleatoriamente.
    Populacao();
    // Calcula o fitness de todos os indivíduos.
    void calcular_fitness_populacao();
    // Retorna o índice do melhor indivíduo.
    int melhor();
    // Inicia o algoritmo evolutivo sobre a população.
    void evoluir_populacao();
};