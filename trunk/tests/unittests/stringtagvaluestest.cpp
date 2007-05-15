
#include "stringtagvaluestest.h"
#include "stringtagvalues.h"

void StringTagValuesTest::initTestCase() {}
void StringTagValuesTest::init(){}
void StringTagValuesTest::cleanup(){}
void StringTagValuesTest::cleanupTestCase() {}

void StringTagValuesTest::testStringTagValues()
{
	StringTagValues tagValues;

	//Create a temp file
	QFile file("/tmp/chessx-tagvalues-test.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);
	for (int i = 0; i < 100000; ++i) {
		out << i << "\n";
	}
	file.close();

	//Read the file, and insert in tags
	QTime t;
	t.start();
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine();
		// I sometimes comment this line out, to make testing faster.
		// This is a slow operation.
		//tagValues.add(line);
	}
	qDebug("Inserted 100,000 values without using cache: %d ms", t.elapsed());
	file.close();
	tagValues.clear();

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	in.setDevice(&file);

	t.start();
	tagValues.setCacheEnabled(true);
	while (!in.atEnd()) {
		QString line = in.readLine();
		tagValues.add(line);
	}
	tagValues.setCacheEnabled(false);
	qDebug("Inserted 100,000 values using cache: %d ms", t.elapsed());
	file.close();
	file.remove();


}

void StringTagValuesTest::testInputOutput()
{
	StringTagValues tagValues;
	StringTagValues tagValues2;
	QFile file("/tmp/chessx-tagvaluestream.dat");

	tagValues.add("String 1");
	tagValues.add("String 2");
	tagValues.add("String 3");
	tagValues.add("String 4");
	tagValues.add("String 5");
	tagValues.add("String 6");
	tagValues.add("String 5");
	tagValues.add("String 5");
	tagValues.add("String 2");
	tagValues.add("String 1");
	if (!file.open(QIODevice::WriteOnly))
		return;
	QDataStream out(&file);
	tagValues.write(out);
	file.close();

	if (!file.open(QIODevice::ReadOnly))
		return;
	QDataStream in(&file);
	tagValues2.read(in);
	file.close();
	QCOMPARE(tagValues.count() , tagValues2.count());
	for (int i = 0 ; i < tagValues.count() ; ++i) {
		QCOMPARE(tagValues.value(i) , tagValues2.value(i));
	}

	file.remove();

}
