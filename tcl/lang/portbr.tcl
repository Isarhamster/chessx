# portbr.tcl:
# Scid in Brazilian Portuguese.
# Translated by Gilberto de Almeida Peres.

proc setLanguage_B {} {

# File menu:
menuText B File "Arquivo" 0
menuText B FileNew "Novo..." 0 {Cria uma nova base de dados Scid}
menuText B FileOpen "Abrir..." 0 {Abre uma base de dados Scid existente}
menuText B FileClose "Fechar" 0 {Fecha a base de dados Scid ativa}
menuText B FileFinder "Buscador" 0 {Abre a janela do Buscador de Arquivos}
menuText B FileBookmarks "Favoritos" 0 {Menu de Favoritos (atalho: Ctrl+B)}
menuText B FileBookmarksAdd "Adicionar a Favoritos" 0 \
  {Adiciona o posicao do jogo do banco de dados atual}
menuText B FileBookmarksFile "Arquivar Favorito" 0 \
  {Arquiva um Favorito para a posicao do jogo atual}
menuText B FileBookmarksEdit "Editar favoritos..." 0 \
  {Editar o menu de favoritos}
menuText B FileBookmarksList "Mostrar pastas como lista" 0 \
  {Mostra as pastas de favoritos em lista unica}
menuText B FileBookmarksSub "Mostrar pastas como submenus" 0 \
  {Mostra as pastas de favoritos como submenus}
menuText B FileMaint "Manutencao" 0 {Ferramentas de manutencao de bases de dados Scid}
menuText B FileMaintWin "Janela de Manutencao" 0 \
  {Abre/Fecha a janela de manutencao de bases de dados Scid}
menuText B FileMaintCompact "Compactar base de dados..." 0 \
  {Compacta arquivos de bases de dados, removendo jogos deletados e nomes nao utilizados}
menuText B FileMaintClass "Classificar jogos por ECO..." 2 \
  {Recalcula o codigo ECO de todos os jogos}
menuText B FileMaintSort "Ordenar base de dados..." 0 \
  {Ordena todos os jogos da base de dados}
menuText B FileMaintDelete "Apagar jogos duplicados..." 13 \
  {Encontra jogos duplicados e os marca para exclusao}
menuText B FileMaintTwin "Janela de verificacao de duplicatas" 10 \
  {Abre/atualiza a janela de verificacao de duplicatas}
menuText B FileMaintName "Ortografia de nomes" 14 {Ferramentas de edicao e correcao ortografica de nomes}
menuText B FileMaintNameEditor "Editor de Nomes" 0 \
  {Abre/fecha a janela do editor de nomes}
menuText B FileMaintNamePlayer "Verificacao Ortografica de Nomes de Jogadores..." 11 \
  {Verifica a correcao dos nomes dos jogadores de acordo com o arquivo de correcao ortografica}
menuText B FileMaintNameEvent "Verificacao Ortografica de Nomes de Eventos..." 11 \
  {Verifica a correcao dos nomes de eventos de acordo com o arquivo de verificacao ortografica}
menuText B FileMaintNameSite "Verificacao Ortografica de Lugares..." 11 \
  {Verifica a correcao dos nomes de lugares usando o arquivo de correcao ortografica}
menuText B FileMaintNameRound "Verificacao Ortografica de Rodadas..." 11 \
  {Verificacao dos nomes de rodadas usando o arquivo de correcao ortografica}
menuText B FileReadOnly "Apenas Leitura..." 7 \
  {Trata a base de dados corrente como arquivo de leitura, impedindo mudancas}
menuText B FileSwitch "Mudar a base de dados" 0 \
  {Muda para uma base de dados diferente, já aberta} 
menuText B FileExit "Sair" 0 {Encerrar o Scid}
menuText B FileMaintFixBase "Corrigir base corrompida" 0 {Tenta corrigir uma base corrompida}

# Edit menu:
menuText B Edit "Editar" 0
menuText B EditAdd "Adiciona variante" 0 {Adiciona variante do movimento}
menuText B EditDelete "Deleta Variante" 0 {Exclui variante do movimento}
menuText B EditFirst "Converte para Primeira Variante" 14 \
  {Faz com que uma variante seja a primeira da lista}
menuText B EditMain "Converte variante para Linha Principal" 24 \
  {Faz com que uma variante se torne a Linha Principal}
menuText B EditTrial "Experimentar variante" 0 \
  {Inicia/Para experimentacao, para testar alguma nova ideia no tabuleiro}
menuText B EditStrip "Limpar Comentarios e Variantes" 2 \
  {Limpa comentarios e variantes no jogo atual}
menuText B EditUndo "Desfazer" 0 {Desfaz última mudança no jogo}
menuText B EditRedo "Repetir" 0 {Repete última mudança no jogo}
menuText B EditStripComments "Limpar Comentarios" 0 \
  {Limpa comentarios e anotacoes no jogo atual}
menuText B EditStripVars "Limpar Variantes" 0 \
  {Limpa todas as variantes no jogo atual}
menuText B EditStripBegin "Movimentos a partir do início" 1 \
  {Remove movimentos a partir do início do jogo} 
menuText B EditStripEnd "Movimentos até o final do jogo" 0 \
  {Remove movimentos até o final do jogo} 
menuText B EditReset "Limpar a base de trabalho" 0 \
  {Limpa completamente a base de trabalho}
menuText B EditCopy "Copiar jogo para a base de trabalho" 0 \
  {Copia o jogo corrente para a base de trabalho}
menuText B EditPaste "Colar jogo da base de trabalho" 1 \
  {Cola o jogo ativo da base de trabalho}
menuText B EditPastePGN "Colar como jogo em PGN..." 10 \
  {Interpreta o texto na área de transferência como um jogo em notação PGN e cola na posição corrente} 
menuText B EditSetup "Configura posicao inicial..." 12 \
  {Configura a posicao inicial para o jogo}
menuText B EditCopyBoard "Copia posição" 6 \
  {Copia o tabuleiro corrente em notação FEN para a área de transferência} 
menuText B EditPasteBoard "Colar Posicao" 12 \
  {Configura a posicao inicial a partir da area de transferencia}
# ====== TODO To be translated ======
menuText B ConfigureScid "Preferences..." 0 {Configure all options for SCID}

# Game menu:
menuText B Game "Jogo" 0
menuText B GameNew "Limpar Jogo" 0 \
  {Limpa o jogo corrente, descartando qualquer alteracao}
menuText B GameFirst "Primeiro Jogo" 5 {Carrega o primeiro jogo filtrado}
menuText B GamePrev "Jogo Anterior" 5 {Carrega o jogo anterior}
menuText B GameReload "Recarrega o Jogo atual" 3 \
  {Recarrega o jogo, descartando qualquer alteracao}
menuText B GameNext "Proximo Jogo" 5 {Carrega o proximo jogo}
menuText B GameLast "Ultimo Jogo" 8 {Carrega o ultimo jogo}
menuText B GameRandom "Carrega jogo aleatório" 8 {Carrega um jogo filtrado aleatoriamente} 
menuText B GameNumber "Carrega Jogo Numero..." 5 \
  {Carrega um jogo pelo seu numero}
menuText B GameReplace "Salvar: Substituir Jogo..." 8 \
  {Salva o jogo e substitui a versao antiga}
menuText B GameAdd "Salvar: Adicionar Jogo..." 9 \
  {Salva este jogo como um novo jogo na base de dados}
menuText B GameDeepest "Identificar Abertura" 0 \
  {Vai para a posicao mais avancada da partida, de acordo com o codigo ECO}
menuText B GameGotoMove "Ir para o movimento numero..." 5 \
  {Avanca o jogo ate o movimento desejado}
menuText B GameNovelty "Pesquisa Novidade..." 7 \
  {Procura o primeiro movimento deste jogo que nao tenha sido jogado antes}

# Search Menu:
menuText B Search "Pesquisa" 0
menuText B SearchReset "Limpar Filtragem" 0 {Limpa o criterio de pesquisa para incluir todos os jogos}
menuText B SearchNegate "Inverter Filtragem" 0 {Inverte o criterio de pesquisa para incluir apenas os jogos que nao atendem o criterio}
menuText B SearchCurrent "Posicao Atual..." 0 {Pesquisa a posicao atual do tabuleiro}
menuText B SearchHeader "Cabecalho..." 0 {Pesquisa por cabecalho (jogador, evento, etc)}
menuText B SearchMaterial "Material/Padrao..." 0 {Pesquisa por material ou padrao de posicao}
menuText B SearchUsing "Usar arquivo de opcoes de filtro..." 0 {Pesquisa usando arquivo com opcoes de filtro}

# Windows menu:
menuText B Windows "Janelas" 0
menuText B WindowsComment "Editor de Comentarios" 0 {Abre/fecha o editor de comentarios}
menuText B WindowsGList "Lista de Jogos" 0 {Abre/fecha a janela com a lista de jogos}
menuText B WindowsPGN "Notacao PGN" 0 \
  {Abre/fecha a janela com a notacao PGN do jogo}
menuText B WindowsPList "Procura Jogador" 2 {Abre/Fecha o buscador de jogadores} 
menuText B WindowsTmt "Buscador de Torneio" 2 {Abre/Fecha o buscador de torneio}
menuText B WindowsSwitcher "Intercambio de bases de dados" 0 \
  {Abre/fecha a janela de intercambio de bases de dados}
menuText B WindowsMaint "Manutencao" 0 \
  {Abre/fecha a janela de manutencao}
menuText B WindowsECO "Listagem ECO" 0 {Abre/fecha a janela de listagem de codigo ECO}
menuText B WindowsStats "Estatisticas" 0 \
  {Abre/fecha a janela de estatisticas}
menuText B WindowsTree "Arvore" 0 {Abre/fecha a janela da Arvore de pesquisa}
menuText B WindowsTB "Tabela base de Finais" 1 \
  {Abre/fecha a janela da tabela base de finais}
menuText B WindowsBook "Janela do Livro de Aberturas" 0 {Abre/Fecha  a janela do livro de aberturas}
menuText B WindowsCorrChess "Janela de Correspondencia" 0 {Abre/Fecha a janela de correspondencia}

# Tools menu:
menuText B Tools "Ferramentas" 0
menuText B ToolsAnalysis "Analisador #1..." 0 \
  {Inicia ou para o 1o. Analisador}
menuText B ToolsAnalysis2 "Analisador #2..." 17 \
  {Inicia ou para o 2o. Analisador}
menuText B ToolsCross "Tabela de Cruzamento" 0 {Mostra a tabela de cruzamentos do torneio para o jogo corrente}
menuText B ToolsEmail "Gerenciador de e-mails" 0 \
  {Abre/fecha a janela do gerenciador de e-mails}
menuText B ToolsFilterGraph "Gréfico de filtro" 7 \
  {Abre/Fecha a janela do gráfico de filtro} 
menuText B ToolsAbsFilterGraph "Grafico de Filtro Absoluto" 7 {Abre/Fecha a janela do grafico de filtro por valor absoluto}
menuText B ToolsOpReport "Relatorio de abertura" 0 \
  {Gera um relatorio de abertura para a posicao corrente}
menuText B ToolsOpenBaseAsTree "Abre base como árvore" 0   {Abre uma base e a utiliza numa janela de Árvore}
menuText B ToolsOpenRecentBaseAsTree "Abre base recente como árvore" 0   {Abre uma base recente e a utiliza numa janela de Árvore}
menuText B ToolsTracker "Acompanha Peça"  0 {Abre a janela de acompanhamento de peças} 
menuText B ToolsTraining "Treinamento"  0 {Ferramentas de Treinamento (táticas, aberturas,...) }
menuText B ToolsTacticalGame "Jogo Tático"  0 {Jogar com táticas}
menuText B ToolsSeriousGame "Jogo Sério"  0 {Jogar um jogo sério}
menuText B ToolsTrainOpenings "Aberturas"  0 {Treinamento com um repertório}
menuText B ToolsTrainReviewGame "Rever jogo"  0 {Acompanhar movimentos de um jogo}
menuText B ToolsTrainTactics "Táticas"  0 {Exercícios de tática}
menuText B ToolsTrainCalvar "Cálculo de Variantes"  0 {Treinamento de cálculo de variantes}
menuText B ToolsTrainFindBestMove "Procurar melhor movimento"  0 {Procura o melhor movimento}
menuText B ToolsTrainFics "Jogar na internet"  0 {Jogar em  freechess.org}
menuText B ToolsEngineTournament "Torneio de motores de Xadrez"  0 {Inicia um torneio entre motores de Xadrez}
menuText B ToolsBookTuning "Sintonizar Livro" 0 {Sintonizar livro de aberturas}
menuText B ToolsConnectHardware "Conectar Hardware" 0 {Conectar hardware externo}
menuText B ToolsConnectHardwareConfigure "Configurar..." 0 {Configurar hardware e conexão externa}
menuText B ToolsConnectHardwareNovagCitrineConnect "Conectar Novag Citrine" 0 {Conectar Novag Citrine}
menuText B ToolsConnectHardwareInputEngineConnect "Conectar Engine de Entrada" 0 {Conectar engine de entrada (ex. DGT)}
menuText B ToolsPInfo "Informacao do Jogador"  0 \
  {Abre/atualiza a janela de informacao do jogador}
menuText B ToolsPlayerReport "Relatório de jogador..." 3 \
  {Gera um relatório de jogador} 
menuText B ToolsRating "Grafico de Rating" 0 \
  {Mostra, em um grafico, a evolucao do rating de um jogador}
menuText B ToolsScore "Grafico de Resultados" 0 {Mostra a janela com o grafico dos resultados}
menuText B ToolsExpCurrent "Exporta jogo corrente" 8 \
  {Grava o jogo corrente em um arquivo texto}
menuText B ToolsExpCurrentPGN "Exporta para PGN..." 15 \
  {Grava o jogo corrente em um arquivo PGN}
menuText B ToolsExpCurrentHTML "Exporta para HTML..." 15 \
  {Grava o jogo corrente em um arquivo HTML}
menuText B ToolsExpCurrentHTMLJS "Exportar jogo para arquivo HTML e JavaScript..." 15 {Escreve o jogo corrente em um arquivo HTML e JavaScript}  
menuText B ToolsExpCurrentLaTeX "Exporta para LaTex..." 15 \
  {Grava o jogo corrente em um arquivo LaTex}
menuText B ToolsExpFilter "Exporta jogos filtrados" 1 \
  {Exporta todos os jogos filtrados para um arquivo texto}
menuText B ToolsExpFilterPGN "Exporta jogos filtrados - PGN..." 17 \
  {Exporta todos os jogos filtrados para um arquivo PGN}
menuText B ToolsExpFilterHTML "Exporta jogos filtrados - HTML..." 17 \
  {Exporta todos os jogos filtrados para um arquivo HTML}
menuText B ToolsExpFilterHTMLJS "Exporta filtro para arquivo HTML e JavaScript..." 17 {Escreve todos os jogos filtrados para um arquivo HTML e JavaScript}  
menuText B ToolsExpFilterLaTeX "Exporta jogos filtrados - LaTex..." 17 \
  {Exporta todos os jogos filtrados para um arquivo LaTex}
menuText B ToolsImportOne "Importa PGN texto..." 0 \
  {Importa jogo de um texto em PGN}
menuText B ToolsImportFile "Importa arquivo de jogos PGN..." 7 \
  {Importa jogos de um arquivo PGN}
menuText B ToolsStartEngine1 "Iniciar engine 1" 0  {Inicia engine 1}
menuText B ToolsStartEngine2 "Iniciar engine 2" 0  {Inicia engine 2}
menuText B ToolsCaptureBoard "Capturar posicao atual..." 5  {Salva uma imagem da posicao atual.}
menuText B Play "Jogar" 0
menuText B CorrespondenceChess "Xadrez por correspondência" 0 {Funções para jogar xadrez por correspondência usando eMail e Xfcc}
menuText B CCConfigure "Configurar..." 0 {Configura ferramentas externas e configuração geral}
menuText B CCConfigRelay "Observar jogos..." 10 {Configura jogos para observar}
menuText B CCOpenDB "Abrir base..." 0 {Abre a base de correspondência padrão}
menuText B CCRetrieve "Recupera jogos" 0 {Recupera jogos via (Xfcc-)}
menuText B CCInbox "Processa caixa de entrada" 0 {Processa todos os arquivos na caixa de entrada do Scid}
menuText B CCSend "Enviar movimento" 0 {Envia seu movimento via eMail ou (Xfcc-)}
menuText B CCResign "Resignar" 0 {Aceita a derrota (não via eMail)}
menuText B CCClaimDraw "Declarar empate" 0 {Envia o movimento e declara empate (não via eMail)}
menuText B CCOfferDraw "Oferecer empate" 0 {Envia o movimento e oferece empate (não via eMail)}
menuText B CCAcceptDraw "Aceitar empate" 0 {Aceita uma oferta de empate (não via eMail)}
menuText B CCNewMailGame "Novo jogo por eMail..." 0 {Inicia um novo jogo por eMail}
menuText B CCMailMove "Enviar movimento..." 0 {Envia movimento ao oponente via eMaail}
menuText B CCGamePage "Página do jogo..." 0 {Chama o jogo através do browser}
menuText B CCEditCopy "Copiar Lista de Jogos para a base de cópia" 0 {Copia os jogos no formato CSV para a base de cópia}

# Options menu:
menuText B Options "Opcoes" 0
menuText B OptionsBoardGraphics "Casas..." 0 {Seleciona texturas para as casas}
translate B OptionsBGW {Seleciona texturas para as casas}
translate B OptionsBoardGraphicsText {Seleciona arquivos gráficos para as casas brancas e pretas:}
menuText B OptionsBoardNames "Meus nomes de jogadores..." 0 {Edita meus nomes de jogadores} 
menuText B OptionsExport "Exportacao" 0 {Muda as opcoes de exportacao de texto}
menuText B OptionsFonts "Fontes" 0 {Muda os fontes}
menuText B OptionsFontsRegular "Normal" 0 {Fonte Normal}
menuText B OptionsFontsMenu "Menu" 0 {Change the menu font} 
menuText B OptionsFontsSmall "Pequeno" 0 {Fonte pequeno}
menuText B OptionsFontsTiny "Pequeno" 0 {Muda o fonte pequeno}
menuText B OptionsFontsFixed "Fixo" 0 {Fonte de largura fixa}
menuText B OptionsGInfo "Informacoes do Jogo" 0 {Opcoes de informacao do jogo}
menuText B OptionsLanguage "Linguagem" 0 {Menu de selecao de linguagem}
menuText B OptionsMovesTranslatePieces "Traduzir Peças" 0 {Traduz a primeira letra do nome das peças}
menuText B OptionsMovesHighlightLastMove "Destacar último movimento" 0 {Destaca o último movimento}
menuText B OptionsMovesHighlightLastMoveDisplay "Mostrar" 0 {Mostra o destaque do último movimento}
menuText B OptionsMovesHighlightLastMoveWidth "Espessura" 0 {Espessura da linha}
menuText B OptionsMovesHighlightLastMoveColor "Cor" 0 {Cor da linha}
menuText B OptionsMovesHighlightLastMoveArrow "seta de inclusao" 0 {Seta de Inclusao Destacada}
menuText B OptionsMoves "Movimentos" 0 {Opcoes para entrada dos movimentos}
menuText B OptionsMovesAnimate "Tempo de animação" 1 \
  {Define a quantidade de tempo usada para animar os movimentos} 
menuText B OptionsMovesDelay "Tempo de atraso p/ Jogo automatico..." 1 \
  {Define o tempo de espera antes de entrar no modo de jogo automatico}
menuText B OptionsMovesCoord "Entrada de movimentos por coordenadas" 0 \
  {Aceita o estilo de entrada de movimentos por coordenadas ("g1f3")}
menuText B OptionsMovesSuggest "Mostrar movimentos sugeridos" 0 \
  {Liga/desliga sugestao de movimentos}
menuText B OptionsShowVarPopup "Mostrar janela de variantes" 0 {Liga/Desliga a janela de variantes}  
menuText B OptionsMovesSpace "Adicionar espaços após o número do movimento" 0 {Adiciona espaços após o número do movimento}  
menuText B OptionsMovesKey "Auto completar" 0 \
  {Liga/desliga auto completar a partir do que for digitado}
menuText B OptionsMovesShowVarArrows "Mostrar Setas para variantes" 0 {Liga/Desliga as setas que mostram movimentos em variantes}
menuText B OptionsMovesGlossOfDanger "Codigos de Cor para Perigo" 0 {Liga/Desliga os codigos de cor para perigo}
menuText B OptionsNumbers "Formato de Numeros" 0 {Selecione o formato usado para numeros}
menuText B OptionsTheme "Tema" 0 {Muda a aparência da interface}
menuText B OptionsWindows "Janelas" 0 {Opcoes para Janelas}
menuText B OptionsSounds "Sons" 2 {Configura sons para anunciar os movimentos} 
# ====== TODO To be translated ======
menuText B OptionsResources "Resources..." 0 {Choose resource files and folders}
menuText B OptionsWindowsDock "Estacionar janelas" 0 {Estaciona as janelas}
menuText B OptionsWindowsSaveLayout "Salvar layout" 0 {Salva o layout das janelas}
menuText B OptionsWindowsRestoreLayout "Restaurar layout" 0 {Restaura layout}
menuText B OptionsWindowsShowGameInfo "Mostrar Informações do Jogo" 0 {Mostra informações do jogo}
menuText B OptionsWindowsAutoLoadLayout "Carregar primeiro layout na entrada" 0 {Carrega automaticamente o primeiro layout ao entrar na aplicação}
menuText B OptionsECO "Carregar arquivo ECO" 7 {Carrega o arquivo com a classificacao ECO}
menuText B OptionsSpell "Carregar arquivo de verificacao ortografica" 6 \
  {Carrega o arquivo de verificacao ortografica do Scid}
menuText B OptionsTable "Diretorio de tabelas de base" 0 \
  {Selecione um arquivo de tabela de base; todas as tabelas nesse diretorio serao usadas}
menuText B OptionsRecent "Arquivos recentes..." 0 \
  {Muda o número de arquivos recentes no menu Arquivo} 
menuText B OptionsBooksDir "Diretório de Livros" 0 {Define o diretório para os livros de aberturas}
menuText B OptionsTacticsBasesDir "Diretório de bases" 0 {Define o diretório para as bases de treinamento de táticas}
menuText B OptionsPhotosDir "Diretorio de Fotos" 0 {Define o diretorio base de fotos}
menuText B OptionsThemeDir "Carregar Tema(s)"  0 {Carrega arquivo de tema para a tela}
menuText B OptionsSave "Salvar Configuracao" 0 \
  "Salva a configuracao no arquivo $::optionsFile"
menuText B OptionsAutoSave "Salva Opcoes ao sair" 0 \
  {Salva automaticamente todas as opcoes quando sair do Scid}

# Help menu:
menuText B Help "Ajuda" 0
menuText B HelpContents "Conteudo" 0 {Show the help contents page}
menuText B HelpIndex "Indice" 0 {Indice da Ajuda}
menuText B HelpGuide "Consulta Rapida" 0 {Mostra a pagina de consulta rapida}
menuText B HelpHints "Dicas" 0 {Mostra a pagina de dicas}
menuText B HelpContact "Informacoes para contato" 0 {Mostra a pagina com informacoes para contato}
menuText B HelpTip "Dica do dia" 0 {Mostra uma dica util do Scid}
menuText B HelpStartup "Janela de Inicializacao" 0 {Mostra a janela de inicializacao}
menuText B HelpAbout "Sobre Scid" 0 {Informacoes sobre o Scid}

# Game info box popup menu:
menuText B GInfoHideNext "Ocultar proximo movimento" 0
menuText B GInfoMaterial "Mostra valor de material" 0
menuText B GInfoFEN "Mostra Diagrama FEN" 16
menuText B GInfoMarks "Mostra setas e casas coloridas" 7
menuText B GInfoWrap "Quebra de linhas longas" 0
menuText B GInfoFullComment "Mostrar comentario completo" 8
menuText B GInfoPhotos "Mostrar Fotos" 5 
menuText B GInfoTBNothing "Tabelas de Base: nada" 12
menuText B GInfoTBResult "Tabelas de Base: apenas resultado" 12
menuText B GInfoTBAll "Tabelas de Base: resultado e melhores movimentos" 19
menuText B GInfoDelete "Recuperar este jogo" 0
menuText B GInfoMark "Desmarcar este jogo" 0
menuText B GInfoInformant "Configurar valores do informante" 0

# General buttons:
translate B Back {Voltar}
translate B Browse {Navegar} 
translate B Cancel {Cancelar}
translate B Continue {Continuar}
translate B Clear {Limpar}
translate B Close {Fechar}
translate B Contents {Conteúdo} 
translate B Defaults {Defaults}
translate B Delete {Deletar}
translate B Graph {Grafico}
translate B Help {Ajuda}
translate B Import {Importar}
translate B Index {Indice}
translate B LoadGame {Carrega jogo}
translate B BrowseGame {Listar jogo}
translate B MergeGame {Fazer merge do jogo}
translate B MergeGames {Merge dos jogos}
translate B Preview {Visualizacao}
translate B Revert {Reverter}
translate B Save {Salvar}
translate B Search {Pesquisar}
translate B Stop {Parar}
translate B Store {Guardar}
translate B Update {Atualizar}
translate B ChangeOrient {Muda orientacao da janela}
translate B ShowIcons {Mostrar Ícones} 
translate B None {Nenhum}
translate B First {Primeiro}
translate B Current {Atual}
translate B Last {Ultimo}

# General messages:
translate B game {jogo}
translate B games {jogos}
translate B move {movimento}
translate B moves {movimentos}
translate B all {tudo}
translate B Yes {Sim}
translate B No {Nao}
translate B Both {Ambos}
translate B King {Rei}
translate B Queen {Dama}
translate B Rook {Torre}
translate B Bishop {Bispo}
translate B Knight {Cavalo}
translate B Pawn {Peao}
translate B White {Branco}
translate B Black {Preto}
translate B Player {Jogador}
translate B Rating {Rating}
translate B RatingDiff {Diferenca de Rating (Brancas - Pretas)}
translate B AverageRating {Rating Médio} 
translate B Event {Evento}
translate B Site {Lugar}
translate B Country {Pais}
translate B IgnoreColors {Ignorar cores}
translate B Date {Data}
translate B EventDate {Evento data}
translate B Decade {Década} 
translate B Year {Ano}
translate B Month {Mes}
translate B Months {Janeiro Fevereiro Marco Abril Maio Junho Julho Agosto Setembro Outubro Novembro Dezembro}
translate B Days {Dom Seg Ter Qua Qui Sex Sab}
translate B YearToToday {Anos ate hoje}
translate B YearToTodayTooltip {Define a data para 1 ano atrás}
translate B Result {Resultado}
translate B Round {Rodada}
translate B Length {Tamanho}
translate B ECOCode {ECO}
translate B ECO {ECO}
translate B Deleted {Apagado}
translate B SearchResults {Resultados da Pesquisa}
translate B OpeningTheDatabase {Abrindo a Base de Dados}
translate B Database {Base de dados}
translate B Filter {Filtro}
translate B noGames {nenhum jogo}
translate B allGames {todos os jogos}
translate B empty {vazio}
translate B clipbase {base de trabalho}
translate B score {Pontuacao}
translate B StartPos {Posicao Inicial}
translate B Total {Total}
translate B readonly {apenas leitura}

# Standard error messages:
translate B ErrNotOpen {Esta base não está aberta.} 
translate B ErrReadOnly {Esta base é apenas para leitura; não pode ser alterada.} 
translate B ErrSearchInterrupted {Pesquisa interrompida; os resultados estão incompletos.} 

# Game information:
translate B twin {duplicata}
translate B deleted {apagado}
translate B comment {comentario}
translate B hidden {oculto}
translate B LastMove {Ultimo movimento}
translate B NextMove {Proximo}
translate B GameStart {Inicio do jogo}
translate B LineStart {Inicio da linha}
translate B GameEnd {Fim do jogo}
translate B LineEnd {Fim da linha}

# Player information:
translate B PInfoAll {Resultados para <b>todos</b> os jogos}
translate B PInfoFilter {Resultados para os jogos <b>filtrados</b>}
translate B PInfoAgainst {Resultados contra}
translate B PInfoMostWhite {Aberturas mais comuns com as Brancas}
translate B PInfoMostBlack {Aberturas mais comuns com as Pretas}
translate B PInfoRating {Historico de Rating}
translate B PInfoBio {Biografia}
translate B PInfoEditRatings {Editar Ratings} 
# ====== TODO To be translated ======
translate B PInfoEloFile {File}

# Tablebase information:
translate B Draw {Empate}
translate B stalemate {mate afogado}
translate B withAllMoves {com todos os movimentos}
translate B withAllButOneMove {com um movimento a menos}
translate B with {com}
translate B only {apenas}
translate B lose {derrota}
translate B loses {derrotas}
translate B allOthersLose {qualquer outro perde}
translate B matesIn {mate em}
translate B hasCheckmated {recebeu xeque-mate}
translate B longest {mais longo}
translate B WinningMoves {Movimentos vencedores} 
translate B DrawingMoves {Movimentos para empate} 
translate B LosingMoves {Movimentos perdedores} 
translate B UnknownMoves {Movimentos com resultado desconhecido} 
translate B SideLongest {Lado    Mov. p/    %     %     %\n}
translate B toNoveMate {a mover   mate      V     E     D\n}
translate B NoSummary {\nSem resumo para esta tablebase.}
translate B MutualZugzwang {Zugzwangs Nutuos: }
translate B SelectetZugzwang {\nPosicoes de zugzwang selecionadas:}
translate B WdrawBloss {Branca empata, Preta perde:}
translate B BdrawWloss {Preta empata, Branca perde:}
translate B AllLoss {" Quem mover perde   :     " }

# Tip of the day:
translate B Tip {Dica}
translate B TipAtStartup {Dica ao iniciar}

# Tree window menus: ***
menuText B TreeFile "Arquivo" 0
menuText B TreeFileFillWithBase "Carregar Cache com base" 0 {Carrega todos os jogos da base corrente no Cache}
menuText B TreeFileFillWithGame "Carregar Cache com jogo" 0 {Carrega o jogo corrente da base corrente no Cache}
menuText B TreeFileSetCacheSize "Tamanho do Cache" 0 {Define o tamanho do cache}
menuText B TreeFileCacheInfo "Informação do Cache" 0 {Informações sobre a utilização do cache}
menuText B TreeFileSave "Salvar arquivo de cache" 0 \
  {Salvar o arquivo de cache da arvore (.stc)}
menuText B TreeFileFill "Criar arquivo de cache" 0 \
  {Enche o arquivo de cache com as posicoes comuns na abertura}
menuText B TreeFileBest "Lista dos melhores jogos" 0 \
  {Mostra a lista dos melhores jogos da arvore}
menuText B TreeFileGraph "Janela de Grafico" 0 \
  {Mostra o grafico para este galho da arvore}
menuText B TreeFileCopy "Copiar texto da arvore para a area de transferencia" \
  1 {Copiar texto da arvore para a area de transferencia}
menuText B TreeFileClose "Fechar janela de arvore" 0 {Fechar janela de arvore}
menuText B TreeMask "Máscara" 0
menuText B TreeMaskNew "Nova" 0 {Nova máscara}
menuText B TreeMaskOpen "Abrir" 0 {Abrir máscara}
menuText B TreeMaskOpenRecent "Abrir recente" 0 {Abre máscara recente}
menuText B TreeMaskSave "Salvar" 0 {Salva máscara}
menuText B TreeMaskClose "Fechar" 0 {Fecha máscara}
menuText B TreeMaskFillWithGame "Preencher com jogo" 0 {Preenche máscara com jogo}
menuText B TreeMaskFillWithBase "Preencher com base" 0 {Preenche a máscara com todos os jogos da base}
menuText B TreeMaskInfo "Info" 0 {Mostrar estatisticas para a máscara corrente}
menuText B TreeMaskDisplay "Mostrar mapa da máscara" 0 {Mostra os dados da máscara em forma de árvore}
menuText B TreeMaskSearch "Pesquisar" 0 {Pesquisa na máscara corrente}
menuText B TreeSort "Ordenar" 0
menuText B TreeSortAlpha "Alfabetica" 0
menuText B TreeSortECO "ECO" 0
menuText B TreeSortFreq "Frequencia" 0
menuText B TreeSortScore "Pontuacao" 0
menuText B TreeOpt "Opcoes" 0
menuText B TreeOptSlowmode "Modo Lento" 0 {Modo lento para atualizações (mais acurado)}
menuText B TreeOptFastmode "Modo Rápido" 0 {Modo rápido para atualizações (sem transsposições de movimentos)}
menuText B TreeOptFastAndSlowmode "Modo rápido e lento" 0 {Modo rápido e lento para atualizações}
menuText B TreeOptStartStop "Atualização automática" 0 {Liga/Desliga a atualização automática da janela de árvore}
menuText B TreeOptLock "Lock" 0 {Trava/Destrava a arvore para o banco corrente}
menuText B TreeOptTraining "Treinamento" 0 \
  {Liga/Desliga o modo treinamento na arvore}
menuText B TreeOptAutosave "Salvar automaticamente arquivo de cache" 0 \
  {Salvar automaticamente o arquivo de cache quando fechar a janela de arvore}
menuText B TreeHelp "Ajuda" 0
menuText B TreeHelpTree "Ajuda para arvore" 0
menuText B TreeHelpIndex "Indice da Ajuda" 0
translate B SaveCache {Salvar Cache}
translate B Training {Treinamento}
translate B LockTree {Travamento}
translate B TreeLocked {Travada} 
translate B TreeBest {Melhor}
translate B TreeBestGames {Melhores jogos da arvore}
# Note: the next message is the tree window title row. After editing it,
# check the tree window to make sure it lines up with the actual columns.
translate B TreeTitleRow \
  {    Mov.   ECO       Frequencia   Score  AvElo Perf AvYear %Empat} 
translate B TreeTotal {TOTAL}
translate B DoYouWantToSaveFirst {Quer salvar primeiro?}
translate B AddToMask {Adicionar à máscara}
translate B RemoveFromMask {Remover da máscara}
translate B AddThisMoveToMask {Adicionar este movimento à máscara}
translate B SearchMask {Pesquisar na máscara}
translate B DisplayMask {Mostrar máscara}
translate B Nag {Código Nag}
translate B Marker {Marcador}
translate B Include {Incluir}
translate B Exclude {Excluir}
translate B MainLine {Linha Principal}
translate B Bookmark {Marcador}
translate B NewLine {Nova Linha}
translate B ToBeVerified {Verificar}
translate B ToTrain {Para treinar}
translate B Dubious {Duvidoso}
translate B ToRemove {Para remover}
translate B NoMarker {Sem marcador}
translate B ColorMarker {Cor}
translate B WhiteMark {Branca}
translate B GreenMark {Verde}
translate B YellowMark {Amarela}
translate B BlueMark {Azul}
translate B RedMark {Vermelha}
translate B CommentMove {Comentar movimento}
translate B CommentPosition {Comentar posição}
translate B AddMoveToMaskFirst {Adicionar movimento à máscara primeiro}
translate B OpenAMaskFileFirst {Abrir uma máscara primeiro}
translate B Positions {Posições}
translate B Moves {Movimentos}

# Finder window:
menuText B FinderFile "Arquivo" 0
menuText B FinderFileSubdirs "Buscar nos subdiretorios" 0
menuText B FinderFileClose "Fecha buscador de arquivos" 0
menuText B FinderSort "Ordenar" 0
menuText B FinderSortType "Tipo" 0
menuText B FinderSortSize "Tamanho" 0
menuText B FinderSortMod "Modificado" 0
menuText B FinderSortName "Nome" 0
menuText B FinderSortPath "Caminho" 0
menuText B FinderTypes "Tipos" 0
menuText B FinderTypesScid "Bases Scid" 0
menuText B FinderTypesOld "Bases Scid antigas" 0
menuText B FinderTypesPGN "Arquivos PGN" 0
menuText B FinderTypesEPD "Arquivos EPD (book)" 0
menuText B FinderTypesRep "Arquivos de Repertorio" 0
menuText B FinderHelp "Ajuda" 0
menuText B FinderHelpFinder "Ajuda do Buscador" 0
menuText B FinderHelpIndex "Indice da Ajuda" 0
translate B FileFinder {Buscador de Arquivos}
translate B FinderDir {Diretorio}
translate B FinderDirs {Diretorios}
translate B FinderFiles {Arquivos}
translate B FinderUpDir {Acima}
translate B FinderCtxOpen {Abrir}
translate B FinderCtxBackup {Backup}
translate B FinderCtxCopy {Copiar}
translate B FinderCtxMove {Mover}
translate B FinderCtxDelete {Apagar}

# Player finder:
menuText B PListFile "Arquivo" 0
menuText B PListFileUpdate "Atualizar" 0
menuText B PListFileClose "Close Player Finder" 0 
menuText B PListSort "Ordenar" 0
menuText B PListSortName "Name" 0 
menuText B PListSortElo "Elo" 0
menuText B PListSortGames "Jogos" 0
menuText B PListSortOldest "Mais antigo" 0 
menuText B PListSortNewest "Mais novo" 0 

# Tournament finder:
menuText B TmtFile "Arquivo" 0
menuText B TmtFileUpdate "Atualizar" 0
menuText B TmtFileClose "Fecha Buscador de Torneios" 0
menuText B TmtSort "Ordenar" 0
menuText B TmtSortDate "Data" 0
menuText B TmtSortPlayers "Jogadores" 0
menuText B TmtSortGames "Jogos" 0
menuText B TmtSortElo "Elo" 0
menuText B TmtSortSite "Lugar" 0
menuText B TmtSortEvent "Evento" 1
menuText B TmtSortWinner "Vencedor" 0
translate B TmtLimit "Limite de Lista"
translate B TmtMeanElo "Menor Elo"
translate B TmtNone "Nenhum torneio encontrado."

# Graph windows:
menuText B GraphFile "Arquivo" 0
menuText B GraphFileColor "Salvar como Postscript Colorido..." 12
menuText B GraphFileGrey "Salvar como Postscript Cinza..." 23
menuText B GraphFileClose "Fecha janela" 6
menuText B GraphOptions "Opcoes" 0
menuText B GraphOptionsWhite "Branco" 0
menuText B GraphOptionsBlack "Preto" 0
menuText B GraphOptionsBoth "Ambos" 0
menuText B GraphOptionsPInfo "Informacao do Jogador" 0
# ====== TODO To be translated ======
menuText B GraphOptionsEloFile "Elo from rating file" 0
# ====== TODO To be translated ======
menuText B GraphOptionsEloDB "Elo from database" 0
translate B GraphFilterTitle "Gráfico de filtro: frequencia por 1000 jogos" 
translate B GraphAbsFilterTitle "Gráfico de filtro: frequencia dos jogos"
translate B ConfigureFilter {Configurar Eixos X para Ano, Rating e Movimentos}
translate B FilterEstimate "Estimar"
translate B TitleFilterGraph "Scid: Gráfico de Filtro"

# Analysis window:
translate B AddVariation {Adicionar variante}
translate B AddAllVariations {Adicionar todas as Variantes}
translate B AddMove {Adicionar movimento}
translate B Annotate {Anotar}
translate B ShowAnalysisBoard {Mostrar tabuleiro de análise}
translate B ShowInfo {Mostrar informações do engine}
translate B FinishGame {Encerrar jogo}
translate B StopEngine {Parar engine}
translate B StartEngine {Iniciar engine}
translate B LockEngine {Travar engine na posição corrente}
translate B AnalysisCommand {Comando de Analise}
translate B PreviousChoices {Escolhas Anteriores}
translate B AnnotateTime {Define o tempo entre movimentos em segundos}
translate B AnnotateWhich {Adiciona variante}
translate B AnnotateAll {Parar movimentos de ambos os lados}
translate B AnnotateAllMoves {Anotar todos os movimentos}
translate B AnnotateWhite {Apenas para movimentos das Brancas}
translate B AnnotateBlack {Apenas para movimentos das Pretas}
translate B AnnotateBlundersOnly {Quando o mivimento for um erro crasso}
translate B AnnotateBlundersOnlyScoreChange {As análises indicam um erro crasso, com mudança de score de/para: }
translate B BlundersThreshold {Limite}
translate B ScoreAllMoves {Analise todos os movimentos}
translate B LowPriority {Baixa prioridade da CPU} 
translate B ClickHereToSeeMoves {Clique aqui para ver os movimentos}
translate B ConfigureInformant {Informante}
translate B Informant!? {Movimento interessante}
translate B Informant? {Movimento pobre}
translate B Informant?? {Erro crasso}
translate B Informant?! {Movimento duvidoso}
translate B Informant+= {Branca tem alguma vantagem}
translate B Informant+/- {Branca tem vantagem moderada}
translate B Informant+- {Branca tem vantagem decisiva}
translate B Informant++- {O jogo é considerado como ganho}
translate B Book {Livro}
translate B OtherBookMoves {Livro do oponente}
translate B OtherBookMovesTooltip {Movimentos para os quais o oponente tem resposta}

# Analysis Engine open dialog:
translate B EngineList {Lista de Programas de Analise}
translate B EngineName {Nome}
translate B EngineCmd {Comando}
translate B EngineArgs {Parametros}
translate B EngineDir {Diretorio}
translate B EngineElo {Elo}
translate B EngineTime {Data}
translate B EngineNew {Novo}
translate B EngineEdit {Editar}
translate B EngineRequired {Campos em negrito são obrigatórios; os demais são opcionais}

# PGN window menus:
menuText B PgnFile "Arquivo" 0
menuText B PgnFileCopy "Copiar o jogo para a área de transferência" 0 
menuText B PgnFilePrint "Imprimir para arquivo..." 0
menuText B PgnFileClose "Fechar janela PGN" 0
menuText B PgnOpt "Monitor" 0
menuText B PgnOptColor "Monitor Colorido" 0
menuText B PgnOptShort "Cabecalho curto (3 linhas)" 0
menuText B PgnOptSymbols "Anotacoes simbolicas" 0
menuText B PgnOptIndentC "Identar comentarios" 0
menuText B PgnOptIndentV "Identar variantes" 7
menuText B PgnOptColumn "Estilo Coluna (um movimento por linha)" 0
menuText B PgnOptSpace "Espaco apos o numero do movimento" 0
menuText B PgnOptStripMarks "Remover os códigos coloridos das casas/setas" 1 
menuText B PgnOptBoldMainLine "Use negrito para movimentos da linha principal" 4 
menuText B PgnColor "Cores" 0
menuText B PgnColorHeader "Cabecalho..." 0
menuText B PgnColorAnno "Anotacoes..." 0
menuText B PgnColorComments "Comentarios..." 0
menuText B PgnColorVars "Variantes..." 0
menuText B PgnColorBackground "Cor de fundo..." 0
menuText B PgnColorMain "Linha principal..." 0
menuText B PgnColorCurrent "Cor de fundo do movimento atual..." 1
menuText B PgnHelp "Ajuda" 0
menuText B PgnHelpPgn "Ajuda PGN" 0
menuText B PgnHelpIndex "Indice" 0
translate B PgnWindowTitle {Notação do Jogo - jogo %u} 

# Crosstable window menus:
menuText B CrosstabFile "Arquivo" 0
menuText B CrosstabFileText "Imprime para arquivo texto..." 9
menuText B CrosstabFileHtml "Imprime para arquivo HTML..." 9
menuText B CrosstabFileLaTeX "Imprime para arquivo LaTex..." 9
menuText B CrosstabFileClose "Fechar tabela de cruzamentos" 0
menuText B CrosstabEdit "Editar" 0
menuText B CrosstabEditEvent "Evento" 0
menuText B CrosstabEditSite "Lugar" 0
menuText B CrosstabEditDate "Data" 0
menuText B CrosstabOpt "Monitor" 0
menuText B CrosstabOptAll "Todos contra todos" 0
menuText B CrosstabOptSwiss "Suiço" 0
menuText B CrosstabOptKnockout "Knockout" 0
menuText B CrosstabOptAuto "Automatico" 0
menuText B CrosstabOptAges "Idade em anos" 0
menuText B CrosstabOptNats "Nacionalidades" 0
menuText B CrosstabOptRatings "Ratings" 0
menuText B CrosstabOptTitles "Titulos" 0
menuText B CrosstabOptBreaks "Scores de desempate" 0
menuText B CrosstabOptDeleted "Incluir jogos apagados" 8 
menuText B CrosstabOptColors "Cores (apenas para tabela Swiss)" 0
menuText B CrosstabOptColumnNumbers "Colunas numeradas (apenas tabelas Todos-contra-Todos)" 2 
menuText B CrosstabOptGroup "Pontuacao do Grupo" 0
menuText B CrosstabSort "Ordenar" 0
menuText B CrosstabSortName "Nome" 0
menuText B CrosstabSortRating "Rating" 0
menuText B CrosstabSortScore "Pontuacao" 0
menuText B CrosstabColor "Cor" 0
menuText B CrosstabColorPlain "Texto puro" 0
menuText B CrosstabColorHyper "Hipertexto" 0
menuText B CrosstabHelp "Ajuda" 0
menuText B CrosstabHelpCross "Ajuda para tabela de cruzamentos" 0
menuText B CrosstabHelpIndex "Indice da Ajuda" 0
translate B SetFilter {Setar filtro}
translate B AddToFilter {Adicionar ao filtro}
translate B Swiss {Suiço}
translate B Category {Categoria} 

# Opening report window menus:
menuText B OprepFile "Arquivo" 0
menuText B OprepFileText "Imprimir para arquivo texto..." 9
menuText B OprepFileHtml "Imprimir para arquivo HTML..." 9
menuText B OprepFileLaTeX "Imprimir para arquivo LaTex..." 9
menuText B OprepFileOptions "Opcoes..." 0
menuText B OprepFileClose "Fechar janela de relatorio" 0
menuText B OprepFavorites "Favoritos" 1 
menuText B OprepFavoritesAdd "Adicionar Relatório..." 0 
menuText B OprepFavoritesEdit "Editar Relatório de Favoritos..." 0 
menuText B OprepFavoritesGenerate "Gerar Relatórios..." 0 
menuText B OprepHelp "Ajuda" 0
menuText B OprepHelpReport "Ajuda para Relatorio de abertura" 0
menuText B OprepHelpIndex "Indice da Ajuda" 0

# Header search:
translate B HeaderSearch {Busca por cabecalho}
translate B EndSideToMove {Lado que tem o mivimento ao final do jogo} 
translate B GamesWithNoECO {Jogos sem ECO?}
translate B GameLength {Tamanho do jogo}
translate B FindGamesWith {Encontrar jogos com}
translate B StdStart {Inicio padrao}
translate B Promotions {Promocoes}
translate B Comments {Comentarios}
translate B Variations {Variantes}
translate B Annotations {Anotacoes}
translate B DeleteFlag {Apagar Flag}
translate B WhiteOpFlag {Abertura Brancas}
translate B BlackOpFlag {Abertura Pretas}
translate B MiddlegameFlag {Meio-jogo}
translate B EndgameFlag {Final}
translate B NoveltyFlag {Novidade}
translate B PawnFlag {Estrutura de Peoes}
translate B TacticsFlag {Tatica}
translate B QsideFlag {Jogo na ala da Dama}
translate B KsideFlag {Jogo na ala do Rei}
translate B BrilliancyFlag {Brilhantismo}
translate B BlunderFlag {Erro!!!}
translate B UserFlag {Usuario}
translate B PgnContains {PGN contem texto}
translate B Annotator {Anotador}
translate B Cmnts {Apenas jogos anotados}

# Game list window:
translate B GlistNumber {Numero}
translate B GlistWhite {Branco}
translate B GlistBlack {Preto}
translate B GlistWElo {B-Elo}
translate B GlistBElo {P-Elo}
translate B GlistEvent {Evento}
translate B GlistSite {Lugar}
translate B GlistRound {Rodada}
translate B GlistDate {Data}
translate B GlistYear {Ano}
translate B GlistEDate {Evento-Data}
translate B GlistResult {Resultado}
translate B GlistLength {Tamanho}
translate B GlistCountry {Pais}
translate B GlistECO {ECO}
translate B GlistOpening {Abertura}
translate B GlistEndMaterial {Material-Final}
translate B GlistDeleted {Apagado}
translate B GlistFlags {Sinalizador}
translate B GlistVars {Variantes}
translate B GlistComments {Comentarios}
translate B GlistAnnos {Anotacoes}
translate B GlistStart {Iniciar}
translate B GlistGameNumber {Numero do Jogo}
translate B GlistAverageElo {Elo Medio}
translate B GlistRating {Rating}
translate B GlistFindText {Encontrar texto}
translate B GlistMoveField {Mover}
translate B GlistEditField {Configurar}
translate B GlistAddField {Adicionar}
translate B GlistDeleteField {Remover}
translate B GlistWidth {Largura}
translate B GlistAlign {Alinhar}
translate B GlistAlignL {Alinhar: esquerda}
translate B GlistAlignR {Alinhar: direita}
translate B GlistAlignC {Alinhar: centro}
translate B GlistColor {Cor}
translate B GlistSep {Separador}
translate B GlistCurrentSep {--  Atual  --}
translate B GlistNewSort {Novo}
translate B GlistAddToSort {Adicionar}
translate B GsortSort {Classificar...}
translate B GsortDate {Data}
translate B GsortYear {Ano}
translate B GsortEvent {Evento}
translate B GsortSite {Local}
translate B GsortRound {Rodada}
translate B GsortWhiteName {Nome Brancas}
translate B GsortBlackName {Nome Pretas}
translate B GsortECO {ECO}
translate B GsortResult {Resultado}
translate B GsortMoveCount {Movimentos}
translate B GsortAverageElo {Elo medio}
translate B GsortCountry {Pais}
translate B GsortDeleted {Apagado}
translate B GsortEventDate {Data Evento}
translate B GsortWhiteElo {Elo Brancas}
translate B GsortBlackElo {Elo Pretas}
translate B GsortComments {Comentarios}
translate B GsortVariations {Variantes}
translate B GsortNAGs {NAGs}
translate B GsortAscending {Ascendente}
translate B GsortDescending {Descendente}
translate B GsortAdd {Adicionar}
translate B GsortStore {Salvar}
translate B GsortLoad {Carregar}
translate B GlistRemoveThisGameFromFilter  {Remover este jogo do filtro}
translate B GlistRemoveGameAndAboveFromFilter  {Remover jogo (e todos antes dele) do filtro}
translate B GlistRemoveGameAndBelowFromFilter  {Remover jogo (e todos depois dele) do filtro}
translate B GlistDeleteGame {Recuperar este jogo} 
translate B GlistDeleteAllGames {Apagar todos os jogos no filtro} 
translate B GlistUndeleteAllGames {Recuperar todos os jogos no filtro} 
translate B GlistMergeGameInBase {Incorporar jogo em outra base} 

# Maintenance window:
translate B DatabaseName {Nome da base de dados:}
translate B TypeIcon {Icone de Tipo:}
translate B NumOfGames {Jogos:}
translate B NumDeletedGames {Jogos deletados:}
translate B NumFilterGames {Jogos no filtro:}
translate B YearRange {Faixa de Anos:}
translate B RatingRange {Faixa de Rating:}
translate B Description {Descrição} 
translate B Flag {Sinalizador}
translate B CustomFlags {Flags customizadas}
translate B DeleteCurrent {Deletar jogo corrente}
translate B DeleteFilter {Deletar jogos filtrados}
translate B DeleteAll {Deletar todos os jogos}
translate B UndeleteCurrent {Recuperar jogo corrente}
translate B UndeleteFilter {Recuperar jogos filtrados}
translate B UndeleteAll {Recuperar todos os jogos}
translate B DeleteTwins {Deletar duplicatas}
translate B MarkCurrent {Marcar jogo corrente}
translate B MarkFilter {Marcar jogos filtrados}
translate B MarkAll {Marcar todos os jogos}
translate B UnmarkCurrent {Desmarcar jogo corrente}
translate B UnmarkFilter {Desmarcar jogos filtrados}
translate B UnmarkAll {Desmarcar todos os jogos}
translate B Spellchecking {Verificacao Ortografica}
translate B Players {Jogadores}
translate B Events {Eventos}
translate B Sites {Lugares}
translate B Rounds {Rodadas}
translate B DatabaseOps {Operacoes na base de dados}
translate B ReclassifyGames {Jogos classificados por ECO}
translate B CompactDatabase {Compactar base de dados}
translate B SortDatabase {Ordenar base de dados}
translate B AddEloRatings {Adicionar ratings}
translate B AutoloadGame {Carregar autom. o jogo numero}
translate B StripTags {Remover tags PGN} 
translate B StripTag {Remover tag} 
translate B Cleaner {Limpador}
translate B CleanerHelp {
O Limpador do Scid executara todas as acoes de manutencao selecionadas da lista abaixo, no banco corrente.

As configuracoes atuais na classificacao por ECO e dialogos de exclusao de duplicatas serao aplicadas se voce escolher estas funcoes.
}
translate B CleanerConfirm {
Uma vez iniciado, o Limpador nao podera ser interrompido!

Esta operacao pode levar muito tempo para ser executada em uma grande base de dados, dependendo das funcoes selecionadas e das configuracoes atuais.

Voce esta certo de que quer iniciar as acoes de manutencao selecionadas?
}
translate B TwinCheckUndelete {para virar; "u" recupera ambos)}
translate B TwinCheckprevPair {Par anterior}
translate B TwinChecknextPair {Próximo par}
translate B TwinChecker {Scid: Verificador de duplicatas}
translate B TwinCheckTournament {Jogos no torneio:}
translate B TwinCheckNoTwin {Nenhuma duplicata  }
translate B TwinCheckNoTwinfound {Nenhuma duplicata foi detectada para este jogo.\nPara mostrar duplicatas nesta janela, você deve usar primeiro a função "Apagar Duplicatas...". }
translate B TwinCheckTag {Compartilhar tags...}
translate B TwinCheckFound1 {Scid encontrou $result duplicatas}
translate B TwinCheckFound2 { e definiu suas flags de exclusão}
translate B TwinCheckNoDelete {Não há jogos para excluir nesta base.}
translate B TwinCriteria1 { Suas configurações para encontrar duplicatas podem \nfazer com que não duplicatas com movimentos similares sejam marcadas como se fossem.}
translate B TwinCriteria2 {É recomendado que, se você selecionar "Não" para "mesmos movimentos", você selecione "Sim" para as cores, evento, local, rodada, ano e mês.\nVocê quer continuar e excluir as duplicatas mesmo assim? }
translate B TwinCriteria3 {É recomendado que você especifique "Sim" para pelo menos duas das configurações de "mesmo local", "mesma rodada" e "mesmo ano".\nVocê quer continuar e excluir as duplicatas mesmo assim?}
translate B TwinCriteriaConfirm {Scid: Confirme as configurações de duplicatas}
translate B TwinChangeTag "Alterar as seguintes tags:\n\n"
translate B AllocRatingDescription "Este comando utilizará o arquivo corrente de verificação ortográfica para adicionar ratings Elo aos jogos nesta base. Sempre que um jogador não tiver um rating corrente, mas seu rating no momento do jogo estiver listado no arquivo de verificação, esse rating será adicionado."
translate B RatingOverride "Sobrescrever ratings existentes diferentes de zero?"
translate B AddRatings "Adicionar ratings a:"
translate B AddedRatings {Scid adicionou $r ratings Elo em $g jogos.}
translate B NewSubmenu "Novo Submenu"

# Comment editor:
translate B AnnotationSymbols  {Simbolos de Anotacao:}
translate B Comment {Comentario:}
translate B InsertMark {Inserir marca} 
translate B InsertMarkHelp {
Insere/remove marca: Seleciona cor, tipo, casa.
Insere/remove seta: Clique com o botão direito em duas casas.
} 

# Nag buttons in comment editor:
translate B GoodMove {Bom Movimento} 
translate B PoorMove {Movimento Pobre} 
translate B ExcellentMove {Movimento Excelente} 
translate B Blunder {Erro!!!} 
translate B InterestingMove {Movimento Interessante} 
translate B DubiousMove {Movimento Duvidoso} 
translate B WhiteDecisiveAdvantage {Branca tem vantagem decisiva} 
translate B BlackDecisiveAdvantage {Preta tem vantagem decisiva} 
translate B WhiteClearAdvantage {Branca tem clara vantagem} 
translate B BlackClearAdvantage {Preta tem clara vantagem} 
translate B WhiteSlightAdvantage {Branca tem alguma vantagem} 
translate B BlackSlightAdvantage {Preta tem alguma vantagem} 
translate B Equality {Igualdade} 
translate B Unclear {Não claro} 
translate B Diagram {Diagrama} 

# Board search:
translate B BoardSearch {Pesquisa Tabuleiro}
translate B FilterOperation {Operacao no filtro corrente:}
translate B FilterAnd {E (Filtro restrito)}
translate B FilterOr {OU (Adicionar ao filtro)}
translate B FilterIgnore {IGNORAR (Limpar filtro)}
translate B SearchType {Tipo de pesquisa:}
translate B SearchBoardExact {Posicao exata (todas as pecas nas mesmas casas)}
translate B SearchBoardPawns {Peoes (mesmo material, todos os peoes nas mesmas casas)}
translate B SearchBoardFiles {Colunas (mesmo material, todos os peoes na mesma coluna)}
translate B SearchBoardAny {Qualquer (mesmo material, peoes e pecas em qualquer posicao)}
translate B SearchInRefDatabase { Pesquisa na base }
translate B LookInVars {Olhar nas variantes}

# Material search:
translate B MaterialSearch {Pesquisa Material}
translate B Material {Material}
translate B Patterns {Padroes}
translate B Zero {Zero}
translate B Any {Qualquer}
translate B CurrentBoard {Tabuleiro corrente}
translate B CommonEndings {Finais comuns}
translate B CommonPatterns {Padroes comuns}
translate B MaterialDiff {Diferenca de Material}
translate B squares {casas}
translate B SameColor {Mesma cor}
translate B OppColor {Cor oposta}
translate B Either {Qualquer}
translate B MoveNumberRange {Faixa do numero de movimentos}
translate B MatchForAtLeast {Conferem por pelo menos}
translate B HalfMoves {meios movimentos}

# Common endings in material search:
translate B EndingPawns {Finais com Peões} 
translate B EndingRookVsPawns {Torre contra Peões} 
translate B EndingRookPawnVsRook {Torre e 1 Peão contra Torre} 
translate B EndingRookPawnsVsRook {Torre e Peões contra Torre} 
translate B EndingRooks {Finais Torre contra Torre} 
translate B EndingRooksPassedA {Finais Torre contra Torre com Peão passado na coluna a} 
translate B EndingRooksDouble {Finais com Torres dobradas} 
translate B EndingBishops {Finais Bispo contra Bispo} 
translate B EndingBishopVsKnight {Finais Bispo contra Cavalo} 
translate B EndingKnights {Finais Cavalo contra Cavalo} 
translate B EndingQueens {Finais Dama contra Dama} 
translate B EndingQueenPawnVsQueen {Dama e 1 Peão contra Dama} 
translate B BishopPairVsKnightPair {Meio-jogo com 2 Bispos contra 2 Cavalos} 

# Common patterns in material search:
translate B PatternWhiteIQP {IQP Branca} 
translate B PatternWhiteIQPBreakE6 {IQP Branca: avanço d4-d5 contra e6} 
translate B PatternWhiteIQPBreakC6 {IQP Branca: avanço d4-d5 contra c6} 
translate B PatternBlackIQP {IQP Preta} 
translate B PatternWhiteBlackIQP {IQP Branca vs. IQP Preta} 
translate B PatternCoupleC3D4 {Peões Brancos isolados em c3+d4} 
translate B PatternHangingC5D5 {Peões Pretos em c5 e d5} 
translate B PatternMaroczy {Maroczy Center (com Peões em c4 e e4)} 
translate B PatternRookSacC3 {Sacrifício de Torre em c3} 
translate B PatternKc1Kg8 {O-O-O vs. O-O (Rc1 vs. Rg8)} 
translate B PatternKg1Kc8 {O-O vs. O-O-O (Rg1 vs. Rc8)} 
translate B PatternLightFian {Fianchettos nas casas brancas(Bispo-g2 vs. Bispo-b7)} 
translate B PatternDarkFian {Fianchettos nas casas pretas (Bispo-b2 vs. Bispo-g7)} 
translate B PatternFourFian {Quatro Fianchettos (Bispos em b2,g2,b7,g7)} 

# Game saving:
translate B Today {Hoje}
translate B ClassifyGame {Classificar Jogo}

# Setup position:
translate B EmptyBoard {Tabuleiro vazio}
translate B InitialBoard {Tabuleiro Inicial}
translate B SideToMove {Lado que move}
translate B MoveNumber {No. do Movimento}
translate B Castling {Roque}
translate B EnPassantFile {coluna En Passant}
translate B ClearFen {Limpar FEN}
translate B PasteFen {Colar FEN}
translate B SaveAndContinue {Salvar e continuar}
translate B DiscardChangesAndContinue {Descartar alterações\ne continuar}
translate B GoBack {Voltar}

# Replace move dialog:
translate B ReplaceMove {Substituir movimento}
translate B AddNewVar {Adicionar nova variante}
translate B NewMainLine {Nova linha principal}
translate B ReplaceMoveMessage {Um movimento ja existe nesta posicao.

Voce pode substitui-lo, descartar todos os movimentos que o seguem, ou adicionar seu movimento como uma nova variante.

(Voce pode evitar que esta mensagem apareca no futuro desligando a opcao "Perguntar antes de substituir movimentos" no menu Opcoes:Movimentos.)}

# Make database read-only dialog:
translate B ReadOnlyDialog {Se voce tornar esta base de dados apenas para leitura, nenhuma alteracao sera permitida.
Nenhum jogo podera ser salvo ou substituido, e nenhuma flag de exclusao podera ser alterada.
Qualquer ordenacao ou resultados de classificacao por ECO serao temporarios.

Para poder tornar a base de dados atualizavel novamente, feche-a e abra-a novamente.

Voce realmente quer que esta base de dados seja apenas de leitura?}

# Clear game dialog:
translate B ClearGameDialog {Este jogo foi alterado.

Voce realmente quer continuar e descartar as mudancas feitas?
}

# Exit dialog:
translate B ExitDialog {Voce quer realmente sair do Scid?}
translate B ExitUnsaved {As bases a seguir tem mudanças nos jogos que não foram salvas. Se você sair agora, essas mudanças serão perdidas.} 

# Import window:
translate B PasteCurrentGame {Colar jogo corrente}
translate B ImportHelp1 {Introduzir ou colar um jogo em formato PGN no quadro acima.}
translate B ImportHelp2 {Quaisquer erros ao importar o jogo serao mostrados aqui.}
translate B OverwriteExistingMoves {Sobrescrever movimentos existentes ?}

# ECO Browser:
translate B ECOAllSections {todas as secoes ECO}
translate B ECOSection {secao ECO}
translate B ECOSummary {Resumo para}
translate B ECOFrequency {Frequencia de subcodigos para}

# Opening Report:
translate B OprepTitle {Relatorio de Abertura}
translate B OprepReport {Relatorio}
translate B OprepGenerated {Gerado por}
translate B OprepStatsHist {Estatisticas e Historico}
translate B OprepStats {Estatisticas}
translate B OprepStatAll {Todas as partidas do relatorio}
translate B OprepStatBoth {Ambos com rating}
translate B OprepStatSince {Desde}
translate B OprepOldest {Jogos mais antigos}
translate B OprepNewest {Jogos mais recentes}
translate B OprepPopular {Popularidade Atual}
translate B OprepFreqAll {Frequencia em todos os anos:   }
translate B OprepFreq1   {No ultimo ano: }
translate B OprepFreq5   {Nos ultimos 5 anos: }
translate B OprepFreq10  {Nos ultimos 10 anos: }
translate B OprepEvery {uma vez em cada %u jogos}
translate B OprepUp {ate %u%s de todos os anos}
translate B OprepDown {menos que %u%s de todos os anos}
translate B OprepSame {nenhuma mudanca em todos os anos}
translate B OprepMostFrequent {Jogadores mais frequentes}
translate B OprepMostFrequentOpponents {Oponentes mais frequentes} 
translate B OprepRatingsPerf {Ratings e Desempenho}
translate B OprepAvgPerf {Ratings e desempenho medios}
translate B OprepWRating {Rating Brancas}
translate B OprepBRating {Rating Pretas}
translate B OprepWPerf {Desempenho Brancas}
translate B OprepBPerf {Desempenho Pretas}
translate B OprepHighRating {Jogos com o maior rating medio}
translate B OprepTrends {Tendencias de Resultados}
translate B OprepResults {Qtd. e frequencia de resultados}
translate B OprepLength {Tamanho do jogo}
translate B OprepFrequency {Frequencia}
translate B OprepWWins {Brancas vencem: }
translate B OprepBWins {Pretas vencem:  }
translate B OprepDraws {Empates:        }
translate B OprepWholeDB {toda a base de dados}
translate B OprepShortest {Vitorias mais rapidas}
translate B OprepMovesThemes {Movimentos e Temas}
translate B OprepMoveOrders {Ordem dos movimentos para atingir a posicao do relatorio}
translate B OprepMoveOrdersOne \
  {Houve apenas uma ordem de movimentos que atinge esta posicao: }
translate B OprepMoveOrdersAll \
  {Houve apenas %u ordens de movimentos que atingem esta posicao:}
translate B OprepMoveOrdersMany \
  {Houve %u ordens de movimentos que atingem esta posicao. As %u primeiras sao:}
translate B OprepMovesFrom {Movimentos da posicao do relatorio}
translate B OprepMostFrequentEcoCodes {Códigos ECO mais frequentes} 
translate B OprepThemes {Temas Posicionais}
translate B OprepThemeDescription {Frequencia dos temas nos primeiros  %u movimentos de cada jogo} 
translate B OprepThemeSameCastling {Roque do mesmo lado}
translate B OprepThemeOppCastling {Roques opostos}
translate B OprepThemeNoCastling {Ninguem efetuou o roque}
translate B OprepThemeKPawnStorm {Tempestade de Peoes no lado do Rei}
translate B OprepThemeQueenswap {Damas ja trocadas}
translate B OprepThemeWIQP {Peão branco da Dama isolado} 
translate B OprepThemeBIQP {Peão preto da Dama isolado} 
translate B OprepThemeWP567 {Peao Branco na 5/6/7a fila}
translate B OprepThemeBP234 {Peao Preto na 2/3/4a fila}
translate B OprepThemeOpenCDE {Colunas c/d/e abertas}
translate B OprepTheme1BishopPair {Um lado tem o par de Bispos}
translate B OprepEndgames {Finais}
translate B OprepReportGames {Jogos no Relatorio}
translate B OprepAllGames {Todos os jogos}
translate B OprepEndClass {Material ao fim de cada jogo}
translate B OprepTheoryTable {Tabela de Teoria}
translate B OprepTableComment {Gerada a partir dos %u jogos com rating mais alto.}
translate B OprepExtraMoves {Movimentos com nota extra na Tabela de Teoria}
translate B OprepMaxGames {Qtde. Maxima de jogos na tabela de teoria}
translate B OprepViewHTML {Visualizar HTML} 
translate B OprepViewLaTeX {Visualizar LaTeX} 

# Player Report:
translate B PReportTitle {Relatorio do Jogador} 
translate B PReportColorWhite {com as Brancas} 
translate B PReportColorBlack {com as Pretas} 
translate B PReportMoves {após %s} 
translate B PReportOpenings {Aberturas} 
translate B PReportClipbase {Limpa base de cópia e copia os jogos encontrados para ela} 

# Piece Tracker window:
translate B TrackerSelectSingle {Use o botão esquerdo do mouse para selecionar esta peça.} 
translate B TrackerSelectPair {Botão esquerdo seleciona esta peça; botão direito seleciona sua parceira.}
translate B TrackerSelectPawn {Botão esquerdo seleciona este Peão; botão direito seleciona os 8 Peões}
translate B TrackerStat {Estatisticas}
translate B TrackerGames {% jogos com movimentos para a casa}
translate B TrackerTime {% tempo em cada casa}
translate B TrackerMoves {Movimentos}
translate B TrackerMovesStart {Entre o número do movimento a partir do qual o acompanhamento deve ser feito.}
translate B TrackerMovesStop {Entre com o numero do movimento onde deve parar o acompanhamento.}

# Game selection dialogs:
translate B SelectAllGames {Todos os jogos na base de dados}
translate B SelectFilterGames {Apenas jogos no filtro}
translate B SelectTournamentGames {Somente jogos no torneio atual}
translate B SelectOlderGames {Somente jogos antigos}

# Delete Twins window:
translate B TwinsNote {Para serem duplicatas, dois jogos devem ter pelo menos os mesmos dois jogadores, alem de criterios que voce pode definir abaixo. Quando um par de duplicatas e encontrado, o jogo menor e deletado. Dica: e melhor fazer a verificacao ortografica da base de dados antes de remover duplicatas, pois isso melhora o processo de deteccao de duplicatas. }
translate B TwinsCriteria {Criterio: Duplicatas devem ter...}
translate B TwinsWhich {Jogos a examinar}
translate B TwinsColors {Jogadores com a mesma cor?}
translate B TwinsEvent {Mesmo evento?}
translate B TwinsSite {Mesmo lugar?}
translate B TwinsRound {Mesma rodada?}
translate B TwinsYear {Mesmo ano?}
translate B TwinsMonth {Mesmo mes?}
translate B TwinsDay {Mesmo dia?}
translate B TwinsResult {Mesmo resultado?}
translate B TwinsECO {Mesmo codigo ECO?}
translate B TwinsMoves {Mesmos movimentos?}
translate B TwinsPlayers {Comparacao dos nomes dos jogadores:}
translate B TwinsPlayersExact {Comparacao exata}
translate B TwinsPlayersPrefix {Primeiras 4 letras apenas}
translate B TwinsWhen {Quando deletar duplicatas}
translate B TwinsSkipShort {Ignorar todos os jogos com menos de 5 movimentos?}
translate B TwinsUndelete {Recuperar todos os jogos antes?}
translate B TwinsSetFilter {Definir filtro para todas as duplicatas deletadas?}
translate B TwinsComments {Manter sempre os jogos com comentarios?}
translate B TwinsVars {Manter sempre os jogos com variantes?}
translate B TwinsDeleteWhich {Excluir qual jogo:} 
translate B TwinsDeleteShorter {Menor jogo} 
translate B TwinsDeleteOlder {Menor numero de jogo} 
translate B TwinsDeleteNewer {Maior numero de jogo} 
translate B TwinsDelete {Deletar jogos}

# Name editor window:
translate B NameEditType {Tipo de nome para editar}
translate B NameEditSelect {Jogos para editar}
translate B NameEditReplace {Substituir}
translate B NameEditWith {com}
translate B NameEditMatches {Confere: Pressione Ctrl+1 a Ctrl+9 para selecionar}
translate B CheckGames {Marcar jogos}
translate B CheckGamesWhich {Marcar jogos}
translate B CheckAll {Todos os jogos}
translate B CheckSelectFilterGames {Somente jogos filtrados}

# Classify window:
translate B Classify {Classificar}
translate B ClassifyWhich {Que jogos devem ser classificados por ECO}
translate B ClassifyAll {Todos os Jogos (substituir codigos ECO antigos)}
translate B ClassifyYear {Todos os jogos do ultimo ano}
translate B ClassifyMonth {Todos os jogos do ultimo mes}
translate B ClassifyNew {Somente jogos ainda sem codigo ECO}
translate B ClassifyCodes {Codigos ECO a serem usados}
translate B ClassifyBasic {Codigos Basicos apenas ("B12", ...)}
translate B ClassifyExtended {Extensoes Scid ("B12j", ...)}

# Compaction:
translate B NameFile {Arquivo de nomes}
translate B GameFile {Arquivo de jogos}
translate B Names {Nomes}
translate B Unused {Nao usado}
translate B SizeKb {Tamanho (kb)}
translate B CurrentState {Estado Atual}
translate B AfterCompaction {Apos compactacao}
translate B CompactNames {Compactar arquivo de nomes}
translate B CompactGames {Compactar arquivo de nomes}
translate B NoUnusedNames "Não há nomes não utilizados, então o arquivo já está totalmente compactado."
translate B NoUnusedGames "O arquivo de jogo já está totalmente compactado."
translate B GameFileCompacted {O arquivo de jogos para a base está compactado.}

# Sorting:
translate B SortCriteria {Criterio}
translate B AddCriteria {Adicionar criterio}
translate B CommonSorts {Ordenacoes comuns}
translate B Sort {Ordenar}

# Exporting:
translate B AddToExistingFile {Adicionar jogos a um arquivo existente?}
translate B ExportComments {Exportar comentarios?}
translate B ExportVariations {Exportar variantes?}
translate B IndentComments {Identar Comentarios?}
translate B IndentVariations {Identar Variantes?}
translate B ExportColumnStyle {Estilo Coluna (um movimento por linha)?}
translate B ExportSymbolStyle {Estilo de anotacao simbolica:}
translate B ExportStripMarks {Remover códigos de marca de casa/seta dos comentários?} 

# Goto game/move dialogs:
translate B LoadGameNumber {Entre o numero do jogo a ser carregado:}
translate B GotoMoveNumber {Ir p/ o lance no.:}

# Copy games dialog:
translate B CopyGames {Copiar jogos}
translate B CopyConfirm {
 Voce realmente quer copiar
 os [::utils::thousands $nGamesToCopy] jogos filtrados
 da base de dados "$fromName"
 para a base de dados "$targetName"?
}
translate B CopyErr {Copia nao permitida}
translate B CopyErrSource {a base de dados origem}
translate B CopyErrTarget {a base de dados destino}
translate B CopyErrNoGames {nao tem jogos que atendam o filtro}
translate B CopyErrReadOnly {e apenas de leitura}
translate B CopyErrNotOpen {nao esta aberta}

# Colors:
translate B LightSquares {Casas Brancas}
translate B DarkSquares {Casas Pretas}
translate B SelectedSquares {Casas selecionadas}
translate B SuggestedSquares {Casas Sugeridas}
translate B WhitePieces {Pecas Brancas}
translate B BlackPieces {Pecas Pretas}
translate B WhiteBorder {Borda Branca}
translate B BlackBorder {Borda Preta}

# Novelty window:
translate B FindNovelty {Buscar Novidade}
translate B Novelty {Novidade}
translate B NoveltyInterrupt {Busca interrompida}
translate B NoveltyNone {Nenhuma novidade encontrada}
translate B NoveltyHelp {
Scid buscara o primeiro movimento do jogo atual que alcanca uma posicao nao encontrada na base selecionada ou no arquivo ECO.
}

# Sounds configuration:
translate B SoundsFolder {Pasta de arquivos de som} 
translate B SoundsFolderHelp {A pasta deve conter os arquivos King.wav, a.wav, 1.wav, etc} 
translate B SoundsAnnounceOptions {Opções de anuncio de movimentos} 
translate B SoundsAnnounceNew {Anuncia novos movimentos quando eles são feitos} 
translate B SoundsAnnounceForward {Anuncia movimentos quando avançar um movimento} 
translate B SoundsAnnounceBack {Anuncia movimentos quando retroceder um movimento} 
translate B SoundsSoundDisabled {O Scid não conseguiu encontrar o pacote de áudio Snack na inicialização;\nSound está desativado.}

# Upgrading databases:
translate B Upgrading {Atualizando}
translate B ConfirmOpenNew {
Esta e uma base em formato antigo (Scid 3) que nao pode ser aberta pelo Scid 4, mas uma versao no novo formato (Scid 4) ja foi criada.

Voce quer abrir a nova versao da base Scid 4?
}
translate B ConfirmUpgrade {
Esta e uma base em formato antigo (Scid 3). Uma versao da base no novo formato deve ser criada antes de poder ser usada no Scid 4.

A atualizacao criara uma nova versao da base; isto nao altera nem remove os registros originais.

Este processo pode levar algum tempo, mas so precisa ser feito uma vez e pode ser cancelado se estiver demorando muito.

Voce quer atualizar esta base agora?
}

# Recent files options:
translate B RecentFilesMenu {Numero de arquivos recentes no menu Arquivo} 
translate B RecentFilesExtra {Numero de arquivos recentes no submenu extra} 

# My Player Names options:
translate B MyPlayerNamesDescription {
Entre com uma lista dos nomes de jogadores preferidos, abaixo, um nome por linha. Substitutos (ex. "?" para qualquer caracter único, "*" para qualquer sequencia de caracteres) são permitidas.

Cada vez que um jogo com um jogador da lista é carregado, o tabuleiro da janela principal sofrerá rotação, se necessário, para mostrar o jogo da perspectiva desse jogador..
} 
translate B showblunderexists {mostra que existe um erro crasso}
translate B showblundervalue {mostra o valor do erro crasso}
translate B showscore {mostra o score}
translate B coachgame {auxilio ao jogo}
translate B configurecoachgame {configura auxilio ao jogo}
translate B configuregame {Configuração do jogo}
translate B Phalanxengine {Engine Phalanx}
translate B Coachengine {Engine de auxilio}
translate B difficulty {dificuldade}
translate B hard {dificil}
translate B easy {facil}
translate B Playwith {Jogar com}
translate B white {branca}
translate B black {preta}
translate B both {ambas}
translate B Play {Jogar}
translate B Noblunder {Nenhum erro crasso}
translate B blunder {erro crasso}
translate B Noinfo {-- Sem informações --}
translate B PhalanxOrTogaMissing {Phalanx ou Toga não encontrado}
translate B moveblunderthreshold {movimento e um erro crasso se a perda e maior que}
translate B limitanalysis {limita tempo de analise do engine}
translate B seconds {segundos}
translate B Abort {Abortar}
translate B Resume {Continuar}
translate B OutOfOpening {Fora da abertura}
translate B NotFollowedLine {Você não seguiu a linha}
translate B DoYouWantContinue {Voce quer continuar ?}
translate B CoachIsWatching {Auxilio esta observando}
translate B Ponder {Pensando permanentemente}
translate B LimitELO {Limite da força ELO}
translate B DubiousMovePlayedTakeBack {Movimento duvidoso, quer voltar ?}
translate B WeakMovePlayedTakeBack {Movimento fraco, quer voltar ?}
translate B BadMovePlayedTakeBack {Movimento ruim, quer voltar ?}
translate B Iresign {Eu desisto}
translate B yourmoveisnotgood {seu movimento nao e bom}
translate B EndOfVar {Fim da variante}
translate B Openingtrainer {Treinador de aberturas}
translate B DisplayCM {Mostrar movimentos candidatos}
translate B DisplayCMValue {Mostrar valor dos movimentos candidatos}
translate B DisplayOpeningStats {Mostrar estatisticas}
translate B ShowReport {Mostrar relatorio}
translate B NumberOfGoodMovesPlayed {bom movimentos jogados}
translate B NumberOfDubiousMovesPlayed {movimentos duvidosos jogados}
translate B NumberOfMovesPlayedNotInRepertoire {movimentos jogados não existem no repertorio}
translate B NumberOfTimesPositionEncountered {vezes a posição foi encontrada}
translate B PlayerBestMove  {Permitir somente os melhores movimentos}
translate B OpponentBestMove {Oponente joga os melhores movimentos}
translate B OnlyFlaggedLines {Somente linhas marcadas}
translate B resetStats {Reiniciar estatisticas}
translate B Repertoiretrainingconfiguration {Configuração do repertorio de treinamento}
translate B Loadingrepertoire {Carregando repertorio}
translate B Movesloaded {Movimentos carregados}
translate B Repertoirenotfound {Repertorio não encontrado}
translate B Openfirstrepertoirewithtype {Abra primeiro uma base de repertorio com o ícone/tipo definido do lado direito}
translate B Movenotinrepertoire {Movimento não existe no repertorio}
translate B PositionsInRepertoire {Posições no repertorio}
translate B PositionsNotPlayed {Posições não jogadas}
translate B PositionsPlayed {Posições jogadas}
translate B Success {Successo}
translate B DubiousMoves {Movimentos duvidosos}
translate B OutOfRepertoire {Fora do Repertorio}
translate B ConfigureTactics {Configurar taticas}
translate B ResetScores {Reiniciar scores}
translate B LoadingBase {Carregando base}
translate B Tactics {Taticas}
translate B ShowSolution {Mostrar solução}
translate B NextExercise {Proximo exercicio}
translate B PrevExercise {exercicio anterior}
translate B StopTraining {Parar treinamento}
translate B Next {Proximo}
translate B ResettingScore {Reinicializando score}
translate B LoadingGame {Carregando jogo}
translate B MateFound {Mate encontrado}
translate B BestSolutionNotFound {Melhor solução NÃO encontrada !}
translate B MateNotFound {Mate não encontrado}
translate B ShorterMateExists {Existe mate mais curto}
translate B ScorePlayed {Score obtido}
translate B Expected {esperado}
translate B ChooseTrainingBase {Escolher base de treinamento}
translate B Thinking {Pensando}
translate B AnalyzeDone {Analise completa}
translate B WinWonGame {Win venceu o jogo}
translate B Lines {Linhas}
translate B ConfigureUCIengine {Configurar engine UCI}
translate B SpecificOpening {Abertura especifica}
translate B StartNewGame {Iniciar novo jogo}
translate B FixedLevel {Nivel fixo}
translate B Opening {Abertura}
translate B RandomLevel {Nivel aleatorio}
translate B StartFromCurrentPosition {Iniciar a paartir da posição corrente}
translate B FixedDepth {Profundidade fixa}
translate B Nodes {Nós} 
translate B Depth {Profundidade}
translate B Time {Tempo} 
translate B SecondsPerMove {Segundos por movimento}
translate B Engine {Engine}
translate B TimeMode {Modo de tempo}
translate B TimeBonus {Bonus de tempo}
translate B TimeMin {min}
translate B TimeSec {seg}
translate B AllExercisesDone {Todos os exercicios completos}
translate B MoveOutOfBook {Movimento fora do livro}
translate B LastBookMove {Ultimo movimento do livro}
translate B AnnotateSeveralGames {Anotar diversos jogos\ndo atual até :}
translate B FindOpeningErrors {Encontrar erros na abertura}
translate B MarkTacticalExercises {Marcar exercicios taticos}
translate B UseBook {Usar livro}
translate B MultiPV {Variantes multiplas}
translate B Hash {Varrer memória}
translate B OwnBook {Usar livro do engine}
translate B BookFile {Livro de aberturas}
translate B AnnotateVariations {Anotar variantes}
translate B ShortAnnotations {Anotações curtas}
translate B addAnnotatorTag {Adicionar tag do anotador}
translate B AddScoreToShortAnnotations {Adicionar o score às anotações curtas}
translate B Export {Exportar}
translate B BookPartiallyLoaded {Livro parcialmente carregado}
translate B Calvar {Calculo de variantes}
translate B ConfigureCalvar {Configuração}
translate B Reti {Reti}
translate B English {Inglesa}
translate B d4Nf6Miscellaneous {1.d4 Nf6 Diversas}
translate B Trompowsky {Trompowsky}
translate B Budapest {Budapest}
translate B OldIndian {India antiga}
translate B BenkoGambit {Gambito Benko}
translate B ModernBenoni {Modern Benoni}
translate B DutchDefence {Defesa Holandesa}
translate B Scandinavian {Escandinava}
translate B AlekhineDefence {Defesa Alekhine}
translate B Pirc {Pirc}
translate B CaroKann {Caro-Kann}
translate B CaroKannAdvance {Caro-Kann Advance}
translate B Sicilian {Siciliana}
translate B SicilianAlapin {Siciliana Alapin}
translate B SicilianClosed {Siciliana Fechada}
translate B SicilianRauzer {Siciliana Rauzer}
translate B SicilianDragon {Siciliana Dragao}
translate B SicilianScheveningen {Siciliana Scheveningen}
translate B SicilianNajdorf {Siciliana Najdorf}
translate B OpenGame {Jogo aberto}
translate B Vienna {Vienna}
translate B KingsGambit {Gambito do Rei}
translate B RussianGame {Russa}
translate B ItalianTwoKnights {Italiana/Dois Cavalos}
translate B Spanish {Espanhola}
translate B SpanishExchange {Espanhola Trocas}
translate B SpanishOpen {Espanhola Aberta}
translate B SpanishClosed {Espanhola Fechada}
translate B FrenchDefence {Defesa Francesa}
translate B FrenchAdvance {Francesa Avanco}
translate B FrenchTarrasch {Francesa Tarrasch}
translate B FrenchWinawer {Francesa Winawer}
translate B FrenchExchange {Francesa Trocas}
translate B QueensPawn {Peao da Dama}
translate B Slav {Eslava}
translate B QGA {QGA}
translate B QGD {QGD}
translate B QGDExchange {QGD Trocas}
translate B SemiSlav {Semi-Eslava}
translate B QGDwithBg5 {QGD com Bg5}
translate B QGDOrthodox {QGD Ortodoxa}
translate B Grunfeld {Grünfeld}
translate B GrunfeldExchange {Grünfeld Trocas}
translate B GrunfeldRussian {Grünfeld Russa}
translate B Catalan {Catalan}
translate B CatalanOpen {Catalan Aberta}
translate B CatalanClosed {Catalan Fechada}
translate B QueensIndian {India da Dama}
translate B NimzoIndian {Nimzo-India}
translate B NimzoIndianClassical {Nimzo-India Classica}
translate B NimzoIndianRubinstein {Nimzo-India Rubinstein}
translate B KingsIndian {India do Rei}
translate B KingsIndianSamisch {India do Rei Sämisch}
translate B KingsIndianMainLine {India do Rei Linha Principal}
translate B ConfigureFics {Configurar FICS}
translate B FICSGuest {Login como convidado}
translate B FICSServerPort {Porta do Servidor}
translate B FICSServerAddress {Endereço IP}
translate B FICSRefresh {Atualizar}
translate B FICSTimesealPort {Porta Timeseal}
translate B FICSSilence {Silencio}
translate B FICSOffers {Ofertas}
translate B FICSConsole {Console}
translate B FICSGames {Jogos}
translate B FICSUnobserve {Parar de observar jogo}
translate B FICSProfile {Exibir seu perfil e historico}
translate B FICSRelayedGames {Jogos conectados}
translate B FICSFindOpponent {Encontrar Oponente}
translate B FICSTakeback {Voltar}
translate B FICSTakeback2 {Voltar 2}
translate B FICSInitTime {Tempo inicial (min)}
translate B FICSIncrement {Incremento (seg)}
translate B FICSRatedGame {Jogo com rate}
translate B FICSAutoColour {automatico}
translate B FICSManualConfirm {confirmar manualmente}
translate B FICSFilterFormula {Filtro com formula}
translate B FICSIssueSeek {Busca item}
translate B FICSChallenge {Desafiar}
translate B FICSAccept {aceitar}
translate B FICSDecline {recusar}
translate B FICSColour {Cor}
translate B FICSSend {enviar}
translate B FICSConnect {Conectar}
translate B FICSdefaultuservars {Usar variaveis default}
translate B FICSObserveconfirm {Voce quer observar o jogo}
translate B FICSpremove {Habilita Pre-movimento}
translate B GameReview {Rever jogo}
translate B GameReviewTimeExtended {Tempo estendido}
translate B GameReviewMargin {Mergem de erro}
translate B GameReviewAutoContinue {Continuar automaticamente quando o movimento estiver correto}
translate B GameReviewReCalculate {Usar tempo estendido}
translate B GameReviewAnalyzingMovePlayedDuringTheGame {Analisando movimento feito durante o jogo}
translate B GameReviewAnalyzingThePosition {Analisando a posição}
translate B GameReviewEnterYourMove {Entre seu movimento}
translate B GameReviewCheckingYourMove {Verificando seu movimento}
translate B GameReviewYourMoveWasAnalyzed {Seu movimento foi analisado}
translate B GameReviewYouPlayedSameMove {Você usou o mesmo movimento como na resposta}
translate B GameReviewScoreOfYourMove {Score do seu movimento}
translate B GameReviewGameMoveScore {Score do movimento do jogo}
translate B GameReviewEngineScore {Score do Engine}
translate B GameReviewYouPlayedLikeTheEngine {Voce jogou como o engine}
translate B GameReviewNotEngineMoveButGoodMove {Não é o movimento do engine, mas é um bom movimento}
translate B GameReviewMoveNotGood {Este movimento não é bom; seu score e}
translate B GameReviewMovesPlayedLike {Movimentos semelhantes}
translate B GameReviewMovesPlayedEngine {Movimentos semelhantes aos do engine}
translate B CCDlgConfigureWindowTitle {Configurar Xadrez por correspondencia}
translate B CCDlgCGeneraloptions {Opções Gerais}
translate B CCDlgDefaultDB {Base default:}
translate B CCDlgInbox {Caixa de Entrada (caminho):}
translate B CCDlgOutbox {Caixa de Saida (caminho):}
translate B CCDlgXfcc {Configuraçãao do Xfcc:}
translate B CCDlgExternalProtocol {Tratamento de protocolo externo (ex. Xfcc)}
translate B CCDlgFetchTool {Ferramenta de busca:}
translate B CCDlgSendTool {Ferramenta de envio:}
translate B CCDlgEmailCommunication {Comunicação por eMail}
translate B CCDlgMailPrg {Programa de Mail:}
translate B CCDlgBCCAddr {Endereço Cópia Oculta:}
translate B CCDlgMailerMode {Modo:}
translate B CCDlgThunderbirdEg {ex. Thunderbird, Mozilla Mail, Icedove...}
translate B CCDlgMailUrlEg {ex. Evolution}
translate B CCDlgClawsEg {ex. Sylpheed Claws}
translate B CCDlgmailxEg {ex. mailx, mutt, nail...}
translate B CCDlgAttachementPar {Parametro de anexos:}
translate B CCDlgInternalXfcc {Usar suporte internal Xfcc}
translate B CCDlgConfirmXfcc {Confirmar movimentos}
translate B CCDlgSubjectPar {Parametro de Assunto:}
translate B CCDlgDeleteBoxes {Esvaziar caixas de entrada e saída}
translate B CCDlgDeleteBoxesText {Voce quer realmente esvaziar as caixas de Entrada e Saida usadas para o Xadrez por correspondência? Esta operação exige uma novaa sincronização para mostrar o ultimo estado dos seus jogos}
translate B CCDlgConfirmMove {Confirmar movimento}
translate B CCDlgConfirmMoveText {Se voce confirmar, o movimento indicado a seguir e os comentários serão enviados para o servidor:}
translate B CCDlgDBGameToLong {Linha principal inconsistente}
translate B CCDlgDBGameToLongError {A linha principal na sua base é maior do que o jogo que está na caixa de entrada. Se a caixa de entrada contem jogos correntes, isto é logo após uma sincronização, alguns movimentos foram adicionados erroneamente à linha principal na base.\nNeste caso, por favor, diminua a linha principal para (no maximo) movimentos\n}
translate B CCDlgStartEmail {Iniciar novo jogo por eMail}
translate B CCDlgYourName {Seu nome:}
translate B CCDlgYourMail {Seu eMail:}
translate B CCDlgOpponentName {Nome do Oponente:}
translate B CCDlgOpponentMail {eMail do Oponente:}
translate B CCDlgGameID {ID do jogo (unico):}
translate B CCDlgTitNoOutbox {Scid: Caixa de Saida}
translate B CCDlgTitNoInbox {Scid: Caixa de Entrada}
translate B CCDlgTitNoGames {Scid: Nenhum jogo por correspondência}
translate B CCErrInboxDir {Diretorio da Caixa de Entrada:}
translate B CCErrOutboxDir {Diretorio da Caixa de Saida:}
translate B CCErrDirNotUsable {não existe ou não está acessivel!\nPor favor, verifique e corrija a configuração.}
translate B CCErrNoGames {não contem nenhum jogo!\nPor favor, localize-os primeiro.}
translate B CCDlgTitNoCCDB {Scid: Nenhuma base de correspondência}
translate B CCErrNoCCDB {Nenhuma base do tipo 'Correspondencia' está aberta. Por favor, abra uma antes de usar as funcoes do xadrez por correspondencia.}
translate B CCFetchBtn {Busca jogos no servidor e processa a Caixa de Entrada}
translate B CCPrevBtn {Ir para o jogo anterior}
translate B CCNextBtn {Ir para o proximo jogo}
translate B CCSendBtn {Enviar movimento}
translate B CCEmptyBtn {Esvaziar caixas de entrada e saida}
translate B CCHelpBtn {Ajuda sobre icones e indicadores de estado.\nPara ajuda geral, use a tecla F1!}
translate B CCDlgServerName {Nome do Servidor:}
translate B CCDlgLoginName  {Login Name:}
translate B CCDlgPassword   {Senha:}
translate B CCDlgShowPassword {Mostra a Senha}
translate B CCDlgURL        {Xfcc-URL:}
translate B CCDlgRatingType {Tipo de Rating:}
translate B CCDlgDuplicateGame {ID de jogo não único}
translate B CCDlgDuplicateGameError {Este jogo existe mais de uma vez em sua base. Exclua todas as duplicatas e compacte seu arquivo de jogos (Arquivo/Manutenção/Compactar Base).}
translate B CCDlgSortOption {Ordenando:}
translate B CCDlgListOnlyOwnMove {Somente jogos nos quais tenho o movimento}
translate B CCOrderClassicTxt {Local, Evento, Rodada, Resultado, Branca, Preta}
translate B CCOrderMyTimeTxt {Meu Relógio}
translate B CCOrderTimePerMoveTxt {Tempo por movimento até o próximo controle de tempo}
translate B CCOrderStartDate {Data de Inicio}
translate B CCOrderOppTimeTxt {Relogio do Oponente}

translate B CCDlgConfigRelay {Observar jogos}
translate B CCDlgConfigRelayHelp {Consulta a pagina de jogos em http://www.iccf-webchess.com e mostra o jogo a ser observado.  Se voce enxergar o tabuleiro, copie a URL do seu navegador para a lista abaixo. Apenas uma URL por linha!\nExemplo: http://www.iccf-webchess.com/MakeAMove.aspx?id=266452}

translate B ExtHWConfigConnection {Configurar hardware externo}
translate B ExtHWPort {Porta}
translate B ExtHWEngineCmd {Comando do Engine}
translate B ExtHWEngineParam {Parametro do Engine}
translate B ExtHWShowButton {Mostrar botão}
translate B ExtHWHardware {Hardware}
translate B ExtHWNovag {Novag Citrine}
translate B ExtHWInputEngine {Entrar com o Engine}
translate B ExtHWNoBoard {Nenhum tabuleiro}
translate B NovagReferee {Arbitro}
translate B IEConsole {Entrar na console do Engine}
translate B IESending {Movimentos enviados para}
translate B IESynchronise {Sincronizar}
translate B IERotate  {Girar}
translate B IEUnableToStart {Não é possivel iniciar Engine:}
translate B DoneWithPosition {Posição definida}
translate B Board {Tabuleiro}
translate B showGameInfo {Mostrar informações do jogo}
translate B autoResizeBoard {Tamanho automático do tabuleiro}
translate B DockTop {Mover para cima}
translate B DockBottom {Mover para o final}
translate B DockLeft {Mover para a esquerda}
translate B DockRight {Mover para a direita}
translate B Undock {Desacoplar}
translate B ChangeIcon {Alterar icone...}
translate B NewGameListWindow {Lista de Novos Jogos}
translate B LoadatStartup {Carregar ao inicializar}
translate B ShowHideDB {Exibe/Oculta base de dados}
translate B ChangeFilter {Altera filtro}
translate B ChangeLayout {Carrega/Salva/Altera criterio de ordenacao e layout das colunas}
translate B ShowHideStatistic {Exibe/Oculta Estatisticas}
translate B BoardFilter {Mostra apenas jogos que atendam a posicao atual no tabuleiro}
translate B CopyGameTo {Copiar jogo para}
translate B FindBar {Barra de Pesquisa}
translate B FindCurrentGame {Pesquisar jogo atual}
translate B DeleteGame {Excluir jogo}
translate B UndeleteGame {Recuperar jogo deletado}
translate B ResetSort {Reiniciar ordenacao}
translate B ConvertNullMove {Converter movimentos nullos para comentarios}
translate B SetupBoard {Definir tabuleiro}
translate B Rotate {Rotacionar}
translate B SwitchColors {Trocar cores}
translate B FlipBoard {Virar o tabuleiro}
translate B ImportPGN {Importar jogo em PGN}
translate B ImportingFiles {Importar arquivos PGN para}
translate B ImportingFrom {Importando de}
translate B ImportingIn {Importar jogos para}
translate B UseLastTag {Usar ultimas\ntags do jogo}
translate B Random {Aleatorio}
translate B BackToMainline {Voltar para linha principal}
translate B LeaveVariant {Deixar variante}
translate B Autoplay {Jogo automatico}
translate B ShowHideCoords {Exibir/Ocultar Coord.}
translate B ShowHideMaterial {Exibir/Ocultar Material}
translate B FullScreen {Tela Cheia}
translate B FilterStatistic {Estatisticas de Filtro}
translate B MakeCorrections {Fazer Correcoes}
translate B Surnames {Sobrenomes}
translate B Ambiguous {Ambiguo}
translate B OptionsToolbar "Barra de Ferramentas da Janela Principal"
translate B OptionsBoard "Tabuleiro"
translate B OptionsMenuColor "Define Cores dos Menus"
translate B OptionsBoardSize "Tamanho do Tabuleiro"
translate B OptionsBoardPieces "Estilo de Pecas no Tabuleiro"
# ====== TODO To be translated ======
translate B OptionsInternationalization "Internationalization"
translate B MenuColorForeground "Cor do Texto"
translate B MenuColorBackground "Cor de Fundo"
translate B MenuColorSelect "Selecionar"
translate B OptionsTablebaseDir "Selecione até 4 pastas tabela base de finais:"
}

# end of portbr.tcl
