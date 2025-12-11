# O problema de alocação de aulas
Em universidades e escolas, aulas precisam receber um horário e uma sala. Tal problema de otimização é complexo e envolve muitas variáveis e restrições. Por isso, algoritmos genéticos são uma ótima opção, dada sua grande capacidade de exploração do espaço de busca. Este trabalho busca desenvolver um algoritmo genético que resolva este problema, baseando-se no artigo (https://egrove.olemiss.edu/cgi/viewcontent.cgi?article=1442&context=etd) e acrescentando estratégias aprendidas durante a disciplina SSC0713 - Sistemas Evolutivos e Aplicados à Robótica, ministrada pelo professor Eduardo do Valle Simões. Por fim, foi desenvolvida também uma versão simplificada para resolver a alocação de aulas usando as disciplinas do ICMC - USP.

# O algoritmo genético
Cada indivíduo da população é representado por um vetor com todas as aulas existentes, e cada uma delas deve receber um horário e uma sala, respeitando restrições leves e duras. As restrições duras devem ser obedecidas para que o cronograma seja válido, enquanto as leves indicam condições favoráveis e que, idealmente, devem ser atendidas.  
Para melhorar o desempenho do algoritmo, diversas técnicas foram implementadas:

## Mutação variável
Quando não há melhora no melhor indivíduo, a taxa de mutação é aumentada, incentivando a diversidade da população, espalhando os indivíduos pelo espaço de busca e possibilitando que novos ótimos locais sejam encontrados.

## Predação
A cada 10 gerações, o pior indíviduo é eliminado e substituído por um aleatório, que traz diversidade à população.

# Visualização dos resultados
Para ver a grade horária, o código em C++ salva a melhor solução em um arquivo chamado melhor_solucao.json, que é usado pelo script gui.py para gerar uma janela com as grades horárias de todas as turmas. Disciplinas que conflitam com outras são exibidas com fundo vermelho.

# Como rodar
Para compilar o código:  
`g++ -Wall -std=c++20 ag.cpp -o ag`  
Para rodar:  
`./ag`  
Para gerar a janela com a grade horária:  
`python3 gui.py`
