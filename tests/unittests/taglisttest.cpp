
#include "taglisttest.h"
#include "taglist.h"

void TagListTest::initTestCase() {}
void TagListTest::init() {}
void TagListTest::cleanup() {}
void TagListTest::cleanupTestCase() {}

void TagListTest::testTagNameMap()
{
    TagList tagList;

    for(int i = 0; i < TagNames.count(); ++i)
    {
        QCOMPARE(tagList.stringFromTag((Tag)i) , TagNames[i]);
    }

    QCOMPARE(tagList.stringFromTag((Tag)TagNames.count()) , QString(""));
    tagList.add("MyCustomTag", "Value1");
    QCOMPARE(tagList.stringFromTag((Tag)(tagList.count() - 1)) , QString("MyCustomTag"));
    tagList.add("WhiteTeamCountry", "RSA");

    for(int i = 0; i < TagNames.count(); ++i)
    {
        QCOMPARE(tagList.tagFromString(TagNames[i]) , i);
    }

    QCOMPARE(tagList.tagFromString("MyCustomTag") , tagList.count() - 2);
    QCOMPARE(tagList.tagFromString("WhiteTeamCountry") , tagList.count() - 1);
}

void TagListTest::testTagList()
{
    TagList tagList;

    int index1 = tagList.add(TagWhite, "Player 1");
    int index2 = tagList.add(TagBlack, "Player 2");
    int index3 = tagList.add(TagWhite, "Player 2");
    int index4 = tagList.add(TagBlack, "Player 3");
    int index5 = tagList.add("White", "Player 3");
    int index6 = tagList.add("Black", "Player 4");
    int index7 = tagList.add("EventDate", "????.??.??");
    int index8 = tagList.add(TagEventDate, "2004.??.??");
    int index9 = tagList.add("TagWithStrangeName", "ItsValue");

    QCOMPARE(index2 , index3);
    QVERIFY(index1 != index2);
    QVERIFY(index1 != index4);
    QVERIFY(index2 != index4);
    QCOMPARE(index4 , index5);

    QCOMPARE(tagList.tagValue(TagWhite, index1), QString("Player 1"));
    QCOMPARE(tagList.tagValue(TagBlack, index2), QString("Player 2"));
    QCOMPARE(tagList.tagValue(TagWhite, index3), QString("Player 2"));
    QCOMPARE(tagList.tagValue(TagBlack, index4), QString("Player 3"));
    QCOMPARE(tagList.tagValue(TagBlack, index5), QString("Player 3"));
    QCOMPARE(tagList.tagValue(TagBlack, index6), QString("Player 4"));
    QCOMPARE(tagList.tagValue(TagEventDate, index7), QString("????.??.??"));
    QCOMPARE(tagList.tagValue(TagEventDate, index8), QString("2004.??.??"));
    QCOMPARE(tagList.tagValue("TagWithStrangeName", index9) , QString("ItsValue"));

}
