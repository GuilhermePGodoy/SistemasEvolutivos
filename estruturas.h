#pragma once // Evita que o header seja incluído múltiplas vezes

#include <string>
#include <vector>
#include <memory>

constexpr int TAM_POP = 100;
constexpr int N_AULAS = 20;
constexpr int MAX_GEN = 1000;

constexpr int N_SALAS = 10;
constexpr int N_HORARIOS = 10;
constexpr int N_PROFESSORES = 10;
constexpr int N_TURMAS = 10;
constexpr int N_DISCIPLINAS = 10;

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

struct Turma{
    int id;
    std::string nome;
};

struct Disciplina{
    int id;
    std::shared_ptr<Professor> professor;
    std::shared_ptr<Turma> turma;
};

struct Aula{
    std::shared_ptr<Horario> horario;
    std::shared_ptr<Sala> sala;
    std::shared_ptr<Disciplina> disciplina;
};

class Cronograma{
private:
    std::vector<Aula> aulas;
    double fitness;

public:
    // Construtor padrão.
    Cronograma();
    // Cria vetor de aulas com as disciplinas na ordem em que
    // foram passadas no vector, com valores aleatórios nos outros campos.
    Cronograma(const std::vector<std::shared_ptr<Disciplina>>& disciplinas);
    // Adiciona uma aula ao cronograma.
    void adicionar_aula(Aula aula);
    // Calcula o valor da função de fitness para o indivíduo e o guarda.
    void calcular_fitness();
    // Retorna a aula i do cronograma.
    Aula get_aula(size_t i) const;
    // Retorna o valor salvo do fitness do indivíduo.
    double get_fitness() const;
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
    // Retorna o indivíduo i.
    Cronograma get_individuo(size_t i) const;
    // Retorna o índice do melhor indivíduo.
    int get_melhor() const;
    // Inicia o algoritmo evolutivo sobre a população.
    void evoluir_populacao();
};