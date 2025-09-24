#-------------------------------------------------------------------------
# SCRIPT PARA PLOTAR A CONVERGÊNCIA DO FITNESS DO ALGORITMO GENÉTICO
#-------------------------------------------------------------------------

# Passo 1: Defina os nomes dos arquivos e o título do gráfico
arquivo_entrada <- "fitness_por_geracao.txt"
arquivo_saida <- "grafico_convergencia_fitness.png"
titulo_grafico <- "Evolução do Fitness do Melhor Indivíduo por Geração"

#-------------------------------------------------------------------------
# Passo 2: Leia e prepare os dados
#-------------------------------------------------------------------------

# Verifique se o arquivo de dados existe para evitar erros
if (!file.exists(arquivo_entrada)) {
  stop("Erro: O arquivo de dados '", arquivo_entrada, "' nao foi encontrado.")
}

# Leia os dados do arquivo .txt.
# Como não há cabeçalho, header = FALSE.
# col.names dá um nome para a nossa coluna de dados.
dados <- read.table(arquivo_entrada, header = FALSE, col.names = c("Fitness"))

# Crie o eixo X: as gerações.
# Começando da geração 0, como você pediu.
# Se temos N linhas, as gerações vão de 0 a N-1.
dados$Geracao <- 0:(nrow(dados) - 1)

cat("Dados lidos do arquivo '", arquivo_entrada, "': ", nrow(dados), " geracoes.\n", sep="")

#-------------------------------------------------------------------------
# Passo 3: Gere e salve o gráfico
#-------------------------------------------------------------------------

# Abra o dispositivo gráfico para salvar como PNG
png(filename = arquivo_saida, width = 800, height = 600)

# Use a função plot() para criar o gráfico
plot(x = dados$Geracao, y = dados$Fitness,
     type = "b",  # "b" para "both" (pontos e linhas)
     main = titulo_grafico,
     xlab = "Geração",
     ylab = "Melhor Fitness",
     col = "blue", # Cor da linha e dos pontos
     lwd = 2,      # Largura da linha (line width)
     pch = 19,     # Tipo de ponto (círculo sólido)
     ylim = c(0, 1.1) # Garante que o eixo Y vá de 0 a um pouco mais de 1.0
)

# Adiciona uma grade ao fundo para facilitar a leitura
grid()

# Adiciona uma linha de referência em y=1.0 para marcar a solução ótima
abline(h = 1.0, col = "red", lty = 2, lwd = 2)

# Adiciona a legenda
legend("bottomright",
       legend = c("Fitness por Geração", "Solução Ótima (Fitness = 1.0)"),
       col = c("blue", "red"),
       lty = c(1, 2),       # Estilo da linha (sólida, tracejada)
       pch = c(19, NA),    # Estilo do ponto (círculo, nenhum)
       bty = "n")          # Remove a caixa da legenda

# Fecha o dispositivo gráfico, o que efetivamente salva o arquivo PNG
dev.off()

cat("Grafico salvo em '", arquivo_saida, "'.\n", sep="")