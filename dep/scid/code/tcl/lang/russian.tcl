### russian.tcl:
#  Russian language support for Scid.
#  Contributed by Alex Sedykh.
#  Contributed by Sergey Nikolaevich Koyankin (koyankin(at)yandex.ru).
#  Contributed by Stepan Kashuba (ihaterabbids(at)gmail.com).
#  Untranslated messages are marked with a "***" comment.
#  Untranslated help page sections are in <NEW>...</NEW> tags.

proc setLanguage_R {} {

# File menu:
menuText R File "Файл" 0
menuText R FileNew "Новый..." 0 {Создать новую базу данных Scid}
menuText R FileOpen "Открыть..." 0 {Открыть базу данных Scid}
menuText R FileClose "Закрыть" 0 {Закрыть активную базу данных Scid}
menuText R FileFinder "Поиск" 0 {Открыть окно поиска файла}
menuText R FileBookmarks "Закладки" 2 {Меню закладок (комбинация: Ctrl+B)}
menuText R FileBookmarksAdd "Добавить закладку" 0 \
  {Отметить партию или позицию в текущей базе данных}
menuText R FileBookmarksFile "Архивировать закладку" 0 \
  {Архивировать закладку для текущей партии или позиции}
menuText R FileBookmarksEdit "Редактировать закладки..." 0 \
  {Редактировать меню закладок}
menuText R FileBookmarksList "Показать папки как простой список" 0 \
  {Показать папки закладок, как обычный список, а не подменю}
menuText R FileBookmarksSub "Показать папки как подменю" 1 \
  {Показать папки закладок как подменю, а не простой список}
menuText R FileMaint "Обслуживание" 2 {Инструменты обслуживания базы данных Scid}
menuText R FileMaintWin "Окно обслуживания" 0 \
  {Открыть/закрыть окно обслуживания базы данных Scid}
menuText R FileMaintCompact "Сжать базу данных..." 0 \
  {Сжать файлы базы данных, выкинуть удалённые партии и неиспользуемые имена}
menuText R FileMaintClass "ECO-классификация партии..." 0 \
  {Пересчитать коды ECO для всех партий}
menuText R FileMaintSort "Сортировать базу данных..." 2 \
  {Сортировать все партии в базе данных}
menuText R FileMaintDelete "Удалить партии-двойники..." 0 \
  {Найти партии-двойники и пометить их для удаления}
menuText R FileMaintTwin "Окно проверки двойников" 14 \
  {Открыть/обновить окно проверки двойников}
menuText R FileMaintName "Правописание имён" 0 {Редактирование имён и инструменты правописания}
menuText R FileMaintNameEditor "Редактор имён" 0 \
  {Открыть/закрыть окно редактора имён}
menuText R FileMaintNamePlayer "Проверка имён игроков..." 9 \
  {Проверка имён игроков с помощью файла правописания}
menuText R FileMaintNameEvent "Проверка названий турниров..." 18 \
  {Проверка названий турниров с помощью файла правописания}
menuText R FileMaintNameSite "Проверка названий мест..." 18 \
  {Проверка названий мест с помощью файла правописания}
menuText R FileMaintNameRound "Проверка названий раундов..." 20 \
  {Проверка названий раундов с помощью файла правописания}
menuText R FileReadOnly "Только для чтения..." 0 \
  {Трактовать текущую базу данных как "только для чтения", предотвращая изменения}
menuText R FileSwitch "Переключить базу данных" 6 \
  {Переключить на другую открытую базу данных}
menuText R FileExit "Выход" 0 {Выход из Scid}
menuText R FileMaintFixBase "Исправить повреждённую базу" 0 {Попытаться исправить повреждённую базу}

# Edit menu:
menuText R Edit "Правка" 0
menuText R EditAdd "Добавить вариант" 0 {Добавить вариант к этому ходу партии}
menuText R EditDelete "Удалить вариант" 0 {Удалить вариант для этого хода}
menuText R EditFirst "Сделать вариант первым" 0 \
  {Продвинуть вариант на первое место в списке}
menuText R EditMain "Заменить главный план вариантом" 0 \
  {Сделать вариант основным планом партии}
menuText R EditTrial "Попробовать вариант" 0 \
  {Запустить/закончить пробный режим, для проверки идеи на доске}
menuText R EditStrip "Убрать" 1 {Убрать комментарии или варианты из партии}
menuText R EditUndo "Отменить" 0 {Отменить изменения в последней партии}
menuText R EditRedo "Вернуть" 0 {Вернуть изменения в последней партии}
menuText R EditStripComments "Комментарии" 0 \
  {Убрать все комментарии и аннотации из этой партии}
menuText R EditStripVars "Варианты" 0 {Убрать все варианты из этой партии}
menuText R EditStripBegin "Двигаться от начала" 1 \
  {Вырезает ходы от начала партии}
menuText R EditStripEnd "Двигаться к концу" 0 \
  {Вырезает ходы к концу партии}
menuText R EditReset "Очистить " 0 \
  {Полностью очистить буферную базу}
menuText R EditCopy "Скопировать эту партию в буферную базу" 1 \
  {Скопировать эту партию в буферную базу}
menuText R EditPaste "Вставить последнюю партию из буферной базы" 0 \
  {Вставить активную партию из буферной базы здесь}
menuText R EditPastePGN "Вставить текст из буфера обмена как PGN партию..." 18 \
  {Интерпетировать текст в буфере обмена как партию в PGN нотации и вставить сюда}
menuText R EditSetup "Установить начальную позицию..." 2 \
  {Установить стартовую позицию для этой партии}
menuText R EditCopyBoard "Копировать позицию" 4 \
  {Копировать текущую позицию в нотации FEN в выбранный текст (буфер обмена)}
menuText R EditPasteBoard "Вставить начальную позицию" 3 \
  {Вставить начальную позицию из текущего выбранного текста (буфера обмена)}
menuText R ConfigureScid "Настройки..." 0 {Настроить все опции SCID}

# Game menu:
menuText R Game "Партия" 0
menuText R GameNew "Новая партия" 0 \
  {Установить партию в начальное положение, отбросив все изменения}
menuText R GameFirst "Загрузить первую партию" 0 {Загрузить первую фильтрованную партию}
menuText R GamePrev "Загрузить предыдущую партию" 1 {Загрузить предыдущую фильтрованную партию}
menuText R GameReload "Перезагрузить текущую партию" 1 \
  {Перезагрузить эту партию, сбросив все сделанные изменения}
menuText R GameNext "Загрузить следующую партию" 2 {Загрузить следующую фильтрованную партию}
menuText R GameLast "Загрузить последнюю партию" 3 {Загрузить последнюю фильтрованную партию}
menuText R GameRandom "Загрузить случайную партию" 4 {Загрузить случайную фильтрованную партию}
menuText R GameNumber "Загрузить партию номер..." 6 \
  {Загрузить партию, введя её номер}
menuText R GameReplace "Сохранить: заменить партию..." 0 \
  {Сохранить эту партию, заменив старую версию}
menuText R GameAdd "Сохранить: добавить новую партию..." 2 \
  {Сохранить эту партию в базу данных, как новую}
menuText R GameDeepest "Определить дебют" 0 \
  {Найти самую позднюю позицию партии, имеющуюся в книге ECO}
menuText R GameGotoMove "Перейти к ходу номер..." 5 \
  {Перейти к определённому ходу текущей партии}
menuText R GameNovelty "Найти новинку..." 2 \
  {Найти первый ход в этой партии, который раньше не применялся}

# Search Menu:
menuText R Search "Поиск" 1
menuText R SearchReset "Сбросить фильтр" 0 {Сбросить фильтр, теперь все партии включены}
menuText R SearchNegate "Инвертировать фильтр" 0 {Обратить фильтр, включить только исключённые партии}
menuText R SearchCurrent "Текущая позиция..." 0 {Поиск текущей позиции}
menuText R SearchHeader "Заголовок..." 0 {Поиск по заголовку (игрок, турнир, и т. д.)}
menuText R SearchMaterial "Материал/позиция..." 0 {Поиск по материалу или образцам позиции}
menuText R SearchUsing "Использовать файл поиска..." 0 {Поиск с использованием файла с установками поиска}

# Windows menu:
menuText R Windows "Окна" 0
menuText R WindowsComment "Редактор комментариев" 0 {Открыть/закрыть редактор комментариев}
menuText R WindowsGList "Список партий" 0 {Открыть/закрыть окно списка партий}
menuText R WindowsPGN "Окно PGN" 0 \
  {Открыть/закрыть окно PGN (нотации партии)}
menuText R WindowsPList "Поиск игрока" 2 {Открыть/закрыть окно поиска игрока}
menuText R WindowsTmt "Поиск турнира" 0 {Открыть/закрыть окно поиска турниров}
menuText R WindowsSwitcher "Выбор базы данных" 1 \
  {Открыть/закрыть окно переключателя баз данных}
menuText R WindowsMaint "Окно обслуживания" 1 \
  {Открыть/закрыть окно обслуживания}
menuText R WindowsECO "Просмотрщик ECO" 4 {Открыть/закрыть окно просмотра ECO}
menuText R WindowsStats "Окно статистики" 2 \
  {Открыть/закрыть окно фильтрованной статистики}
menuText R WindowsTree "Окно дерева" 10 {Открыть/закрыть окно дерева}
menuText R WindowsTB "Окно таблиц эндшпиля" 10 {Открыть/закрыть окно таблиц эндшпиля}
menuText R WindowsBook "Окно книги" 0 {Открыть/закрыть окно книги}
menuText R WindowsCorrChess "Окно переписки" 0 {Открыть/закрыть окно переписки}

# Tools menu:
menuText R Tools "Инструменты" 2
menuText R ToolsAnalysis "Аналитический движок №1..." 0 \
  {Запустить/остановить первый аналитический шахматный движок}
menuText R ToolsAnalysis2 "Аналитический движок №2..." 22 \
  {Запустить/остановить второй аналитический шахматный движок}
menuText R ToolsCross "Турнирная таблица" 0 {Показать турнирную таблицу для этой партии}
menuText R ToolsEmail "Менеджер писем" 0 {Открыть/закрыть окно шахматного менеджера писем}
menuText R ToolsFilterGraph "Диаграмма фильтра" 0 {Открыть/закрыть окно фильтрованной диаграммы}
menuText R ToolsAbsFilterGraph "Диаграмма фильтра абсолютных значений" 7 {Открыть/закрыть окно диаграммы фильтра абсолютных значений}
menuText R ToolsOpReport "Дебютный отчёт" 0 {Генерировать дебютный отчёт для текущей позиции}
menuText R ToolsOpenBaseAsTree "Открыть базу в виде дерева" 0 {Открывает базу и использует её в древовидном окне}
menuText R ToolsOpenRecentBaseAsTree "Открыть последнюю базу как дерево" 0   {Открывает последнюю базу и использует её в древовидном окне}
menuText R ToolsTracker "Положение фигуры"  4 {Открыть окно положения фигуры}
menuText R ToolsTraining "Обучение"  0 {Обучающие инструменты (тактика, дебюты, ...)}
menuText R ToolsTacticalGame "Тактические партии"  0 {Играть тактические партии}
menuText R ToolsSeriousGame "Серьёзная игра"  0 {Играть серьёзную партию}
menuText R ToolsTrainOpenings "Дебюты"  0 {Тренировки с репертуаром}
menuText R ToolsTrainReviewGame "Обзор партии"  0 {Обдумывание сделанных ходов}
menuText R ToolsTrainTactics "Тактика"  0 {Решать тактические задачи}
menuText R ToolsTrainCalvar "Расчёт вариантов"  0 {Тренировка расчёта вариантов}
menuText R ToolsTrainFindBestMove "Поиск лучшего хода"  0 {Искать лучший ход}
menuText R ToolsTrainFics "Играть на FICS"  0 {Игра на сервере freechess.org}
menuText R ToolsEngineTournament "Турнир движков"  0 {Начать турнир между шахматными движками}
menuText R ToolsBookTuning "Настройка книги" 0 {Настройка книги}
menuText R ToolsConnectHardware "Подключение к оборудованию" 0 {Подключение к внешнему оборудованию}
menuText R ToolsConnectHardwareConfigure "Настройка..." 0 {Настройка внешнего оборудования и подключения}
menuText R ToolsConnectHardwareNovagCitrineConnect "Соединение с компьютером Novag Citrine" 0 {Соединение с компьютером Novag Citrine}
menuText R ToolsConnectHardwareInputEngineConnect "Соединение с входящим движком" 0 {Соединение Scid с входящим движком (например, доска DGT)}

menuText R ToolsPInfo "Информация об игроке"  1 \
  {Открыть/обновить окно информации об игроке}
menuText R ToolsPlayerReport "Отчёт об игроке..." 3 \
  {Generate a player report}
menuText R ToolsRating "Диаграмма рейтинга" 1 \
  {Диаграмма истории рейтинга для игроков текущей партии}
menuText R ToolsScore "Диаграмма счёта" 2 {Показать окно диаграммы счёта}
menuText R ToolsExpCurrent "Экспорт текущей партии" 0 \
  {Записать текущую партию в текстовый файл}
menuText R ToolsExpCurrentPGN "Экспорт партии в файл PGN..." 0 \
  {Записать текущую партию в файл PGN}
menuText R ToolsExpCurrentHTML "Экспорт партии в файл HTML..." 0 \
  {Записать текущую партию в файл HTML}
menuText R ToolsExpCurrentHTMLJS "Экспорт партии в HTML и JavaScript файл..." 0 {Записать текущую партию HTML и JavaScript файл}
menuText R ToolsExpCurrentLaTeX "Экспорт партии в файл LaTeX..." 0 \
  {Записать текущую партию в файл LaTeX}
menuText R ToolsExpFilter "Экспорт всех фильтрованных партий" 11 \
  {Записать все фильтрованные партии в текстовый файл}
menuText R ToolsExpFilterPGN "Экспорт фильтрованных партий в файл PGN..." 0 \
  {Записать все фильтрованные партии в файл PGN}
menuText R ToolsExpFilterHTML "Экспорт фильтрованных партий в файл HTML..." 0 \
  {Записать все фильтрованные партии в файл HTML}
menuText R ToolsExpFilterHTMLJS "Экспорт фильтрованных партий в HTML и JavaScript файл..." 17 {Записывает все фильтрованные партии в HTML и JavaScript файл}
menuText R ToolsExpFilterLaTeX "Экспорт фильтрованных партий в файл LaTeX..." 3 \
  {Записать все фильтрованные партии в файл LaTeX}
menuText R ToolsImportOne "Импорт одной партии PGN..." 0 \
  {Импорт партии из текстового файла PGN}
menuText R ToolsImportFile "Импорт файла партий PGN..." 9 {Импорт партий из файла PGN}
menuText R ToolsStartEngine1 "Запустить движок 1" 0  {Запускает движок 1}
menuText R ToolsStartEngine2 "Запустить движок 2" 0  {Запускает движок 2}
menuText R ToolsCaptureBoard "Фиксировать текущую позицию..." 0  {Сохранить текущую позицию как изображение.}

# Play menue
menuText R Play "Играть" 1

# --- Correspondence Chess
menuText R CorrespondenceChess "Шахматы по переписке" 0 {Функции для игры в шахматы по переписке через электронную почту и Xfcc}
menuText R CCConfigure "Настройка..." 0 {Настройки внешних инструментов и основные установки}
menuText R CCConfigRelay "Настройка слежения.." 10 {Настроить партии для наблюдения}
menuText R CCOpenDB "Открыть базу данных..." 0 {Открыть базу данных переписки по-умолчанию}
menuText R CCRetrieve "Получить партии" 0 {Получить партии через внешний (Xfcc-)помощник}
menuText R CCInbox "Обработать Входящие" 0 {Обработать все файлы в папке Входящие}
menuText R CCSend "Послать ход" 0 {Послать ваш ход через электронную почту или внешний (Xfcc-)помощник}

menuText R CCResign "Сдаться" 0 {Сдаться (не через электронную почту)}
menuText R CCClaimDraw "Требовать ничью" 0 {Послать ход и потребовать ничью (не через электронную почту)}
menuText R CCOfferDraw "Предложить ничью" 0 {Послать ход и предложить ничью (не через электронную почту)}
menuText R CCAcceptDraw "Принять ничью" 0 {Принять запрос о ничьей (не через электронную почту)}

menuText R CCNewMailGame "Новая партия по электронной почте..." 0 {Начинает новую партию по электронной почте}
menuText R CCMailMove "Письмо с ходом..." 0 {Послать сопернику письмо с ходом по электронной почте}
menuText R CCGamePage "Страница партии..." 0 {Позвать в партию через веб-браузер}

# menu in cc window:
menuText R CCEditCopy "Копировать список партий в буферную базу" 0 {Копирует партии как CSV список в буферную базу}

#  B    GHiJKL    Q  TUV XYZ

# Options menu:
menuText R Options "Настройки" 0
menuText R OptionsBoardGraphics "Поле..." 0 {Выбор текстур для полей}
translate R OptionsBGW {Выбор текстур для полей}
translate R OptionsBoardGraphicsText {Выбор графических файлов для белых и чёрных полей:}
menuText R OptionsBoardNames "Имя моего игрока..." 0 {Редактировать имя моего игрока}
menuText R OptionsExport "Экспорт" 0 {Изменить установки экспорта}
menuText R OptionsFonts "Шрифты" 0 {Изменить шрифты}
menuText R OptionsFontsRegular "Обычные" 0 {Изменить обычные шрифты}
menuText R OptionsFontsMenu "Меню" 0 {Изменить шрифты меню}
menuText R OptionsFontsSmall "Малые" 1 {Изменить малые шрифты}
menuText R OptionsFontsTiny "Крошечные" 0 {Изменить крошечные шрифты}
menuText R OptionsFontsFixed "Моноширинные" 0 {Изменить моноширинные шрифты}
menuText R OptionsGInfo "Информация о партии" 0 {Настройки информации о партии}
menuText R OptionsLanguage "Язык" 0 {Меню выбора языка}
menuText R OptionsMovesTranslatePieces "Перевод фигур" 0 {Перевести первый символ фигуры}
menuText R OptionsMovesHighlightLastMove "Подсветить последний ход" 0 {Подсветить последний ход}
menuText R OptionsMovesHighlightLastMoveDisplay "Показать" 0 {Показать  последние подсвеченные ходы}
menuText R OptionsMovesHighlightLastMoveWidth "Толщина" 0 {Толщина линии}
menuText R OptionsMovesHighlightLastMoveColor "Цвет" 0 {Цвет линии}
menuText R OptionsMovesHighlightLastMoveArrow "Включить стрелки" 0 {Включить стрелки с подсветкой}
menuText R OptionsMoves "Ходы" 0 {Установки для ходов}
menuText R OptionsMovesAnimate "Время анимации" 1 \
  {Установить количество времени, используемое для анимации ходов}
menuText R OptionsMovesDelay "Временная задержка автоигры..." 0 \
  {Установить время задержки для режима автоигры}
menuText R OptionsMovesCoord "Координаты ходов" 1 \
  {Принять стиль записи ходов с координатами ("g1f3")}
menuText R OptionsMovesSuggest "Показать рекомендуемые ходы" 0 \
  {Включить/выключить советуемые ходы}
menuText R OptionsShowVarPopup "Показать окно вариантов" 0 {Включает/выключает отображение окна вариантов}
menuText R OptionsMovesSpace "Добавлять пробелы после номера хода" 0 {Добавлять пробелы после номера хода}
menuText R OptionsMovesKey "Клавиатурное завершение" 0 \
  {Включить/выключить автозавершение клавиатурных ходов}
menuText R OptionsMovesShowVarArrows "Показывать стрелки в вариантах" 0 {Включить/выключить отображение стрелок ходов в вариантах}
menuText R OptionsMovesGlossOfDanger "Подсветка угроз" 0 {Вкл./выкл. подсветку угроз}
menuText R OptionsNumbers "Числовой формат" 0 {Выбрать числовой формат}
menuText R OptionsTheme "Темы" 0 {Смена внешнего вида интерфейса}
menuText R OptionsWindows "Окна" 0 {Установки окон}
menuText R OptionsSounds "Звуки" 2 {Настройки звука оповещения о ходе}
menuText R OptionsResources "Ресурсы..." 0 {Выбрать ресурсные каталоги и файлы}
menuText R OptionsWindowsDock "Закрепить окна" 0 {Закрепить окна}
menuText R OptionsWindowsSaveLayout "Сохранить расположение" 0 {Сохранить расположение}
menuText R OptionsWindowsRestoreLayout "Восстановить расположение" 0 {Восстановить расположение}
menuText R OptionsWindowsShowGameInfo "Показать информацию о партии" 0 {Показать информацию о партии}
menuText R OptionsWindowsAutoLoadLayout "Автозагрузка первого расположения" 0 {Автозагрузка первого расположения на старте}
menuText R OptionsECO "Загрузить файл ECO" 2 {Загрузить файл классификации ECO}
menuText R OptionsSpell "Загрузить файл проверки правописания" 4 {Загрузить в Scid файл проверки правописания}
menuText R OptionsTable "Директория таблиц" 15 \
  {Выбрать файл таблицы; все таблицы в этой директории будут использованы}
menuText R OptionsRecent "Недавно используемые файлы..." 2 \
  {Изменить количество недавно используемых файлов в меню "Файл"}
menuText R OptionsBooksDir "Директория книг" 0 {Установить директорию книг дебютов}
menuText R OptionsTacticsBasesDir "Директория баз" 0 {Установить директорию баз тактик (тренировок)}
menuText R OptionsPhotosDir "Директория фото" 0 {Установить директорию фото}
menuText R OptionsThemeDir "Файл/ы тем:"  0 { Загрузить файл темы интерфейса }
menuText R OptionsSave "Сохранить установки" 0 "Сохранить все установки в файл $::optionsFile"
menuText R OptionsAutoSave "Автосохранение установок при выходе" 0 \
  {Автосохранение всех установок при выходе из программы}

# Help menu:
menuText R Help "Помощь" 2
menuText R HelpContents "Содержание" 0 {Показать страницу контекстной помощи}
menuText R HelpIndex "Указатель" 0 {Показать содержание помощи}
menuText R HelpGuide "Быстрая справка" 0 {Показать страницу быстрой справки}
menuText R HelpHints "Советы" 1 {Показать страницу советов}
menuText R HelpContact "Контактная информация" 0 {Показать контактную информацию}
menuText R HelpTip "Подсказка дня" 0 {Показать полезную подсказку}
menuText R HelpStartup "Окно запуска" 2 {Показать окно запуска}
menuText R HelpAbout "О Scid" 2 {Информация о Scid}

# Game info box popup menu:
menuText R GInfoHideNext "Спрятать следующий ход" 0
menuText R GInfoMaterial "Показать оценку материала" 0
menuText R GInfoFEN "Показать FEN" 1
menuText R GInfoMarks "Показать цветом поля и стрелки" 2
menuText R GInfoWrap "Завернуть длинные строки" 0
menuText R GInfoFullComment "Показать полные комментарии" 3
menuText R GInfoPhotos "Показать фото" 4
menuText R GInfoTBNothing "Табличные базы: ничего" 0
menuText R GInfoTBResult "Табличные базы: только результат" 3
menuText R GInfoTBAll "Табличные базы: результат и лучшие ходы" 4
menuText R GInfoDelete "(Восстановить) удалить эту партию" 1
menuText R GInfoMark "(Снять отметку) отметить эту партию" 1
menuText R GInfoInformant "Настройка значений подсказчика" 0

# General buttons:
translate R Back {Назад}
translate R Browse {Просмотр}
translate R Cancel {Отмена}
translate R Continue {Продолжить}
translate R Clear {Очистить}
translate R Close {Закрыть}
translate R Contents {Содержание}
translate R Defaults {По-умолчанию}
translate R Delete {Удалить}
translate R Graph {График}
translate R Help {Помощь}
translate R Import {Импорт}
translate R Index {Индекс}
translate R LoadGame {Загрузка партии}
translate R BrowseGame {Просмотр партии}
translate R MergeGame {Объединение партий}
translate R MergeGames {Объединение партий}
translate R Preview {Предварительный просмотр}
translate R Revert {Возвратиться}
translate R Save {Сохранить}
translate R Search {Поиск}
translate R Stop {Стоп}
translate R Store {Сохранить}
translate R Update {Обновить}
translate R ChangeOrient {Изменить ориентацию окна}
translate R ShowIcons {Показать иконки}
translate R None {Никакой}
translate R First {Первый}
translate R Current {Текущий}
translate R Last {Последний}

# General messages:
translate R game {партия}
translate R games {партии}
translate R move {ход}
translate R moves {ходы}
translate R all {всё}
translate R Yes {Да}
translate R No {Нет}
translate R Both {Оба}
translate R King {Король}
translate R Queen {Ферзь}
translate R Rook {Ладья}
translate R Bishop {Слон}
translate R Knight {Конь}
translate R Pawn {Пешка}
translate R White {Белые}
translate R Black {Чёрные}
translate R Player {Игрок}
translate R Rating {Рейтинг}
translate R RatingDiff {Разница рейтингов (Белые - Чёрные)}
translate R AverageRating {Средний рейтинг}
translate R Event {Событие}
translate R Site {Место}
translate R Country {Страна}
translate R IgnoreColors {Игнорировать цвета}
translate R Date {Дата}
translate R EventDate {Дата турнира}
translate R Decade {Декада}
translate R Year {Год}
translate R Month {Месяц}
translate R Months {Январь Февраль Март Апрель Май Июнь Июль Август Сентябрь Октябрь Ноябрь Декабрь}
translate R Days {Вс Пн Вт Ср Чт Пт Сб}
translate R YearToToday {Текущий год}
translate R YearToTodayTooltip {Установить дату за прошедший год}
translate R Result {Результат}
translate R Round {Раунд}
translate R Length {Длина}
translate R ECOCode {Код ECO}
translate R ECO {ECO}
translate R Deleted {Удалена}
translate R SearchResults {Поиск результатов}
translate R OpeningTheDatabase {Открытие базы данных}
translate R Database {База данных}
translate R Filter {Фильтр}
translate R noGames {Нет партий}
translate R allGames {Все партии}
translate R empty {пусто}
translate R clipbase {буферная база}
translate R score {счёт}
translate R StartPos {Стартовая позиция}
translate R Total {Всего}
translate R readonly {Только для чтения}

# Standard error messages:
translate R ErrNotOpen {Эта база данных не открыта.}
translate R ErrReadOnly {Эта база данных только для чтения; она не может быть изменена.}
translate R ErrSearchInterrupted {Поиск был прерван; результаты неполные.}

# Game information:
translate R twin {двойник}
translate R deleted {удалена}
translate R comment {комментарий}
translate R hidden {скрытый}
translate R LastMove {Последний ход}
translate R NextMove {Следующий ход}
translate R GameStart {Начало партии}
translate R LineStart {Начало плана}
translate R GameEnd {Конец партии}
translate R LineEnd {Конец плана}

# Player information:
translate R PInfoAll {Результаты <b>всех</b> партий}
translate R PInfoFilter {Результаты <b>фильтрованных</b> партий}
translate R PInfoAgainst {Результаты против}
translate R PInfoMostWhite {Наиболее частые дебюты за белых}
translate R PInfoMostBlack {Наиболее частые дебюты за чёрных}
translate R PInfoRating {История рейтинга}
translate R PInfoBio {Биография}
translate R PInfoEditRatings {Редактировать рейтинги}
translate R PInfoEloFile {Файл}

# Tablebase information:
translate R Draw {Ничья}
translate R stalemate {пат}
translate R withAllMoves {со всеми ходами}
translate R withAllButOneMove {со всеми, кроме одного хода}
translate R with {с}
translate R only {только}
translate R lose {проиграно}
translate R loses {проиграли}
translate R allOthersLose {все остальные проиграны}
translate R matesIn {мат за}
translate R hasCheckmated {шах и мат}
translate R longest {самый длинный}
translate R WinningMoves {Выигрышные ходы}
translate R DrawingMoves {Ничейные ходы}
translate R LosingMoves {Проигрышные ходы}
translate R UnknownMoves {Ходы, приводящие к неизвестному результату}
translate R SideLongest {Сторона    Самый длинный    %     %     %\n}
translate R toNoveMate {право хода   мат    Победа  Ничья  Поражение\n}
translate R NoSummary {\nНет отчета для этой таблицы.}
translate R MutualZugzwang {Взаимный цугцванг: }
translate R SelectetZugzwang {\nВыбранные позиции цугцванга:}
translate R WdrawBloss {Белыми ничья, Черные проигрывают:}
translate R BdrawWloss {Черными ничья, Белые проигрывают:}
translate R AllLoss {" Прогрывает любой ход:     " }

# Tip of the day:
translate R Tip {Совет}
translate R TipAtStartup {Совет при загрузке}

# Tree window menus:
menuText R TreeFile "Файл" 0
menuText R TreeFileFillWithBase "Заполнить кеш базой" 0 {Заполнить файл кеша всеми партиями из текущей базы}
menuText R TreeFileFillWithGame "Заполнить кеш партией" 0 {Заполнить файл кеша текущей партией из текущей базы}
menuText R TreeFileSetCacheSize "Размер кеша" 0 {Устанавливает размер кеша}
menuText R TreeFileCacheInfo "Информация о кеше" 0 {Получает информацию об используемом кеше}
menuText R TreeFileSave "Сохранить файл кеша" 0 {Сохранить файл кеша дерева (.stc)}
menuText R TreeFileFill "Заполнить файл кеша" 0 \
  {Заполнить файл кеша общими дебютными позициями}
menuText R TreeFileBest "Список лучших партий" 1 {Показать дерево списка лучших партий}
menuText R TreeFileGraph "Окно диаграммы" 0 {Показать диаграмму для ветви этого дерева}
menuText R TreeFileCopy "Скопировать текст дерева в буфер" 1 \
  {Скопировать статистику дерева в буфер}
menuText R TreeFileClose "Закрыть окно дерева" 4 {Закрыть окно дерева}
menuText R TreeMask "Маска" 0
menuText R TreeMaskNew "Новая" 0 {Новая маска}
menuText R TreeMaskOpen "Открыть" 0 {Открыть маску}
menuText R TreeMaskOpenRecent "Открыть последнюю" 0 {Открыть последнюю маску}
menuText R TreeMaskSave "Сохранить" 0 {Сохранить маску}
menuText R TreeMaskClose "Закрыть" 0 {Закрыть маску}
menuText R TreeMaskFillWithGame "Наполнить партией" 0 {Наполняет маску партией}
menuText R TreeMaskFillWithBase "Наполнить базой" 0 {Наполняет маску всеми партиями из базы}
menuText R TreeMaskInfo "Информация" 0 {Показать статистику текущей маски}
menuText R TreeMaskDisplay "Показать карту маски" 0 {Показывает данные маски в древовидной форме}
menuText R TreeMaskSearch "Поиск" 0 {Поиск в текущей маске}
menuText R TreeSort "Сортировка" 0
menuText R TreeSortAlpha "Алфавитная" 0
menuText R TreeSortECO "По коду ECO" 3
menuText R TreeSortFreq "По частоте" 3
menuText R TreeSortScore "По результату" 3
menuText R TreeOpt "Установки" 0
menuText R TreeOptSlowmode "Медленный режим" 0 {Медленный режим обновлений (большая точность)}
menuText R TreeOptFastmode "Быстрый режим" 0 {Быстрый режим обновлений (без перестановки ходов)}
menuText R TreeOptFastAndSlowmode "Быстрый и медленный режим" 0 {Быстрый, а потом медленный режим обновлений}
menuText R TreeOptStartStop "Автообновление" 0 {Переключает автообновление в окне дерева}
menuText R TreeOptLock "Блокировать" 0 {(Раз)блокировать дерево для текущей базы}
menuText R TreeOptTraining "Тренировка" 0 {Включить/выключить режим тренировки}
menuText R TreeOptAutosave "Автосохранение файла кеша" 0 \
  {Автосохранение файла кеша, когда закрывается окно дерева}
menuText R TreeHelp "Помощь" 0
menuText R TreeHelpTree "Помощь по дереву" 0
menuText R TreeHelpIndex "Индекс помощи" 0
translate R SaveCache {Сохранить кеш}
translate R Training {Тренировка}
translate R LockTree {Блокировка}
translate R TreeLocked {Заблокировано}
translate R TreeBest {Лучший}
translate R TreeBestGames {Дерево лучших партий}
# Note: the next message is the tree window title row. After editing it,
# check the tree window to make sure it lines up with the actual columns.
translate R TreeTitleRow \
  {    Ход    ECO       Частота     Счёт   СрЭло Прзв СрГод  %ничьих}
translate R TreeTotal {ИТОГ}
translate R DoYouWantToSaveFirst {Вы хотите сохранить первым}
translate R AddToMask {Добавить в маску}
translate R RemoveFromMask {Удалить из маски}
translate R AddThisMoveToMask {Добавить этот ход в маску}
translate R SearchMask {Поиск в маске}
translate R DisplayMask {Показать маску}
translate R Nag {Nag код}
translate R Marker {Метка}
translate R Include {Включать}
translate R Exclude {Исключать}
translate R MainLine {Главный план}
translate R Bookmark {Закладка}
translate R NewLine {Новая строка}
translate R ToBeVerified {Проверять}
translate R ToTrain {Для тренировки}
translate R Dubious {Сомнительные}
translate R ToRemove {Для удаления}
translate R NoMarker {Без маркеров}
translate R ColorMarker {Цвет}
translate R WhiteMark {Белый}
translate R GreenMark {Зелёный}
translate R YellowMark {Жёлтый}
translate R BlueMark {Голубой}
translate R RedMark {Красный}
translate R CommentMove {Комментировать ход}
translate R CommentPosition {Комментировать позицию}
translate R AddMoveToMaskFirst {Добавить ход в маску первым}
translate R OpenAMaskFileFirst {Открыть первый файл маски}
translate R Positions {Позиции}
translate R Moves {Ходы}

# Finder window:
menuText R FinderFile "Файл" 0
menuText R FinderFileSubdirs "Смотреть в поддиректориях" 0
menuText R FinderFileClose "Закрыть поиск файлов" 0
menuText R FinderSort "Сортировка" 0
menuText R FinderSortType "Тип" 0
menuText R FinderSortSize "Размер" 0
menuText R FinderSortMod "Модифицировано" 0
menuText R FinderSortName "Имя" 0
menuText R FinderSortPath "Путь" 0
menuText R FinderTypes "Типы" 0
menuText R FinderTypesScid "База данных Scid" 0
menuText R FinderTypesOld "Старый формат базы данных Scid" 0
menuText R FinderTypesPGN "Файлы PGN" 0
menuText R FinderTypesEPD "Файлы EPD" 1
menuText R FinderTypesRep "Файлы репертуара" 6
menuText R FinderHelp "Помощь" 0
menuText R FinderHelpFinder "Помощь по поиску файлов" 0
menuText R FinderHelpIndex "Индекс помощи" 0
translate R FileFinder {Поиск файлов}
translate R FinderDir {Директория}
translate R FinderDirs {Директории}
translate R FinderFiles {Файлы}
translate R FinderUpDir {вверх}
translate R FinderCtxOpen {Открыть}
translate R FinderCtxBackup {Дублировать}
translate R FinderCtxCopy {Копировать}
translate R FinderCtxMove {Передвинуть}
translate R FinderCtxDelete {Удалить}

# Player finder:
menuText R PListFile "Файл" 0
menuText R PListFileUpdate "Обновить" 0
menuText R PListFileClose "Закрыть поиск игрока" 0
menuText R PListSort "Сортировка" 0
menuText R PListSortName "Имя" 0
menuText R PListSortElo "Рейтинг Эло" 0
menuText R PListSortGames "Партии" 0
menuText R PListSortOldest "Старейшая" 1
menuText R PListSortNewest "Новейшая" 0

# Tournament finder:
menuText R TmtFile "Файл" 0
menuText R TmtFileUpdate "Обновить" 0
menuText R TmtFileClose "Закрыть поиск турнира" 0
menuText R TmtSort "Сортировка" 0
menuText R TmtSortDate "Дата" 0
menuText R TmtSortPlayers "Игроки" 0
menuText R TmtSortGames "Партии" 0
menuText R TmtSortElo "Рейтинг Эло" 0
menuText R TmtSortSite "Место" 0
menuText R TmtSortEvent "Турнир" 0
menuText R TmtSortWinner "Победитель" 2
translate R TmtLimit "Границы списка"
translate R TmtMeanElo "Наименьшее значение рейтинга Эло"
translate R TmtNone "Ни одного турнира не найдено."

# Graph windows:
menuText R GraphFile "Файл" 0
menuText R GraphFileColor "Сохранить как цветной PostScript..." 14
menuText R GraphFileGrey "Сохранить как чёрно-белый PostScript..." 14
menuText R GraphFileClose "Закрыть окно" 6
menuText R GraphOptions "Параметры" 0
menuText R GraphOptionsWhite "Белые" 0
menuText R GraphOptionsBlack "Чёрные" 0
menuText R GraphOptionsBoth "Оба" 0
menuText R GraphOptionsPInfo "Игрок - информация об игроке" 0
menuText R GraphOptionsEloFile "ЭЛО из файла рейтинга" 0
menuText R GraphOptionsEloDB "ЭЛО из базы партий" 0
translate R GraphFilterTitle "Диаграмма фильтра: частота на 1000 партий"
translate R GraphAbsFilterTitle "Диаграмма фильтра: частота на все партии"
translate R ConfigureFilter {Настройка оси Х по годам, рейтингу и ходам}
translate R FilterEstimate "Оценка"
translate R TitleFilterGraph "Scid: Диаграмма фильтра"

# Analysis window:
translate R AddVariation {Добавить вариант}
translate R AddAllVariations {Добавить все варианты}
translate R AddMove {Добавить ход}
translate R Annotate {Аннотация}
translate R ShowAnalysisBoard {Показать доску анализа}
translate R ShowInfo {Показать информацию движка}
translate R FinishGame {Завершить партию}
translate R StopEngine {Остановить движок}
translate R StartEngine {Запустить движок}
translate R LockEngine {Закрепить движок в этой позиции}
translate R AnalysisCommand {Команда анализа}
translate R PreviousChoices {Предыдущие выборы}
translate R AnnotateTime {Установить время между ходами в секундах}
translate R AnnotateWhich {Добавить варианты}
translate R AnnotateAll {Для ходов обоих сторон}
translate R AnnotateAllMoves {Комментировать все ходы}
translate R AnnotateWhite {Только для ходов белых}
translate R AnnotateBlack {Только для ходов чёрных}
translate R AnnotateBlundersOnly {Когда ход в партии является очевидной ошибкой}
translate R AnnotateBlundersOnlyScoreChange {Анализ отчёта ошибок, которые изменяли счёт от/до:}
translate R BlundersThreshold {Порог}
translate R ScoreAllMoves {Пересчитать все ходы}
translate R LowPriority {Низкий приоритет CPU}
translate R ClickHereToSeeMoves {Кликнуть сюда, чтобы увидеть ходы}
translate R ConfigureInformant {Подсказчика}
translate R Informant!? {Ход подсказчика}
translate R Informant? {Плохой ход}
translate R Informant?? {Грубая ошибка}
translate R Informant?! {Сомнительный ход}
translate R Informant+= {У белых небольшое преимущество}
translate R Informant+/- {У белых умеренное преимущество}
translate R Informant+- {У белых решающее преимущество}
translate R Informant++- {Партия считается выигранной}

# Book window
translate R Book {Книга}
translate R OtherBookMoves {Книга соперника}
translate R OtherBookMovesTooltip {Ходы, к которым противник имеет ответ}

# Analysis Engine open dialog:
translate R EngineList {Список анализирующих движков}
translate R EngineName {Название}
translate R EngineCmd {Команда}
translate R EngineArgs {Параметры}
translate R EngineDir {Директория}
translate R EngineElo {Эло}
translate R EngineTime {Дата}
translate R EngineNew {Новый}
translate R EngineEdit {Редактор}
translate R EngineRequired {Поля, отмеченные жирным шрифтом, заполнять обязательно, остальные - по желанию}

# PGN window menus:
menuText R PgnFile "Файл" 0
menuText R PgnFileCopy "Скопировать игру в буфер обмена" 0
menuText R PgnFilePrint "Печатать в файл..." 0
menuText R PgnFileClose "Закрыть окно PGN" 0
menuText R PgnOpt "Отображение" 0
menuText R PgnOptColor "Цветное отображение" 0
menuText R PgnOptShort "Короткий (трёхстрочный) заголовок" 0
menuText R PgnOptSymbols "Символьная аннотация" 0
menuText R PgnOptIndentC "Комментарии с отступом" 2
menuText R PgnOptIndentV "Варианты с отступом" 0
menuText R PgnOptColumn "В колонку (один ход на строку)" 4
menuText R PgnOptSpace "Пробел после номера хода" 0
menuText R PgnOptStripMarks "Удалить коды цветных полей/стрелок" 0
menuText R PgnOptBoldMainLine "Использовать жирный шрифт для ходов главного плана" 4
menuText R PgnColor "Цвета" 0
menuText R PgnColorHeader "Заголовок..." 0
menuText R PgnColorAnno "Аннотация..." 0
menuText R PgnColorComments "Комментарии..." 0
menuText R PgnColorVars "Варианты..." 0
menuText R PgnColorBackground "Фон..." 0
menuText R PgnColorMain "Главный план..." 0
menuText R PgnColorCurrent "Фон текущего хода..." 1
menuText R PgnHelp "Помощь" 0
menuText R PgnHelpPgn "Помощь по PGN" 0
menuText R PgnHelpIndex "Индекс" 0
translate R PgnWindowTitle {Нотация - партия %u}

# Crosstable window menus:
menuText R CrosstabFile "Файл" 0
menuText R CrosstabFileText "Печатать в текстовый файл..." 11
menuText R CrosstabFileHtml "Печатать в HTML файл..." 11
menuText R CrosstabFileLaTeX "Печатать в LaTeX файл..." 11
menuText R CrosstabFileClose "Закрыть окно турнирной таблицы" 0
menuText R CrosstabEdit "Редактор" 0
menuText R CrosstabEditEvent "Турнир" 0
menuText R CrosstabEditSite "Место" 0
menuText R CrosstabEditDate "Дата" 0
menuText R CrosstabOpt "Отображение" 0
menuText R CrosstabOptAll "Все против всех" 0
menuText R CrosstabOptSwiss "Швейцарская система" 0
menuText R CrosstabOptKnockout "На вылет" 0
menuText R CrosstabOptAuto "Авто" 0
menuText R CrosstabOptAges "Возраст в годах" 2
menuText R CrosstabOptNats "Национальность" 2
menuText R CrosstabOptRatings "Рейтинг" 0
menuText R CrosstabOptTitles "Титул" 0
menuText R CrosstabOptBreaks "Счёт тай-брейков" 0
menuText R CrosstabOptDeleted "Включить удалённые партии" 1
menuText R CrosstabOptColors "Цвета (только для швейцарской системы)" 0
menuText R CrosstabOptColumnNumbers "Цифровые колонки (только для всех против всех)" 2
menuText R CrosstabOptGroup "Групповой счёт" 0
menuText R CrosstabSort "Сортировка" 0
menuText R CrosstabSortName "Имя" 0
menuText R CrosstabSortRating "Рейтинг" 0
menuText R CrosstabSortScore "Счёт" 0
menuText R CrosstabColor "Цвет" 0
menuText R CrosstabColorPlain "Обычный текст" 0
menuText R CrosstabColorHyper "Гипертекст" 0
menuText R CrosstabHelp "Помощь" 0
menuText R CrosstabHelpCross "Помощь по турнирной таблице" 0
menuText R CrosstabHelpIndex "Индекс помощи" 0
translate R SetFilter {Установить фильтр}
translate R AddToFilter {Добавить к фильтру}
translate R Swiss {Швейцарская система}
translate R Category {Категория}

# Opening report window menus:
menuText R OprepFile "Файл" 0
menuText R OprepFileText "Печатать в текстовый файл..." 11
menuText R OprepFileHtml "Печатать в HTML файл..." 11
menuText R OprepFileLaTeX "Печатать в LaTeX файл..." 11
menuText R OprepFileOptions "Установки..." 0
menuText R OprepFileClose "Закрыть окно дебютов" 0
menuText R OprepFavorites "Избранные" 1
menuText R OprepFavoritesAdd "Добавить отчёт..." 0
menuText R OprepFavoritesEdit "Редактировать отчёт по избранным..." 0
menuText R OprepFavoritesGenerate "Создать отчёты..." 0
menuText R OprepHelp "Помощь" 0
menuText R OprepHelpReport "Помощь по дебютным отчётам" 0
menuText R OprepHelpIndex "Индекс помощи" 0

# Header search:
translate R HeaderSearch {Поиск по заголовку}
translate R EndSideToMove {Перебраться на ту сторону в конце игры}
translate R GamesWithNoECO {Партии без ECO?}
translate R GameLength {Длина партии}
translate R FindGamesWith {Найти партии с флагами}
translate R StdStart {Нестандартный дебют}
translate R Promotions {Продвижения}
translate R Comments {Комментарии}
translate R Variations {Вариации}
translate R Annotations {Аннотации}
translate R DeleteFlag {Удалить флаг}
translate R WhiteOpFlag {Дебют белых}
translate R BlackOpFlag {Дебют чёрных}
translate R MiddlegameFlag {Миттельшпиль}
translate R EndgameFlag {Эндшпиль}
translate R NoveltyFlag {Новинка}
translate R PawnFlag {Пешечная структура}
translate R TacticsFlag {Тактика}
translate R QsideFlag {Игра на ферзевом фланге}
translate R KsideFlag {Игра на королевском фланге}
translate R BrilliancyFlag {Великолепно}
translate R BlunderFlag {Ошибка}
translate R UserFlag {Пользователь}
translate R PgnContains {Текст PGN}
translate R Annotator {Комментатор}
translate R Cmnts {Только комментированные партии}

# Game list window:
translate R GlistNumber {Номер}
translate R GlistWhite {Белые}
translate R GlistBlack {Чёрные}
translate R GlistWElo {Б-Эло}
translate R GlistBElo {Ч-Эло}
translate R GlistEvent {Турнир}
translate R GlistSite {Место}
translate R GlistRound {Раунд}
translate R GlistDate {Дата}
translate R GlistYear {Год}
translate R GlistEDate {Дата турнира}
translate R GlistResult {Результат}
translate R GlistLength {Длина}
translate R GlistCountry {Страна}
translate R GlistECO {ECO}
translate R GlistOpening {Дебют}
translate R GlistEndMaterial {Конечный материал}
translate R GlistDeleted {Удалённые}
translate R GlistFlags {Флаги}
translate R GlistVars {Варианты}
translate R GlistComments {Комментарии}
translate R GlistAnnos {Аннотации}
translate R GlistStart {Старт}
translate R GlistGameNumber {Номер партии}
translate R GlistAverageElo {Средний рейтинг Эло}
translate R GlistRating {Рейтинг}
translate R GlistFindText {Найти текст}
translate R GlistMoveField {Ходы}
translate R GlistEditField {Конфигурация}
translate R GlistAddField {Добавить}
translate R GlistDeleteField {Удалить}
translate R GlistWidth {Ширина}
translate R GlistAlign {Выравнивание}
translate R GlistAlignL {Выравнивание: слева}
translate R GlistAlignR {Выравнивание: справа}
translate R GlistAlignC {Выравнивание: по центру}
translate R GlistColor {Цвет}
translate R GlistSep {Разделитель}
translate R GlistCurrentSep {-- Текущий --}
translate R GlistNewSort {Новый}
translate R GlistAddToSort {Добавить}


# base sorting
translate R GsortSort {Сортировка...}
translate R GsortDate {Дата}
translate R GsortYear {Год}
translate R GsortEvent {Событие}
translate R GsortSite {Местоположение}
translate R GsortRound {Раунд}
translate R GsortWhiteName {Имя белого игрока}
translate R GsortBlackName {Имя чёрного игрока}
translate R GsortECO {ECO}
translate R GsortResult {Результат}
translate R GsortMoveCount {Число ходов}
translate R GsortAverageElo {Средний рейтинг Эло}
translate R GsortCountry {Страна}
translate R GsortDeleted {Удалённые}
translate R GsortEventDate {Дата турнира}
translate R GsortWhiteElo {Рейтинг белого игрока}
translate R GsortBlackElo {Рейтинг чёрного игрока}
translate R GsortComments {Комментарии}
translate R GsortVariations {Варианты}
translate R GsortNAGs {NAG'и}
translate R GsortAscending {Восходящие}
translate R GsortDescending {Нисходящие}
translate R GsortAdd {Добавить}
translate R GsortStore {Сохранить}
translate R GsortLoad {Загрузить}

# menu shown with right mouse button down on game list.
translate R GlistRemoveThisGameFromFilter  {Удалить эту партию из фильтра}
translate R GlistRemoveGameAndAboveFromFilter  {Удалить партию (и все другие выше неё) из фильтра}
translate R GlistRemoveGameAndBelowFromFilter  {Удалить партию (и все другие ниже неё) из фильтра}
translate R GlistDeleteGame {(Восстановить) удалить эту партию}
translate R GlistDeleteAllGames {Удалить все партии из фильтра}
translate R GlistUndeleteAllGames {Восстановить все партии в фильтре}
translate R GlistMergeGameInBase {Объединить партию из другой базы}

# Maintenance window:
translate R DatabaseName {Название базы данных:}
translate R TypeIcon {Тип иконки:}
translate R NumOfGames {Партии:}
translate R NumDeletedGames {Удаленные партии:}
translate R NumFilterGames {Партии в фильтре:}
translate R YearRange {Диапазон годов:}
translate R RatingRange {Диапазон рейтинга:}
translate R Description {Описание}
translate R Flag {Флаг}
translate R CustomFlags {Нестандартные флаги}
translate R DeleteCurrent {Удалить текущую партию}
translate R DeleteFilter {Удалить фильтрованные партии}
translate R DeleteAll {Удалить все партии}
translate R UndeleteCurrent {Восстановить текущую партию}
translate R UndeleteFilter {Восстановить фильтрованные партии}
translate R UndeleteAll {Восстановить все партии}
translate R DeleteTwins {Удалить двойные партии}
translate R MarkCurrent {Отметить текущую партию}
translate R MarkFilter {Отметить фильтрованные партии}
translate R MarkAll {Отметить все партии}
translate R UnmarkCurrent {Снять отметку с текущей партии}
translate R UnmarkFilter {Снять отметку с фильтрованных партий}
translate R UnmarkAll {Снять отметку со всех партий}
translate R Spellchecking {Проверка правописания}
translate R Players {Игроки}
translate R Events {Турниры}
translate R Sites {Место}
translate R Rounds {Раунды}
translate R DatabaseOps {Операции с базой данных}
translate R ReclassifyGames {Партии с классифицированным ECO}
translate R CompactDatabase {Сжать базу данных}
translate R SortDatabase {Сортировать базу данных}
translate R AddEloRatings {Добавить рейтинги Эло}
translate R AutoloadGame {Автозагрузка номера партии}
translate R StripTags {Удалить теги PGN}
translate R StripTag {Удалить теги}
translate R Cleaner {Чистильщик}
translate R CleanerHelp {
Чистильщик Scid произведёт над текущей базой данных все обслуживающие действия, которые вы выбрали в списке ниже.

Текущие установки классификации ECO и двойные диалоги удаления будут применены, если вы выберите эти функции.
}
translate R CleanerConfirm {
Если режим чистильщика стартовал, он не может быть прерван!

Это может занять много времени на большой базе данных, в зависимости от функций, которые вы выбрали, и их текущих установок.

Вы уверены, что хотите начать обслуживание функций, которые вы выбрали?
}
# Twinchecker
translate R TwinCheckUndelete {сбросить; "u" восстанавливает оба)}
translate R TwinCheckprevPair {Предыдущая пара}
translate R TwinChecknextPair {Следующая пара}
translate R TwinChecker {Scid: Контролёр партий-дубликатов}
translate R TwinCheckTournament {Партий в турнире:}
translate R TwinCheckNoTwin {Нет дубликатов}
translate R TwinCheckNoTwinfound {Для этой партии не найдено дубликатов.\nДля показа дубликатов с использованием этого окна, вы должны вначале использовать функцию "Удалить партии-дубликаты...".}
translate R TwinCheckTag {Распределённые признаки...}
translate R TwinCheckFound1 {Scid нашёл $result партий-дубликатов}
translate R TwinCheckFound2 { и установил для них флаги для удаления}
translate R TwinCheckNoDelete {В этой базе данных нет партий для удаления.}
translate R TwinCriteria1 { Ваши настройки для поиска дубликатов могут потенциально привести\nк тому, что разные партии с похожими ходами могут быть помечены, как дубликаты.}
translate R TwinCriteria2 {Рекомендуется в том случае, когда вы выбираете "Нет" для "одинаковых ходов", вы должны выбрать "Да" для настроек цветов, турниров, местоположений, раундов, годов и месяцев.\nХотите ли вы продолжить и удалить дубликаты в любом случае?}
translate R TwinCriteria3 {Рекомендуется в том случае, когда вы выбираете "Да" минимум для двух настроек "тоже местоположение", "тот же раунд" и "тот же год".\nХотите ли вы продолжить и удалить дубликаты в любом случае?}
translate R TwinCriteriaConfirm {Scid: Подтвердить настройки партий-дубликатов}
translate R TwinChangeTag "Изменение тегов следующих партий:\n\n"
translate R AllocRatingDescription "Эта команда использует текущий файл коррекции для добавления рейтинга Эло к партиям в этой базе данных. Везде, где игрок не имел текущий рейтинг хотя его/её рейтинг в той партии описан в файле коррекции, рейтинг будет добавлен."
translate R RatingOverride "Перезаписать существующие ненулевые рейтинги?"
translate R AddRatings "Добавить рейтинг в:"
translate R AddedRatings {Scid добавил $r рейтингов Эло в $g партий.}

#Bookmark editor
translate R NewSubmenu "Новое вложенное меню"

# Comment editor:
translate R AnnotationSymbols  {Символы аннотации:}
translate R Comment {Комментарии:}
translate R InsertMark {Вставить закладку}
translate R InsertMarkHelp {
Вставить/удалить метку: Выбор цвета, типа, поля.
Вставить/удалить указатель: Правый клик по двум полям.
}

# Nag buttons in comment editor:
translate R GoodMove {Хороший ход}
translate R PoorMove {Плохой ход}
translate R ExcellentMove {Отличный ход}
translate R Blunder {Грубая ошибка}
translate R InterestingMove {Интересный ход}
translate R DubiousMove {Сомнительный ход}
translate R WhiteDecisiveAdvantage {Белые имеют решающее преимущество}
translate R BlackDecisiveAdvantage {Чёрные имеют решающее преимущество}
translate R WhiteClearAdvantage {Белые имеют чистое преимущество}
translate R BlackClearAdvantage {Чёрное имеют чистое преимущество}
translate R WhiteSlightAdvantage {Белые имеют малое преимущество}
translate R BlackSlightAdvantage {Чёрные имеют малое преимущество}
translate R Equality {Равенство}
translate R Unclear {Непонятно}
translate R Diagram {Диаграмма}

# Board search:
translate R BoardSearch {Поиск позиции}
translate R FilterOperation {Действия над текущим фильтром:}
translate R FilterAnd {И (ограничивающий фильтр)}
translate R FilterOr {ИЛИ (добавить к фильтру)}
translate R FilterIgnore {ИГНОРИРОВАТЬ (сбросить фильтр)}
translate R SearchType {Тип поиска:}
translate R SearchBoardExact {Точная позиция (все фигуры на тех же полях)}
translate R SearchBoardPawns {Пешки (тот же материал, все пешки на тех же полях)}
translate R SearchBoardFiles {Ряды (тот же материал, все пешки на тех же рядах)}
translate R SearchBoardAny {Любая (тот же материал, пешки и фигуры в любом месте)}
translate R SearchInRefDatabase { Поиск в базе }
translate R LookInVars {Посмотреть в вариантах}

# Material search:
translate R MaterialSearch {Поиск материала}
translate R Material {Материал}
translate R Patterns {Образцы}
translate R Zero {Ноль}
translate R Any {Любой}
translate R CurrentBoard {Текущая позиция}
translate R CommonEndings {Простые эндшпили}
translate R CommonPatterns {Простые образцы}
translate R MaterialDiff {Материальная разница}
translate R squares {поля}
translate R SameColor {Тот же цвет}
translate R OppColor {Противоположный цвет}
translate R Either {Или}
translate R MoveNumberRange {Диапазон номеров ходов}
translate R MatchForAtLeast {Совпадает по крайней мере}
translate R HalfMoves {полуходов}

# Common endings in material search:
translate R EndingPawns {Пешечные эндшпили}
translate R EndingRookVsPawns {Ладья против пешки}
translate R EndingRookPawnVsRook {Ладья и одна пешка против ладьи}
translate R EndingRookPawnsVsRook {Ладья и пешки против ладьи}
translate R EndingRooks {Ладейные эндшпили}
translate R EndingRooksPassedA {Эндшпили "Ладья против ладьи с проходной пешкой"}
translate R EndingRooksDouble {Эндшпили с двумя ладьями}
translate R EndingBishops {Эндшпили "Слон против слона"}
translate R EndingBishopVsKnight {Эндшпили "Слон против коня"}
translate R EndingKnights {Эндшпили "Конь против коня"}
translate R EndingQueens {Ферзёвые эндшпили}
translate R EndingQueenPawnVsQueen {Ферзь и одна пешка против ферзя}
translate R BishopPairVsKnightPair {Миттельшпиль "Два слона против двух коней"}

# Common patterns in material search:
translate R PatternWhiteIQP {Белые IQP}
translate R PatternWhiteIQPBreakE6 {Белые IQP: d4-d5 прорыв против e6}
translate R PatternWhiteIQPBreakC6 {Белые IQP: d4-d5 прорыв против c6}
translate R PatternBlackIQP {Чёрные IQP}
translate R PatternWhiteBlackIQP {Белые IQP против чёрных IQP}
translate R PatternCoupleC3D4 {Белые c3+d4 удерживают несколько пешек}
translate R PatternHangingC5D5 {Чёрные подвешивают пешки на c5 и d5}
translate R PatternMaroczy {Центр Мароци (с пешками на c4 и e4)}
translate R PatternRookSacC3 {Жертва ладьи на c3}
translate R PatternKc1Kg8 {Рокировка O-O-O против O-O (Kc1 против Kg8)}
translate R PatternKg1Kc8 {Рокировка O-O против O-O-O (Kg1 против Kc8)}
translate R PatternLightFian {Фианкеттирование на белых полях (слон g2 против слона b7)}
translate R PatternDarkFian {Фианкеттирование на чёрных полях (слон b2 против слона g7)}
translate R PatternFourFian {Четыре фианкеттирования (слоны на b2, g2, b7, g7)}

# Game saving:
translate R Today {Сегодня}
translate R ClassifyGame {Классификационная игра}

# Setup position:
translate R EmptyBoard {Пустая доска}
translate R InitialBoard {Начальная позиция}
translate R SideToMove {Очередь хода}
translate R MoveNumber {Номер хода}
translate R Castling {Рокировка}
translate R EnPassantFile {Файл проходов}
translate R ClearFen {Очистить FEN}
translate R PasteFen {Вставить FEN}

translate R SaveAndContinue {Сохранить и продолжить}
translate R DiscardChangesAndContinue {Отказаться от изменений\nи продолжить}
translate R GoBack {Отказаться от хода}

# Replace move dialog:
translate R ReplaceMove {Заменить ход}
translate R AddNewVar {Добавить новый вариант}
translate R NewMainLine {Новый главный план}
translate R ReplaceMoveMessage {Здесь уже есть ход.

Вы можете заменить его, уничтожив все ходы после него, или добавить ваш ход, как новый вариант.

(Вы можете избежать появления этого сообщения в будущем, выключив установку "Спросить перед заменой ходов" в меню "Настройки: Меню ходов".)}

# Make database read-only dialog:
translate R ReadOnlyDialog {Если вы сделаете эту базу данных только для чтения, то не будет позволено никаких изменений.
Партии не могут быть изменены или заменены, удалённые флаги не могут быть изменены.
Результаты любой сортировки и классификации ECO будут временными.

Вы можете легко сделать базу данных модифицируемой с помощью закрытия и перезагрузки.

Вы действительно желаете сделать эту базу данных только для чтения?}

# Clear game dialog:
translate R ClearGameDialog {Эта партия была изменена.

Вы действительно желаете продолжить, отменив все сделанные изменения?
}

# Exit dialog:
translate R ExitDialog {Вы действительно хотите выйти из Scid?}
translate R ExitUnsaved {Следующая база данных имеет не сохраненные изменённые партии. Если вы выйдите сейчас, изменения будут утеряны.}

# Import window:
translate R PasteCurrentGame {Вставить текущую партию}
translate R ImportHelp1 {Ввести или вставить партию в формате PGN в область выше.}
translate R ImportHelp2 {Все ошибки импортирования партии будут показаны здесь.}
translate R OverwriteExistingMoves {Перезаписать существующий ход?}

# ECO Browser:
translate R ECOAllSections {всех секций ECO}
translate R ECOSection {Секция ECO}
translate R ECOSummary {Суммарно для}
translate R ECOFrequency {Частота суб-кодов для}

# Opening Report:
translate R OprepTitle {Открыть отчёт}
translate R OprepReport {Отчёт}
translate R OprepGenerated {Генерированный}
translate R OprepStatsHist {Статистика и история}
translate R OprepStats {Статистика}
translate R OprepStatAll {Все отчётные партии}
translate R OprepStatBoth {Оба с рейтингом}
translate R OprepStatSince {С}
translate R OprepOldest {Старейшие партии}
translate R OprepNewest {Новейшие партии}
translate R OprepPopular {Текущие популярные}
translate R OprepFreqAll {Частота за все года: }
translate R OprepFreq1   {В последний год: }
translate R OprepFreq5   {В последние пять лет: }
translate R OprepFreq10  {В последние десять лет: }
translate R OprepEvery {один раз каждые %u партий}
translate R OprepUp {выше - %u%s из всех лет}
translate R OprepDown {ниже - %u%s из всех лет}
translate R OprepSame {нет изменений за все года}
translate R OprepMostFrequent {Наиболее частые игроки}
translate R OprepMostFrequentOpponents {Наиболее частые соперники}
translate R OprepRatingsPerf {Рейтинги и производительность}
translate R OprepAvgPerf {Средние рейтинги и производительность}
translate R OprepWRating {Рейтинг белых}
translate R OprepBRating {Рейтинг чёрных}
translate R OprepWPerf {Производительность белых}
translate R OprepBPerf {Производительность чёрных}
translate R OprepHighRating {Партии с наибольшим среднем рейтингом}
translate R OprepTrends {Результирующие тенденции}
translate R OprepResults {Результирующие длины и частоты}
translate R OprepLength {Длина партии}
translate R OprepFrequency {Частота}
translate R OprepWWins {Белые выиграли: }
translate R OprepBWins {Чёрные выиграли: }
translate R OprepDraws {Ничьи:      }
translate R OprepWholeDB {Вся база данных}
translate R OprepShortest {Самые короткие победы}
translate R OprepMovesThemes {Ходы и темы}
translate R OprepMoveOrders {Порядок ходов для достижения отчётной позиции}
translate R OprepMoveOrdersOne \
  {Найден только один порядок ходов для достижения отчётной позиции:}
translate R OprepMoveOrdersAll \
  {Найдено %u порядков ходов для достижения отчётной позиции:}
translate R OprepMoveOrdersMany \
  {Найдено %u порядков ходов для достижения отчётной позиции. Верхние %u:}
translate R OprepMovesFrom {Ходы из отчётной позиции}
translate R OprepMostFrequentEcoCodes {Наиболее частые ECO коды}
translate R OprepThemes {Позиционные темы}
translate R OprepThemeDescription {Частота темы в первых %u ходах каждой партии}
translate R OprepThemeSameCastling {Односторонняя рокировка}
translate R OprepThemeOppCastling {Противоположная рокировка}
translate R OprepThemeNoCastling {Оба короля не рокированы}
translate R OprepThemeKPawnStorm {Штурм королевскими пешками}
translate R OprepThemeQueenswap {Ферзевой размен}
translate R OprepThemeWIQP {Белые изолированны королевской пешкой}
translate R OprepThemeBIQP {Чёрные изолированны королевской пешкой}
translate R OprepThemeWP567 {Белые пешки на 5/6/7 горизонтали}
translate R OprepThemeBP234 {Чёрные пешки на 2/3/4 горизонтали}
translate R OprepThemeOpenCDE {Открыты c/d/e вертикали}
translate R OprepTheme1BishopPair {Только одна сторона имеет слоновую пару}
translate R OprepEndgames {Эндшпили}
translate R OprepReportGames {Отчётные партии}
translate R OprepAllGames    {Все партии}
translate R OprepEndClass {Материал в конце каждой партии}
translate R OprepTheoryTable {Теоретическая таблица}
translate R OprepTableComment {Генерировано из %u высокорейтинговых партий.}
translate R OprepExtraMoves {Внешние заметки к ходам в теоретической таблице}
translate R OprepMaxGames {Максимум партий в теоретической таблице}
translate R OprepViewHTML {Просмотр HTML}
translate R OprepViewLaTeX {Просмотр LaTeX}

# Player Report:
translate R PReportTitle {Отчёт по игроку}
translate R PReportColorWhite {с Белыми фигурами}
translate R PReportColorBlack {с Чёрными фигурами}
translate R PReportMoves {после %s}
translate R PReportOpenings {Дебюты}
translate R PReportClipbase {Отчистить буферную базу и скопировать в неё игры матча}

# Piece Tracker window:
translate R TrackerSelectSingle {Левая кнопка мышки выбирает эту фигуру.}
translate R TrackerSelectPair {Левая кнопка мышки выбирает эту фигуру; правая выбирает все одинаковые фигуры.}
translate R TrackerSelectPawn {Левая кнопка мышки выбирает эту фигуру; правая выбирает все 8 пешек.}
translate R TrackerStat {Статистика}
translate R TrackerGames {% партий с ходами на этом поле}
translate R TrackerTime {% времени на каждом поле}
translate R TrackerMoves {Ходы}
translate R TrackerMovesStart {Введите номер хода, с которого начинается трассировка.}
translate R TrackerMovesStop {Введите номер хода, которым заканчивается трассировка.}

# Game selection dialogs:
translate R SelectAllGames {Все партии в базе данных}
translate R SelectFilterGames {Только фильтрованные партии}
translate R SelectTournamentGames {Только партии текущего турнира}
translate R SelectOlderGames {Только партии старше}

# Delete Twins window:
translate R TwinsNote {Чтобы быть дубликатами, две партии должны как минимум иметь тех же игроков, и критерии, которые вы установите ниже. Когда пара дубликатов найдена, более короткая партия удаляется. Подсказка: перед поиском дубликатов лучше проверить правописание, это облегчит поиск.}
translate R TwinsCriteria {Критерий: дубликаты должны иметь...}
translate R TwinsWhich {Проверка какие партии}
translate R TwinsColors {Игрок играет тем же цветом?}
translate R TwinsEvent {Тот же турнир?}
translate R TwinsSite {То же место?}
translate R TwinsRound {Тот же раунд?}
translate R TwinsYear {Тот же год?}
translate R TwinsMonth {Тот же месяц?}
translate R TwinsDay {Тот же день?}
translate R TwinsResult {Тот же результат?}
translate R TwinsECO {Тот же код ECO?}
translate R TwinsMoves {Те же ходы?}
translate R TwinsPlayers {Сравнить имена игроков:}
translate R TwinsPlayersExact {Полное совпадение}
translate R TwinsPlayersPrefix {Только первые 4 буквы}
translate R TwinsWhen {Когда удалены двойные партии}
translate R TwinsSkipShort {Игнорировать все партии, где меньше 5 ходов?}
translate R TwinsUndelete {Восстановить все партии сначала?}
translate R TwinsSetFilter {Установить фильтр для всех удалённых дубликатов?}
translate R TwinsComments {Всегда держать партии с комментариями?}
translate R TwinsVars {Всегда держать партии с вариантами?}
translate R TwinsDeleteWhich {Удалить какую партию:}
translate R TwinsDeleteShorter {Более короткая партия}
translate R TwinsDeleteOlder {Меньший номер партии}
translate R TwinsDeleteNewer {Больший номер партии}
translate R TwinsDelete {Удалить партии}

# Name editor window:
translate R NameEditType {Тип имени для редактирования}
translate R NameEditSelect {Партии для редактирования}
translate R NameEditReplace {Заменить}
translate R NameEditWith {с}
translate R NameEditMatches {Совпадения: нажмите Ctrl+1 - Ctrl+9 для выбора}

# Check games window:
translate R CheckGames {Проверенные партии}
translate R CheckGamesWhich {Проверенные партии}
translate R CheckAll {Все партии}
translate R CheckSelectFilterGames {Только фильтрованные партии}

# Classify window:
translate R Classify {Классифицировать}
translate R ClassifyWhich {Партии с классифицированными ECO}
translate R ClassifyAll {Все партии (переписать старые ECO)}
translate R ClassifyYear {Все партии, сыгранные за последний год}
translate R ClassifyMonth {Все партии, сыгранные за последний месяц}
translate R ClassifyNew {Только партии без кода ECO}
translate R ClassifyCodes {Коды ECO для использования}
translate R ClassifyBasic {Только основные коды ("B12", ...)}
translate R ClassifyExtended {Расширения Scid ("B12j", ...)}

# Compaction:
translate R NameFile {Файл имён}
translate R GameFile {Файл партий}
translate R Names {Имена}
translate R Unused {Не использовано}
translate R SizeKb {Размер (КиБ)}
translate R CurrentState {Текущее состояние}
translate R AfterCompaction {После сжатия}
translate R CompactNames {Сжатый файл имён}
translate R CompactGames {Сжатый файл партий}
translate R NoUnusedNames "Нет неиспользуемых имён, поэтому имя файла уже сжато полностью."
translate R NoUnusedGames "Файл партии уже полностью сжат."
translate R GameFileCompacted {Имя партии для базы данных было сжато.}

# Sorting:
translate R SortCriteria {Критерий}
translate R AddCriteria {Добавить критерий}
translate R CommonSorts {Общие сортировки}
translate R Sort {Сортировка}

# Exporting:
translate R AddToExistingFile {Добавить партии в существующий файл?}
translate R ExportComments {Экспортировать комментарии?}
translate R ExportVariations {Экспортировать варианты?}
translate R IndentComments {Комментарии с отступом?}
translate R IndentVariations {Варианты с отступом?}
translate R ExportColumnStyle {В колонку (один ход на строку)?}
translate R ExportSymbolStyle {Стиль символьной аннотации:}
translate R ExportStripMarks {Удалить маркированные коды полей/стрелок из комментариев?}

# Goto game/move dialogs:
translate R LoadGameNumber {Введите номер партии для загрузки:}
translate R GotoMoveNumber {Перейти к ходу номер:}

# Copy games dialog:
translate R CopyGames {Скопировать партии}
translate R CopyConfirm {
 Вы действительно желаете скопировать
 [::utils::thousands $nGamesToCopy] фильтрованных партий
 из базы данных "$fromName"
 в базу данных "$targetName"?
}
translate R CopyErr {Не могу скопировать партии}
translate R CopyErrSource {исходная база данных}
translate R CopyErrTarget {целевая база данных}
translate R CopyErrNoGames {нет партий при таком фильтре}
translate R CopyErrReadOnly {только для чтения}
translate R CopyErrNotOpen {не открыта}

# Colors:
translate R LightSquares {Светлые поля}
translate R DarkSquares {Тёмные поля}
translate R SelectedSquares {Выбранные поля}
translate R SuggestedSquares {Поля подсказанных ходов}
translate R WhitePieces {Белые фигуры}
translate R BlackPieces {Чёрные фигуры}
translate R WhiteBorder {Белые границы}
translate R BlackBorder {Чёрные границы}

# Novelty window:
translate R FindNovelty {Найти новинку}
translate R Novelty {Новинка}
translate R NoveltyInterrupt {Поиск новинки прерван}
translate R NoveltyNone {В этой партии новинок не найдено}
translate R NoveltyHelp {
Scid найдёт первый ход в текущей партии, который приведёт к позиции, отсутствующей в текущей базе данных и дебютной книге.
}

# Sounds configuration:
translate R SoundsFolder {Директория звуковых файлов}
translate R SoundsFolderHelp {Каталог должен содержать файлы King.wav, a.wav, 1.wav и т. п.}
translate R SoundsAnnounceOptions {Опции уведомления о ходе}
translate R SoundsAnnounceNew {Уведомлять о новых ходах, когда они сделаны}
translate R SoundsAnnounceForward {Уведомлять о ходе, когда сделан один ход вперёд}
translate R SoundsAnnounceBack {Уведомлять о сокращении или отмене одного хода}
translate R SoundsSoundDisabled {Scid не смог найти аудио пакет Snack при запуске; \nSound отключен.}

# Upgrading databases:
translate R Upgrading {Модернизация}
translate R ConfirmOpenNew {
Это старый формат (Scid 3) базы данных, который не может быть открыт в Scid 4, но версия в новом формате (Scid 4) уже создана.

Вы желаете открыть версию нового формата базы данных?
}
translate R ConfirmUpgrade {
Это старый формат (Scid 3) базы данных. Новый формат базы данных должен быть создан перед тем, как использовать его в Scid 4.

Обновление создаст новый формат базы данных; это не изменит и не удалит оригинальные файлы.

Это может занять время, но это делается только один раз. Вы можете отказаться, если это занимает слишком много времени.

Вы желаете обновить базу данных сейчас?
}

# Recent files options:
translate R RecentFilesMenu {Число недавно загруженных файлов в файловом меню}
translate R RecentFilesExtra {Число недавно загруженных файлов во внешнем подменю}

# My Player Names options:
translate R MyPlayerNamesDescription {
Ввести список привилегированных имён игроков ниже, по одному имени на строку. Маски разрешены (например, "?" для любого единичного символа, "*" для любой последовательности символов)

Каждый раз, когда партия с игроком в списке загружена, главное окно шахматной доски будет при необходимости поворачивается, чтобы показать партию от перспективы игрока.
}

#Coach
translate R showblunderexists {показать существующие ошибки}
translate R showblundervalue {показать вес ошибок}
translate R showscore {показать счёт}
translate R coachgame {тренировочная партия}
translate R configurecoachgame {Настроить тренировочную партию}
translate R configuregame {Настройка партии}
translate R Phalanxengine {Движок Phalanx}
translate R Coachengine {Движок тренера}
translate R difficulty {Сложность}
translate R hard {высокая}
translate R easy {низкая}
translate R Playwith {Играть с}
translate R white {белые}
translate R black {чёрные}
translate R both {оба}
translate R Play {Играть}
translate R Noblunder {Без ошибок}
translate R blunder {ошибка}
translate R Noinfo {-- Нет информации --}
translate R PhalanxOrTogaMissing {Phalanx или Toga не найдены}
translate R moveblunderthreshold {ход ошибочен, если потеряно более чем}
translate R limitanalysis {предельное время анализа движка}
translate R seconds {секунд}
translate R Abort {Прервать}
translate R Resume {Продолжить}
translate R OutOfOpening {Выход из дебюта}
translate R NotFollowedLine {Вы не следуете заданному плану}
translate R DoYouWantContinue {Вы хотите продолжить?}
translate R CoachIsWatching {Тренер наблюдает}
translate R Ponder {Постоянное размышление}
translate R LimitELO {Предел рейтинга Эло}
translate R DubiousMovePlayedTakeBack {Сделан сомнительный ход, вы хотите его отменить?}
translate R WeakMovePlayedTakeBack {Сделан слабый ход, вы хотите его отменить?}
translate R BadMovePlayedTakeBack {Сыгран плохой ход, вы хотите его отменить?}
translate R Iresign {Я сдаюсь}
translate R yourmoveisnotgood {ваш ход не хороший}
translate R EndOfVar {Конец вариантов}
translate R Openingtrainer {Тренер дебюта}
translate R DisplayCM {Показать возможный ход}
translate R DisplayCMValue {Показать вес возможного хода}
translate R DisplayOpeningStats {Показать статистику}
translate R ShowReport {Показать отчёт}
translate R NumberOfGoodMovesPlayed {сделаны хорошие ходы}
translate R NumberOfDubiousMovesPlayed {сделаны сомнительные ходы}
translate R NumberOfMovesPlayedNotInRepertoire {сделанные ходы не из репертуара}
translate R NumberOfTimesPositionEncountered {раз позиция встречается}
translate R PlayerBestMove  {Позволять только лучшие ходы}
translate R OpponentBestMove {Противник играет только лучшие ходы}
translate R OnlyFlaggedLines {Только помеченные строки}
translate R resetStats {Сбросить статистику}
translate R Repertoiretrainingconfiguration {Конфигурация тренировочного репертуара}
translate R Loadingrepertoire {Загрузка репертуара}
translate R Movesloaded {Ходы загружены}
translate R Repertoirenotfound {Репертуар не найден}
translate R Openfirstrepertoirewithtype {Открыть первую базу данных репертуара с иконкой/набором классов на правой панели}
translate R Movenotinrepertoire {Ход не из репертуара}
translate R PositionsInRepertoire {Позиция в репертуаре}
translate R PositionsNotPlayed {Позиция не игралась}
translate R PositionsPlayed {Позиция игралась}
translate R Success {Успех}
translate R DubiousMoves {Сомнительные ходы}
translate R OutOfRepertoire {Выход из репертуара}
translate R ConfigureTactics {Настройка тактики}
translate R ResetScores {Сброс счёта}
translate R LoadingBase {Загрузка базы}
translate R Tactics {Тактика}
translate R ShowSolution {Показать решение}
translate R NextExercise {Следующее упражнение}
translate R PrevExercise {Предыдущее упражнение}
translate R StopTraining {Остановить тренировку}
translate R Next {Следующий}
translate R ResettingScore {Сбросить счёт}
translate R LoadingGame {Загрузка партии}
translate R MateFound {Поиск мата}
translate R BestSolutionNotFound {Лучшее решение НЕ найдено!}
translate R MateNotFound {Мат не найден}
translate R ShorterMateExists {Кротчайший мат существует}
translate R ScorePlayed {Упражнение сыграно}
translate R Expected {ожидаемый}
translate R ChooseTrainingBase {Выбор тренировочной базы}
translate R Thinking {Размышление}
translate R AnalyzeDone {Анализ закончен}
translate R WinWonGame {Победа в выигрышной партии}
translate R Lines {Планы}
translate R ConfigureUCIengine {Настройка UCI движка}
translate R SpecificOpening {Особый дебют}
translate R StartNewGame {Начать новую партию}
translate R FixedLevel {Фиксированный уровень}
translate R Opening {Дебют}
translate R RandomLevel {Случайный уровень}
translate R StartFromCurrentPosition {Начать с текущей позиции}
translate R FixedDepth {Фиксированная глубина}
translate R Nodes {Узлы}
translate R Depth {Глубина}
translate R Time {Время}
translate R SecondsPerMove {Секунд на ход}
translate R Engine {Движок}
translate R TimeMode {Режим времени}
translate R TimeBonus {Время + премия}
translate R TimeMin {Мин}
translate R TimeSec {с}
translate R AllExercisesDone {Все задания выполнены}
translate R MoveOutOfBook {Ходы не по дебютной книге}
translate R LastBookMove {Ход из последней книги}
translate R AnnotateSeveralGames {Аннотировать несколько партий\nот текущей до:}
translate R FindOpeningErrors {Поиск ошибок дебюта}
translate R MarkTacticalExercises {Пометить тактические задания}
translate R UseBook {Использовать книгу}
translate R MultiPV {Множество вариантов}
translate R Hash {Хешировать память}
translate R OwnBook {Использовать книгу движка}
translate R BookFile {Открытые книги}
translate R AnnotateVariations {Комментировать варианты}
translate R ShortAnnotations {Показать комментарии}
translate R addAnnotatorTag {Добавить тег аннотирующего}
translate R AddScoreToShortAnnotations {Добавить счёт в короткие аннотации}
translate R Export {Экспорт}
translate R BookPartiallyLoaded {Книга частично загружена}
translate R Calvar {Расчёт вариантов}
translate R ConfigureCalvar {Настройки}
# Opening names used in tacgame.tcl
translate R Reti {Дебют Рети}
translate R English {Английское начало}
translate R d4Nf6Miscellaneous {Различные 1.d4 Nf6}
translate R Trompowsky {Атака Тромповского}
translate R Budapest {Будапештский гамбит}
translate R OldIndian {Староиндийская защита}
translate R BenkoGambit {Гамбит Бенко}
translate R ModernBenoni {Защита Модерн-Бенони}
translate R DutchDefence {Голландская защита}
translate R Scandinavian {Скандинавская защита}
translate R AlekhineDefence {Защита Алёхина}
translate R Pirc {Защита Пирца-Уфимцева}
translate R CaroKann {Защита Каро-Канн}
translate R CaroKannAdvance {Расширенная защита Каро-Канн}
translate R Sicilian {Сицилианская защита}
translate R SicilianAlapin {Сицилианская защита, Вариант Алапина}
translate R SicilianClosed {Сицилианская защита, Закрытый вариант}
translate R SicilianRauzer {Сицилианская защита, Атака Раузера}
translate R SicilianDragon {Сицилианская защита, Вариант дракона}
translate R SicilianScheveningen {Сицилианская защита, Шевенингенский вариант}
translate R SicilianNajdorf {Сицилианская защита, Вариант Найдорфа}
translate R OpenGame {Открытая партия}
translate R Vienna {Венская партия}
translate R KingsGambit {Королевский гамбит}
translate R RussianGame {Русская партия}
translate R ItalianTwoKnights {Итальянская партия/Два коня}
translate R Spanish {Испанская партии}
translate R SpanishExchange {Испанская партия, Разменный вариант}
translate R SpanishOpen {Испанская партия, Открытый вариант}
translate R SpanishClosed {Испанская партия, Закрытый вариант}
translate R FrenchDefence {Французская защита}
translate R FrenchAdvance {Французская защита, Продвинутый вариант}
translate R FrenchTarrasch {Французская защита, Система Тарраша}
translate R FrenchWinawer {Французская защита, Вариант Винавера}
translate R FrenchExchange {Французская защита, Разменный вариант}
translate R QueensPawn {Дебют ферзевых пешек}
translate R Slav {Славянская защита}
translate R QGA {Принятый ферзевый гамбит}
translate R QGD {Отказанный ферзевый гамбит}
translate R QGDExchange {Отказанный ферзевый гамбит, Разменная система}
translate R SemiSlav {Отказанный ферзевый гамбит, Славянская защита}
translate R QGDwithBg5 {Отказанный ферзевый гамбит с Bg5}
translate R QGDOrthodox {Отказанный ферзевый гамбит, Ортодоксальная защита}
translate R Grunfeld {Грюнфельд}
translate R GrunfeldExchange {Размен Грюнфельда}
translate R GrunfeldRussian {Вариант Грюнфельда русской партии}
translate R Catalan {Каталонское начало}
translate R CatalanOpen {Каталонское начало, Открытый вариант}
translate R CatalanClosed {Каталонское начало, Открытый вариант}
translate R QueensIndian {Новоиндийская защита}
translate R NimzoIndian {Новоиндийская защита, Вариант Нимцовича}
translate R NimzoIndianClassical {Новоиндийская защита, Классический вариант Нимцовича}
translate R NimzoIndianRubinstein {Новоиндийская защита, Система Рубинштейна}
translate R KingsIndian {Староиндийская защита}
translate R KingsIndianSamisch {Староиндийская защита, Система Земиша}
translate R KingsIndianMainLine {Староиндийская защита, Штыковой удар}

# FICS
translate R ConfigureFics {Настройки FICS}
translate R FICSGuest {Гостевой вход}
translate R FICSServerPort {Порт сервера}
translate R FICSServerAddress {IP адрес}
translate R FICSRefresh {Обновить}
translate R FICSTimesealPort {Порт Timeseal}
translate R FICSSilence {Фильтр консоли}
translate R FICSOffers {Предложения}
translate R FICSConsole {Консоль}
translate R FICSGames {Партии}
translate R FICSUnobserve {Остановить просмотр партии}
translate R FICSProfile {Просмотреть вашу историю и параметры пользователя}
translate R FICSRelayedGames {Переданные партии}
translate R FICSFindOpponent {Поиск соперника}
translate R FICSTakeback {Отменить 1 ход}
translate R FICSTakeback2 {Отменить 2 хода}
translate R FICSInitTime {Начальное время (мин)}
translate R FICSIncrement {Приращение (с)}
translate R FICSRatedGame {Рейтинговая партия}
translate R FICSAutoColour {автоматически}
translate R FICSManualConfirm {подтверждение вручную}
translate R FICSFilterFormula {Фильтровать по формуле}
translate R FICSIssueSeek {Выдать поиск}
translate R FICSChallenge {Вызов}
translate R FICSAccept {принять}
translate R FICSDecline {отклонить}
translate R FICSColour {Цвета}
translate R FICSSend {послать}
translate R FICSConnect {Соединение}
translate R FICSdefaultuservars {Использовать переменные по-умолчанию}
translate R FICSObserveconfirm {Вы хотите наблюдать за партией?}
translate R FICSpremove {Разрешить предварительный ход}

# Game review
translate R GameReview {Обзор партии}
translate R GameReviewTimeExtended {Продлённое время}
translate R GameReviewMargin {Поле ошибок}
translate R GameReviewAutoContinue {Автоматически продолжать, когда ход корректный}
translate R GameReviewReCalculate {Использовать расширенное время}
translate R GameReviewAnalyzingMovePlayedDuringTheGame {Анализ сделанных во время партии ходов}
translate R GameReviewAnalyzingThePosition {Анализ позиции}
translate R GameReviewEnterYourMove {Введите ваш ход}
translate R GameReviewCheckingYourMove {Проверка вашего хода}
translate R GameReviewYourMoveWasAnalyzed {Ваш ход проанализирован}
translate R GameReviewYouPlayedSameMove {Этот ход вы уже совершали в матче}
translate R GameReviewScoreOfYourMove {Счёт вашего хода}
translate R GameReviewGameMoveScore {Счёт хода игры}
translate R GameReviewEngineScore {Счёт движка}
translate R GameReviewYouPlayedLikeTheEngine {Вы сыграли как и движок}
translate R GameReviewNotEngineMoveButGoodMove {Ход не как у движка, но всё равно хороший}
translate R GameReviewMoveNotGood {Предупреждаю, этот ход не хороший}
translate R GameReviewMovesPlayedLike {Ходы сделаны как}
translate R GameReviewMovesPlayedEngine {Ходы сделаны как у движка}

# Correspondence Chess Dialogs:
translate R CCDlgConfigureWindowTitle {Настройка шахмат по переписке}
translate R CCDlgCGeneraloptions {Основные настройки}
translate R CCDlgDefaultDB {База данных по-умолчанию:}
translate R CCDlgInbox {Входящие (путь):}
translate R CCDlgOutbox {Исходящие (путь):}
translate R CCDlgXfcc {Конфигурация Xfcc:}
translate R CCDlgExternalProtocol {Драйвер внешнего протокола (например, Xfcc)}
translate R CCDlgFetchTool {Инструмент для приёма партий:}
translate R CCDlgSendTool {Инструмент для отправки партий:}
translate R CCDlgEmailCommunication {Передача по электронной почте}
translate R CCDlgMailPrg {Почтовая программа:}
translate R CCDlgBCCAddr {Адрес (B)CC:}
translate R CCDlgMailerMode {Режим:}
translate R CCDlgThunderbirdEg {например, Thunderbird, Mozilla Mail, Icedove...}
translate R CCDlgMailUrlEg {например, Evolution}
translate R CCDlgClawsEg {например, Sylpheed Claws}
translate R CCDlgmailxEg {например, mailx, mutt, nail...}
translate R CCDlgAttachementPar {Параметр вложения:}
translate R CCDlgInternalXfcc {Использовать встроенную поддержку Xfcc}
translate R CCDlgConfirmXfcc {Подтвердить ходы}
translate R CCDlgSubjectPar {Параметр темы:}
translate R CCDlgDeleteBoxes {Очистить Входящие/Исходящие}
translate R CCDlgDeleteBoxesText {Вы действительно хотите очистить ваши папки с Входящими и Исходящими для партий в шахматы по переписке? Это потребует новой синхронизации для показа последнего состояния ваших партий}
translate R CCDlgConfirmMove {Подтвердить ход}
translate R CCDlgConfirmMoveText {Если вы подтверждаете, то следующий ход и комментарий будут посланы на сервер:}
translate R CCDlgDBGameToLong {Противоречивый главный план}
translate R CCDlgDBGameToLongError {Главный план в вашей базе данных длиннее, чем в партии в папке Входящих. Если Входящие содержат текущие партии, то есть они сразу после синхронизации, то некоторые ходы были добавлены в главный план базы данных ошибочно.\n

В этом случае, пожалуйста, укоротите главную ветвь максимально до хода
}


translate R CCDlgStartEmail {Начать новую партию по почте}
translate R CCDlgYourName {Ваше имя:}
translate R CCDlgYourMail {Ваш адрес электронной почты:}
translate R CCDlgOpponentName {Имя соперника:}
translate R CCDlgOpponentMail {Адрес электронной почты соперника:}
translate R CCDlgGameID {Идентификатор партии (уникальный):}

translate R CCDlgTitNoOutbox {Scid: Исходящие шахмат по переписке}
translate R CCDlgTitNoInbox {Scid: Входящие шахмат по переписке}
translate R CCDlgTitNoGames {Scid: Нет партий в шахматы по переписке}
translate R CCErrInboxDir {Папка Входящие партий в шахматы по переписке:}
translate R CCErrOutboxDir {Папка Исходящие партий в шахматы по переписке:}
translate R CCErrDirNotUsable {не существует или недоступно!\nПроверьте, пожалуйста, корректность настроек.}
translate R CCErrNoGames {не содержит ни одной партии!\nВначале, пожалуйста, загрузите их.}

translate R CCDlgTitNoCCDB {Scid: Нет базы данных переписки}
translate R CCErrNoCCDB {Тип открытой базы данных не 'Переписка'. Пожалуйста, откройте соответствующую базу для использования функций переписки.}

translate R CCFetchBtn {Получить партии с сервера и обработать папку Входящие}
translate R CCPrevBtn {Перейти к предыдущей партии}
translate R CCNextBtn {Перейти к следующей партии}
translate R CCSendBtn {Послать ход}
translate R CCEmptyBtn {Отчистить папки Входящих и Исходящих}
translate R CCHelpBtn {Помощь на иконках и статусных индикаторах.\nДля получения помощи нажмите F1!}

translate R CCDlgServerName {Имя сервера:}
translate R CCDlgLoginName  {Логин:}
translate R CCDlgPassword   {Пароль:}
translate R CCDlgShowPassword {Показать пароль}
translate R CCDlgURL        {Адрес Xfcc:}
translate R CCDlgRatingType {Тип рейтинга:}

translate R CCDlgDuplicateGame {Не уникальный ID партии}
translate R CCDlgDuplicateGameError {Эта партия содержится более чем один раз в вашей базе данных. Пожалуйста, удалите дубликаты и упорядочите файл ваших партий (Файл/Обслуживание/Упорядочивание базы данных).}

translate R CCDlgSortOption {Сортировка:}
translate R CCDlgListOnlyOwnMove {Только партии, в которых я ходил}
translate R CCOrderClassicTxt {Местность, Событие, Раунд, Результат, Белые, Чёрные}
translate R CCOrderMyTimeTxt {Мои часы}
translate R CCOrderTimePerMoveTxt {Время на ход до следующего контроля времени}
translate R CCOrderStartDate {Начальная дата}
translate R CCOrderOppTimeTxt {Часы соперника}

translate R CCDlgConfigRelay {Настройка наблюдения ICCF}
translate R CCDlgConfigRelayHelp {Перейти на страницу партии http://www.iccf-webchess.com и показать наблюдаемую партию. Если вы видите шахматную доску, то скопируейте URL из вашего браузера в список ниже. Только один URL в строку!\nНапример: http://www.iccf-webchess.com/MakeAMove.aspx?id=266452}

# Connect Hardware dialogs
translate R ExtHWConfigConnection {Настройка внешнего оборудования}
translate R ExtHWPort {Порт}
translate R ExtHWEngineCmd {Команда движка}
translate R ExtHWEngineParam {Параметры движка}
translate R ExtHWShowButton {Показ}
translate R ExtHWHardware {Оборудование}
translate R ExtHWNovag {Шахматный компьютер Novag Citrine}
translate R ExtHWInputEngine {Входной движок}
translate R ExtHWNoBoard {Нет доски}
translate R NovagReferee {Судья}

# Input Engine dialogs
translate R IEConsole {Консоль входного движка}
translate R IESending {Послать ходы в}
translate R IESynchronise {Синхронизировать}
translate R IERotate  {Вращать}
translate R IEUnableToStart {Невозможно запустить входной движок:}


# Calculation of Variations
translate R DoneWithPosition {Закончено с позицией}

translate R Board {Доска}
translate R showGameInfo {Показать информацию о партии}
translate R autoResizeBoard {Автоматически изменять размер доски}
translate R DockTop {Двигаться вверх}
translate R DockBottom {Двигаться вниз}
translate R DockLeft {Двигаться влево}
translate R DockRight {Двигаться вправо}
translate R Undock {Расстыковать}

# Switcher window
translate R ChangeIcon {Сменить иконку...}
translate R NewGameListWindow {Новое окно списка партий}
translate R LoadatStartup {Загружать при запуске программы}
translate R ShowHideDB {Показать базы партий}
translate R ChangeFilter {Изменить фильтр}
translate R ChangeLayout {Загрузить/Сохранить/Изменить сортировку и расположение столбцов}
translate R ShowHideStatistic {Показать статистику}
translate R BoardFilter {Показать игры с текущей позицией на доске}
translate R CopyGameTo {Копировать игру в}
translate R FindBar {Строка поиска}
translate R FindCurrentGame {Найти текущую игру}
translate R DeleteGame {Удалить игру}
translate R UndeleteGame {Отменить удаление игры}
translate R ResetSort {Сбросить сортировку}
translate R ConvertNullMove {Преобразовать пустые ходы в комментарии}
translate R SetupBoard {Настроить доску}
translate R Rotate {Повернуть}
translate R SwitchColors {Переключить цвета}
translate R FlipBoard {Повернуть доску}
translate R ImportPGN {Импортировать файл PGN}
translate R ImportingFiles {Импортировать файлы PGN в}
translate R ImportingFrom {Импортировать из}
translate R ImportingIn {Импортировать игры в}
translate R UseLastTag {Использовать теги\nпоследней игры}
translate R Random {Случайный}
translate R BackToMainline {Вернуться к основной линии}
translate R LeaveVariant {Покинуть вариант}
translate R Autoplay {Автоходы}
translate R ShowHideCoords {Показать координаты}
translate R ShowHideMaterial {Показать материал}
translate R FullScreen {На весь экран}
translate R FilterStatistic {Статистика фильтра}
translate R MakeCorrections {Внести исправления}
translate R Surnames {Отчества}
translate R Ambiguous {Неясный}
translate R OptionsToolbar "Инструментальная панель"
translate R OptionsBoard "Шахматная доска"
translate R OptionsMenuColor "Цвета меню"
translate R OptionsBoardSize "Размер доски"
translate R OptionsBoardPieces "Стиль фигур"
translate R OptionsInternationalization "Локализация"
translate R MenuColorForeground "Цвет текста"
translate R MenuColorBackground "Фон"
translate R MenuColorSelect "Выбрать"
translate R OptionsTablebaseDir "Выберите до 4 директорий с базами таблиц:"
}
# end of russian.tcl
