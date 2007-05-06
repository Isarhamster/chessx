#include "common.h"
#include "board.h"
#include "game.h"

#include <QFile>

static QMap<quint64,QString> m_ecoPositions;

bool loadEcoFile(const QString& ecoFile)
{
	m_ecoPositions.clear();

	QFile file(ecoFile);
	file.open(QIODevice::ReadOnly);
	QTextStream ecoStream(&file);

	QString line;
	Board board;
	QString ecoCode;
	QRegExp ecoRegExp("[A-Z]\\d{2}[a-z]?");
	QStringList tokenList;
	QString token;
	Move move;

	while(!ecoStream.atEnd()) {
		line = ecoStream.readLine();

		//ignore comments and blank lines
		if(line.startsWith("#") || line == "") {
			continue;
		}

		//if line starts with eco code, store and begin new line
		if(line.indexOf(ecoRegExp) == 0) {
			ecoCode = line.section(' ', 0, 0);
			ecoCode += " " + line.section('"', 1, 1);
			board.setStandardPosition();
			line = line.section('"', 2);
		}

		//parse any moves on line
		tokenList = line.split(" ");
		for (QStringList::Iterator iterator = tokenList.begin(); iterator != tokenList.end(); iterator++) {
				token = *iterator;
				if(token == "*") {
					m_ecoPositions.insert(board.getHashValue(), ecoCode);
					continue;
				}
				if(token.contains('.')) {
					token = token.section('.', 1, 1);
				}
				if(token != "") {
					move = board.singleMove(token);
					if(board.isLegal(move)) {
						board.doMove(move);
					} else {
						m_ecoPositions.clear();
						return false;
					}
				}
	  }

	}

	return m_ecoPositions.count();
}

#include <iostream>
using namespace std;

int main()
{

	loadEcoFile("chessx.eco.txt");

	QFile file("chessx.eco");
	file.open(QIODevice::WriteOnly);
	QDataStream sout(&file);
	sout << COMPILED_ECO_FILE_ID;
	sout << m_ecoPositions;
	file.close();

	file.open(QIODevice::ReadOnly);
	QDataStream sin(&file);
	quint32 id;
	sin >> id;
	sin >> m_ecoPositions;

	if (id != COMPILED_ECO_FILE_ID)
		cout << "Bad compiled eco file" << endl;

	file.close();
}
