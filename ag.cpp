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

// Cria dados de teste.
void inicializar_dados_de_teste() {
    std::cout << "Inicializando dados de teste..." << std::endl;

    // Garante que os vetores estão vazios antes de começar
    salas.clear();
    horarios.clear();
    professores.clear();
    turmas.clear();
    disciplinas.clear();
    
    // --- Cria Salas ---
    for (int i = 0; i < N_SALAS; ++i) {
        // Gera um nome "Sala 101", "Sala 102", etc.
        std::string numero_sala = "Sala " + std::to_string(101 + i);
        // Capacidade varia de 20 a 70
        int capacidade = 20 + (i * 5); 
        salas.push_back(std::make_shared<Sala>(Sala{i, numero_sala, capacidade}));
    }

    // --- Cria Horarios ---
    for (int i = 0; i < N_HORARIOS; ++i) {
        std::string descricao_horario = "Horario " + std::to_string(i + 1);
        horarios.push_back(std::make_shared<Horario>(Horario{i, descricao_horario}));
    }

    // --- Cria Professores ---
    for (int i = 0; i < N_PROFESSORES; ++i) {
        std::string nome_professor = "Prof. " + std::to_string(i + 1);
        professores.push_back(std::make_shared<Professor>(Professor{i, nome_professor}));
    }

    // --- Cria Turmas ---
    for (int i = 0; i < N_TURMAS; ++i) {
        std::string nome_turma = "Turma " + std::to_string(i + 1);
        turmas.push_back(std::make_shared<Turma>(Turma{i, nome_turma}));
    }

    // --- Cria Disciplinas e as associa a Professores e Turmas ---
    for (int i = 0; i < N_DISCIPLINAS; ++i) {
        // Associa a disciplina 'i' ao professor 'i' e à turma 'i' (exemplo simples)
        // O operador % garante que o índice nunca saia dos limites do vetor.
        auto professor_associado = professores[i % professores.size()];
        auto turma_associada = turmas[i % turmas.size()];

        disciplinas.push_back(std::make_shared<Disciplina>(Disciplina{i, professor_associado, turma_associada}));
    }

    std::cout << "Dados inicializados com sucesso." << std::endl;
    std::cout << "----------------------------------" << std::endl;
}


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
    cout << "----------------------------------" << endl;
    cout << "Inicializando cronograma aleatoriamente." << endl;
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

    cout << "Cronograma inicializado com sucesso." << endl;
    cout << "----------------------------------" << endl;
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
    cout << "----------------------------------" << endl;
    cout << "Calculando fitness de um indivíduo." << endl;
    int conflitos = 0;
    for(size_t i = 0; i < aulas.size()-1; i++){
        for(size_t j = i+1; j < aulas.size(); j++){
        if(aulas[i].horario->id == aulas[j].horario->id){
            if(aulas[i].disciplina->professor->id == aulas[j].disciplina->professor->id){
                conflitos++;
                aulas[i].observacao = "CONFLITO COM AULA " + to_string(j);
            }

            if(aulas[i].disciplina->turma->id == aulas[j].disciplina->turma->id){
                conflitos++;
                aulas[i].observacao = "CONFLITO COM AULA " + to_string(j);
            }

            if(aulas[i].sala->id == aulas[j].sala->id){
                conflitos++;
                aulas[i].observacao = "CONFLITO COM AULA " + to_string(j);
            }
        }
        }
    }
    fitness = funcao_fitness(conflitos);

    cout << "Fitness calculado com sucesso. Total de conflitos: " << conflitos << endl;
    cout << "----------------------------------" << endl;
}

// Define a observação da aula i.
void Cronograma::set_observacao(size_t i, string obs){
    aulas[i].observacao = obs;
}

// Retorna o valor salvo do fitness do indivíduo.
double Cronograma::get_fitness() const {
    return fitness;
}

// Retorna a aula i do cronograma.
Aula Cronograma::get_aula(size_t i) const {
    return aulas[i];
}

// Imprime o cronograma.
void Cronograma::imprimir(void) const {
    // Acessa o membro 'fitness' diretamente
    std::cout << "--- Cronograma (Fitness: " << this->fitness << ") ---" << std::endl;
    std::cout << "Total de Aulas: " << N_AULAS << std::endl;

    // Usa um laço para percorrer e imprimir cada aula
    for (int i = 0; i < N_AULAS; ++i) {
        // Acessa o membro 'aulas' diretamente
        const Aula& aula = this->aulas[i];

        // Checagens de segurança para garantir que os ponteiros não são nulos
        std::string nome_disciplina = aula.disciplina ? std::to_string(aula.disciplina->id) : "N/A";
        std::string nome_professor = (aula.disciplina && aula.disciplina->professor) ? aula.disciplina->professor->nome : "N/A";
        std::string nome_turma = (aula.disciplina && aula.disciplina->turma) ? aula.disciplina->turma->nome : "N/A";
        std::string numero_sala = aula.sala ? aula.sala->numero : "N/A";
        std::string horario_desc = aula.horario ? aula.horario->horario : "N/A";
        std::string aula_observacao = aula.observacao;

        std::cout << "Aula " << i << ": "
                  << "Disciplina " << nome_disciplina
                  << " (Prof: " << nome_professor << ", Turma: " << nome_turma << ")"
                  << " | Sala: " << numero_sala
                  << " | Horario: " << horario_desc
                  << " | Observacao: " << aula_observacao
                  << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

// Gera a população inicial aleatoriamente.
Populacao::Populacao(){
    cout << "Gerando população aleatoriamente." << endl;

    melhor = -1;
    fitness_melhor = -1;
    gen = 0;
    for(int i = 0; i < TAM_POP; i++){
        Cronograma individuo = Cronograma(disciplinas);
        individuos.push_back(individuo);
    }
    cout << "População gerada com sucesso." << endl;
    cout << "----------------------------------" << endl;
}

// Calcula o fitness de todos os indivíduos.
void Populacao::calcular_fitness_populacao(){
    cout << "Fitness da população sendo calculado." << endl;
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

    cout << "Fitness da população calculado." << endl;
    cout << "----------------------------------" << endl;
}

// Retorna o indivíduo i.
Cronograma Populacao::get_individuo(size_t i) const {
    return individuos[i];
}

// Retorna o índice do melhor indivíduo.
int Populacao::get_melhor() const {
    return melhor;
}

int Populacao::get_gen() const {
    return gen;
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
        filho.set_observacao(i, "");
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

    for(; gen < MAX_GEN; gen++){
        cout << "**********************************" << endl;
        cout << "GERAÇÃO " << gen << endl;
        cout << "**********************************" << endl;
     
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

            cout << "----------------------------------" << endl;
            cout << "Cruzamento dos indivíduos " << indice_pai1 << " e " << indice_pai2 << endl;
            filho = uniform_crossover(pai1, pai2);
            cout << "----------------------------------" << endl;

            // IMPLEMENTAR MUTAÇÃO AQUI

            individuos_novos.push_back(filho);
        }

        individuos = individuos_novos;

        calcular_fitness_populacao();

        if(fitness_melhor == 1.0) // Um cronograma válido foi gerado.
            break;
    }

    if(fitness_melhor == 1.0){
        cout << "Indivíduo " << melhor << " tem fitness igual a 1." << endl;
        individuos[melhor].imprimir();
    }
    else
        cout << "Não foi encontrada uma solução." << endl;

    cout << "\n\nImprimindo um cronograma com conflitos:" << endl;
    individuos[2].imprimir();
}

int main(void){
    inicializar_dados_de_teste();
    
    Populacao populacao;
    populacao.evoluir_populacao();
    cout << "Número de gerações: " << populacao.get_gen() << endl;

    return 0;
}