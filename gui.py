import tkinter as tk
from tkinter import ttk
import json

# --- Constantes para a Grade ---
DIAS_SEMANA = ["segunda-feira ", "terça-feira ", "quarta-feira ", "quinta-feira ", "sexta-feira "]
HORARIOS_AULA = ["08:10", "10:10", "14:20", "16:20"]
COR_CONFLITO = "#E57373"  # Um tom de vermelho claro
COR_NORMAL = "#F0F0F0"    # Cor de fundo padrão do sistema

def carregar_dados_solucao(caminho_arquivo):
    """Lê o arquivo JSON e processa os dados, agrupando as aulas por turma."""
    try:
        with open(caminho_arquivo, 'r', encoding='utf-8') as f:
            dados = json.load(f)
    except FileNotFoundError:
        print(f"Erro: Arquivo '{caminho_arquivo}' nao encontrado.")
        return None, {}

    # Agrupa as aulas pela turma
    aulas_por_turma = {}
    for aula in dados.get("aulas", []):
        turma = aula.get("turma")
        if turma not in aulas_por_turma:
            aulas_por_turma[turma] = []
        aulas_por_turma[turma].append(aula)
        
    return dados.get("fitness"), aulas_por_turma

def criar_interface():
    """Cria e configura a janela principal com as abas e a grade horária."""
    fitness, aulas_por_turma = carregar_dados_solucao("pior_solucao.json")

    if not aulas_por_turma:
        return

    # --- Criação da Janela Principal ---
    root = tk.Tk()
    root.title(f"Visualizador de Grade Horária (Fitness: {fitness:.4f})")
    root.geometry("900x400")

    # --- Criação do Widget de Abas (Notebook) ---
    notebook = ttk.Notebook(root)
    notebook.pack(expand=True, fill='both', padx=10, pady=10)

    # Mapeia a descrição do horário para a posição na grade (linha, coluna)
    mapa_horarios = {
        "08:10": 0, "10:10": 1, "14:20": 2, "16:20": 3
    }
    mapa_dias = {
        "SEG": 0, "TER": 1, "QUA": 2, "QUI": 3, "SEX": 4
    }

    # --- Loop para Criar uma Aba para Cada Turma ---
    for turma, aulas in sorted(aulas_por_turma.items()):
        # Cria um Frame para ser o conteúdo da aba
        frame_aba = ttk.Frame(notebook, padding="10")
        notebook.add(frame_aba, text=turma)

        # --- Cria a Grade (Grid) dentro do Frame ---
        # Headers dos dias da semana
        for col, dia in enumerate(DIAS_SEMANA):
            label_dia = ttk.Label(frame_aba, text=dia, font=("Arial", 10, "bold"), anchor="center")
            label_dia.grid(row=0, column=col + 1, sticky="nsew", ipadx=5, ipady=5)

        # Headers dos horários
        for row, horario in enumerate(HORARIOS_AULA):
            label_horario = ttk.Label(frame_aba, text=horario, font=("Arial", 10, "bold"), anchor="center")
            label_horario.grid(row=row + 1, column=0, sticky="nsew", ipadx=5, ipady=5)
        
        # Configura as colunas e linhas para expandirem com a janela
        for i in range(len(DIAS_SEMANA) + 1):
            frame_aba.grid_columnconfigure(i, weight=1)
        for i in range(len(HORARIOS_AULA) + 1):
            frame_aba.grid_rowconfigure(i, weight=1)

        # --- Preenche a Grade com as Aulas ---
        for aula in aulas:
            try:
                # Extrai o dia e a hora da string de horário (ex: "SEG 08:10")
                dia_str, hora_str = aula["horario"].split(' ')
                
                row = mapa_horarios.get(hora_str)
                col = mapa_dias.get(dia_str)
                
                if row is not None and col is not None:
                    cor_fundo = COR_CONFLITO if aula["tem_conflito"] else COR_NORMAL
                    
                    # Cria um frame para o slot para controlar cor e borda
                    slot_frame = tk.Frame(frame_aba, background=cor_fundo, borderwidth=1, relief="solid")
                    slot_frame.grid(row=row + 1, column=col + 1, sticky="nsew", padx=1, pady=1)

                    # Formata o texto a ser exibido
                    texto_aula = (f"{aula['disciplina']}\n"
                                  f"{aula['professor']}\n"
                                  f"Sala: {aula['sala']}")
                    
                    label_aula = ttk.Label(slot_frame, text=texto_aula, background=cor_fundo, anchor="center", justify="center")
                    label_aula.pack(expand=True, fill="both", padx=5, pady=5)
                    
            except (ValueError, KeyError) as e:
                print(f"Aviso: Ignorando aula com horario invalido '{aula.get('horario')}'. Erro: {e}")

    # Inicia o loop principal da interface
    root.mainloop()

if __name__ == "__main__":
    criar_interface()