#include <qapplication.h>
#include <qtextstream.h>
#include "tags.h"

static QTextStream out(stdout, IO_WriteOnly);


void testTags(){
QString fname = "/home/ejner/chessX/tagfiles/test";
Tags tags;
if (tags.removeTagsFile(fname))
  out << "removed " << fname << "\n";
else{
  out << "error removing " << fname << "\n";
  return;
}
if (tags.create(fname))
  out << "created " << fname << "\n";
else{
  out << "error creating " << fname << "\n";
  return;
}

tags.close();
if (tags.open(fname))
  out << "opened " << fname << "\n";
else{
  out << "error opening " << fname << "\n";
  return;
}

out << "added a tag: " << tags.add(Tags::Name,"Karpov, A") << "\n";
out << "added a tag: " << tags.add(Tags::Name,"Kasparov, G") << "\n";
out << "added a tag: " << tags.add(Tags::Name,"Anand, W") << "\n";
out << "added a tag: " << tags.add(Tags::Name,"Holst, A") << "\n";
out << "added a tag: " << tags.add(Tags::Name,"Hansen, Soren Bech") << "\n";
out << "added a tag: " << tags.add(Tags::Name,"Rudolf, Michal") << "\n";
out << "added a tag: " << tags.add(Tags::Name,"Hoggarth, William") << "\n";
out << "added a tag: " << tags.add(Tags::Name,"Hopfgartner, Heinz R") << "\n";
out << "added a tag: " << tags.add(Tags::Name,"Roets, Marius") << "\n";
out << "added a tag: " << tags.add(Tags::Event,"IECG-WC") << "\n";
out << "added a tag: " << tags.add(Tags::Event,"IECF-WC") << "\n";
out << "added a tag: " << tags.add(Tags::Site,"Scheveningen") << "\n";
out << "added a tag: " << tags.add(Tags::Site,"Wijk-Aan-Zee") << "\n";
out << "added a tag: " << tags.add(Tags::Site,"Los Angeles") << "\n";

out << "count tags of name type: " << tags.count(Tags::Name) << "\n";
out << "count tags of event type: " << tags.count(Tags::Event) << "\n";
out << "count tags of site type: " << tags.count(Tags::Site) << "\n";

for (int i=0; i<tags.count(Tags::Name); i++){
  out << "name tag " << i << ": " << tags.name(Tags::Name, i) << "\n";
}

out << "index for Anand =" << tags.index(Tags::Name,"Anand, W") << "\n";
tags.remove(Tags::Name,2);
out << "removed index 2\n";


QValueVector<bool> vb = tags.find(Tags::Name,"Ka");
for (int i=0; i<tags.count(Tags::Name); i++){
  if (vb[i])
    out << "index " << i << " matches prefix Ka \n";
  else
    out << "index " << i << " does not match prefix Ka \n";
}

for (int i=0; i<tags.count(Tags::Name); i++){
  out << "name tag " << i << ": " << tags.name(Tags::Name, i) << "\n";
}

out << "file size is " << tags.tagFileSize() << "\n";

out << "closing " << fname << "\n";
tags.close();
}



int main(int argv, char* argc[]) 
{
  QApplication app(argv, argc);
  testTags();
  return 0;
}


