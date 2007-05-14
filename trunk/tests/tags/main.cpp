#include <qapplication.h>
#include <qtextstream.h>
#include "tags.h"

static QTextStream out(stdout, IO_WriteOnly);

void testTags()
{
	QString fname = "/home/ejner/chessX/tagfiles/test";
	Tags tags(fname);

// add some values to known tags
	out << "added a value: " << tags.add(Tags::PlayerName, "Karpov, A0") << "\n";
	out << "added a value: " << tags.add(Tags::PlayerName, "Kasparov, G1") << "\n";
	out << "added a value: " << tags.add(Tags::PlayerName, "Anand, W2") << "\n";
	out << "added a value: " << tags.add(Tags::PlayerName, "Holst, A3") << "\n";
	out << "added a value: " << tags.add(Tags::PlayerName, "Hansen, Søren Bech4") << "\n";
	out << "added a value: " << tags.add(Tags::PlayerName, "Rudolf, Michal5") << "\n";
	out << "added a value: " << tags.add(Tags::PlayerName, "Hoggarth, William6") << "\n";
	out << "added a value: " << tags.add(Tags::PlayerName, "Hopfgartner, Heinz R7") << "\n";
	out << "added a value: " << tags.add(Tags::PlayerName, "Roets, Marius8") << "\n";
	out << "added a value: " << tags.add(Tags::Event, "IECG-WC0") << "\n";
	out << "added a value: " << tags.add(Tags::Event, "IECF-WC1") << "\n";
	out << "added a value: " << tags.add(Tags::Site, "Scheveningen0") << "\n";
	out << "added a value: " << tags.add(Tags::Site, "Wijk-Aan-Zee1") << "\n";
	out << "added a value: " << tags.add(Tags::Site, "Los Angeles2") << "\n";

	tags.writeFile();
	tags.readFile();

// look up by index
	for (int i = 0; i < Tags::Source; i++) {
		out << "number of values of " << tags.tagName(i) << " tag type: " << tags.count(i) << "\n";
		for (int j = 0; j < tags.count(i); j++) {
			out << "Tag Name: " << tags.tagName(i) << ": value for index " << j << ": " << tags.value(i, j) << "\n";
		}
	}

// search by value
	out << "index for Anand =" << tags.valueId(Tags::PlayerName, "Anand, W2") << "\n";

// remove values
	tags.remove(Tags::PlayerName, 2);
	out << "removed index 2\n";
	for (int i = 0; i < tags.count(Tags::PlayerName); i++) {
		out << "playername tag value, index " << i << ": " << tags.value(Tags::PlayerName, i) << " has frequency: " << tags.valueFrequency(Tags::PlayerName, i) << "\n";
	}

// match strings
	QValueVector<bool> vb = tags.find(Tags::PlayerName, "Ka");
	for (uint i = 0; i < vb.count(); i++) {
		if (vb[i])
			out << "index " << i << " matches prefix Ka \n";
		else
			out << "index " << i << " does not match prefix Ka \n";
	}
	vb = tags.find(Tags::Site, "L");
	for (uint i = 0; i < vb.count(); i++) {
		if (vb[i])
			out << "index " << i << " matches prefix L \n";
		else
			out << "index " << i << " does not match prefix L \n";
	}


// define some custom tags and values, and extract information on them
	QString s = "Difficulty";
	int id = tags.defineTag(s);
	out << s << " tag got index = " << id << "\n";
	s = "Opening Play";
	id = tags.defineTag(s);
	out << s << " tag got index = " << id << "\n";
	out << "All tag names: " << "\n";
	QStringList qsl = tags.tagNames();
	for (QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it) {
		out << "tag name: " << *it << " has index = " << tags.tagId(*it) << "\n";
	}
	out << "Custom tag names: " << "\n";
	qsl = tags.tagNames(Tags::Custom);
	for (QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it) {
		out << "tag name: " << *it << " has index = " << tags.tagId(*it) << "\n";
	}

	tags.writeFile();
	tags.readFile();
	out << "after write/read...: " << "\n";

	out << "All tag names: " << "\n";
	qsl = tags.tagNames();
	for (QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it) {
		out << "tag name: " << *it << " has index = " << tags.tagId(*it) << "\n";
	}
	out << "Custom tag names: " << "\n";
	qsl = tags.tagNames(Tags::Custom);
	for (QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it) {
		out << "tag name: " << *it << " has index = " << tags.tagId(*it) << "\n";
	}
	out << "Known tag names: " << "\n";
	qsl = tags.tagNames(Tags::Known);
	for (QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it) {
		out << "tag name: " << *it << " has index = " << tags.tagId(*it) << "\n";
	}
	out << "Known optional tag names: " << "\n";
	qsl = tags.tagNames(Tags::KnownOptional);
	for (QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it) {
		out << "tag name: " << *it << " has index = " << tags.tagId(*it) << "\n";
	}
	out << "Known mandatory tag names: " << "\n";
	qsl = tags.tagNames(Tags::KnownMandatory);
	for (QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it) {
		out << "tag name: " << *it << " has index = " << tags.tagId(*it) << "\n";
	}

	s = "Opening Play";
	id = tags.tagId(s);
	out << "added a value: " << tags.add(id, "Nimzo") << "\n";
	out << "added a value: " << tags.add(id, "Nimzo") << "\n";
	out << "added a value: " << tags.add(id, "Bogo") << "\n";
	out << "added a value: " << tags.add(id, "Bogo") << "\n";
	out << "added a value: " << tags.add(id, "Bogo") << "\n";
	out << "added a value: " << tags.add(id, "HyperAcceleratedDragon") << "\n";
	out << "added a value: " << tags.add(id, "Tromp") << "\n";
	out << "added a value: " << tags.add(id, "DynamicEnglish") << "\n";
	out << "added a value: " << tags.add(id, "DynamicEnglish") << "\n";
	out << "added a value: " << tags.add(id, "DynamicEnglish") << "\n";
	tags.remove(id, tags.valueId(id, "DynamicEnglish"));
	out << "removed a value " << "\n";

	s = "Difficulty";
	id = tags.tagId(s);
	out << "added a value: " << tags.add(id, "Hard") << "\n";
	out << "added a value: " << tags.add(id, "Easy") << "\n";
	out << "added a value: " << tags.add(id, "Not so tough") << "\n";
	out << "added a value: " << tags.add(id, "A bit hard") << "\n";
	for (int i = 0; i < tags.count(id); i++) {
		out << s << " tag value, index " << i << ": " << tags.value(id, i) << " has frequency: " << tags.valueFrequency(id, i) << "\n";
	}
// remove values
	tags.remove(id, 2);
	out << "removed index 2\n";
	for (int i = 0; i < tags.count(id); i++) {
		out << s << " tag value, index " << i << ": " << tags.value(id, i) << " has frequency: " << tags.valueFrequency(id, i) << "\n";
	}
	tags.writeFile();
	tags.readFile();
	for (int i = 0; i < tags.count(id); i++) {
		out << s << " tag value, index " << i << ": " << tags.value(id, i) << " has frequency: " << tags.valueFrequency(id, i) << "\n";
	}

// undefine a custom tag
	s = "Difficulty";
	out << "undefining tag " << s << "\n";
	tags.unDefineTag(s);

	qsl = tags.tagNames(Tags::Custom);
	for (QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it) {
		out << "custom tag name: " << *it << " has index = " << tags.tagId(*it) << "\n";
	}

	tags.writeFile();
	tags.readFile();
	qsl = tags.tagNames(Tags::Custom);
	for (QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it) {
		out << "custom tag name: " << *it << " has index = " << tags.tagId(*it) << "\n";
	}
	s = "Difficulty";
	id = tags.tagId(s);
	out << "Difficulty id: " << id << "\n";
	s = "Opening Play";
	id = tags.tagId(s);
	out << "Opening Play id: " << id << "\n";
	for (int i = 0; i < tags.count(id); i++) {
		out << s << " tag value, index " << i << ": " << tags.value(id, i) << " has frequency: " << tags.valueFrequency(id, i) << "\n";
	}

	tags.writeFile();
	tags.readFile();
	for (int i = 0; i < tags.count(id); i++) {
		out << s << " tag value, index " << i << ": " << tags.value(id, i) << " has frequency: " << tags.valueFrequency(id, i) << "\n";
	}


}



void testTags2()
{
// for performance testing
// with DB2003-4, 419.00 players, 118.000 events, 131.000 sites
// and 1.400 rounds, Scid namefile is 8,4MB

	QString fname = "/home/ejner/chessX/tagfiles/test";
	Tags tags(fname);

	QString s;
// add 419.000 players
	for (int i = 0; i < 418000; i++) {
		s.setNum(i);
		tags.add(Tags::PlayerName, "Karpov,A" + s);
	}
// add 118.000 events

	for (int i = 0; i < 117000; i++) {
		s.setNum(i);
		tags.add(Tags::Event, "Berlin" + s);
	}
// add 131.000 sites
	for (int i = 0; i < 130000; i++) {
		s.setNum(i);
		tags.add(Tags::Site, "WijkAanZee" + s);
	}
// add 1.400 rounds
	for (int i = 0; i < 1300; i++) {
		s.setNum(i);
		tags.add(Tags::Round, "1" + s);
	}

	out << "count tags of name type: " << tags.count(Tags::PlayerName) << "\n";
	out << "count tags of event type: " << tags.count(Tags::Event) << "\n";
	out << "count tags of site type: " << tags.count(Tags::Site) << "\n";
	out << "count tags of round type: " << tags.count(Tags::Round) << "\n";

	tags.writeFile();
//tags.readFile();
// look up by index
//out << "Tag Name: " << tags.tagName(Tags::PlayerName) << ": value for index " << 0 << ": " << tags.value(Tags::PlayerName, 0) << "\n";
//out << "Tag Name: " << tags.tagName(Tags::PlayerName) << ": value for index " << 100000 << ": " << tags.value(Tags::PlayerName, 100000) << "\n";

}


int main(int argv, char* argc[])
{
	QApplication app(argv, argc);
	testTags();
	//testTags2();
	return 0;
}


