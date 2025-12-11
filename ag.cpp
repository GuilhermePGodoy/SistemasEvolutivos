//Para compilar: g++ -std=c++17 -Wall -fopenmp ag.cpp -o ag

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include <random>
#include <fstream>
#include <iomanip>
#include <omp.h>
#include "json.hpp"
#include "estruturas.h"

using namespace std;
using json = nlohmann::json;

// =============== VARIÁVEIS GLOBAIS ===============

double TAXA_MUTACAO = TAXA_BASE_MUTACAO; // Para aplicar mutação variável.
int MULT_MUTACAO = 1;
int MULT_MAX = 2;

// =================================================


// =============== PROTÓTIPOS DE FUNÇÕES ===============

int aleatorio(int max);

// =====================================================


// =============== DADOS ===============

vector<shared_ptr<Sala>> salas;
vector<shared_ptr<Horario>> horarios;
vector<shared_ptr<Professor>> professores;
vector<shared_ptr<Turma>> turmas;
vector<shared_ptr<Disciplina>> disciplinas;

vector<string> dias = {"segunda-feira ", "terça-feira ", "quarta-feira ", "quinta-feira ", "sexta-feira "};
vector<string> horarios_possiveis = {"08:10", "10:10", "14:20", "16:20"};

// ======================================


// =============== INICIALIZAÇÃO DOS DADOS ===============

// Cria dados de teste.
// void inicializar_dados_de_teste() {
//     // std::cout << "Inicializando dados de teste..." << std::endl;

//     // Garante que os vetores estão vazios antes de começar
//     salas.clear();
//     horarios.clear();
//     professores.clear();
//     turmas.clear();
//     disciplinas.clear();
    
//     // --- Cria Salas ---
//     for (int i = 0; i < N_SALAS; ++i) {
//         // Gera um nome "Sala 101", "Sala 102", etc.
//         string numero_sala = "Sala " + to_string(101 + i);

//         // Capacidade varia de 20 a 70
//         int capacidade = 20 + (i * 5); 
//         salas.push_back(std::make_shared<Sala>(Sala{i, numero_sala, capacidade}));
//     }

//     // --- Cria Horarios ---
//     int id = 0;
//     for (auto dia : dias)
//         for (auto horario : horarios_possiveis) {
//             // cout << "id horario " << id << endl;
//             std::string descricao_horario = dia + horario;
//             horarios.push_back(std::make_shared<Horario>(Horario{id, descricao_horario}));
//             id++;
//     }

//     // --- Cria Professores ---
//     for (int i = 0; i < N_PROFESSORES; ++i) {
//         std::string nome_professor = "Prof. " + std::to_string(i + 1);
//         professores.push_back(std::make_shared<Professor>(Professor{i, nome_professor}));
//     }

//     // --- Cria Turmas ---
//     for (int i = 0; i < N_TURMAS; ++i) {
//         std::string nome_turma = "Turma " + std::to_string(i + 1);
//         int num_alunos = 20 + i;
//         turmas.push_back(std::make_shared<Turma>(Turma{i, num_alunos, nome_turma}));
//     }

//     // --- Cria Disciplinas e as associa a Professores e Turmas ---
//     for (int i = 0; i < N_DISCIPLINAS; ++i) {
//         // Associa a disciplina 'i' ao professor 'i' e à turma 'i' (exemplo simples)
//         // O operador % garante que o índice nunca saia dos limites do vetor.
//         auto professor_associado = professores[aleatorio(65000) % professores.size()];
//         auto turma_associada = turmas[aleatorio(65000) % turmas.size()];

//         disciplinas.push_back(std::make_shared<Disciplina>(Disciplina{i, professor_associado, turma_associada}));
//     }

//     // std::cout << "Dados inicializados com sucesso." << std::endl;
//     // std::cout << "----------------------------------" << std::endl;
// }

void ler_arquivo_entrada(const string& nome_arquivo){
    ifstream f(nome_arquivo);
    if(!f.is_open()){
        cerr << "Erro ao abrir o arquivo.\n";
        return;
    }

    salas.clear();
    horarios.clear();
    professores.clear();
    turmas.clear();
    disciplinas.clear();

    map<int, shared_ptr<Professor>> mapa_professores;
    map<int, shared_ptr<Turma>> mapa_turmas;

    json dados = json::parse(f);

    int id = 1;
    for(const auto& sala : dados["salas"]){
        auto ptr = make_shared<Sala>(Sala{
            id,
            sala["numero"].get<string>(),
            sala["campus"].get<int>(),
            sala["tipo"].get<string>(),
            sala["capacidade"].get<int>()
        });
        salas.push_back(ptr);
        id++;
    }

    id = 1;
    for(const auto& horario : dados["horarios"]){
        auto ptr = make_shared<Horario>(Horario{
            id,
            horario["horario"].get<string>()
        });
        horarios.push_back(ptr);
        id++;
    }

    for(const auto& professor : dados["professores"]){
        auto ptr = make_shared<Professor>(Professor{
            professor["id"].get<int>(),
            professor["nome"].get<string>()
        });
        professores.push_back(ptr);

        mapa_professores[professor["id"].get<int>()] = ptr;
    }

    for(const auto& turma : dados["turmas"]){
        auto ptr = make_shared<Turma>(Turma{
            turma["id"].get<int>(),
            turma["turma"].get<string>(),
            turma["numero alunos"].get<int>(),
        });
        turmas.push_back(ptr);

        mapa_turmas[turma["id"].get<int>()] = ptr;
    }

    id = 1;
    for(const auto& disciplina : dados["disciplinas"]){

            // cout << "1 " << disciplina["nome"].get<string>() << endl;
            // cout << "2 " << mapa_professores[disciplina["id_professor"].get<int>()] << endl;
            // cout << "3 " << mapa_turmas[disciplina["id_turma"].get<int>()] << endl;
            // cout << "4 " << disciplina["aulas semana"].get<int>() << endl;
            // cout << "5 " << disciplina["tipo"].get<string>() << endl;

        auto ptr = make_shared<Disciplina>(Disciplina{
            id,
            disciplina["nome"].get<string>(),
            mapa_professores[disciplina["id_professor"].get<int>()],
            mapa_turmas[disciplina["id_turma"].get<int>()],
            disciplina["aulas semana"].get<int>(),
            disciplina["tipo"].get<string>(),
            1 // campus. Aqui não estamos considerando disciplinas no campus 2.
        });
        id++;
    }
}

// ==================================================


// =============== FUNÇÕES AUXILIARES ===============

// Gera número aleatório no range (0, max).
int aleatorio(int max) {
    static random_device rd;
    static mt19937 motor(rd());

    uniform_int_distribution<int> distribuicao(0, max-1);

    return distribuicao(motor);
}

size_t gerar_bitmask(int tam){
    size_t bitmask = 0;
    for(int i = 0; i < tam; i++)
        bitmask = (bitmask << 1) | aleatorio(2);

    return bitmask;
}

// ================================================================


// =============== FUNÇÕES PARA O ALGORITMO GENÉTICO ===============

// Função de fitness usada para avaliar os indivíduos.
double funcao_fitness(int conflitos){
    return 1.0/(conflitos+1);
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
    size_t bitmask = gerar_bitmask(N_AULAS);

    for(int i = 0; i < N_AULAS; i++){
        if((bitmask >> i) & 1)
            filho.adicionar_aula(pai1.get_aula(i)); // Se 1, gene i vem do pai1.
        else
            filho.adicionar_aula(pai2.get_aula(i)); // Senão, vem do pai2.
        
        filho.set_observacao(i, "");
    }

    return filho;
}

// Com chance TAXA_MUTACAO, realiza mutação escolhendo, com igual probabilidade,
// os genes entre o filho original e um indivíduo aleatório.
void realizar_mutacao_1(Cronograma& filho){
    if(aleatorio(100) < TAXA_MUTACAO*100){
        // cout << "----------------------------------" << endl;
        // cout << "Realizando mutação" << endl;

        Cronograma ind_aleatorio = Cronograma(disciplinas);
        size_t bitmask = gerar_bitmask(N_AULAS);

        for(int i = 0; i < N_AULAS; i++){
            if((bitmask >> i) & 1) // Se 1, gene i vem do aleatório.
                filho.set_aula(i, ind_aleatorio.get_aula(i));
        }
        // cout << "Mutação realizada" << endl;
        // cout << "----------------------------------" << endl;
    }
}

// Com chance TAXA_MUTACAO, cada gene vem do indivíduo aleatório.
void realizar_mutacao_2(Cronograma& filho){
    // cout << "Realizando mutação com taxa = " << TAXA_MUTACAO << endl;
    Cronograma ind_aleatorio = Cronograma(disciplinas);
    
    for(int i = 0; i < N_AULAS; i++){
        if((aleatorio(100) < TAXA_MUTACAO*100)) // Se true, gene i vem do aleatório.
            filho.set_aula(i, ind_aleatorio.get_aula(i));
    }
}

// ============================================================


// =============== MÉTODOS DA CLASSE CRONOGRAMA ===============

// Construtor padrão.
Cronograma::Cronograma(){
    fitness = 0.0;
}

// Cria vetor de aulas com as disciplinas na ordem em que
// foram passadas no vector, com valores aleatórios nos outros campos.
Cronograma::Cronograma(const vector<shared_ptr<Disciplina>>& disciplinas){
    // cout << "----------------------------------" << endl;
    // cout << "Inicializando cronograma aleatoriamente." << endl;
    conflitos_leves = conflitos_duros = 0;

    for(auto disciplina : disciplinas){
        for(int i = 0; i < disciplina->num_aulas_semana; i++){
            Aula aula;
            i = aleatorio(N_HORARIOS);
            aula.horario = horarios[i];
            i = aleatorio(N_SALAS);
            aula.sala = salas[i];
            aula.disciplina = disciplina;

            aulas.push_back(aula);
        }
    }

    // cout << "Cronograma inicializado com sucesso." << endl;
    // cout << "----------------------------------" << endl;
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
    // cout << "----------------------------------" << endl;
    // cout << "Calculando fitness de um indivíduo." << endl;
    int PESO_CONFLITOS_DUROS = 100, PESO_CONFLITOS_LEVES = 1;
    conflitos_leves = conflitos_duros = 0;
    int penalidade = 0, tem_conflito = 0;

    for(size_t i = 0; i < aulas.size()-1; i++){

        // HARD CONSTRAINT
        //Conflitos relacionados à capacidade das salas.
        if(aulas[i].sala->capacidade < aulas[i].disciplina->turma->num_alunos){
            conflitos_duros++;
            tem_conflito++;
        }

        //Relação com o campus no qual a aula foi alocada.
        if(aulas[i].sala->campus != aulas[i].disciplina->campus){
            conflitos_duros++;
            tem_conflito++;
        }

        // Relação com o tipo da aula.
        if(aulas[i].sala->tipo != aulas[i].disciplina->tipo){
            conflitos_duros++;
            tem_conflito++;
        }

        for(size_t j = i+1; j < aulas.size(); j++){
        tem_conflito = 0;

        // HARD CONSTRAINTS
        // Conflitos relacionados a horários.
        if(aulas[i].horario->id == aulas[j].horario->id){ 
            // Mesmo professor dando duas aulas ao mesmo tempo.
            if(aulas[i].disciplina->professor->id == aulas[j].disciplina->professor->id){
                conflitos_duros++;
                tem_conflito++;
            }

            // Mesma turma tendo duas aulas ao mesmo tempo.
            if(aulas[i].disciplina->turma->id == aulas[j].disciplina->turma->id){
                conflitos_duros++;
                tem_conflito++;
            }

            // Mesma sala sendo usada para duas aulas ao mesmo tempo.
            if(aulas[i].sala->id == aulas[j].sala->id){
                conflitos_duros++;
                tem_conflito++;
            }

            // SOFT CONSTRAINTS

        }
        
        // SOFT CONSTRAINT
        // Aulas em campi diferentes preferencialmente não são consecutivas.
        if((aulas[i].disciplina->professor->id == aulas[j].disciplina->professor->id
                || aulas[i].disciplina->turma->id == aulas[j].disciplina->turma->id) &&
            ((aulas[j].horario->id % 2 == 1 && aulas[i].horario->id == aulas[j].horario->id + 1)
                || (aulas[i].horario->id % 2 == 1 && aulas[j].horario->id == aulas[i].horario->id + 1))){
            
            if((aulas[i].disciplina->campus != aulas[i].disciplina->campus)){
                conflitos_leves++;
            }
        }

        if(tem_conflito)
            aulas[i].observacao = (aulas[i].observacao == "")
                                  ? "CONFLITO COM AULAS " + to_string(j) + "(" + to_string(tem_conflito) + ")"
                                  : aulas[i].observacao + ", " + to_string(j) + "(" + to_string(tem_conflito) + ")";
        }
    }
    penalidade = conflitos_leves*PESO_CONFLITOS_LEVES + conflitos_duros*PESO_CONFLITOS_DUROS;
    fitness = funcao_fitness(penalidade);

    // cout << "Fitness calculado com sucesso. Total de conflitos: " << conflitos << endl;
    // cout << "----------------------------------" << endl;
}

// Define a observação da aula i.
void Cronograma::set_observacao(size_t i, string obs){
    aulas[i].observacao = obs;
}

// Retorna o valor salvo do fitness do indivíduo.
double Cronograma::get_fitness() const {
    return fitness;
}

// Retorna número de conflitos leves.
int Cronograma::get_conflitos_leves() const {
    return conflitos_leves;
}

// Retorna número de conflitos duros.
int Cronograma::get_conflitos_duros() const {
    return conflitos_duros;
}

// Retorna a aula i do cronograma.
Aula Cronograma::get_aula(size_t i) const {
    return aulas[i];
}

// Inclui nova_aula na posição i do cronograma;
void Cronograma::set_aula(size_t i, Aula nova_aula){
    aulas[i] = nova_aula;
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
        std::string nome_turma = (aula.disciplina && aula.disciplina->turma) ? aula.disciplina->turma->turma : "N/A";
        std::string numero_sala = aula.sala ? aula.sala->numero : "N/A";
        std::string horario_desc = aula.horario ? aula.horario->horario : "N/A";
        std::string aula_observacao = aula.observacao;

        std::cout << "Aula " << i << ": "
                  << "Disciplina " << nome_disciplina
                  << " (Prof: " << nome_professor << ", Turma: " << nome_turma << ")"
                  << " | Sala: " << numero_sala
                  << " | Horario: " << aula.horario->id << " " << horario_desc
                  << " | Observacao: " << aula_observacao
                  << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

// ===========================================================


// =============== MÉTODOS DA CLASSE POPULAÇÃO ===============

// Gera um JSON com os dados do melhor indivíduo, para plotar a GUI.
void Populacao::salvar_solucao_em_json(const string& nome_arquivo, size_t ind) const {
    if (this->melhor == -1) {
        std::cout << "Nenhuma solucao para salvar." << std::endl;
        return;
    }

    const Cronograma& melhor_cronograma = this->individuos[ind];
    json json_output;
    json_output["fitness"] = melhor_cronograma.get_fitness();
    
    json_output["aulas"] = json::array();

    // Loop para verificar conflitos antes de salvar
    std::vector<bool> tem_conflito(N_AULAS, false);
    for (size_t i = 0; i < N_AULAS; ++i) {
        for (size_t j = i + 1; j < N_AULAS; ++j) {
            const Aula& a1 = melhor_cronograma.get_aula(i);
            const Aula& a2 = melhor_cronograma.get_aula(j);
            if (a1.horario->id == a2.horario->id) {
                if (a1.disciplina->professor->id == a2.disciplina->professor->id ||
                    a1.disciplina->turma->id == a2.disciplina->turma->id ||
                    a1.sala->id == a2.sala->id) {
                    tem_conflito[i] = true;
                    tem_conflito[j] = true;
                }
            }
        }
    }

    // Adiciona cada aula ao JSON
    for (size_t i = 0; i < N_AULAS; ++i) {
        const Aula& aula = melhor_cronograma.get_aula(i);
        json aula_json;
        aula_json["disciplina"] = "Disc " + std::to_string(aula.disciplina->id);
        aula_json["professor"] = aula.disciplina->professor->nome;
        aula_json["turma"] = aula.disciplina->turma->turma;
        aula_json["sala"] = aula.sala->id;
        aula_json["horario"] = aula.horario->horario;
        aula_json["tem_conflito"] = tem_conflito[i]; // Adiciona a flag de conflito
        json_output["aulas"].push_back(aula_json);
    }
    
    // Salva o JSON em um arquivo
    std::ofstream arquivo(nome_arquivo);
    arquivo << std::setw(4) << json_output << std::endl;
    std::cout << "Solucao salva em " << nome_arquivo << std::endl;
}

// Gera a população inicial aleatoriamente.
Populacao::Populacao(){
    // cout << "Gerando população aleatoriamente." << endl;

    melhor = -1;
    fitness_melhor_anterior = -1;
    fitness_melhor = -1;
    gen = 0;
    for(int i = 0; i < TAM_POP; i++){
        Cronograma individuo = Cronograma(disciplinas);
        individuos.push_back(individuo);
    }
    // cout << "População gerada com sucesso." << endl;
    // cout << "----------------------------------" << endl;
}

// Calcula o fitness de todos os indivíduos.
void Populacao::calcular_fitness_populacao(){
    // cout << "Fitness da população sendo calculado." << endl;
    size_t i = 0;
    fitness_melhor_anterior = fitness_melhor;
    fitness_melhor = -1;
    fitness_pior = 2;
    for (auto& individuo : individuos) {
        individuo.calcular_fitness();
        if(individuo.get_fitness() > fitness_melhor){
            melhor = i;
            fitness_melhor = individuo.get_fitness();
        }

        if(individuo.get_fitness() < fitness_pior){
            pior = i;
            fitness_pior = individuo.get_fitness();
        }
        i++;
    }

    // cout << "Fitness da população calculado." << endl;
    // cout << "melhor: " << fitness_melhor << " melhor_anterior: " << fitness_melhor_anterior << endl;
    // cout << "----------------------------------" << endl;
}

// Retorna o indivíduo i.
Cronograma Populacao::get_individuo(size_t i) const {
    return individuos[i];
}

// Retorna o índice do melhor indivíduo.
int Populacao::get_melhor() const {
    return melhor;
}

// Retorna o índice do pior indivíduo.
int Populacao::get_pior() const {
    return pior;
}

int Populacao::get_gen() const {
    return gen;
}

// Substitui o pior indivíduo por um novo aleatório, e calcula seu fitness.
void Populacao::realizar_predacao(){
    individuos[pior] = Cronograma(disciplinas);
    calcular_fitness_populacao();
}

// Realiza a predação por síntese:
//  Elimina o pior indivíduo;
//  Cria um indivíduo com cada gene mais frequente na população inteira.
void Populacao::realizar_predacao_sintese(){
    Cronograma novo_individuo;

    cout << "--------------------------------------------------------------" << endl;
    cout << "Fitness do pior antes da predação: " << individuos[pior].get_fitness() << endl;

    for(int aula = 0; aula < N_AULAS; aula++){
        int comparador_genes[TAM_POP] = {}; // comparador_genes[i] = 1 => ind[i] tem aula atual igual à de alguém.
        int maior = 0; // indice do indivíduo com a aula que mais se repete; se tudo diferente, pega o 0.
        int num_aulas_maior = 0; // número de vezes da aula que mais se repete.
        for(int ind = 0; ind < TAM_POP; ind++){
            int num_iguais = 1; // número de vezes que a aula do individuo[ind] se repete na população.
            
            if(comparador_genes[ind] == 1) // indivíduo atual já tem aula igual a alguém.
                continue;

            //cout << "Analisando individuo " << ind << endl;

            // Achamos um indivíduo com uma aula nova
            comparador_genes[ind] = 1; // aula do ind é igual à dele mesmo.
            for(int outro_ind = ind+1; outro_ind < TAM_POP; outro_ind++){
                if(comparador_genes[outro_ind] == 1)
                    continue;

                if(individuos[outro_ind].get_aula(aula) == individuos[ind].get_aula(aula)){
                    comparador_genes[outro_ind] = 1;
                    num_iguais++;

                    //cout << "ind[" << outro_ind << "] tem aula igual a ind[" << ind << "]\n";
                }
            }

            if(num_iguais > num_aulas_maior){
                maior = ind;
                num_aulas_maior = num_iguais;
            }
        }
        //cout << "indivíduo com mais aulas iguais à dele: " << maior << " num de iguais: " << num_aulas_maior << endl;

        novo_individuo.adicionar_aula(individuos[maior].get_aula(aula));
    }

    individuos[pior] = novo_individuo;
    calcular_fitness_populacao();
    //individuos[pior].imprimir();
    cout << "Fitness do pior depois da predação: " << individuos[pior].get_fitness() << endl;

    cout << "--------------------------------------------------------------" << endl;
}

// Inicia o algoritmo evolutivo sobre a população.
// Usa Torneio para escolher os pais.
void Populacao::evoluir_populacao(){

    Cronograma filho;
    size_t i;
    size_t indice_pai1, indice_pai2;
    int grau = 2; // Número de indivíduos por torneio.

    ofstream arquivo_fitness("fitness_por_geracao.txt");

    calcular_fitness_populacao();

    for(; gen < MAX_GEN; gen++){

        // Aplica a técnica de taxa de mutação variável.
        // cout << "----------------------TAXAS-----------------------" << endl; 
        // cout << fitness_melhor << " " << fitness_melhor_anterior << endl;
        // cout << "--------------------------------------------------" << endl; 
        if(fitness_melhor == fitness_melhor_anterior){ // Aumenta a taxa de mutação se não houve mudança no melhor...
            if(MULT_MUTACAO < MULT_MAX){ // ... até um dado limite.
                MULT_MUTACAO++;
                TAXA_MUTACAO = TAXA_BASE_MUTACAO*MULT_MUTACAO;
            }
        }
        else{ // Restaura a taxa de mutação original se houve mudança no melhor.
            MULT_MUTACAO = 1;
            TAXA_MUTACAO = TAXA_BASE_MUTACAO;
        }
        // cout << "**********************************" << endl;
        // cout << "GERAÇÃO " << gen << endl;
        // cout << "**********************************" << endl;

        // Realização da predação periodicamente.
        if(!(gen % GEN_POR_CICLO_PREDACAO))
            realizar_predacao_sintese();

        arquivo_fitness << fitness_melhor << "\n";
     
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

            // cout << "----------------------------------" << endl;
            // cout << "Cruzamento dos indivíduos " << indice_pai1 << " e " << indice_pai2 << endl;
            filho = uniform_crossover(pai1, pai2);
            // cout << "----------------------------------" << endl;

            // Mutação.
            // Com probabilidade TAXA_MUTACAO, gera um indivíduo aleatório
            // que passará alguns genes para o filho gerado.
            realizar_mutacao_2(filho);

            individuos_novos.push_back(filho);
        }

        individuos = individuos_novos;

        calcular_fitness_populacao();

        //if(individuos[melhor].get_conflitos_duros() == 0){
        //    cout << "Melhor não tem conflitos duros. Número de conflitos leves: " 
        //         << individuos[melhor].get_conflitos_leves() << endl;
        //}

        if(fitness_melhor == 1.0){ // Um cronograma válido foi gerado.
            arquivo_fitness << fitness_melhor;
            break;
        }
    }

    // if(fitness_melhor == 1.0)
    //     cout << "Indivíduo " << melhor << " tem fitness igual a 1." << endl;
    // else
    //     cout << "Não foi encontrada uma solução." << endl;

    // cout << "Melhor indivíduo:" << endl;
    // individuos[melhor].imprimir();
    // cout << "\n\n Pior indivíduo:" << endl;
    // individuos[pior].imprimir();
}

// =============================================

int simulacao(){
    Populacao populacao;
        
    populacao.evoluir_populacao();
    // cout << "------------------------" << endl;
    // cout << "Número de gerações: " << populacao.get_gen() << endl;
    // cout << "Fitness do melhor indivíduo: " << populacao.get_individuo(populacao.get_melhor()).get_fitness() << endl;
    // cout << "------------------------" << endl;

    // Salva o melhor cronograma em um JSON para uso na interface gráfica.
    populacao.salvar_solucao_em_json("melhor_solucao.json", populacao.get_melhor());
    // Salva o pior em outro arquivo para ver os conflitos.
    populacao.salvar_solucao_em_json("pior_solucao.json", populacao.get_pior());

    return populacao.get_gen();
}

int main(void){
    //inicializar_dados_de_teste(); // Modifica variáveis globais; tem que estar fora da região paralela.

    ler_arquivo_entrada("dadosICMC.json");

    // SIMULAÇÃO PARALELA
    // Cria 8 threads que evoluirão populações independentemente.
    // #pragma omp parallel num_threads(8)
    // {
    //     simulacao();
    // }


    // CÓDIGO DE TESTES
    // int NUM_SIMULACOES = 100;
    // int soma_gen;
    // vector<int> maximos_mutacao = {1, 2, 3};
    // for(auto max : maximos_mutacao){
    //     MULT_MAX = max;
    //     soma_gen = 0;
    //     cout << "Com multiplicador máximo de " << MULT_MAX << ":" << endl;
    //     for(int i = 0; i < NUM_SIMULACOES; i++){
    //         soma_gen += simulacao();
    //     }
    //     cout << "Média de número de gerações: " << (double) soma_gen/NUM_SIMULACOES << endl;
    // }

    cout << "Número de gerações: " << simulacao() << endl;

    return 0;
}