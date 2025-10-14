#pragma once // Evita que o header seja incluído múltiplas vezes

#include <string>
#include <vector>
#include <memory>

constexpr int TAM_POP = 100;
constexpr int N_AULAS = 64;
constexpr int MAX_GEN = 1000;

constexpr int N_SALAS = 20;
constexpr int N_HORARIOS = 20;
constexpr int N_PROFESSORES = 20;
constexpr int N_TURMAS = 64;
constexpr int N_DISCIPLINAS = 64;
constexpr int GEN_POR_CICLO_PREDACAO = 10;

constexpr double TAXA_BASE_MUTACAO = 0.01;

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
    int num_alunos;
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
    std::string observacao;
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
    // Define a observação da aula i.
    void set_observacao(size_t i, std::string obs);
    // Retorna a aula i do cronograma.
    Aula get_aula(size_t i) const;
    // Inclui nova_aula na posição i do cronograma;
    void set_aula(size_t i, Aula nova_aula);
    // Retorna o valor salvo do fitness do indivíduo.
    double get_fitness() const;
    // Imprime o cronograma.
    void imprimir(void) const;
};

class Populacao{
private:
    std::vector<Cronograma> individuos;
    int melhor;
    double fitness_melhor;
    double fitness_melhor_anterior;
    int pior;
    double fitness_pior;
    int gen;

public:
    // Gera a população inicial aleatoriamente.
    Populacao();
    // Cria um JSON com a melhor solução.
    void salvar_melhor_solucao_em_json(const std::string& nome_arquivo) const;
    // Calcula o fitness de todos os indivíduos.
    void calcular_fitness_populacao();
    // Substitui o pior indivíduo por um novo aleatório, e calcula seu fitness.
    void realizar_predacao();
    // Retorna o indivíduo i.
    Cronograma get_individuo(size_t i) const;
    // Retorna o índice do melhor indivíduo.
    int get_melhor() const;
    // Retorna a o número da geração atual da população.
    int get_gen() const;
    // Inicia o algoritmo evolutivo sobre a população.
    void evoluir_populacao();
};